/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"         /* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */

#ifdef FF_USBH_DISK_ENABLE
#include "usbh_diskio.h"
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
#include "sram_diskio.h"
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
#include "sd_diskio.h"
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
#include "sdram_diskio.h"
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
#include "user_diskio.h"
#endif /* FF_USER_DISK_ENABLE */

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
#ifdef FF_USBH_DISK_ENABLE
        case USBH_DISK:
            stat = USBH_GetDiskStatus(pdrv);
            return stat;
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
        case SRAM_DISK:
            stat = SRAM_GetDiskStatus(pdrv);
            return stat;
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
        case SD_DISK:
            stat = SD_GetDiskStatus(pdrv);
            return stat;
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
        case SDRAM_DISK:
            stat = SDRAM_GetDiskStatus(pdrv);
            return stat;
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
        case USER_DISK:
            stat = USER_GetDiskStatus(pdrv);
            return stat;
#endif /* FF_USER_DISK_ENABLE */

        default:
            break;
    }

    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;

    switch (pdrv)
    {
#ifdef FF_USBH_DISK_ENABLE
        case USBH_DISK:
            stat = USBH_DiskInitialize(pdrv);
            return stat;
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
        case SRAM_DISK:
            stat = SRAM_DiskInitialize(pdrv);
            return stat;
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
        case SD_DISK:
            stat = SD_DiskInitialize(pdrv);
            return stat;
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
        case SDRAM_DISK:
            stat = SDRAM_DiskInitialize(pdrv);
            return stat;
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
        case USER_DISK:
            stat = USER_DiskInitialize(pdrv);
            return stat;
#endif /* FF_USER_DISK_ENABLE */

        default:
            break;
    }

    return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive nmuber to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res;

    switch (pdrv)
    {
#ifdef FF_USBH_DISK_ENABLE
        case USBH_DISK:
            res = USBH_DiskRead(pdrv, buff, sector, count);
            return res;
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
        case SRAM_DISK:
            res = SRAM_DiskRead(pdrv, buff, sector, count);
            return res;
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
        case SD_DISK:
            res = SD_DiskRead(pdrv, buff, sector, count);
            return res;
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
        case SDRAM_DISK:
            res = SDRAM_DiskRead(pdrv, buff, sector, count);
            return res;
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
        case USER_DISK:
            res = USER_DiskRead(pdrv, buff, sector, count);
            return res;
#endif /* FF_USER_DISK_ENABLE */

        default:
            break;
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive nmuber to identify the drive */
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res;

    switch (pdrv)
    {
#ifdef FF_USBH_DISK_ENABLE
        case USBH_DISK:
            res = USBH_DiskWrite(pdrv, buff, sector, count);
            return res;
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
        case SRAM_DISK:
            res = SRAM_DiskWrite(pdrv, buff, sector, count);
            return res;
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
        case SD_DISK:
            res = SD_DiskWrite(pdrv, buff, sector, count);
            return res;
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
        case SDRAM_DISK:
            res = SDRAM_DiskWrite(pdrv, buff, sector, count);
            return res;
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
        case USER_DISK:
            res = USER_DiskWrite(pdrv, buff, sector, count);
            return res;
#endif /* FF_USER_DISK_ENABLE */

        default:
            break;
    }

    return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive nmuber (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res;

    switch (pdrv)
    {
#ifdef FF_USBH_DISK_ENABLE
        case USBH_DISK:
            res = USBH_DiskIoctl(pdrv, cmd, buff);
            return res;
#endif /* FF_USBH_DISK_ENABLE */

#ifdef FF_SRAM_DISK_ENABLE
        case SRAM_DISK:
            res = SRAM_DiskIoctl(pdrv, cmd, buff);
            return res;
#endif /* FF_SRAM_DISK_ENABLE */

#ifdef FF_SD_DISK_ENABLE
        case SD_DISK:
            res = SD_DiskIoctl(pdrv, cmd, buff);
            return res;
#endif /* FF_SD_DISK_ENABLE */

#ifdef FF_SDRAM_DISK_ENABLE
        case SDRAM_DISK:
            res = SDRAM_DiskIoctl(pdrv, cmd, buff);
            return res;
#endif /* FF_SDRAM_DISK_ENABLE */

#ifdef FF_USER_DISK_ENABLE
        case USER_DISK:
            res = USER_DiskIoctl(pdrv, cmd, buff);
            return res;
#endif /* FF_USER_DISK_ENABLE */

        default:
            break;
    }

    return RES_PARERR;
}
