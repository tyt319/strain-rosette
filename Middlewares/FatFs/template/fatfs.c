/**
 * @file        fatfs.c
 *
 * @brief       FatFs file system application
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

/* Includes ***************************************************************/
#include "fatfs.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Get Drive Path
 * 
 * @param  driveNum: Drive number
 *         This parameter can be one of the following values:
 *          @arg USBH_DISK
 *          @arg SD_DISK
 *          @arg SRAM_DISK
 *          @arg SDRAM_DISK
 *          @arg USER_DISK
 * 
 * @retval char*: Drive path
 */
char* FATFS_GetDrivePath(BYTE driveNum)
{
    static char path[3];

    if (driveNum < FF_VOLUMES)
    {
        path[0] = '0' + driveNum;
        path[1] = ':';
        path[2] = '\0';
    }
    else
    {
        path[0] = '\0';
    }

    return path;
}

/**
 * @brief  Get time from RTC
 * 
 * @retval DWORD: Time
 */
DWORD get_fattime(void)
{
    return 0;
}
