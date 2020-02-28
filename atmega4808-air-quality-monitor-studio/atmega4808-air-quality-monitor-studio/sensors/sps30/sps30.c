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
#include "sps30.h"
#include "s_shdlc.h"
#include "s_uart.h"

/******************************************************************************
 *  macro definitions
 *****************************************************************************/
#define SPS_ADDR (0x00)
#define SPS_CMD_START_MEASUREMENT (0x00)
#define SPS_SUBCMD_MEASUREMENT_START                                           \
    { 0x01, 0x03 }
#define SPS_CMD_READ_MEASUREMENT (0x03)

#define NULL ((void*)0)

/*******************************************************************************
 * @Name
 *    void sps30_start_measurement(void) 
 * @Param
 *    none
 * @Returns
 *    none
 * @Description
 *    Start measurement by sending command 
 *******************************************************************************/
void sps30_start_measurement(void) 
{
    struct sensirion_shdlc_rx_header header;
    volatile uint8_t param_buf[] = SPS_SUBCMD_MEASUREMENT_START;

   sensirion_shdlc_xcv(SPS_ADDR, SPS_CMD_START_MEASUREMENT,
                               sizeof(param_buf), param_buf, 0, &header, NULL);
}

/*******************************************************************************
 * @Name
 *    void sps30_read_measurement(float *measurement) 
 * @Param
 *    address of variable 
 * @Returns
 *    none
 * @Description
 *    read data from sensor and stores 
 *******************************************************************************/
void sps30_read_measurement(float *measurement) 
{
    struct sensirion_shdlc_rx_header header = {0};
    float data[10] = {0.0};
    uint16_t idx = 0;
    volatile uint32_t *u32_data = (uint32_t *)data;
    uint32_t tmp = 0;

    sensirion_shdlc_xcv(SPS_ADDR, SPS_CMD_READ_MEASUREMENT, 0, NULL,
                              sizeof(data), &header, (uint8_t *)data);

    
    ++idx;
    tmp = be32_to_cpu(u32_data[idx]);
    *measurement =  *(float *)&tmp;


//    idx = 0;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->mc_1p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->mc_2p5 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->mc_4p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->mc_10p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->nc_0p5 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->nc_1p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->nc_2p5 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->nc_4p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->nc_10p0 = *(float *)&tmp;
//    ++idx;
//    tmp = be32_to_cpu(u32_data[idx]);
//    measurement->typical_particle_size = *(float *)&tmp;
//    ++idx;


}



