/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
*
* @defgroup nrf_dev_button_radio_tx_example_main main.c
* @{
* @ingroup nrf_dev_button_radio_tx_example
*
* @brief Radio Transceiver Example Application main file.
*
* This file contains the source code for a sample application using the NRF_RADIO peripheral.
*
*/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <nrf.h>
#include <bsp.h>
#include <nrf_delay.h>
#include "common.h"


static char                   packet[PACKET_STATIC_LENGTH];

/**@brief Function for sending packet.
 */
void send_packet()
{
    NRF_RADIO->PACKETPTR = (uint32_t)&packet;
    memset(packet, 0, sizeof(packet));
    // send the packet:
    NRF_RADIO->EVENTS_READY = 0U;
    NRF_RADIO->TASKS_TXEN   = 1;

    while (NRF_RADIO->EVENTS_READY == 0U)
    {
        // wait
    }
    NRF_RADIO->EVENTS_END  = 0U;
    NRF_RADIO->TASKS_START = 1U;

    while (NRF_RADIO->EVENTS_END == 0U)
    {
        // wait
    }

    NRF_RADIO->EVENTS_DISABLED = 0U;
    // Disable radio
    NRF_RADIO->TASKS_DISABLE = 1U;

    while (NRF_RADIO->EVENTS_DISABLED == 0U)
    {
        // wait
    }
}



/**
 * @brief Function for application main entry.
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
    LEDS_CONFIGURE(LEDS_MASK);
    LEDS_OFF(LEDS_MASK);

    // Set radio configuration parameters
    radio_configure2();

    // Set payload pointer
    //NRF_RADIO->PACKETPTR = (uint32_t)&packet;


    while (true)
    {
        send_packet();
        LEDS_ON(1 << LED_1);
        LEDS_ON(1 << LED_2);
        nrf_delay_ms(20);
        LEDS_OFF(LEDS_MASK);
        nrf_delay_ms(20);
        //packet = 0;
    }
}


/**
 *@}
 **/
