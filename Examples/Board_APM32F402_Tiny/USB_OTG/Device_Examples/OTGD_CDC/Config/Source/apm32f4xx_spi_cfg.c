/**
 * @file        apm32f4xx_spi_cfg.c
 *
 * @brief       This file provides configuration support for SPI
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

/* Includes ***************************************************************/
#include "apm32f4xx_spi_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

/* External functions *****************************************************/

/**
 * @brief   SPI1 configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_SPI1_Config(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT   ;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 0;

    if (DAL_SPI_Init(&hspi1) != DAL_OK)
    {
        Error_Handler();
    }
    DAL_SPI_DMAResume(&hspi1);
}

/**
 * @brief   SPI2 configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_SPI2_Config(void)
{
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_SLAVE;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 0;

    if (DAL_SPI_Init(&hspi2) != DAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief   SPI MSP Init
 *
 * @param  hspi   Pointer to a SPI_HandleTypeDef structure that contains
 *                the configuration information for the specified SPI module
 *
 * @retval  None
 */
void DAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        /* Enable SPI1 clock */
        __DAL_RCM_SPI1_CLK_ENABLE();

        /* Enable SPI1 GPIO clock */
        __DAL_RCM_GPIOA_CLK_ENABLE();

        /* Configure SPI1 SCK MISO and MOSI pin */
        GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

        DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }

    // if (hspi->Instance == SPI2)
    // {
    //     /* Enable SPI2 clock */
    //     __DAL_RCM_SPI2_CLK_ENABLE();

    //     /* Enable SPI2 GPIO clock */
    //     __DAL_RCM_GPIOB_CLK_ENABLE();

    //     /* Configure SPI2 NSS SCK MISO and MOSI pin */
    //     GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    //     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    //     GPIO_InitStruct.Pull = GPIO_NOPULL;
    //     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    //     DAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // }
}

/**
 * @brief   SPI MSP DeInit
 *
 * @param  hspi   Pointer to a SPI_HandleTypeDef structure that contains
 *                the configuration information for the specified SPI module
 * @retval  None
 */
void DAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hspi->Instance == SPI1)
    {
        /* Enable SPI1 clock */
        __DAL_RCM_SPI1_CLK_ENABLE();
        /* Enable SPI1 GPIO clock */
        __DAL_RCM_GPIOA_CLK_ENABLE();

        /* Configure SPI1 pins */
        GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* === 新增：使能 SPI1 全局中断 === */
        DAL_NVIC_SetPriority(SPI1_IRQn, 3, 0);   // 优先级可自定义，此处设为 3
        DAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
    else if (hspi->Instance == SPI2)
    {
        /* Reset SPI2 */
        __DAL_RCM_SPI2_FORCE_RESET();
        __DAL_RCM_SPI2_RELEASE_RESET();

        /* DeInit SPI2 GPIO */
        DAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    }
}
