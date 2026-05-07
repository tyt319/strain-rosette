/**
 * @file        board_apm32f402_tiny.c
 *
 * @brief       This file provides firmware functions to manage Leds and key buttons
 *
 * @version     V1.0.0
 *
 * @date        2024-12-27
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

/* Includes ***************************************************************/
#include "board_apm32f402_tiny.h"

#if defined(BOARD_APM32F402_TINY)

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define LED_NUM                     2U
#define BUTTON_NUM                  2U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
GPIO_TypeDef* LED_PORT[LED_NUM] = {LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t LED_PIN[LED_NUM] = {LED2_PIN, LED3_PIN};

GPIO_TypeDef* BUTTON_PORT[BUTTON_NUM] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT};
const uint16_t BUTTON_PIN[BUTTON_NUM] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN};
const IRQn_Type BUTTON_IRQn[BUTTON_NUM] = {KEY1_BUTTON_EINT_IRQ_NUM, KEY2_BUTTON_EINT_IRQ_NUM};

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Board LED configuration
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  BSP status
 */
DAL_StatusTypeDef BOARD_LED_Config(BOARD_LED_T led)
{
    DAL_StatusTypeDef status = DAL_OK;

    GPIO_InitTypeDef  GPIO_InitStruct = {0U};

    switch(led)
    {
        case LED2:
            LED2_GPIO_CLK_ENABLE();
            break;

        case LED3:
            LED3_GPIO_CLK_ENABLE();
            break;
    }

    /* Configure the LED pin */
    GPIO_InitStruct.Pin = LED_PIN[led];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    DAL_GPIO_Init(LED_PORT[led], &GPIO_InitStruct);
    
    DAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_SET);

    return status;
}

/**
 * @brief   Board button configuration
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *              @arg BUTTON_KEY2: Key2 Push Button
 *
 * @param       Button_Mode: Specifies Button mode.
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_MODE_GPIO: Button will be used as simple IO
 *              @arg BUTTON_MODE_EINT: Button will be connected to EINT line
 *                   with interrupt generation capability
 *
 * @retval  BSP status
 */
DAL_StatusTypeDef BOARD_BUTTON_Config(BOARD_BUTTON_T button, BOARD_BUTTON_MODE_T mode)
{
    DAL_StatusTypeDef status = DAL_OK;

    GPIO_InitTypeDef  GPIO_InitStruct = {0U};
    
    switch(button)
    {
        case BUTTON_KEY1:
            KEY1_BUTTON_GPIO_CLK_ENABLE();
            break;

        case BUTTON_KEY2:
            KEY2_BUTTON_GPIO_CLK_ENABLE();
            break;
    }
    
    if (mode == BUTTON_MODE_GPIO)
    {
        /* Configure the BUTTON pin */
        GPIO_InitStruct.Pin = BUTTON_PIN[button];
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

        DAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
    }
    
    if (mode == BUTTON_MODE_EINT)
    {
        /* Configure the BUTTON pin */
        GPIO_InitStruct.Pin = BUTTON_PIN[button];
        GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

        DAL_GPIO_Init(BUTTON_PORT[button], &GPIO_InitStruct);
        
        /* Enable and set Button EINT Interrupt to the lowest priority */
        DAL_NVIC_SetPriority((IRQn_Type)(BUTTON_IRQn[button]), 0x0FU, 0U);
        DAL_NVIC_EnableIRQ((IRQn_Type)(BUTTON_IRQn[button]));
    }
    
    return status;
}

/**
 * @brief   Turn on the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_On(BOARD_LED_T led)
{
    DAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_RESET);
}

/**
 * @brief   Turn off the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_Off(BOARD_LED_T led)
{
    DAL_GPIO_WritePin(LED_PORT[led], LED_PIN[led], GPIO_PIN_SET);
}

/**
 * @brief   Toggles the selected LED
 *
 * @param   led: Specifies the Led to be configured
 *              This parameter can be one of following parameters:
 *              @arg LED2
 *              @arg LED3
 *
 * @retval  None
 */
void BOARD_LED_Toggle(BOARD_LED_T led)
{
    DAL_GPIO_TogglePin(LED_PORT[led], LED_PIN[led]);
}

/**
 * @brief   Returns the selected button state
 *
 * @param   button: Specifies the button to be configured
 *              This parameter can be one of following parameters:
 *              @arg BUTTON_KEY1: Key1 Push Button
 *              @arg BUTTON_KEY2: Key2 Push Button
 *
 * @retval  The button GPIO pin value.
 */
GPIO_PinState BOARD_BUTTON_GetState(BOARD_BUTTON_T button)
{
    return DAL_GPIO_ReadPin(BUTTON_PORT[button], BUTTON_PIN[button]);
}

#endif /* BOARD_APM32F402_TINY */
