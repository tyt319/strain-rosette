/**
 * @file        board_lan8720.c
 *
 * @brief       This file provides firmware functions to manage LAN8720
 *
 * @version     V1.0.0
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

/* Includes ***************************************************************/
#include "board_lan8720.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define LAN8720_MAX_DEV_ADDR        ((uint32_t)31U)

/* Private typedef ********************************************************/

/* Private variables ******************************************************/
LAN8720_INFO_T lan8720Info;

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Initializes LAN8720
 *
 * @param   None
 *
 * @retval  LAN8720 status
 */
uint32_t LAN8720_Init(void)
{
    uint32_t tickCnt;
    uint32_t addr;
    uint32_t regVal;

    if (lan8720Info.status == LAN8720_STATUS_INIT)
    {
        if (LAN8720_PHY_InitCallback() == 0U)
        {
            lan8720Info.status = LAN8720_STATUS_OK;
        }

        lan8720Info.devAddr = LAN8720_MAX_DEV_ADDR + 1U;
        
        for (addr = 0U; addr <= LAN8720_MAX_DEV_ADDR; addr++)
        {
            if (LAN8720_PHY_ReadRegCallback(addr, LAN8720_REG_SMR, &regVal) < 0)
            {
                lan8720Info.status = LAN8720_STATUS_ERROR;
                continue;
            }
            
            if ((regVal & 0x1FU) == addr)
            {
                lan8720Info.devAddr = addr;
                lan8720Info.status = LAN8720_STATUS_OK;
                break;
            }
        }
        
        if (lan8720Info.devAddr > LAN8720_MAX_DEV_ADDR)
        {
            lan8720Info.status = LAN8720_STATUS_ERROR;
        }
        
        if (lan8720Info.status == LAN8720_STATUS_OK)
        {
            /* Reset LAN8720 */
            if (LAN8720_PHY_WriteRegCallback(lan8720Info.devAddr, \
                                             LAN8720_REG_BCR, \
                                             LAN8720_REG_BCR_SOFT_RESET) >= 0)
            {
                if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                                LAN8720_REG_BCR, \
                                                &regVal) >= 0)
                {
                    tickCnt = LAN8720_PHY_GetTickCallback();
                    
                    while (regVal & LAN8720_REG_BCR_SOFT_RESET)
                    {
                        if ((LAN8720_PHY_GetTickCallback() - tickCnt) <= LAN8720_SOFTWARE_RST)
                        {
                            if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                                            LAN8720_REG_BCR, \
                                                            &regVal) < 0)
                            {
                                /* PHY error */
                                lan8720Info.status = LAN8720_STATUS_ERROR;
                            }
                        }
                        else
                        {
                            /* PHY timeout */
                            lan8720Info.status = LAN8720_STATUS_TIMEOUT;
                        }
                    }
                }
                else
                {
                    /* PHY error */
                    lan8720Info.status = LAN8720_STATUS_ERROR;
                }
            }
            else
            {
                /* PHY error */
                lan8720Info.status = LAN8720_STATUS_ERROR;
            }
        }
    }

    /* Enable autonego */
    LAN8720_PHY_WriteRegCallback(lan8720Info.devAddr, LAN8720_REG_BCR, LAN8720_REG_BCR_AUTONEGO_EN);

    if (lan8720Info.status == LAN8720_STATUS_OK)
    {
        tickCnt = LAN8720_PHY_GetTickCallback();
        
        while ((LAN8720_PHY_GetTickCallback() - tickCnt) <= LAN8720_INIT_TIMEOUT)
        {
            
        }

        /* PHY ready */
        lan8720Info.status = LAN8720_STATUS_READY;
    }

    return lan8720Info.status;
}

/**
 * @brief   De initializes LAN8720
 *
 * @param   None
 *
 * @retval  LAN8720 status
 */
uint32_t LAN8720_DeInit(void)
{
    if (lan8720Info.status == LAN8720_STATUS_READY)
    {
        if (LAN8720_PHY_UninitCallback() < 0)
        {
            return LAN8720_STATUS_ERROR;
        }

        lan8720Info.status = LAN8720_STATUS_INIT;
    }

    return LAN8720_STATUS_OK;
}

/**
 * @brief   Get link status from LAN8720
 *
 * @param   None
 *
 * @retval  Link status
 */
