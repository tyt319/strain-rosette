/**
 * @file        apm32f4xx_dal_cfg.h
 *
 * @brief       DAL configuration file
 *
 * @version     V1.0.0
 *
 * @date        2024-08-01
 *
 * @attention
 *
 *  Copyright (C) 2024-2025 Geehy Semiconductor
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


/* Define to prevent recursive inclusion */
#ifndef APM32F4xx_DAL_CFG_H
#define APM32F4xx_DAL_CFG_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Configuration settings for assert enable */
/* #define USE_FULL_ASSERT     1U */

/* DAL module configuration */
#define DAL_MODULE_ENABLED
//#define DAL_ADC_MODULE_ENABLED
//#define DAL_CAN_MODULE_ENABLED
//#define DAL_CRC_MODULE_ENABLED
#define DAL_DMA_MODULE_ENABLED
#define DAL_FLASH_MODULE_ENABLED
#define DAL_GPIO_MODULE_ENABLED
//#define DAL_EINT_MODULE_ENABLED
//#define DAL_I2C_MODULE_ENABLED
//#define DAL_SMBUS_MODULE_ENABLED
//#define DAL_I2S_MODULE_ENABLED
//#define DAL_IWDT_MODULE_ENABLED
#define DAL_PMU_MODULE_ENABLED
#define DAL_RCM_MODULE_ENABLED
//#define DAL_RTC_MODULE_ENABLED
#define DAL_SPI_MODULE_ENABLED
//#define DAL_TMR_MODULE_ENABLED
//#define DAL_UART_MODULE_ENABLED
//#define DAL_USART_MODULE_ENABLED
//#define DAL_IRDA_MODULE_ENABLED
//#define DAL_SMARTCARD_MODULE_ENABLED
//#define DAL_WWDT_MODULE_ENABLED
#define DAL_CORTEX_MODULE_ENABLED
#define DAL_PCD_MODULE_ENABLED
//#define DAL_HCD_MODULE_ENABLED

/* Value of the external high speed oscillator in Hz */
#if !defined  (HSE_VALUE)
  #define HSE_VALUE              8000000U
#endif /* HSE_VALUE */

/* Timeout for external high speed oscillator in ms */
#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    100U
#endif /* HSE_STARTUP_TIMEOUT */

/* Value of the internal high speed oscillator in Hz */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE              8000000U
#endif /* HSI_VALUE */

/* Value of the internal low speed oscillator in Hz */
#if !defined  (LSI_VALUE)
 #define LSI_VALUE               35000U
#endif /* LSI_VALUE */

/* Value of the external low speed oscillator in Hz */
#if !defined  (LSE_VALUE)
 #define LSE_VALUE               32768U
#endif /* LSE_VALUE */

/* Timeout for external low speed oscillator in ms */
#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    5000U
#endif /* LSE_STARTUP_TIMEOUT */

/* System Configuration */
#define  VDD_VALUE                    3300U /*!< Value of VDD in mv */
#define  TICK_INT_PRIORITY            0x0FU /*!< tick interrupt priority */
#define  USE_RTOS                     0U
#define  PREFETCH_ENABLE              1U
#define  INSTRUCTION_CACHE_ENABLE     1U
#define  DATA_CACHE_ENABLE            1U

/* DAL peripheral register callbacks */
#define  USE_DAL_ADC_REGISTER_CALLBACKS         0U /* ADC register callback disabled       */
#define  USE_DAL_CAN_REGISTER_CALLBACKS         0U /* CAN register callback disabled       */
#define  USE_DAL_HCD_REGISTER_CALLBACKS         0U /* HCD register callback disabled       */
#define  USE_DAL_I2C_REGISTER_CALLBACKS         0U /* I2C register callback disabled       */
#define  USE_DAL_I2S_REGISTER_CALLBACKS         0U /* I2S register callback disabled       */
#define  USE_DAL_IRDA_REGISTER_CALLBACKS        0U /* IRDA register callback disabled      */
#define  USE_DAL_PCD_REGISTER_CALLBACKS         0U /* PCD register callback disabled       */
#define  USE_DAL_RTC_REGISTER_CALLBACKS         0U /* RTC register callback disabled       */
#define  USE_DAL_SMARTCARD_REGISTER_CALLBACKS   0U /* SMARTCARD register callback disabled */
#define  USE_DAL_SMBUS_REGISTER_CALLBACKS       0U /* SMBUS register callback disabled     */
#define  USE_DAL_SPI_REGISTER_CALLBACKS         0U /* SPI register callback disabled       */
#define  USE_DAL_TMR_REGISTER_CALLBACKS         0U /* TMR register callback disabled       */
#define  USE_DAL_UART_REGISTER_CALLBACKS        0U /* UART register callback disabled      */
#define  USE_DAL_USART_REGISTER_CALLBACKS       0U /* USART register callback disabled     */
#define  USE_DAL_WWDT_REGISTER_CALLBACKS        0U /* WWDT register callback disabled      */

