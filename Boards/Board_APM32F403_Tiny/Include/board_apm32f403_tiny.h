/**
 * @file        board_apm32f403_tiny.h
 *
 * @brief       This file contains definitions for APM32F403_TINY's Leds, push-buttons hardware resources
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
#ifndef BOARD_APM32F403_TINY_H
#define BOARD_APM32F403_TINY_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"

/* Exported macro *********************************************************/

/**
 * @brief LED2
 */
#define LED2_PIN                        GPIO_PIN_6
#define LED2_GPIO_PORT                  GPIOB
#define LED2_GPIO_CLK_ENABLE()          __DAL_RCM_GPIOB_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()         __DAL_RCM_GPIOB_CLK_DISABLE()

/**
 * @brief LED3
 */
#define LED3_PIN                        GPIO_PIN_7
#define LED3_GPIO_PORT                  GPIOB
#define LED3_GPIO_CLK_ENABLE()          __DAL_RCM_GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()         __DAL_RCM_GPIOB_CLK_DISABLE()

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                 GPIO_PIN_1
#define KEY1_BUTTON_GPIO_PORT           GPIOA
#define KEY1_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOA_CLK_ENABLE()
#define KEY1_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOA_CLK_DISABLE()
#define KEY1_BUTTON_EINT_IRQ_NUM        EINT1_IRQn

/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                 GPIO_PIN_0
#define KEY2_BUTTON_GPIO_PORT           GPIOA
#define KEY2_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOA_CLK_ENABLE()
#define KEY2_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOA_CLK_DISABLE()
#define KEY2_BUTTON_EINT_IRQ_NUM        EINT0_IRQn

/* Exported typedef *******************************************************/

/**
 * @brief LED type
 */
typedef enum
{
    LED2 = 0U,
    LED3 = 1U,
} BOARD_LED_T;

/**
 * @brief Button type
 */
typedef enum
{
    BUTTON_KEY1 = 0U,
    BUTTON_KEY2 = 1U,
} BOARD_BUTTON_T;

/**
 * @brief Button mode
 */
typedef enum
{
    BUTTON_MODE_GPIO = 0U,
    BUTTON_MODE_EINT = 1U
} BOARD_BUTTON_MODE_T;

/* Exported function prototypes *******************************************/

/* Peripheral initialization functions */
DAL_StatusTypeDef BOARD_LED_Config(BOARD_LED_T led);
DAL_StatusTypeDef BOARD_BUTTON_Config(BOARD_BUTTON_T button, BOARD_BUTTON_MODE_T mode);

/* Peripheral control functions */
void BOARD_LED_On(BOARD_LED_T led);
void BOARD_LED_Off(BOARD_LED_T led);
void BOARD_LED_Toggle(BOARD_LED_T led);
GPIO_PinState BOARD_BUTTON_GetState(BOARD_BUTTON_T button);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_APM32F403_TINY_H */
