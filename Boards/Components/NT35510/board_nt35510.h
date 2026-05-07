/**
 * @file        board_nt35510.h
 *
 * @brief       This file contains definitions for NT35510 hardware resources
 *
 * @version     V1.0.0
 *
 * @date        2023-12-01
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
#ifndef BOARD_NT35510_H
#define BOARD_NT35510_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"

/* Exported macro *********************************************************/

/**
 * @brief NT35510_Status NT35510 status
 */
#define NT35510_STATUS_INIT                     0x00U
#define NT35510_STATUS_READY                    0x01U
#define NT35510_STATUS_ERROR                    0x02U
#define NT35510_STATUS_OK                       0x03U
#define NT35510_STATUS_TIMEOUT                  0x04U

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/
uint32_t NT35510_Init(void);
uint32_t NT35510_DeInit(void);
uint32_t NT35510_RegConfig(void);

void NT35510_DelayCallback(uint32_t delay);
int32_t NT35510_IO_InitCallback(void);
int32_t NT35510_IO_UninitCallback(void);
void NT35510_WriteCmdCallback(uint16_t cmd);
void NT35510_WriteDataCallback(uint16_t data);
void NT35510_WriteCmdDataCallback(uint16_t cmd, uint16_t data);
uint16_t NT35510_ReadDataCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_NT35510_H */
