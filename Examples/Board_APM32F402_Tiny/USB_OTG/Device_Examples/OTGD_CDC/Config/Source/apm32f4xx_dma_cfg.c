/**
 * @file        apm32f4xx_dma_cfg.c
 *
 * @brief       This file provides configuration support for DMA
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
#include "apm32f4xx_dma_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

/* Private function prototypes ********************************************/
/* 给 SPI 绑定 DMA 的内部接口 */
void SPI1_DMALink(DMA_HandleTypeDef *rx, DMA_HandleTypeDef *tx);
/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   DMA configuration
 *
 * @param   None
 *
 * @retval  None
 */

void DAL_DMA_Config(void)
{
    __DAL_RCM_DMA1_CLK_ENABLE();

    // ===================== SPI1_RX = DMA1_Channel2 =====================
    hdma_spi1_rx.Instance = DMA1_Channel2;
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc   = DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_rx.Init.MemDataAlignment   = DMA_MDATAALIGN_BYTE;
    hdma_spi1_rx.Init.Mode    = DMA_NORMAL;
    hdma_spi1_rx.Init.Priority= DMA_PRIORITY_VERY_HIGH;

    if (DAL_DMA_Init(&hdma_spi1_rx) != DAL_OK)
        Error_Handler();

    // ===================== SPI1_TX = DMA1_Channel3 =====================
    hdma_spi1_tx.Instance = DMA1_Channel3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc   = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment   = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode    = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority= DMA_PRIORITY_HIGH;

    if (DAL_DMA_Init(&hdma_spi1_tx) != DAL_OK)
        Error_Handler();

    /* 绑定 DMA 到 SPI1 */
    SPI1_DMALink(&hdma_spi1_rx, &hdma_spi1_tx);
    DAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    DAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);
    DAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 2, 0);
    DAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 2, 1);
}

/* 供 SPI 调用，实现 __DAL_LINKDMA */
void SPI1_DMALink(DMA_HandleTypeDef *rx, DMA_HandleTypeDef *tx)
{
    extern SPI_HandleTypeDef hspi1;
    __DAL_LINKDMA(&hspi1, hdmarx, *rx);
    __DAL_LINKDMA(&hspi1, hdmatx, *tx);
}
// DMA1 通道2中断 (SPI1_RX)
void DMA1_Channel2_IRQHandler(void)
{
    DAL_DMA_IRQHandler(&hdma_spi1_rx);
}

// DMA1 通道3中断 (SPI1_TX)
void DMA1_Channel3_IRQHandler(void)
{
    DAL_DMA_IRQHandler(&hdma_spi1_tx);
}
