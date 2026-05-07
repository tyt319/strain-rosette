/**
 * @file        board_apm32f407_eval.h
 *
 * @brief       This file contains definitions for APM32F407_EVAL's Leds, push-buttons hardware resources
 *
 * @version     V2.0.0
 *
 * @date        2023-07-31
 *
 * @attention
 *
 *  Copyright (C) 2023 Geehy Semiconductor
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
#ifndef BOARD_APM32F407_EVAL_H
#define BOARD_APM32F407_EVAL_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"

/* Exported macro *********************************************************/

/**
 * @brief LED1
 */
#define LED1_PIN                        GPIO_PIN_0
#define LED1_GPIO_PORT                  GPIOC
#define LED1_GPIO_CLK_ENABLE()          __DAL_RCM_GPIOC_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()         __DAL_RCM_GPIOC_CLK_DISABLE()

/**
 * @brief LED2
 */
#define LED2_PIN                        GPIO_PIN_2
#define LED2_GPIO_PORT                  GPIOC
#define LED2_GPIO_CLK_ENABLE()          __DAL_RCM_GPIOC_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()         __DAL_RCM_GPIOC_CLK_DISABLE()

/**
 * @brief LED3
 */
#define LED3_PIN                        GPIO_PIN_3
#define LED3_GPIO_PORT                  GPIOC
#define LED3_GPIO_CLK_ENABLE()          __DAL_RCM_GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()         __DAL_RCM_GPIOC_CLK_DISABLE()

/**
 * @brief Key1 push-button
 */
#define KEY1_BUTTON_PIN                 GPIO_PIN_0
#define KEY1_BUTTON_GPIO_PORT           GPIOB
#define KEY1_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOB_CLK_ENABLE()
#define KEY1_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOB_CLK_DISABLE()
#define KEY1_BUTTON_EINT_IRQ_NUM        EINT0_IRQn

/**
 * @brief Key2 push-button
 */
#define KEY2_BUTTON_PIN                 GPIO_PIN_1
#define KEY2_BUTTON_GPIO_PORT           GPIOB
#define KEY2_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOB_CLK_ENABLE()
#define KEY2_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOB_CLK_DISABLE()
#define KEY2_BUTTON_EINT_IRQ_NUM        EINT1_IRQn

/**
 * @brief Key3 push-button
 */
#define KEY3_BUTTON_PIN                 GPIO_PIN_12
#define KEY3_BUTTON_GPIO_PORT           GPIOF
#define KEY3_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOF_CLK_ENABLE()
#define KEY3_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOF_CLK_DISABLE()
#define KEY3_BUTTON_EINT_IRQ_NUM        EINT15_10_IRQn

/**
 * @brief Key4 push-button
 */
#define KEY4_BUTTON_PIN                 GPIO_PIN_5
#define KEY4_BUTTON_GPIO_PORT           GPIOA
#define KEY4_BUTTON_GPIO_CLK_ENABLE()   __DAL_RCM_GPIOA_CLK_ENABLE()
#define KEY4_BUTTON_GPIO_CLK_DISABLE()  __DAL_RCM_GPIOA_CLK_DISABLE()
#define KEY4_BUTTON_EINT_IRQ_NUM        EINT9_5_IRQn

/* Exported typedef *******************************************************/

/**
 * @brief LED type
 */
typedef enum
{
    LED1 = 0U,
    LED2 = 1U,
    LED3 = 2U,
} BOARD_LED_T;

/**
 * @brief Button type
 */
typedef enum
{
    BUTTON_KEY1 = 0U,
    BUTTON_KEY2 = 1U,
    BUTTON_KEY3 = 2U,
    BUTTON_KEY4 = 3U,
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

#endif /* BOARD_APM32F407_EVAL_H */
