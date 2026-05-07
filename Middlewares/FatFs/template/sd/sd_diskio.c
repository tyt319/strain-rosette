/**
 * @file        sd_diskio.c
 *
 * @brief       SD Disk I/O driver
 *
 * @attention
 *
 *  Copyright (C) 2024 Geehy Semiconductor
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

#include "ffconf.h"

#ifdef FF_SD_DISK_ENABLE

/* Includes ***************************************************************/
#include "sd_diskio.h"

/* Private includes *******************************************************/
#include "apm32f4xx_device_cfg.h"
#include "board_sd.h"

/* Private macro **********************************************************/
#define SD_TIMEOUT          3000U
#define SD_BLOCK_SIZE       512U

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Get Drive Status
 * 
 * @param  lun: Logical unit number
 * 
 * @retval DSTATUS: Drive status
 */
DSTATUS SD_GetDiskStatus(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    if (BOARD_SD_ReadCardState() == SD_OK)
    {
        stat &= ~STA_NOINIT;
    }

    return stat;
}

/**
 * @brief  Initialize Disk Drive
 * 
 * @param  lun: Logical unit number
 * 
 * @retval DSTATUS: Drive status
 */
DSTATUS SD_DiskInitialize(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    if (BOARD_SD_Init() == SD_OK)
    {
        if (BOARD_SD_ReadCardState() == SD_OK)
        {
            stat &= ~STA_NOINIT;
        }
    }

    return stat;
}

/**
 * @brief  Read Sector(s)
 * 
 * @param  lun: Logical unit number
 * 
 * @param  buff: Data buffer to store read data
 * 
 * @param  sector: Sector number to read
 * 
 * @param  count: Number of sectors to read
 * 
 * @retval DRESULT: Operation result
 */
DRESULT SD_DiskRead(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_PARERR;

    if (BOARD_SD_ReadBlocks((uint32_t*)buff, (uint32_t) (sector), count, SD_TIMEOUT) == SD_OK)
    {
        while (BOARD_SD_ReadCardState() != SD_OK)
        {

        }
        res = RES_OK;
    }

    return res;
}

/**
 * @brief  Write Sector(s)
 * 
 * @param  lun: Logical unit number
 * 
 * @param  buff: Data buffer to write
 * 
 * @param  sector: Sector number to write
 * 
 * @param  count: Number of sectors to write
 * 
 * @retval DRESULT: Operation result
 */
DRESULT SD_DiskWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_PARERR;

    if (BOARD_SD_WriteBlocks((uint32_t*)buff, (uint32_t)(sector), count, SD_TIMEOUT) == SD_OK)
    {
        while (BOARD_SD_ReadCardState() != SD_OK)
        {

        }
        res = RES_OK;
    }

    return res;
}

/**
 * @brief  I/O Control
 * 
 * @param  lun: Logical unit number
 * 
 * @param  cmd: Control command code
 * 
 * @param  buff: Data buffer
 * 
 * @retval DRESULT: Operation result
 */
DRESULT SD_DiskIoctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;
    DAL_SD_CardInfoTypeDef cardInfo;

    switch (cmd)
    {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            BOARD_SD_ReadCardInfo(&cardInfo);
            *(DWORD*)buff = cardInfo.LogBlockNbr;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            BOARD_SD_ReadCardInfo(&cardInfo);
            *(WORD*)buff = cardInfo.LogBlockSize;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            BOARD_SD_ReadCardInfo(&cardInfo);
            *(DWORD*)buff = cardInfo.LogBlockSize / SD_BLOCK_SIZE;
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
            break;
    }

    return res;
}

#endif /* FF_SD_DISK_ENABLE */
