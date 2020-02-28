/**
 * \file
 *
 * \brief EVSYS related functionality implementation.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \addtogroup doc_driver_evsys
 *
 * \section doc_driver_evsys_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <evsys.h>

/**
 * \brief Initialize EVSYS interface
 *
 * \return Initialization status.
 */
int8_t EVENT_SYSTEM_0_init()
{

	// EVSYS.CHANNEL0 = EVSYS_GENERATOR_OFF_gc; /* Off */

	EVSYS.CHANNEL1 = EVSYS_GENERATOR_RTC_PIT3_gc; /* Periodic Interrupt Timer output 3 */

	// EVSYS.CHANNEL2 = EVSYS_GENERATOR_OFF_gc; /* Off */

	// EVSYS.CHANNEL3 = EVSYS_GENERATOR_OFF_gc; /* Off */

	EVSYS.CHANNEL4 = EVSYS_GENERATOR_PORT1_PIN6_gc; /* Port 1 Pin 6 */

	// EVSYS.CHANNEL5 = EVSYS_GENERATOR_OFF_gc; /* Off */

	// EVSYS.USERCCLLUT0A = EVSYS_CHANNEL_OFF_gc; /* Off */

	EVSYS.USERCCLLUT1A = EVSYS_CHANNEL_CHANNEL1_gc; /* Connect user to event channel 1 */

	// EVSYS.USERCCLLUT2A = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERCCLLUT3A = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERCCLLUT0B = EVSYS_CHANNEL_OFF_gc; /* Off */

	EVSYS.USERCCLLUT1B = EVSYS_CHANNEL_CHANNEL4_gc; /* Connect user to event channel 4 */

	// EVSYS.USERCCLLUT2B = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERCCLLUT3B = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERUSART0 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERUSART1 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERUSART2 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERUSART3 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERTCB0 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERTCB1 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERTCB2 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERTCB3 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERTCA0 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USERADC0 = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTA = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTB = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTC = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTD = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTE = EVSYS_CHANNEL_OFF_gc; /* Off */

	// EVSYS.USEREVOUTF = EVSYS_CHANNEL_OFF_gc; /* Off */

	return 0;
}
