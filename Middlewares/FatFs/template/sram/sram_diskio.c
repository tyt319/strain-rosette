/**
 * @file        sram_diskio.c
 *
 * @brief       SRAM Disk I/O driver
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

#ifdef FF_SRAM_DISK_ENABLE

/* Includes ***************************************************************/
#include "sram_diskio.h"

/* Private includes *******************************************************/
#include <string.h>
#include "apm32f4xx_device_cfg.h"
#include "board_sram.h"

/* Private macro **********************************************************/
#define SRAM_BLOCK_SIZE     512U

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
DSTATUS SRAM_GetDiskStatus(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    return stat;
}

/**
 * @brief  Initialize Disk Drive
 * 
 * @param  lun: Logical unit number
 * 
 * @retval DSTATUS: Drive status
 */
DSTATUS SRAM_DiskInitialize(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    BOARD_SRAM_Init();

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
DRESULT SRAM_DiskRead(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    memcpy(buff, (uint8_t *)(SRAM_DEVICE_ADDR + (sector * SRAM_BLOCK_SIZE)), SRAM_BLOCK_SIZE * count);

    return RES_OK;
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
DRESULT SRAM_DiskWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    memcpy((uint8_t *)(SRAM_DEVICE_ADDR + (sector * SRAM_BLOCK_SIZE)), buff, SRAM_BLOCK_SIZE * count);

    return RES_OK;
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
DRESULT SRAM_DiskIoctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;

    switch (cmd)
    {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            *(DWORD*)buff = SRAM_DEVICE_SIZE / SRAM_BLOCK_SIZE;
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(DWORD*)buff = SRAM_BLOCK_SIZE;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
            break;
    }

    return res;
}

#endif /* FF_SRAM_DISK_ENABLE */
