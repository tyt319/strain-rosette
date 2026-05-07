/**
 * @file        apm32f4xx_usb_cfg.c
 *
 * @brief       This file provides configuration support for USB
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

/* Includes ***************************************************************/
#include "apm32f4xx_usb_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Initializes the PCD MSP
 *
 * @param  hpcd PCD handle
 *
 * @retval None
 */
void DAL_PCD_MspInit(PCD_HandleTypeDef *hpcd)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(hpcd->Instance == USB_OTG_FS)
    {
        /* Configure USB OTG GPIO */
        __DAL_RCM_GPIOA_CLK_ENABLE();

        /* USB DM, DP pin configuration */
        GPIO_InitStruct.Pin         = GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode        = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull        = GPIO_NOPULL;
        GPIO_InitStruct.Speed       = GPIO_SPEED_FREQ_HIGH;
        DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure USB OTG */
        __DAL_RCM_USB_OTG_FS_CLK_ENABLE();

        /* Configure interrupt */
        DAL_NVIC_SetPriority(OTG_FS_IRQn, 1U, 0U);
        DAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    }
}

/**
 * @brief  DeInitializes PCD MSP
 *
 * @param  hpcd PCD handle
 *
 * @retval None
 */
void DAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd)
{
    if(hpcd->Instance == USB_OTG_FS)
    {
        /* Disable peripheral clock */
        __DAL_RCM_USB_OTG_FS_CLK_DISABLE();

        /* USB DM, DP pin configuration */
        DAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

        /* Disable peripheral interrupt */
        DAL_NVIC_DisableIRQ(OTG_FS_IRQn);
    }
}
