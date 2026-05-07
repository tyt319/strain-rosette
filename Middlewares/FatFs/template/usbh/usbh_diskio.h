/**
 * @file        usbh_diskio.h
 *
 * @brief       Header for usbh_diskio.c file
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
#ifndef USBH_DISKIO_H
#define USBH_DISKIO_H

#ifdef __cplusplus
    extern "C" {
#endif

/* Includes ***************************************************************/
#include "ff.h"
#include "diskio.h"
#include "usbh_msc.h"

/* Exported macro *********************************************************/

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
DSTATUS USBH_GetDiskStatus(BYTE lun);
DSTATUS USBH_DiskInitialize(BYTE lun);
DRESULT USBH_DiskRead(BYTE lun, BYTE *buff, LBA_t sector, UINT count);
DRESULT USBH_DiskWrite(BYTE lun, const BYTE *buff, LBA_t sector, UINT count);
DRESULT USBH_DiskIoctl(BYTE lun, BYTE cmd, void *buff);

#ifdef __cplusplus
}
#endif

#endif /* USBH_DISKIO_H */
