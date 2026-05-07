/**
 * @file        board_ov2640.h
 *
 * @brief       This file contains definitions for OV2640 hardware resources
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
#ifndef BOARD_OV2640_H
#define BOARD_OV2640_H

#ifdef __cplusplus
  extern "C" {
#endif

/* Includes ***************************************************************/
#include "apm32f4xx_dal.h"

/* Exported macro *********************************************************/

/**
 * @brief OV2640_Status OV2640 status
 */
#define OV2640_STATUS_INIT                      0x00U
#define OV2640_STATUS_READY                     0x01U
#define OV2640_STATUS_ERROR                     0x02U
#define OV2640_STATUS_OK                        0x03U
#define OV2640_STATUS_TIMEOUT                   0x04U

/**
 * @brief OV2640_Device_Control_Registers OV2640 device control registers
 */
/* DSP bank (0xFF = 0x00) registers definition */
#define OV2640_DSP_R_BYPASS                     0x05U
#define OV2640_DSP_QS                           0x44U
#define OV2640_DSP_CTRL                         0x50U
#define OV2640_DSP_HSIZE                        0x51U
#define OV2640_DSP_VSIZE                        0x52U
#define OV2640_DSP_XOFFL                        0x53U
#define OV2640_DSP_YOFFL                        0x54U
#define OV2640_DSP_VHYX                         0x55U
#define OV2640_DSP_DPRP                         0x56U
#define OV2640_DSP_TEST                         0x57U
#define OV2640_DSP_ZMOW                         0x5AU
#define OV2640_DSP_ZMOH                         0x5BU
#define OV2640_DSP_ZMHH                         0x5CU
#define OV2640_DSP_BPADDR                       0x7CU
#define OV2640_DSP_BPDATA                       0x7DU
#define OV2640_DSP_CTRL2                        0x86U
#define OV2640_DSP_CTRL3                        0x87U
#define OV2640_DSP_SIZEL                        0x8CU
#define OV2640_DSP_HSIZE8                       0xC0U
#define OV2640_DSP_VSIZE8                       0xC1U
#define OV2640_DSP_CTRL0                        0xC2U
#define OV2640_DSP_CTRL1                        0xC3U
#define OV2640_DSP_R_DVP_SP                     0xD3U
#define OV2640_DSP_IMAGE_MODE                   0xDAU
#define OV2640_DSP_RESET                        0xE0U
#define OV2640_DSP_MS_SP                        0xF0U
#define OV2640_DSP_SS_ID                        0x7FU
#define OV2640_DSP_SS_CTRL                      0xF8U
#define OV2640_DSP_MC_BIST                      0xF9U
#define OV2640_DSP_MC_AL                        0xFAU
#define OV2640_DSP_MC_AH                        0xFBU
#define OV2640_DSP_MC_D                         0xFCU
#define OV2640_DSP_P_STATUS                     0xFEU
#define OV2640_DSP_RA_DLMT                      0xFFU

/* Sensor bank (0xFF = 0x01) registers definition */
#define OV2640_SENSOR_GAIN                      0x00U
#define OV2640_SENSOR_COM1                      0x03U
#define OV2640_SENSOR_REG04                     0x04U
#define OV2640_SENSOR_REG08                     0x08U
#define OV2640_SENSOR_COM2                      0x09U
#define OV2640_SENSOR_PIDH                      0x0AU
#define OV2640_SENSOR_PIDL                      0x0BU
#define OV2640_SENSOR_COM3                      0x0CU
#define OV2640_SENSOR_COM4                      0x0DU
#define OV2640_SENSOR_AEC                       0x10U
#define OV2640_SENSOR_CLKRC                     0x11U
#define OV2640_SENSOR_COM7                      0x12U
#define OV2640_SENSOR_COM8                      0x13U
#define OV2640_SENSOR_COM9                      0x14U
#define OV2640_SENSOR_COM10                     0x15U
#define OV2640_SENSOR_HREFST                    0x17U
#define OV2640_SENSOR_HREFEND                   0x18U
#define OV2640_SENSOR_VSTART                    0x19U
#define OV2640_SENSOR_VEND                      0x1AU
#define OV2640_SENSOR_MIDH                      0x1CU
#define OV2640_SENSOR_MIDL                      0x1DU
#define OV2640_SENSOR_AEW                       0x24U
#define OV2640_SENSOR_AEB                       0x25U
#define OV2640_SENSOR_W                         0x26U
#define OV2640_SENSOR_REG2A                     0x2AU
#define OV2640_SENSOR_FRARL                     0x2BU
#define OV2640_SENSOR_ADDVSL                    0x2DU
#define OV2640_SENSOR_ADDVSH                    0x2EU
#define OV2640_SENSOR_YAVG                      0x2FU
#define OV2640_SENSOR_HSDY                      0x30U
#define OV2640_SENSOR_HEDY                      0x31U
#define OV2640_SENSOR_REG32                     0x32U
#define OV2640_SENSOR_ARCOM2                    0x34U
#define OV2640_SENSOR_REG45                     0x45U
#define OV2640_SENSOR_FLL                       0x46U
#define OV2640_SENSOR_FLH                       0x47U
#define OV2640_SENSOR_COM19                     0x48U
#define OV2640_SENSOR_ZOOMS                     0x49U
#define OV2640_SENSOR_COM22                     0x4BU
#define OV2640_SENSOR_COM25                     0x4EU
#define OV2640_SENSOR_BD50                      0x4FU
#define OV2640_SENSOR_BD60                      0x50U
#define OV2640_SENSOR_REG5D                     0x5DU
#define OV2640_SENSOR_REG5E                     0x5EU
#define OV2640_SENSOR_REG5F                     0x5FU
#define OV2640_SENSOR_REG60                     0x60U
#define OV2640_SENSOR_HISTO_LOW                 0x61U
#define OV2640_SENSOR_HISTO_HIGH                0x62U

