/**
 * @file        user_diskio.c
 *
 * @brief       User Disk I/O driver
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

#ifdef FF_USER_DISK_ENABLE

/* Includes ***************************************************************/
#include "user_diskio.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define USER_BLOCK_SIZE     512

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
DSTATUS USER_GetDiskStatus(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;
    /* Add User Code Begin GetDiskStatus */

    /* Add User Code End GetDiskStatus */
    return stat;
}

/**
 * @brief  Initialize Disk Drive
 * 
 * @param  lun: Logical unit number
 * 
 * @retval DSTATUS: Drive status
 */
DSTATUS USER_DiskInitialize(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;
    /* Add User Code Begin DiskInitialize */

    /* Add User Code End DiskInitialize */
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
DRESULT USER_DiskRead(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_PARERR;
    /* Add User Code Begin DiskRead */

    /* Add User Code End DiskRead */
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
DRESULT USER_DiskWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_PARERR;
    /* Add User Code Begin DiskWrite */

    /* Add User Code End DiskWrite */
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
DRESULT USER_DiskIoctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;
    /* Add User Code Begin DiskIoctl */

    /* Add User Code End DiskIoctl */
    return res;
}

#endif /* FF_USER_DISK_ENABLE */