/* SPI peripheral configuration */

/* SPI CRC FEATURE */
#define USE_SPI_CRC                     1U

/* Include module's header file */
#ifdef DAL_RCM_MODULE_ENABLED
  #include "apm32f4xx_dal_rcm.h"
#endif /* DAL_RCM_MODULE_ENABLED */

#ifdef DAL_GPIO_MODULE_ENABLED
  #include "apm32f4xx_dal_gpio.h"
#endif /* DAL_GPIO_MODULE_ENABLED */

#ifdef DAL_EINT_MODULE_ENABLED
  #include "apm32f4xx_dal_eint.h"
#endif /* DAL_EINT_MODULE_ENABLED */

#ifdef DAL_DMA_MODULE_ENABLED
  #include "apm32f4xx_dal_dma.h"
#endif /* DAL_DMA_MODULE_ENABLED */

#ifdef DAL_CORTEX_MODULE_ENABLED
  #include "apm32f4xx_dal_cortex.h"
#endif /* DAL_CORTEX_MODULE_ENABLED */

#ifdef DAL_ADC_MODULE_ENABLED
  #include "apm32f4xx_dal_adc.h"
#endif /* DAL_ADC_MODULE_ENABLED */

#ifdef DAL_CAN_MODULE_ENABLED
  #include "apm32f4xx_dal_can.h"
#endif /* DAL_CAN_MODULE_ENABLED */

#ifdef DAL_CRC_MODULE_ENABLED
  #include "apm32f4xx_dal_crc.h"
#endif /* DAL_CRC_MODULE_ENABLED */

#ifdef DAL_FLASH_MODULE_ENABLED
  #include "apm32f4xx_dal_flash.h"
#endif /* DAL_FLASH_MODULE_ENABLED */

#ifdef DAL_HCD_MODULE_ENABLED
 #include "apm32f4xx_dal_hcd.h"
#endif /* DAL_HCD_MODULE_ENABLED */

#ifdef DAL_I2C_MODULE_ENABLED
 #include "apm32f4xx_dal_i2c.h"
#endif /* DAL_I2C_MODULE_ENABLED */

#ifdef DAL_I2S_MODULE_ENABLED
 #include "apm32f4xx_dal_i2s.h"
#endif /* DAL_I2S_MODULE_ENABLED */

#ifdef DAL_IRDA_MODULE_ENABLED
 #include "apm32f4xx_dal_irda.h"
#endif /* DAL_IRDA_MODULE_ENABLED */

#ifdef DAL_PCD_MODULE_ENABLED
 #include "apm32f4xx_dal_pcd.h"
#endif /* DAL_PCD_MODULE_ENABLED */

#ifdef DAL_PMU_MODULE_ENABLED
 #include "apm32f4xx_dal_pmu.h"
#endif /* DAL_PMU_MODULE_ENABLED */

#ifdef DAL_RTC_MODULE_ENABLED
 #include "apm32f4xx_dal_rtc.h"
#endif /* DAL_RTC_MODULE_ENABLED */

#ifdef DAL_SMBUS_MODULE_ENABLED
 #include "apm32f4xx_dal_smbus.h"
#endif /* DAL_SMBUS_MODULE_ENABLED */

#ifdef DAL_SPI_MODULE_ENABLED
 #include "apm32f4xx_dal_spi.h"
#endif /* DAL_SPI_MODULE_ENABLED */

#ifdef DAL_SMARTCARD_MODULE_ENABLED
 #include "apm32f4xx_dal_smartcard.h"
#endif /* DAL_SMARTCARD_MODULE_ENABLED */

#ifdef DAL_TMR_MODULE_ENABLED
 #include "apm32f4xx_dal_tmr.h"
#endif /* DAL_TMR_MODULE_ENABLED */

#ifdef DAL_UART_MODULE_ENABLED
 #include "apm32f4xx_dal_uart.h"
#endif /* DAL_UART_MODULE_ENABLED */

#ifdef DAL_USART_MODULE_ENABLED
 #include "apm32f4xx_dal_usart.h"
#endif /* DAL_USART_MODULE_ENABLED */

#ifdef DAL_IWDT_MODULE_ENABLED
 #include "apm32f4xx_dal_iwdt.h"
#endif /* DAL_IWDT_MODULE_ENABLED */

#ifdef DAL_WWDT_MODULE_ENABLED
 #include "apm32f4xx_dal_wwdt.h"
#endif /* DAL_WWDT_MODULE_ENABLED */

/* Assert Component */
#if (USE_FULL_ASSERT == 1U)
    #define ASSERT_PARAM(_PARAM_)                         ((_PARAM_) ? (void)0U : AssertFailedHandler((uint8_t *)__FILE__, __LINE__))
    /* Declaration */
    void AssertFailedHandler(uint8_t *file, uint32_t line);
#else
    #define ASSERT_PARAM(_PARAM_)                         ((void)0U)
#endif /* USE_FULL_ASSERT */

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* APM32F4xx_DAL_CFG_H */