uint32_t LAN8720_GetLinkState(void)
{
    uint32_t regVal;
    
    /* Check link status */
    if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                    LAN8720_REG_BSR, \
                                    &regVal) < 0)
    {
        return LAN8720_STATUS_READ_ERR;
    }

    if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                    LAN8720_REG_BSR, \
                                    &regVal) < 0)
    {
        return LAN8720_STATUS_READ_ERR;
    }
    
    if ((regVal & LAN8720_REG_BSR_LINK_STATUS) == 0U)
    {
        return LAN8720_STATUS_LINK_DOWN;
    }

    /* Check auto negotiaition status */
    if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                    LAN8720_REG_BCR, \
                                    &regVal) < 0)
    {
        return LAN8720_STATUS_READ_ERR;
    }

    if ((regVal & LAN8720_REG_BCR_AUTONEGO_EN) != LAN8720_REG_BCR_AUTONEGO_EN)
    {
        if (((regVal & LAN8720_REG_BCR_SPEED_SELECT) == LAN8720_REG_BCR_SPEED_SELECT) && \
           ((regVal & LAN8720_REG_BCR_DUPLEX_MODE) == LAN8720_REG_BCR_DUPLEX_MODE))
        {
            return LAN8720_STATUS_FULLDUPLEX_100MBITS;
        }
        else if ((regVal & LAN8720_REG_BCR_SPEED_SELECT) == LAN8720_REG_BCR_SPEED_SELECT)
        {
            return LAN8720_STATUS_HALFDUPLEX_100MBITS;
        }
        else if ((regVal & LAN8720_REG_BCR_DUPLEX_MODE) == LAN8720_REG_BCR_DUPLEX_MODE)
        {
            return LAN8720_STATUS_FULLDUPLEX_10MBITS;
        }
        else
        {
            return LAN8720_STATUS_HALFDUPLEX_10MBITS;
        }
    }
    /* Auto nego enabled */
    else
    {
        if (LAN8720_PHY_ReadRegCallback(lan8720Info.devAddr, \
                                        LAN8720_REG_PHYSCSR, \
                                        &regVal) < 0)
        {
            return LAN8720_STATUS_READ_ERR;
        }

        /* Check auto negotiaition done status */
        if ((regVal & LAN8720_REG_PHYSCSR_AUTONEGO_DONE) != LAN8720_REG_PHYSCSR_AUTONEGO_DONE)
        {
            return LAN8720_STATUS_AUTONEGO_NOTDONE;
        }

        if ((regVal & LAN8720_REG_PHYSCSR_HCDSPEEDMASK) == LAN8720_REG_PHYSCSR_100BTX_FD)
        {
            return LAN8720_STATUS_FULLDUPLEX_100MBITS;
        }
        else if ((regVal & LAN8720_REG_PHYSCSR_HCDSPEEDMASK) == LAN8720_REG_PHYSCSR_100BTX_HD)
        {
            return LAN8720_STATUS_HALFDUPLEX_100MBITS;
        }
        else if ((regVal & LAN8720_REG_PHYSCSR_HCDSPEEDMASK) == LAN8720_REG_PHYSCSR_10BT_FD)
        {
            return LAN8720_STATUS_FULLDUPLEX_10MBITS;
        }
        else
        {
            return LAN8720_STATUS_HALFDUPLEX_10MBITS;
        }
    }
}

/**
 * @brief   Initializes PHY
 *
 * @param   None
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t LAN8720_PHY_InitCallback(void)
{
    return 0;
}

/**
 * @brief   De-initializes PHY
 *
 * @param   None
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t LAN8720_PHY_UninitCallback(void)
{
    return 0;
}

/**
 * @brief   Get base tick to handle PHY process
 *
 * @param   None
 *
 * @retval  Tick value
 */
__weak int32_t LAN8720_PHY_GetTickCallback(void)
{
    return 0;
}

/**
 * @brief   Read a PHY register
 *
 * @param   devAddr: PHY port address
 *
 * @param   regAddr: PHY register address
 *
 * @param   regVal: Pointer to the register value
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t LAN8720_PHY_ReadRegCallback(uint32_t devAddr, uint32_t regAddr, uint32_t *regVal)
{
    UNUSED(devAddr);
    UNUSED(regAddr);
    UNUSED(regVal);

    return 0;
}

/**
 * @brief   Write a value to a PHY register
 *
 * @param   devAddr: PHY port address
 *
 * @param   regAddr: PHY register address
 *
 * @param   regVal: Value to be written
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t LAN8720_PHY_WriteRegCallback(uint32_t devAddr, uint32_t regAddr, uint32_t regVal)
{
    UNUSED(devAddr);
    UNUSED(regAddr);
    UNUSED(regVal);

    return 0;
}
