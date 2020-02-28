/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */

#include "cryptoauthlib_main.h"
#include <atmel_start.h>

#include "cryptoauthlib/lib/atca_device.h"
#include "cloud/crypto_client/crypto_client.h"
#include "cryptoauthlib/lib/basic/atca_basic.h"

struct atca_command _gmyCommand;
struct atca_iface   _gmyIface;
struct atca_device  _gMyDevice = {&_gmyCommand, &_gmyIface};

void cryptoauthlib_init(void)
{
	uint8_t rv;

	atcab_init_device(&_gMyDevice);

	rv = atcab_init(&cfg_ateccx08a_i2c_custom);
	if (rv != ATCA_SUCCESS) {
		cryptoDeviceInitialized = false;
	} else {
		atcab_lock_data_slot(0);
		cryptoDeviceInitialized = true;
	}
}
