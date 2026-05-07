/**
 * @file        usbd_cdc_vcp.c
 *
 * @brief       usb device CDC class Virtual Com Port handler
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
#include "usbd_cdc_vcp.h"

/* Private includes *******************************************************/
#include "usb_device_user.h"

/* Private macro **********************************************************/

/* Private variables ******************************************************/
uint8_t cdcTxBuffer[USBD_CDC_TX_BUF_LEN];
uint8_t cdcRxBuffer[USBD_CDC_RX_BUF_LEN];

USBD_CDC_VCP_INFO_T gUsbVCP;

/* Private typedef ********************************************************/

/* USB FS CDC interface handler */
USBD_CDC_INTERFACE_T USBD_CDC_INTERFACE_FS =
{
    "CDC Interface FS",
    USBD_FS_CDC_ItfInit,
    USBD_FS_CDC_ItfDeInit,
    USBD_FS_CDC_ItfCtrl,
    USBD_FS_CDC_ItfSend,
    USBD_FS_CDC_ItfSendEnd,
    USBD_FS_CDC_ItfReceive,
};

/* CDC Line Code Information */
USBD_CDC_LINE_CODING_T USBD_CDC_LINECODE_FS =
{
    115200,     /*!< baud rate */
    0x00,       /*!< stop bits 1 */
    0x00,       /*!< parity none */
    0x08,       /*!< word length 8 bits */
};

/* Private function prototypes ********************************************/

/* External variables *****************************************************/
extern USBD_INFO_T gUsbDeviceFS;

/* External functions *****************************************************/

/**
 * @brief       USB device initializes CDC media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_CDC_ConfigRxBuffer(&gUsbDeviceFS, cdcRxBuffer);
    USBD_CDC_ConfigTxBuffer(&gUsbDeviceFS, cdcTxBuffer, 0);
    
    return usbStatus;
}

/**
 * @brief       USB device deinitializes CDC media handler
 *
 * @param       None
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfDeInit(void)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/**
 * @brief       USB device CDC interface control request handler
 *
 * @param       command: Command code
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfCtrl(uint8_t command, uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    switch(command)
    {
        case USBD_CDC_SEND_ENCAPSULATED_COMMAND:
            break;

        case USBD_CDC_GET_ENCAPSULATED_RESPONSE:
            break;

        case USBD_CDC_SET_COMM_FEATURE:
            break;

        case USBD_CDC_GET_COMM_FEATURE:
            break;

        case USBD_CDC_CLEAR_COMM_FEATURE:
            break;

        /* Line Coding Data Structure 
        *  | Offset(Byte) | Field       | Length | Desc                 |
        *  | 0            | dwDTERate   | 4      | Data Terminal rate   |
        *  | 4            | bCharFormat | 1      | Stop bits            |
        *                                          (0 : 1 Stop bit)
        *                                          (1 : 1.5 Stop bits)
        *                                          (2 : 2 Stop bits)
        *  | 5            | bParityType | 1      | Parity               |
        *                                          (0 : None)
        *                                          (1 : Odd)
        *                                          (2 : Even)
        *                                          (3 : Mark)
        *                                          (4 : Space)
        *  | 6            | bDataBits   | 1      | Data bits            |
        *                                          (5 : 5 bits)
        *                                          (6 : 6 bits)
        *                                          (7 : 7 bits)
        *                                          (8 : 8 bits)
        *                                          (16 : 16 bits)
        */
        case USBD_CDC_SET_LINE_CODING:
            USBD_CDC_LINECODE_FS.baudRate   = (uint32_t)(buffer[0] | \
                                              (buffer[1] << 8) | \
                                              (buffer[2] << 16) | \
                                              (buffer[3] << 24));

            USBD_CDC_LINECODE_FS.format     = buffer[4];
            USBD_CDC_LINECODE_FS.parityType = buffer[5];
            USBD_CDC_LINECODE_FS.WordLen    = buffer[6];
        break;

        case USBD_CDC_GET_LINE_CODING:
            buffer[0] = (uint8_t)(USBD_CDC_LINECODE_FS.baudRate);
            buffer[1] = (uint8_t)(USBD_CDC_LINECODE_FS.baudRate >> 8);
            buffer[2] = (uint8_t)(USBD_CDC_LINECODE_FS.baudRate >> 16);
            buffer[3] = (uint8_t)(USBD_CDC_LINECODE_FS.baudRate >> 24);
            buffer[4] = USBD_CDC_LINECODE_FS.format;
            buffer[5] = USBD_CDC_LINECODE_FS.parityType;
            buffer[6] = USBD_CDC_LINECODE_FS.WordLen;
            break;

        case USBD_CDC_SET_CONTROL_LINE_STATE:
            break;

        case USBD_CDC_SEND_BREAK:
            break;

        default:
            break;
    }

    return usbStatus;
}

/**
 * @brief       USB device CDC interface send handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfSend(uint8_t *buffer, uint16_t length)
{
    USBD_STA_T usbStatus = USBD_OK;

    USBD_CDC_INFO_T *usbDevCDC = (USBD_CDC_INFO_T*)gUsbDeviceFS.devClass[gUsbDeviceFS.classID]->classData;
    
    if(usbDevCDC->cdcTx.state != USBD_CDC_XFER_IDLE)
    {
        return USBD_BUSY;
    }
    
    USBD_CDC_ConfigTxBuffer(&gUsbDeviceFS, buffer, length);
    
    usbStatus = USBD_CDC_TxPacket(&gUsbDeviceFS);
    
    return usbStatus;
}

/**
 * @brief       USB device CDC interface send end event handler
 *
 * @param       epNum: endpoint number
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfSendEnd(uint8_t epNum, uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;

    return usbStatus;
}

/**
 * @brief       USB device CDC interface receive handler
 *
 * @param       buffer: Command data buffer
 *
 * @param       length: Command data length
 *
 * @retval      USB device operation status
 */
USBD_STA_T USBD_FS_CDC_ItfReceive(uint8_t *buffer, uint32_t *length)
{
    USBD_STA_T usbStatus = USBD_OK;
    
    USBD_CDC_ConfigRxBuffer(&gUsbDeviceFS, &buffer[0]);
    
    USBD_CDC_RxPacket(&gUsbDeviceFS);
    
    gUsbVCP.state = USBD_CDC_VCP_REV_UPDATE;
    gUsbVCP.rxUpdateLen = *length;
    
    return usbStatus;
}
