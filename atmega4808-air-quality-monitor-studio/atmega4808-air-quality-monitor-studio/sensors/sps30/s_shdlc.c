/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/******************************************************************************
 *  include files
 *****************************************************************************/
#include "s_shdlc.h"
#include "s_uart.h"
#include "clock_config.h"
#include "usart_basic.h"
#include <util/delay.h>

/******************************************************************************
 *  macro definitions
 *****************************************************************************/
#define SHDLC_START (0x7e)
#define SHDLC_STOP  (0x7e)

#define SHDLC_FRAME_MAX_TX_FRAME_SIZE (10)
#define SHDLC_FRAME_MAX_RX_FRAME_SIZE (100)

/******************************************************************************
 *  local function declarations
 *****************************************************************************/
static uint8_t sensirion_shdlc_check_unstuff(uint8_t data) ;
static uint8_t sensirion_shdlc_unstuff_byte(uint8_t data) ;


/*******************************************************************************
* @Name
*    static uint8_t sensirion_shdlc_check_unstuff(uint8_t data) 
* @Param
*    data to be checked for unstuffing
* @Returns
*    1 or 0
* @Description
*    checks and return if data is 7d or not i.e. need to unstuff or not
 *******************************************************************************/
static uint8_t sensirion_shdlc_check_unstuff(uint8_t data) 
{
    return data == 0x7d;
}

/*******************************************************************************
* @Name
*    static uint8_t sensirion_shdlc_unstuff_byte(uint8_t data)  
* @Param
*    data to be decode in original form
* @Returns
*    original data
* @Description
*    decode data in original value
 *******************************************************************************/
static uint8_t sensirion_shdlc_unstuff_byte(uint8_t data) 
{
    switch (data) {
        case 0x31:
            return 0x11;
        case 0x33:
            return 0x13;
        case 0x5d:
            return 0x7d;
        case 0x5e:
            return 0x7e;
        default:
            return data;
    }
}

/*******************************************************************************
* @Name
*    sensirion_shdlc_xcv()
* @Param
*     ~addr:           recipient address
*     ~cmd:            parameter
*     ~tx_data_len:    data length to send
*     ~tx_data:        data to send
*     ~rx_header:      Memory where the SHDLC header containing the sender address,
*                  command, sensor state and data length is stored
*     ~rx_data:        Memory where the received data is stored
* @Returns
*    none
* @Description
*    transceive (transmit then receive) an SHDLC frame
 *******************************************************************************/
void sensirion_shdlc_xcv(uint8_t addr, uint8_t cmd, uint8_t tx_data_len,
                            const uint8_t *tx_data, uint8_t max_rx_data_len,
                            struct sensirion_shdlc_rx_header *rx_header,
                            uint8_t *rx_data) 
{
    
     sensirion_shdlc_tx(addr, cmd, tx_data_len, tx_data);
	 
	 while(!(USART1.STATUS & USART_DREIF_bm));         //wait for uart to be ready
	 
     sensirion_shdlc_rx(max_rx_data_len, rx_header, rx_data);
}

/*******************************************************************************
* @Name
*    sensirion_shdlc_tx() 
* @Param
*       ~addr:       SHDLC recipient address
*       ~cmd:        command parameter
*       ~data_len:   data length to send
*       ~data:       data to send
* @Returns
*    none
* @Description
*    transmit an SHDLC frame
 *******************************************************************************/
void sensirion_shdlc_tx(uint8_t addr, uint8_t cmd, uint8_t data_len,
                           const uint8_t *data) 
{
    uint16_t len = 0;
    uint8_t tx_frame_buf[SHDLC_FRAME_MAX_TX_FRAME_SIZE];
    
    if(cmd == 0x03)
    {
        tx_frame_buf[len++] = SHDLC_START;
        tx_frame_buf[len++] = addr;
        tx_frame_buf[len++] = cmd;
        tx_frame_buf[len++] = data_len;
        tx_frame_buf[len++] = 0xFC;
        tx_frame_buf[len++] = SHDLC_STOP;
    }
    else if (cmd == 0x00)
    {
        tx_frame_buf[len++] = SHDLC_START;
        tx_frame_buf[len++] = addr;
        tx_frame_buf[len++] = cmd;
        tx_frame_buf[len++] = data_len;
        tx_frame_buf[len++] = 0x01;
        tx_frame_buf[len++] = 0x03;   
        tx_frame_buf[len++] = 0xF9;
        tx_frame_buf[len++] = SHDLC_STOP;
        
    }
	
    sensirion_uart_tx(len, tx_frame_buf);
}

/*******************************************************************************
* @Name
*    sensirion_shdlc_rx() 
* @Param
*       ~data_len:   max data length to receive
*       ~header:     Memory where the SHDLC header containing the sender address,
*              command, sensor state and data length is stored
*       ~data:       Memory where received data is stored
* @Returns
*    none
* @Description
*    receive an SHDLC frame
 *******************************************************************************/
void sensirion_shdlc_rx(uint8_t max_data_len,
                           struct sensirion_shdlc_rx_header *rxh,
                           uint8_t *data) 
{
    uint16_t len;
    uint16_t i;
    uint8_t rx_frame[SHDLC_FRAME_MAX_RX_FRAME_SIZE];
    uint8_t *rx_header = (uint8_t *)rxh;
    uint8_t j;
    uint8_t unstuff_next;

    len = sensirion_uart_rx(2 + (5 + (uint16_t)max_data_len) * 2, rx_frame);
    
    for (unstuff_next = 0, i = 1, j = 0; j < sizeof(*rxh) && i < len - 2; ++i) {
        if (unstuff_next) {
            rx_header[j++] = sensirion_shdlc_unstuff_byte(rx_frame[i]);
            unstuff_next = 0;
        } else {
            unstuff_next = sensirion_shdlc_check_unstuff(rx_frame[i]);
            if (!unstuff_next)
                rx_header[j++] = rx_frame[i];
        }
    }

    for (unstuff_next = 0, j = 0; j < rxh->data_len && i < len - 2; ++i) {
        if (unstuff_next) {
            data[j++] = sensirion_shdlc_unstuff_byte(rx_frame[i]);
            unstuff_next = 0;
        } else {
            unstuff_next = sensirion_shdlc_check_unstuff(rx_frame[i]);
            if (!unstuff_next)
                data[j++] = rx_frame[i];
        }
    }

}

