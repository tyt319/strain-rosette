/**
 * @file        usbh_diskio.c
 *
 * @brief       USB Host Disk I/O driver
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

#ifdef FF_USBH_DISK_ENABLE

/* Includes ***************************************************************/
#include "usbh_diskio.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/
#define USB_SECTOR_SIZE     512
#define USB_BLOCK_SIZE      512
#define USB_LUN_NUMBER      0

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern USBH_INFO_T gUsbHostFS;

/* External functions *****************************************************/

/**
 * @brief  Get Drive Status
 * 
 * @param  lun: Logical unit number
 * 
 * @retval DSTATUS: Drive status
 */
DSTATUS USBH_GetDiskStatus(BYTE lun)
{
    DSTATUS stat = STA_NOINIT;

    if (USBH_MSC_DevStatus(&gUsbHostFS, USB_LUN_NUMBER))
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
DSTATUS USBH_DiskInitialize(BYTE lun)
{
    /* USB host library needs to be initialized before disk initialization */
    return RES_OK;
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
DRESULT USBH_DiskRead(BYTE lun, BYTE *buff, LBA_t sector, UINT count)
{
    DRESULT res = RES_PARERR;
    USBH_MSC_STORAGE_INFO_T storageInfo;
    
    if (USBH_MSC_DevRead(&gUsbHostFS, USB_LUN_NUMBER, sector, buff, count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_ReadDevInfo(&gUsbHostFS, USB_LUN_NUMBER, &storageInfo);

        switch (storageInfo.sense.asc)
        {
            case USBH_SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case USBH_SCSI_ASC_MEDIUM_NOT_PRESENT:
            case USBH_SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                res = RES_NOTRDY;
                break;

            default:
                res = RES_ERROR;
                break;
        }
    }

    return res;
}

/**
 * @brief  Write Sector(s)
 * 
 * @param  lun: Logical unit number
 * 
 * @param  buff: Data buffer to store write data
 * 
 * @param  sector: Sector number to write
 * 
 * @param  count: Number of sectors to write
 * 
 * @retval DRESULT: Operation result
 */
DRESULT USBH_DiskWrite(BYTE lun, const BYTE *buff, LBA_t sector, UINT count)
{
    DRESULT res = RES_PARERR;
    USBH_MSC_STORAGE_INFO_T storageInfo;

    if (USBH_MSC_DevWrite(&gUsbHostFS, USB_LUN_NUMBER, sector, (BYTE*)buff, count) == USBH_OK)
    {
        res = RES_OK;
    }
    else
    {
        USBH_MSC_ReadDevInfo(&gUsbHostFS, USB_LUN_NUMBER, &storageInfo);

        switch (storageInfo.sense.asc)
        {
            case USBH_SCSI_ASC_LOGICAL_UNIT_NOT_READY:
            case USBH_SCSI_ASC_MEDIUM_NOT_PRESENT:
            case USBH_SCSI_ASC_NOT_READY_TO_READY_CHANGE:
                res = RES_NOTRDY;
                break;

            case USBH_SCSI_ASC_WRITE_PROTECTED:
                res = RES_WRPRT;
                break;

            default:
                res = RES_ERROR;
                break;
        }
    }

    return res;
}

/**
 * @brief  Disk I/O Control
 * 
 * @param  lun: Logical unit number
 * 
 * @param  cmd: Control command
 * 
 * @param  buff: Data buffer
 * 
 * @retval DRESULT: Operation result
 */
DRESULT USBH_DiskIoctl(BYTE lun, BYTE cmd, void *buff)
{
    DRESULT res = RES_PARERR;
    USBH_MSC_STORAGE_INFO_T storageInfo;

    switch (cmd)
    {
        case CTRL_SYNC:
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            if (USBH_MSC_ReadDevInfo(&gUsbHostFS, USB_LUN_NUMBER, &storageInfo) == USBH_OK)
            {
                *(DWORD*)buff = storageInfo.capacity.blockSize;
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;

        case GET_BLOCK_SIZE:
            if (USBH_MSC_ReadDevInfo(&gUsbHostFS, USB_LUN_NUMBER, &storageInfo) == USBH_OK)
            {
                *(DWORD*)buff = storageInfo.capacity.blockSize / USB_SECTOR_SIZE;
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;

        case GET_SECTOR_COUNT:
            if (USBH_MSC_ReadDevInfo(&gUsbHostFS, USB_LUN_NUMBER, &storageInfo) == USBH_OK)
            {
                *(DWORD*)buff = storageInfo.capacity.blockNum;
                res = RES_OK;
            }
            else
            {
                res = RES_ERROR;
            }
            break;

        default :
            res = RES_PARERR;
            break;
    }

    return res;
}

#endif /* FF_USBH_DISK_ENABLE */
