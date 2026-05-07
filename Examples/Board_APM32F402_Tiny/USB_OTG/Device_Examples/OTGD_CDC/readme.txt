/**
 * @file        readme.txt
 *
 * @brief       This file is routine instruction
 *
 * @version     V1.0.0
 *
 * @date        2024-08-01
 *
 * @attention
 *
 *  Copyright (C) 2023-2024 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

&par Example Description

This example describes how to use OTG to simulate a CDC device.
When CDC device receive data will send back the same data to USB host.

&par Hardware Description

The USART1 is configured as follows:
    - TX:PA9, RX:PA10
    - BaudRate = 115200 baud
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - Hardware flow control disabled (RTS and CTS signals)
    - Receive and transmit enabled

    - OTG_FS_VBUS(PA9) ----------> X(Use to USART1)
    - OTG_FS_ID(PA10)  ----------> X(Use to USART1)
    - VCC5V            ----------> VBUS
    - OTG_FS_DM(PA11)  ----------> USB_DN
    - OTG_FS_DP(PA12)  ----------> USB_DP
    - GND              ----------> GND

&par Directory contents

  - Device_Examples/OTGD_CDC/Source/apm32f4xx_int.c          Interrupt handlers
  - Device_Examples/OTGD_CDC/Source/main.c                   Main program

&par IDE environment

  - MDK-ARM V5.36
  - EWARM V8.50.5.26295
  - ECLIPSE-EMB V4.24.0

&par Hardware and Software environment

  - This example runs on APM32F402 TINY Devices.
