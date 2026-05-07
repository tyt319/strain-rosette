/**
 * @file        main.h
 *
 * @brief       Header for main.c module
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

/* Define to prevent recursive inclusion */
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"
#include "apm32f4xx_device_cfg.h"
#include "usbd_cdc.h"
#include "usbd_cdc_vcp.h"
#include "ads131m08.h"
#include "bridge_cal.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
/* Exported macro *********************************************************/

/* Exported typedef *******************************************************/

/* Exported function prototypes *******************************************/

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
