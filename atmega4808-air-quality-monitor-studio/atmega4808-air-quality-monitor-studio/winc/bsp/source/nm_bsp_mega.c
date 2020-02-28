/**
 *
 * \file
 *
 * \brief This module contains ATMEGA BSP APIs implementation.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <atmel_start_pins.h>
#include "../include/nm_bsp.h"
#include "../../common/include/nm_common.h"
#include "clock_config.h"
#include <util/delay.h>
#include "conf_winc.h"
#include "port.h"

static tpfNmBspIsr gpfIsr;

ISR(CONF_WIFI_M2M_INT_vect)
{
	if (!(CONF_WIFI_M2M_INT_PIN_get_level()) && gpfIsr) {
		gpfIsr();
	}

	/* Insert your PORTF interrupt handling code here */

	/* Clear interrupt flags */
	VPORTF_INTFLAGS = (1 << 2);
}

/*
 *	@fn		init_chip_pins
 *	@brief	Initialize reset, chip enable and wake pin
 */
static void init_chip_pins(void)
{
	/* Configure control pins as output. */
	CONF_WIFI_M2M_RESET_PIN_set_dir(PORT_DIR_OUT);
	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_dir(PORT_DIR_OUT);
	CONF_WIFI_M2M_WAKE_PIN_set_dir(PORT_DIR_OUT);
}

/*
 *	@fn		nm_bsp_init
 *	@brief	Initialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_init(void)
{
	gpfIsr = NULL;

	/* Initialize chip IOs. */
	init_chip_pins();

	/* Perform chip reset. */
	nm_bsp_reset();

	cpu_irq_enable();

	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_deinit
 *	@brief	De-iInitialize BSP
 *	@return	0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{
	return M2M_SUCCESS;
}

/**
 *	@fn		nm_bsp_reset
 *	@brief	Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_level(false);
	CONF_WIFI_M2M_RESET_PIN_set_level(false);
	nm_bsp_sleep(10);
	CONF_WIFI_M2M_CHIP_ENABLE_PIN_set_level(true);
	nm_bsp_sleep(20);
	CONF_WIFI_M2M_RESET_PIN_set_level(true);
	nm_bsp_sleep(100);
}

/*
 *	@fn		nm_bsp_sleep
 *	@brief	Sleep in units of mSec
 *	@param[IN]	u32TimeMsec
 *				Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
	while (u32TimeMsec--) {
		_delay_ms(1);
	}
}

/*
 *	@fn		nm_bsp_register_isr
 *	@brief	Register interrupt service routine
 *	@param[IN]	pfIsr
 *				Pointer to ISR handler
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
	gpfIsr = pfIsr;

	CONF_WIFI_M2M_INT_PIN_set_dir(PORT_DIR_IN);
	CONF_WIFI_M2M_INT_PIN_set_pull_mode(PORT_PULL_UP);
	CONF_WIFI_M2M_INT_PIN_set_isc(PORT_ISC_FALLING_gc);

	/* Enable interrupt */
	CONF_WIFI_M2M_INT_PIN_set_isc(PORT_ISC_FALLING_gc);
}

/*
 *	@fn		nm_bsp_interrupt_ctrl
 *	@brief	Enable/Disable interrupts
 *	@param[IN]	u8Enable
 *				'0' disable interrupts. '1' enable interrupts
 */
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
	if (u8Enable) {
		CONF_WIFI_M2M_INT_PIN_set_isc(PORT_ISC_FALLING_gc);
	} else {
		CONF_WIFI_M2M_INT_PIN_set_isc(PORT_ISC_INTDISABLE_gc);
	}
}
