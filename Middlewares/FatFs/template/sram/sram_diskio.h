/**
 * @file        sram_diskio.h
 *
 * @brief       Header for sram_diskio.c file
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

/* Define to prevent recursive inclusion */
#ifndef SRAM_DISKIO_H
#define SRAM_DISKIO_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ***************************************************************/
#include "ff.h"
#include "diskio.h"

/* Exported macro *********************************************************/

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
DSTATUS SRAM_GetDiskStatus(BYTE lun);
DSTATUS SRAM_DiskInitialize(BYTE lun);
DRESULT SRAM_DiskRead(BYTE lun, BYTE *buff, DWORD sector, UINT count);
DRESULT SRAM_DiskWrite(BYTE lun, const BYTE *buff, DWORD sector, UINT count);
DRESULT SRAM_DiskIoctl(BYTE lun, BYTE cmd, void *buff);

#ifdef __cplusplus
}
#endif

#endif /* SRAM_DISKIO_H */