/**
 * @brief OV2640_Image_Format OV2640 image format
 */
#define OV2640_IMG_J160x120                     0x00U
#define OV2640_IMG_J320x240                     0x01U
#define OV2640_IMG_UXGA                         0x02U

/**
 * @brief OV2640_Light_Mode OV2640 light mode
 */
#define OV2640_LIGHT_AUTO                       0x00U
#define OV2640_LIGHT_SUNNY                      0x01U
#define OV2640_LIGHT_CLOUDY                     0x02U
#define OV2640_LIGHT_OFFICE                     0x03U
#define OV2640_LIGHT_HOME                       0x04U

/**
 * @brief OV2640_Color_Saturation OV2640 color saturation
 */
#define OV2640_COLOR_SATURATION_N2              0x00U
#define OV2640_COLOR_SATURATION_N1              0x01U
#define OV2640_COLOR_SATURATION_0               0x02U
#define OV2640_COLOR_SATURATION_1               0x03U
#define OV2640_COLOR_SATURATION_2               0x04U

/**
 * @brief OV2640_Brightness OV2640 brightness
 */
#define OV2640_BRIGHTNESS_N2                    0x00U
#define OV2640_BRIGHTNESS_N1                    0x01U
#define OV2640_BRIGHTNESS_0                     0x02U
#define OV2640_BRIGHTNESS_1                     0x03U
#define OV2640_BRIGHTNESS_2                     0x04U

/**
 * @brief OV2640_Contrast OV2640 contrast
 */
#define OV2640_CONTRAST_N2                      0x00U
#define OV2640_CONTRAST_N1                      0x01U
#define OV2640_CONTRAST_0                       0x02U
#define OV2640_CONTRAST_1                       0x03U
#define OV2640_CONTRAST_2                       0x04U

/**
 * @brief OV2640_Special_Effects OV2640 special effects
 */
#define OV2640_EFFECT_COMMON                    0x00U
#define OV2640_EFFECT_NEGATIVE_FILM             0x01U
#define OV2640_EFFECT_BW                        0x02U
#define OV2640_EFFECT_REDDISH                   0x03U
#define OV2640_EFFECT_GREENISH                  0x04U
#define OV2640_EFFECT_BLUISH                    0x05U
#define OV2640_EFFECT_RETRO                     0x06U

/* Exported typedef *******************************************************/

/**
 * @brief OV2640 information
 */
typedef struct
{
    uint32_t status;       /*!< OV2640 status @ref OV2640_Status */
    uint8_t addr;          /*!< OV2640 address */
} OV2640_INFO_T;

/* Exported function prototypes *******************************************/
uint32_t OV2640_Init(uint8_t addr, uint32_t format);
uint32_t OV2640_DeInit(void);
uint32_t OV2640_ReadID(uint8_t addr);
uint32_t OV2640_SetLightMode(uint32_t mode);
uint32_t OV2640_SetColorSaturation(uint32_t sat);
uint32_t OV2640_SetBrightness(uint32_t bright);
uint32_t OV2640_SetContrast(uint32_t contrast);
uint32_t OV2640_SetSpecialEffect(uint32_t effect);
uint32_t OV2640_EnableColorBar(FunctionalState status);
uint32_t OV2640_SetOutputWindow(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);
uint32_t OV2640_SetImageSize(uint16_t width, uint16_t height);
uint32_t OV2640_SetImageWindow(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height);
uint32_t OV2640_SetImageSizeOut(uint16_t width, uint16_t height);

void OV2640_DelayCallback(uint32_t delay);
int32_t OV2640_SCCB_InitCallback(void);
int32_t OV2640_SCCB_UninitCallback(void);
int32_t OV2640_SCCB_ReadRegCallback(uint8_t addr, uint8_t regAddr, uint8_t *regVal);
int32_t OV2640_SCCB_WriteRegCallback(uint8_t addr, uint8_t regAddr, uint8_t regVal);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_OV2640_H */
