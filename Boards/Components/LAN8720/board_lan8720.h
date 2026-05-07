/**
 * @file        board_lan8720.h
 *
 * @brief       This file contains definitions for LAN8720 hardware resources
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

/* Define to prevent recursive inclusion */
#ifndef BOARD_LAN8720_H
#define BOARD_LAN8720_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"

/* Exported macro *********************************************************/

/**
 * @brief LAN8720_Timeout LAN8720 timeout
 */
#define LAN8720_INIT_TIMEOUT        2500U
#define LAN8720_SOFTWARE_RST        500U

/**
 * @brief LAN8720_Status LAN8720 status
 */
#define LAN8720_STATUS_INIT                     0x00U
#define LAN8720_STATUS_READY                    0x01U
#define LAN8720_STATUS_ERROR                    0x02U
#define LAN8720_STATUS_OK                       0x03U
#define LAN8720_STATUS_TIMEOUT                  0x04U
#define LAN8720_STATUS_READ_ERR                 0x05U
#define LAN8720_STATUS_WRITE_ERR                0x06U
#define LAN8720_STATUS_LINK_DOWN                0x07U
#define LAN8720_STATUS_FULLDUPLEX_100MBITS      0x08U
#define LAN8720_STATUS_FULLDUPLEX_10MBITS       0x09U
#define LAN8720_STATUS_HALFDUPLEX_100MBITS      0x0AU
#define LAN8720_STATUS_HALFDUPLEX_10MBITS       0x0BU
#define LAN8720_STATUS_AUTONEGO_NOTDONE         0x0CU

/**
 * @brief LAN8720_Register LAN8720 register
 */
#define LAN8720_REG_BCR             ((uint16_t)0x0000U)
#define LAN8720_REG_BSR             ((uint16_t)0x0001U)
#define LAN8720_REG_PHYI1R          ((uint16_t)0x0002U)
#define LAN8720_REG_PHYI2R          ((uint16_t)0x0003U)
#define LAN8720_REG_ANAR            ((uint16_t)0x0004U)
#define LAN8720_REG_ANLPAR          ((uint16_t)0x0005U)
#define LAN8720_REG_ANER            ((uint16_t)0x0006U)
#define LAN8720_REG_ANNPTR          ((uint16_t)0x0007U)
#define LAN8720_REG_ANNPRR          ((uint16_t)0x0008U)
#define LAN8720_REG_MMDACR          ((uint16_t)0x000DU)
#define LAN8720_REG_MMDAADR         ((uint16_t)0x000EU)
#define LAN8720_REG_ENCTR           ((uint16_t)0x0010U)
#define LAN8720_REG_MCSR            ((uint16_t)0x0011U)
#define LAN8720_REG_SMR             ((uint16_t)0x0012U)
#define LAN8720_REG_TPDCR           ((uint16_t)0x0018U)
#define LAN8720_REG_TCSR            ((uint16_t)0x0019U)
#define LAN8720_REG_SECR            ((uint16_t)0x001AU)
#define LAN8720_REG_SCSIR           ((uint16_t)0x001BU)
#define LAN8720_REG_CLR             ((uint16_t)0x001CU)
#define LAN8720_REG_ISFR            ((uint16_t)0x001DU)
#define LAN8720_REG_IMR             ((uint16_t)0x001EU)
#define LAN8720_REG_PHYSCSR         ((uint16_t)0x001FU)

/**
 *@brief LAN8720 BCR bit definition
 */
#define LAN8720_REG_BCR_SOFT_RESET          ((uint16_t)0x8000U)
#define LAN8720_REG_BCR_LOOPBACK            ((uint16_t)0x4000U)
#define LAN8720_REG_BCR_SPEED_SELECT        ((uint16_t)0x2000U)
#define LAN8720_REG_BCR_AUTONEGO_EN         ((uint16_t)0x1000U)
#define LAN8720_REG_BCR_POWER_DOWN          ((uint16_t)0x0800U)
#define LAN8720_REG_BCR_ISOLATE             ((uint16_t)0x0400U)
#define LAN8720_REG_BCR_RESTART_AUTONEGO    ((uint16_t)0x0200U)
#define LAN8720_REG_BCR_DUPLEX_MODE         ((uint16_t)0x0100U) 

/**
 * @brief LAN8720 BSR bit definition
 */
#define LAN8720_REG_BSR_100BASE_T4          ((uint16_t)0x8000U)
#define LAN8720_REG_BSR_100BASE_TX_FD       ((uint16_t)0x4000U)
#define LAN8720_REG_BSR_100BASE_TX_HD       ((uint16_t)0x2000U)
#define LAN8720_REG_BSR_10BASE_T_FD         ((uint16_t)0x1000U)
#define LAN8720_REG_BSR_10BASE_T_HD         ((uint16_t)0x0800U)
#define LAN8720_REG_BSR_100BASE_T2_FD       ((uint16_t)0x0400U)
#define LAN8720_REG_BSR_100BASE_T2_HD       ((uint16_t)0x0200U)
#define LAN8720_REG_BSR_EXTENDED_STATUS     ((uint16_t)0x0100U)
#define LAN8720_REG_BSR_AUTONEGO_CPLT       ((uint16_t)0x0020U)
#define LAN8720_REG_BSR_REMOTE_FAULT        ((uint16_t)0x0010U)
#define LAN8720_REG_BSR_AUTONEGO_ABILITY    ((uint16_t)0x0008U)
#define LAN8720_REG_BSR_LINK_STATUS         ((uint16_t)0x0004U)
#define LAN8720_REG_BSR_JABBER_DETECT       ((uint16_t)0x0002U)
#define LAN8720_REG_BSR_EXTENDED_CAP        ((uint16_t)0x0001U)

/**
 * @brief LAN8720 PHYSCSR bit definition
 */
#define LAN8720_REG_PHYSCSR_AUTONEGO_DONE   ((uint16_t)0x1000U)
#define LAN8720_REG_PHYSCSR_HCDSPEEDMASK    ((uint16_t)0x001CU)
#define LAN8720_REG_PHYSCSR_10BT_HD         ((uint16_t)0x0004U)
#define LAN8720_REG_PHYSCSR_10BT_FD         ((uint16_t)0x0014U)
#define LAN8720_REG_PHYSCSR_100BTX_HD       ((uint16_t)0x0008U)
#define LAN8720_REG_PHYSCSR_100BTX_FD       ((uint16_t)0x0018U) 

/* Exported typedef *******************************************************/

/**
 * @brief LAN8720 information
 */
typedef struct
{
    uint32_t devAddr;
    uint32_t status;        /*!< LAN8270 status @ref LAN8720_Status */
} LAN8720_INFO_T;

/* Exported function prototypes *******************************************/
uint32_t LAN8720_Init(void);
uint32_t LAN8720_DeInit(void);
uint32_t LAN8720_GetLinkState(void);

int32_t LAN8720_PHY_InitCallback(void);
int32_t LAN8720_PHY_UninitCallback(void);
int32_t LAN8720_PHY_GetTickCallback(void);
int32_t LAN8720_PHY_ReadRegCallback(uint32_t devAddr, uint32_t regAddr, uint32_t *regVal);
int32_t LAN8720_PHY_WriteRegCallback(uint32_t devAddr, uint32_t regAddr, uint32_t regVal);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_LAN8720_H */
