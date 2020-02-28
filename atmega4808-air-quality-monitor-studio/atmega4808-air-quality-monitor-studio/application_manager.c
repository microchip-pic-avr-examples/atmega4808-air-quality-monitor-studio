/*
 * application_manager.c
 *
 * Created: 10/4/2018 1:37:19 PM
 *  Author: I17643
 */

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <atomic.h>
#include <avr/wdt.h>
#include "application_manager.h"
#include "atmel_start_pins.h"
#include "atmel_start.h"
#include "config/IoT_Sensor_Node_config.h"
#include "cloud/cloud_service.h"
#include "cloud/crypto_client/cryptoauthlib_main.h"
#include "cloud/crypto_client/crypto_client.h"
#include "cloud/wifi_service.h"
#if CFG_ENABLE_CLI
#include "cli/cli.h"
#endif
#include "credentials_storage/credentials_storage.h"
#include "led.h"
#include "debug_print.h"

#include "../application/aqm_tasks/display.h"
#include "../application/aqm_tasks/sleep.h"
#include "../application/aqm_tasks/send.h"
#include "../application/aqm_tasks/store.h"
#include "../application/aqm_tasks/read.h"

#define MAIN_DATATASK_INTERVAL 100L
// The debounce time is currently close to 2 Seconds.
#define SW_DEBOUNCE_INTERVAL 1460000L

#define SW0_TOGGLE_STATE SW0_get_level()
#define SW1_TOGGLE_STATE SW1_get_level()

// This will contain the device ID, before we have it this dummy value is the init value which is non-0
char                       attDeviceID[20] = "BAAAAADD1DBAAADD1D";
shared_networking_params_t shared_networking_params;
ATCA_STATUS                retValCryptoClientSerialNumber;

absolutetime_t MAIN_dataTask(void *payload);
timer_struct_t MAIN_dataTasksTimer = {MAIN_dataTask};

extern timer_struct_t DISPLAY_TasksTimer;
extern timer_struct_t SLEEP_TasksTimer;
extern timer_struct_t SEND_TasksTimer;
extern timer_struct_t STORE_TasksTimer;
extern timer_struct_t READ_TasksTimer;

void wifiConnectionStateChanged(uint8_t status);

void application_init()
{
	uint8_t  mode          = WIFI_DEFAULT;
	uint32_t sw0CurrentVal = 0;
	uint32_t sw1CurrentVal = 0;
	uint32_t i             = 0;

	wdt_disable();

	// Initialization of modules before interrupts are enabled
	atmel_start_init();
	
	PIT_initialization();
	DisplayON();				//oled init and display AQM on it	

	LED_test();
#if CFG_ENABLE_CLI
	CLI_init();
	CLI_setdeviceId(attDeviceID);
#endif
	debug_init(attDeviceID);

	ENABLE_INTERRUPTS();

	// Initialization of modules where the init needs interrupts to be enabled
	cryptoauthlib_init();

	if (cryptoDeviceInitialized == false) {
		debug_printError("APP: CryptoAuthInit failed");
	}
	// Get serial number from the ECC608 chip
	retValCryptoClientSerialNumber = CRYPTO_CLIENT_printSerialNumber(attDeviceID);
	if (retValCryptoClientSerialNumber != ATCA_SUCCESS) {
		switch (retValCryptoClientSerialNumber) {
		case ATCA_GEN_FAIL:
			debug_printError("APP: DeviceID generation failed, unspecified error");
			break;
		case ATCA_BAD_PARAM:
			debug_printError("APP: DeviceID generation failed, bad argument");
		default:
			debug_printError("APP: DeviceID generation failed");
			break;
		}
	}
#if CFG_ENABLE_CLI
	CLI_setdeviceId(attDeviceID);
#endif
	debug_setPrefix(attDeviceID);

	// Blocking debounce
	for (i = 0; i < SW_DEBOUNCE_INTERVAL; i++) {
		sw0CurrentVal += SW0_TOGGLE_STATE;
		sw1CurrentVal += SW1_TOGGLE_STATE;
	}
	if (sw0CurrentVal < (SW_DEBOUNCE_INTERVAL / 2)) {
		if (sw1CurrentVal < (SW_DEBOUNCE_INTERVAL / 2)) {
			strcpy(ssid, CFG_MAIN_WLAN_SSID);
			strcpy(pass, CFG_MAIN_WLAN_PSK);
			sprintf((char *)authType, "%d", CFG_MAIN_WLAN_AUTH);
			LED_startBlinkingGreen();
		} else {
			mode = WIFI_SOFT_AP;
		}
	}
	scheduler_timeout_create(&SLEEP_TasksTimer, SLEEP_TASK_INTERVAL);
	scheduler_timeout_create(&DISPLAY_TasksTimer, DISPLAY_TASK_INTERVAL_FIRST);
	scheduler_timeout_create(&SEND_TasksTimer, SEND_TASK_INTERVAL);
	scheduler_timeout_create(&STORE_TasksTimer, STORE_TASK_INTERVAL);
	scheduler_timeout_create(&READ_TasksTimer, READ_TASK_INTERVAL);
		
	wifi_init(wifiConnectionStateChanged, mode);

	if (mode == WIFI_DEFAULT) {
		CLOUD_init(attDeviceID);
		scheduler_timeout_create(&MAIN_dataTasksTimer, MAIN_DATATASK_INTERVAL);
	}

	LED_test();
}

void application_post_provisioning(void)
{
	CLOUD_init(attDeviceID);
	scheduler_timeout_create(&MAIN_dataTasksTimer, MAIN_DATATASK_INTERVAL);
}

// React to the WIFI state change here. Status of 1 means connected, Status of 0 means disconnected
void wifiConnectionStateChanged(uint8_t status)
{
	// If we have no AP access we want to retry
	if (status != 1) {
		// Restart the WIFI module if we get disconnected from the WiFi Access Point (AP)
		CLOUD_reset();
	}
}

// This scheduler will check all tasks and timers that are due and service them
void runScheduler(void)
{
	scheduler_timeout_call_next_callback();
}

// This could be better done with a function pointer (DI) but in the interest of simplicity
//     we avoided that. This is being called from MAIN_dataTask below
void sendToCloud(void);

// This gets called by the scheduler approximately every 100ms
absolutetime_t MAIN_dataTask(void *payload)
{
/*	static time_t previousTransmissionTime = 0;

	// Get the current time. This uses the C standard library time functions
	time_t timeNow = time(NULL);

	// Example of how to send data when MQTT is connected every 1 second based on the system clock
	if (CLOUD_isConnected()) {
		// How many seconds since the last time this loop ran?
		int32_t delta = difftime(timeNow, previousTransmissionTime);

		if (delta >= CFG_SEND_INTERVAL) {
			previousTransmissionTime = timeNow;

			// Call the data task in main.c
			sendToCloud();
		}
	}
*/
	LED_BLUE_set_level(!shared_networking_params.haveAPConnection);
	LED_RED_set_level(!shared_networking_params.haveERROR);
	if (LED_isBlinkingGreen() == false) {
		LED_GREEN_set_level(!CLOUD_isConnected());
	}

	// This is milliseconds managed by the RTC and the scheduler, this return makes the
	//      timer run another time, returning 0 will make it stop
	return MAIN_DATATASK_INTERVAL;
}
