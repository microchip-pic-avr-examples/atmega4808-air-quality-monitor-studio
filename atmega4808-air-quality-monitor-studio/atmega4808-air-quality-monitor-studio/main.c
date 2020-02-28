/*
\file   main.c

\brief  Main source file.

(c) 2018 Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and any
derivatives exclusively with Microchip products. It is your responsibility to comply with third party
license terms applicable to your use of third party software (including open source software) that
may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY
IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
FOR A PARTICULAR PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP
HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO
THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL
CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT
OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS
SOFTWARE.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "application_manager.h"
#include "led.h"
#include "sensors_handling.h"
#include "cloud/cloud_service.h"
#include "debug_print.h"
#include "application/parameters.h"

// This handles messages published from the MQTT server when subscribed
void receivedFromCloud(uint8_t *topic, uint8_t *payload)
{
	char *toggleToken = "\"toggle\":";
	char *subString;

	if ((subString = strstr((char *)payload, toggleToken))) {
		LED_holdYellowOn(subString[strlen(toggleToken)] == '1');
	}

	debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "topic: %s", topic);
	debug_printer(SEVERITY_NONE, LEVEL_NORMAL, "payload: %s", payload);
}

/*
// This will get called every 1 second only while we have a valid Cloud connection
void sendToCloud(void)
{
	static char json[70];

	// This part runs every CFG_SEND_INTERVAL seconds
	int rawTemperature = SENSORS_getTempValue();
	int light          = SENSORS_getLightValue();
	int len
	    = sprintf(json, "{\"Light\":%d,\"Temp\":\"%d.%02d\"}", light, rawTemperature / 100, abs(rawTemperature) % 100);

	if (len > 0) {
		CLOUD_publishData((uint8_t *)json, len);
		LED_flashYellow();
	}
}
*/

int main(void)
{
	application_init();
	
	ParameterInit();

	while (1) {
		runScheduler();
	}

	return 0;
}
