/**
 * @file        board_ov2640.c
 *
 * @brief       This file provides firmware functions to manage OV2640
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

/* Includes ***************************************************************/
#include "board_ov2640.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

OV2640_INFO_T ov2640Info;

/* JPEG 160x120 */
const unsigned char OV2640_160x120_JPEG_REG_TABLE[][2] =
{
    {0xff, 0x01},
    {0x12, 0x40},
    {0x17, 0x11},
    {0x18, 0x43},
    {0x19, 0x00},
    {0x1a, 0x4b},
    {0x32, 0x09},
    {0x4f, 0xca},
    {0x50, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x39, 0x12},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {0x34, 0xc0},
    {0x36, 0x1a},
    {0x06, 0x88},
    {0x07, 0xc0},
    {0x0d, 0x87},
    {0x0e, 0x41},
    {0x4c, 0x00},
    {0xff, 0x00},
    {0xe0, 0x04},
    {0xc0, 0x64},
    {0xc1, 0x4b},
    {0x86, 0x35},
    {0x50, 0x92},
    {0x51, 0xc8},
    {0x52, 0x96},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x57, 0x00},
    {0x5a, 0x28},
    {0x5b, 0x1e},
    {0x5c, 0x00},
    {0xe0, 0x00},
};

/* JPEG 320x240 */
const unsigned char OV2640_320x240_JPEG_REG_TABLE[][2] =
{
    {0xff, 0x01},
    {0x12, 0x40},
    {0x17, 0x11},
    {0x18, 0x43},
    {0x19, 0x00},
    {0x1a, 0x4b},
    {0x32, 0x09},
    {0x4f, 0xca},
    {0x50, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x39, 0x12},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {0x34, 0xc0},
    {0x36, 0x1a},
    {0x06, 0x88},
    {0x07, 0xc0},
    {0x0d, 0x87},
    {0x0e, 0x41},
    {0x4c, 0x00},
    {0xff, 0x00},
    {0xe0, 0x04},
    {0xc0, 0x64},
    {0xc1, 0x4b},
    {0x86, 0x35},
    {0x50, 0x89},
    {0x51, 0xc8},
    {0x52, 0x96},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x57, 0x00},
    {0x5a, 0x50},
    {0x5b, 0x3c},
    {0x5c, 0x00},
    {0xe0, 0x00},
};

/**
*   OV2640 SXGA Init Register table
*   SXGA(800 x 600)
*/
const uint8_t OV2640_SVGA_REG_TABLE[][2] =
{
    {0xff, 0x00},
    {0x2c, 0xff},
    {0x2e, 0xdf},
    {0xff, 0x01},
    {0x3c, 0x32},
    {0x11, 0x00},
    {0x09, 0x02},
    {0x04, 0xD8},
    {0x13, 0xe5},
    {0x14, 0x48},
    {0x2c, 0x0c},
    {0x33, 0x78},
    {0x3a, 0x33},
    {0x3b, 0xfB},
    {0x3e, 0x00},
    {0x43, 0x11},
    {0x16, 0x10},
    {0x39, 0x92},
    {0x35, 0xda},
    {0x22, 0x1a},
    {0x37, 0xc3},
    {0x23, 0x00},
    {0x34, 0xc0},
    {0x36, 0x1a},
    {0x06, 0x88},
    {0x07, 0xc0},
    {0x0d, 0x87},
    {0x0e, 0x41},
    {0x4c, 0x00},
    {0x48, 0x00},
    {0x5B, 0x00},
    {0x42, 0x03},
    {0x4a, 0x81},
    {0x21, 0x99},
    {0x24, 0x40},
    {0x25, 0x38},
    {0x26, 0x82},
    {0x5c, 0x00},
    {0x63, 0x00},
    {0x46, 0x22},
    {0x0c, 0x3c},
    {0x61, 0x70},
    {0x62, 0x80},
    {0x7c, 0x05},
    {0x20, 0x80},
    {0x28, 0x30},
    {0x6c, 0x00},
    {0x6d, 0x80},
    {0x6e, 0x00},
    {0x70, 0x02},
    {0x71, 0x94},
    {0x73, 0xc1},
    {0x3d, 0x34},
    {0x5a, 0x57},
    {0x12, 0x40},
    {0x17, 0x11},
    {0x18, 0x43},
    {0x19, 0x00},
    {0x1a, 0x4b},
    {0x32, 0x09},
    {0x37, 0xc0},
    {0x4f, 0xca},
    {0x50, 0xa8},
    {0x5a, 0x23},
    {0x6d, 0x00},
    {0x3d, 0x38},
    {0xff, 0x00},
    {0xe5, 0x7f},
    {0xf9, 0xc0},
    {0x41, 0x24},
    {0xe0, 0x14},
    {0x76, 0xff},
    {0x33, 0xa0},
    {0x42, 0x20},
    {0x43, 0x18},
    {0x4c, 0x00},
    {0x87, 0xd5},
    {0x88, 0x3f},
    {0xd7, 0x03},
    {0xd9, 0x10},
    {0xd3, 0x82},
    {0xc8, 0x08},
    {0xc9, 0x80},
    {0x7c, 0x00},
    {0x7d, 0x00},
    {0x7c, 0x03},
    {0x7d, 0x48},
    {0x7d, 0x48},
    {0x7c, 0x08},
    {0x7d, 0x20},
    {0x7d, 0x10},
    {0x7d, 0x0e},
    {0x90, 0x00},
    {0x91, 0x0e},
    {0x91, 0x1a},
    {0x91, 0x31},
    {0x91, 0x5a},
    {0x91, 0x69},
    {0x91, 0x75},
    {0x91, 0x7e},
    {0x91, 0x88},
    {0x91, 0x8f},
    {0x91, 0x96},
    {0x91, 0xa3},
    {0x91, 0xaf},
    {0x91, 0xc4},
    {0x91, 0xd7},
    {0x91, 0xe8},
    {0x91, 0x20},
    {0x92, 0x00},
    {0x93, 0x06},
    {0x93, 0xe3},
    {0x93, 0x05},
    {0x93, 0x05},
    {0x93, 0x00},
    {0x93, 0x04},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x96, 0x00},
    {0x97, 0x08},
    {0x97, 0x19},
    {0x97, 0x02},
    {0x97, 0x0c},
    {0x97, 0x24},
    {0x97, 0x30},
    {0x97, 0x28},
    {0x97, 0x26},
    {0x97, 0x02},
    {0x97, 0x98},
    {0x97, 0x80},
    {0x97, 0x00},
    {0x97, 0x00},
    {0xc3, 0xed},
    {0xa4, 0x00},
    {0xa8, 0x00},
    {0xc5, 0x11},
    {0xc6, 0x51},
    {0xbf, 0x80},
    {0xc7, 0x10},
    {0xb6, 0x66},
    {0xb8, 0xA5},
    {0xb7, 0x64},
    {0xb9, 0x7C},
    {0xb3, 0xaf},
    {0xb4, 0x97},
    {0xb5, 0xFF},
    {0xb0, 0xC5},
    {0xb1, 0x94},
    {0xb2, 0x0f},
    {0xc4, 0x5c},
    {0xc0, 0x64},
    {0xc1, 0x4B},
    {0x8c, 0x00},
    {0x86, 0x3D},
    {0x50, 0x00},
    {0x51, 0xC8},
    {0x52, 0x96},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x00},
    {0x5a, 0xC8},
    {0x5b, 0x96},
    {0x5c, 0x00},
    {0xd3, 0x02},
    {0xc3, 0xed},
    {0x7f, 0x00},
    {0xda, 0x09},
    {0xe5, 0x1f},
    {0xe1, 0x67},
    {0xe0, 0x00},
    {0xdd, 0x7f},
    {0x05, 0x00},
};

/** YUV422 register table */
const uint8_t OV2640_YUV422_REG_TABLE[][2] =
{
    {0xFF, 0x00},
    {0xDA, 0x10},
    {0xD7, 0x03},
    {0xDF, 0x00},
    {0x33, 0x80},
    {0x3C, 0x40},
    {0xe1, 0x77},
    {0x00, 0x00},
};

/** JPG register table */
const uint8_t OV2640_JPEG_REG_TABLE[][2] =
{
    {0xff, 0x01},
    {0xe0, 0x14},
    {0xe1, 0x77},
    {0xe5, 0x1f},
    {0xd7, 0x03},
    {0xda, 0x10},
    {0xe0, 0x00},
};

/**
*   OV2640 UVGA Init Register table
*   UXGA(1600 x 1200)
*/
const unsigned char OV2640_UXGA_REG_TABLE[][2] =
{
    {0xff, 0x00},
    {0x2c, 0xff},
    {0x2e, 0xdf},
    {0xff, 0x01},
    {0x3c, 0x32},
    {0x11, 0x00},
    {0x09, 0x02},
    {0x04, 0xD8},
    {0x13, 0xe5},
    {0x14, 0x48},
    {0x2c, 0x0c},
    {0x33, 0x78},
    {0x3a, 0x33},
    {0x3b, 0xfB},
    {0x3e, 0x00},
    {0x43, 0x11},
    {0x16, 0x10},
    {0x4a, 0x81},
    {0x21, 0x99},
    {0x24, 0x40},
    {0x25, 0x38},
    {0x26, 0x82},
    {0x5c, 0x00},
    {0x63, 0x00},
    {0x46, 0x3f},
    {0x0c, 0x3c},
    {0x61, 0x70},
    {0x62, 0x80},
    {0x7c, 0x05},
    {0x20, 0x80},
    {0x28, 0x30},
    {0x6c, 0x00},
    {0x6d, 0x80},
    {0x6e, 0x00},
    {0x70, 0x02},
    {0x71, 0x94},
    {0x73, 0xc1},
    {0x3d, 0x34},
    {0x5a, 0x57},
    {0x12, 0x00},
    {0x11, 0x00},
    {0x17, 0x11},
    {0x18, 0x75},
    {0x19, 0x01},
    {0x1a, 0x97},
    {0x32, 0x36},
    {0x03, 0x0f},
    {0x37, 0x40},
    {0x4f, 0xbb},
    {0x50, 0x9c},
    {0x5a, 0x57},
    {0x6d, 0x80},
    {0x6d, 0x38},
    {0x39, 0x02},
    {0x35, 0x88},
    {0x22, 0x0a},
    {0x37, 0x40},
    {0x23, 0x00},
    {0x34, 0xa0},
    {0x36, 0x1a},
    {0x06, 0x02},
    {0x07, 0xc0},
    {0x0d, 0xb7},
    {0x0e, 0x01},
    {0x4c, 0x00},
    {0xff, 0x00},
    {0xe5, 0x7f},
    {0xf9, 0xc0},
    {0x41, 0x24},
    {0xe0, 0x14},
    {0x76, 0xff},
    {0x33, 0xa0},
    {0x42, 0x20},
    {0x43, 0x18},
    {0x4c, 0x00},
    {0x87, 0xd0},
    {0x88, 0x3f},
    {0xd7, 0x03},
    {0xd9, 0x10},
    {0xd3, 0x82},
    {0xc8, 0x08},
    {0xc9, 0x80},
    {0x7d, 0x00},
    {0x7c, 0x03},
    {0x7d, 0x48},
    {0x7c, 0x08},
    {0x7d, 0x20},
    {0x7d, 0x10},
    {0x7d, 0x0e},
    {0x90, 0x00},
    {0x91, 0x0e},
    {0x91, 0x1a},
    {0x91, 0x31},
    {0x91, 0x5a},
    {0x91, 0x69},
    {0x91, 0x75},
    {0x91, 0x7e},
    {0x91, 0x88},
    {0x91, 0x8f},
    {0x91, 0x96},
    {0x91, 0xa3},
    {0x91, 0xaf},
    {0x91, 0xc4},
    {0x91, 0xd7},
    {0x91, 0xe8},
    {0x91, 0x20},
    {0x92, 0x00},
    {0x93, 0x06},
    {0x93, 0xe3},
    {0x93, 0x02},
    {0x93, 0x02},
    {0x93, 0x00},
    {0x93, 0x04},
    {0x93, 0x00},
    {0x93, 0x03},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x93, 0x00},
    {0x96, 0x00},
    {0x97, 0x08},
    {0x97, 0x19},
    {0x97, 0x02},
    {0x97, 0x0c},
    {0x97, 0x24},
    {0x97, 0x30},
    {0x97, 0x28},
    {0x97, 0x26},
    {0x97, 0x02},
    {0x97, 0x98},
    {0x97, 0x80},
    {0x97, 0x00},
    {0x97, 0x00},
    {0xc3, 0xef},
    {0xff, 0x00},
    {0xba, 0xdc},
    {0xbb, 0x08},
    {0xb6, 0x24},
    {0xb8, 0x33},
    {0xb7, 0x20},
    {0xb9, 0x30},
    {0xb3, 0xb4},
    {0xb4, 0xca},
    {0xb5, 0x43},
    {0xb0, 0x5c},
    {0xb1, 0x4f},
    {0xb2, 0x06},
    {0xc7, 0x00},
    {0xc6, 0x51},
    {0xc5, 0x11},
    {0xc4, 0x9c},
    {0xbf, 0x00},
    {0xbc, 0x64},
    {0xa6, 0x00},
    {0xa7, 0x1e},
    {0xa7, 0x6b},
    {0xa7, 0x47},
    {0xa7, 0x33},
    {0xa7, 0x00},
    {0xa7, 0x23},
    {0xa7, 0x2e},
    {0xa7, 0x85},
    {0xa7, 0x42},
    {0xa7, 0x33},
    {0xa7, 0x00},
    {0xa7, 0x23},
    {0xa7, 0x1b},
    {0xa7, 0x74},
    {0xa7, 0x42},
    {0xa7, 0x33},
    {0xa7, 0x00},
    {0xa7, 0x23},
    {0xc0, 0xc8},
    {0xc1, 0x96},
    {0x8c, 0x00},
    {0x86, 0x3d},
    {0x50, 0x92},
    {0x51, 0x90},
    {0x52, 0x2c},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x88},
    {0x5a, 0x50},
    {0x5b, 0x3c},
    {0x5c, 0x00},
    {0xd3, 0x08},
    {0x7f, 0x00},
    {0xda, 0x00},
    {0xe5, 0x1f},
    {0xe1, 0x67},
    {0xe0, 0x00},
    {0xdd, 0x7f},
    {0x05, 0x00},
    {0xff, 0x00},
    {0xe0, 0x04},
    {0xc0, 0xc8},
    {0xc1, 0x96},
    {0x86, 0x3d},
    {0x50, 0x92},
    {0x51, 0x90},
    {0x52, 0x2c},
    {0x53, 0x00},
    {0x54, 0x00},
    {0x55, 0x88},
    {0x57, 0x00},
    {0x5a, 0x50},
    {0x5b, 0x3c},
    {0x5c, 0x00},
    {0xd3, 0x04},
    {0xe0, 0x00},
    {0xFF, 0x00},
    {0x05, 0x00},
    {0xDA, 0x08},
    {0xda, 0x09},
    {0x98, 0x00},
    {0x99, 0x00},
    {0x00, 0x00},
};

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   Initializes OV2640
 *
 * @param   addr: SCCB address
 *
 * @param   format: Resolution format
 *          This parameter can be one of the following values:
 *          @arg OV2640_IMG_J160x120: JPEG 160x120
 *          @arg OV2640_IMG_J320x240: JPEG 320x240
 *          @arg OV2640_IMG_UXGA:     UXGA(1600 x 1200)
 *
 * @retval  OV2640 status
 */
uint32_t OV2640_Init(uint8_t addr, uint32_t format)
{
    uint32_t i;

    /* Init OV2640 SCCB */
    OV2640_SCCB_InitCallback();

    /* Register SCCB address */
    ov2640Info.addr = addr;

    /* OV2640 software reset */
    /* Select OV2640 Sensor bank register group */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_DSP_RA_DLMT, 0x01U);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_SENSOR_COM7, 0x80U);
    
    OV2640_DelayCallback(200U);

    /* JPEG format */
    if (format != OV2640_IMG_UXGA)
    {
        /* Set OV2640 resolution is SXGA(800 * 600)*/
        for(i = 0; i < sizeof(OV2640_SVGA_REG_TABLE) / 2; i++)
        {
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_SVGA_REG_TABLE[i][0],OV2640_SVGA_REG_TABLE[i][1]);
            OV2640_DelayCallback(1U);
        }

        /* Set to output YUV422 format*/
        for(i = 0; i < (sizeof(OV2640_YUV422_REG_TABLE) / 2); i++)
        {
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_YUV422_REG_TABLE[i][0],OV2640_YUV422_REG_TABLE[i][1]);
            OV2640_DelayCallback(1U);
        }

        /* Set to output JPEG */
        for(i = 0; i < (sizeof(OV2640_JPEG_REG_TABLE) / 2); i++)
        {
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_JPEG_REG_TABLE[i][0],OV2640_JPEG_REG_TABLE[i][1]);
            OV2640_DelayCallback(1U);
        }
        
        OV2640_DelayCallback(10);
        
        switch (format)
        {
            case OV2640_IMG_J160x120:
                for (i = 0U; i < (sizeof(OV2640_160x120_JPEG_REG_TABLE) / 2U); i++)
                {
                    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_160x120_JPEG_REG_TABLE[i][0], OV2640_160x120_JPEG_REG_TABLE[i][1]);
                    OV2640_DelayCallback(1U);
                }
                break;
            
            case OV2640_IMG_J320x240:
                for( i = 0U; i < (sizeof(OV2640_320x240_JPEG_REG_TABLE) / 2U); i++)
                {
                    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_320x240_JPEG_REG_TABLE[i][0], OV2640_320x240_JPEG_REG_TABLE[i][1]);
                    OV2640_DelayCallback(1U);
                }
                break;

            default:
                for (i = 0U; i < (sizeof(OV2640_160x120_JPEG_REG_TABLE) / 2U); i++)
                {
                    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_160x120_JPEG_REG_TABLE[i][0], OV2640_160x120_JPEG_REG_TABLE[i][1]);
                    OV2640_DelayCallback(1U);
                }
                break;
        }
    }
    /* RAW format */
    else
    {
        /* Set OV2640 resolution is UXGA(1600 * 1200)*/
        for(i = 0; i < sizeof(OV2640_UXGA_REG_TABLE) / 2; i++)
        {
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_UXGA_REG_TABLE[i][0], OV2640_UXGA_REG_TABLE[i][1]);
            OV2640_DelayCallback(1U);
        }
    }

    return OV2640_STATUS_OK;
}

/**
 * @brief   De initializes OV2640
 *
 * @param   None
 *
 * @retval  OV2640 status
 */
uint32_t OV2640_DeInit(void)
{
    /* Init OV2640 SCCB */
    OV2640_SCCB_InitCallback();

    /* OV2640 software reset */
    /* Select OV2640 Sensor bank register group */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_DSP_RA_DLMT, 0x01U);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, OV2640_SENSOR_COM7, 0x80U);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Read the OV2640 camera ID
 *
 * @param   addr: SCCB address
 *
 * @retval  OV2640 status
 */
uint32_t OV2640_ReadID(uint8_t addr)
{
    uint32_t pid;
    uint8_t regVal;

    /* Init OV2640 SCCB */
    OV2640_SCCB_InitCallback();

    /* OV2640 software reset */
    /* Select OV2640 Sensor bank register group */
    OV2640_SCCB_WriteRegCallback(addr, OV2640_DSP_RA_DLMT, 0x01U);
    OV2640_SCCB_WriteRegCallback(addr, OV2640_SENSOR_COM7, 0x80U);

    OV2640_DelayCallback(50U);

    /* Read OV2640 PID */
    OV2640_SCCB_WriteRegCallback(addr, OV2640_DSP_RA_DLMT, 0x01U);
    OV2640_SCCB_ReadRegCallback(addr, OV2640_SENSOR_PIDH, &regVal);

    pid = regVal;
    pid <<= 8U;

    OV2640_SCCB_ReadRegCallback(addr, OV2640_SENSOR_PIDL, &regVal);

    pid |= regVal;
    
    return pid;
}

/**
 * @brief   Set OV2640 light mode
 * 
 * @param   mode: Light mode
 *          This parameter can be one of the following values:
 *          @arg OV2640_LIGHT_AUTO: Auto
 *          @arg OV2640_LIGHT_SUNNY: Sunny
 *          @arg OV2640_LIGHT_OFFICE: Office
 *          @arg OV2640_LIGHT_CLOUDY: Cloudy
 *          @arg OV2640_LIGHT_HOME: Home
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetLightMode(uint32_t mode)
{
    /** sunny mode*/
    uint8_t regccVal = 0;
    uint8_t regcdVal = 0;
    uint8_t regceVal = 0;

    /* Set light mode */
    switch (mode)
    {
        case OV2640_LIGHT_AUTO:
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
            OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xC7, 0x10);
            break;

        case OV2640_LIGHT_SUNNY:
            regccVal = 0x5E;
            regcdVal = 0x41;
            regceVal = 0x54;
            break;

        case OV2640_LIGHT_OFFICE:
            regccVal = 0x52;
            regcdVal = 0x41;
            regceVal = 0x66;
            break;

        case OV2640_LIGHT_CLOUDY:
            regccVal = 0x65;
            regcdVal = 0x41;
            regceVal = 0x4F;
            break;

        case OV2640_LIGHT_HOME:
            regccVal = 0x42;
            regcdVal = 0x3F;
            regceVal = 0x71;
            break;

        default:
            regccVal = 0x5E;
            regcdVal = 0x41;
            regceVal = 0x54;
            break;
    }

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    /* AWB OFF  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xC7, 0x40);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xCC, regccVal);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xCD, regcdVal);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xCE, regceVal);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 color saturation
 * 
 * @param   sat: Color saturation
 *          This parameter can be one of the following values:
 *          @arg OV2640_COLOR_SATURATION_N2: Saturation -2
 *          @arg OV2640_COLOR_SATURATION_N1: Saturation -1
 *          @arg OV2640_COLOR_SATURATION_0: Saturation 0
 *          @arg OV2640_COLOR_SATURATION_1: Saturation +1
 *          @arg OV2640_COLOR_SATURATION_2: Saturation +2
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetColorSaturation(uint32_t sat)
{
    uint8_t reg7dVal = 0;
    
    /* Set color saturation */
    reg7dVal = ((sat + 2U) << 4U) | 0x08U;

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x02);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x03);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7dVal);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7dVal);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 brightness
 * 
 * @param   bright: Brightness
 *          This parameter can be one of the following values:
 *          @arg OV2640_BRIGHTNESS_N2: Brightness -2
 *          @arg OV2640_BRIGHTNESS_N1: Brightness -1
 *          @arg OV2640_BRIGHTNESS_0: Brightness 0
 *          @arg OV2640_BRIGHTNESS_1: Brightness +1
 *          @arg OV2640_BRIGHTNESS_2: Brightness +2
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetBrightness(uint32_t bright)
{
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x09);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, bright << 4U);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x00);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 contrast
 * 
 * @param   contrast: Contrast
 *          This parameter can be one of the following values:
 *          @arg OV2640_CONTRAST_N2: Contrast -2
 *          @arg OV2640_CONTRAST_N1: Contrast -1
 *          @arg OV2640_CONTRAST_0: Contrast 0
 *          @arg OV2640_CONTRAST_1: Contrast +1
 *          @arg OV2640_CONTRAST_2: Contrast +2
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetContrast(uint32_t contrast)
{
    uint8_t reg7d0Val = 0;
    uint8_t reg7d1Val = 0;

    switch(contrast)
    {
        case OV2640_CONTRAST_N2:
            reg7d0Val = 0x18;
            reg7d1Val = 0x34;
            break;

        case OV2640_CONTRAST_N1:
            reg7d0Val = 0x1C;
            reg7d1Val = 0x2A;
            break;

        case OV2640_CONTRAST_0:
            reg7d0Val = 0x20;
            reg7d1Val = 0x20;
            break;

        case OV2640_CONTRAST_1:
            reg7d0Val = 0x24;
            reg7d1Val = 0x16;
            break;

        case OV2640_CONTRAST_2:
            reg7d0Val = 0x28;
            reg7d1Val = 0x0C;
            break;

        default:
            reg7d0Val = 0x20;
            reg7d1Val = 0x20;
            break;
    }

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x07);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x20);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7d0Val);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7d1Val);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, 0x06);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 special effects
 * 
 * @param   effect: Special effect
 *          This parameter can be one of the following values:
 *          @arg OV2640_EFFECT_COMMON: Normal
 *          @arg OV2640_EFFECT_NEGATIVE_FILM: Negative film
 *          @arg OV2640_EFFECT_BW: Black and white
 *          @arg OV2640_EFFECT_REDDISH: Reddish
 *          @arg OV2640_EFFECT_GREENISH: Greenish
 *          @arg OV2640_EFFECT_BLUISH: Bluish
 *          @arg OV2640_EFFECT_RETRO: Retro
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetSpecialEffect(uint32_t effect)
{
    uint8_t reg7d0Val = 0;
    uint8_t reg7d1Val = 0;
    uint8_t reg7d2Val = 0;

    switch(effect)
    {
        case OV2640_EFFECT_COMMON:
            reg7d0Val = 0x00;
            reg7d1Val = 0x80;
            reg7d2Val = 0x80;
            break;

        case OV2640_EFFECT_NEGATIVE_FILM:
            reg7d0Val = 0x40;
            reg7d1Val = 0x80;
            reg7d2Val = 0x80;
            break;

        case OV2640_EFFECT_BW:
            reg7d0Val = 0x18;
            reg7d1Val = 0x80;
            reg7d2Val = 0x80;
            break;

        case OV2640_EFFECT_REDDISH:
            reg7d0Val = 0x18;
            reg7d1Val = 0x40;
            reg7d2Val = 0xC0;
            break;

        case OV2640_EFFECT_GREENISH:
            reg7d0Val = 0x18;
            reg7d1Val = 0x40;
            reg7d2Val = 0x40;
            break;

        case OV2640_EFFECT_BLUISH:
            reg7d0Val = 0x18;
            reg7d1Val = 0xA0;
            reg7d2Val = 0x40;
            break;

        case OV2640_EFFECT_RETRO:
            reg7d0Val = 0x18;
            reg7d1Val = 0x40;
            reg7d2Val = 0xA6;
            break;
            
        default:
            reg7d0Val = 0x00;
            reg7d1Val = 0x80;
            reg7d2Val = 0x80;
            break;
    }

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7d0Val);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7C, 0x05);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7d1Val);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x7D, reg7d2Val);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Enable OV2640 color bar mode
 * 
 * @param   status: Enable or disable
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_EnableColorBar(FunctionalState status)
{
    uint8_t regTemp = 0;

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x01);

    OV2640_SCCB_ReadRegCallback(ov2640Info.addr, 0x12, &regTemp);
    regTemp &= ~(1U << 1U);
    if (status != DISABLE)
    {
        regTemp |= (1U << 1U);
    }
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x12, regTemp);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 output window size
 * 
 * @param   x0: Output window x start
 * 
 * @param   y0: Output window y start
 * 
 * @param   width: Output window width
 * 
 * @param   height: Output window height
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetOutputWindow(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height)
{
    uint16_t x1;
    uint16_t y1;
    uint8_t temp;
    x1 = x0 + width / 2;
    y1 = y0 + height / 2;

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x01);
    /* Read the before value of Vref  */
    OV2640_SCCB_ReadRegCallback(ov2640Info.addr, 0x03, &temp);

    temp &= 0xF0;
    temp |= ((y1 & 0x03) << 2) | (y0 & 0x03);
    /* Set the lowest two bits of start and end of Vref  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x03, temp);

    /* Set the high 8 bits of Vref's start  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x19, y0 >> 2);
    /* Set the high 8 bits of Vref's end  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x1A, y1 >> 2);

    /* Read the before value of Href  */
    OV2640_SCCB_ReadRegCallback(ov2640Info.addr, 0x32, &temp);

    temp &= 0xC0;
    temp |= ((x1 & 0x07) << 3) | (x0 & 0x07);

    /* Set the lowest three bits of start and end of Href  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x32, temp);
    /* Set the high 8 bits of Href's start  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x17, x0 >> 3);
    /* Set the high 8 bits of Href's end  */
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x18, x1 >> 3);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 output size of image
 *
 * @param   width: Image horizontal size
 * 
 * @param   height: Image vertical size
 * 
 * @retval  OV2640 status
 * 
 * @note    The image size should be the multiple of 4
 */
uint32_t OV2640_SetImageSize(uint16_t width, uint16_t height)
{
    uint16_t hSize;
    uint16_t vSize;
    uint8_t temp;

    if ((width % 4 != 0) || (height % 4 != 0))
    {
        return OV2640_STATUS_ERROR;
    }

    hSize = width / 4;
    vSize = height / 4;

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x50, (hSize & 0x00));
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x5A, (hSize &  0xFF));
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x5B, (vSize & 0xFF));
    temp = ((hSize >> 8) & 0x03);
    temp |= ((vSize >> 6) & 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x5C, temp);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x00);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set the image window size
 * 
 * @param   offx: XOFF register value
 * 
 * @param   offy: YOFF register value
 * 
 * @param   width: Image horizontal size
 * 
 * @param   height: Image vertical size
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetImageWindow(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t hSize;
    uint16_t vSize;
    uint8_t temp;

    if ((width % 4 != 0) || (height % 4 != 0))
    {
        return OV2640_STATUS_ERROR;
    }

    hSize = width / 4;
    vSize = height / 4;
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x51, hSize & 0xFF);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x52, vSize & 0xFF);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x53, offx & 0xFF);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x54, offy & 0xFF);

    temp = ((vSize >> 1) & 0x80);
    temp |= ((offy >> 4) & 0x70);
    temp |= ((hSize >> 5) & 0x08);
    temp |= ((offx >> 8) & 0x07);

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x55, temp);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x57, (hSize >> 2) & 0x80);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x00);

    return OV2640_STATUS_OK;
}

/**
 * @brief   Set OV2640 image size, that the output resolution of the selected image format
 * 
 * @param   width: Image horizontal size
 * 
 * @param   height: Image vertical size
 * 
 * @retval  OV2640 status
 */
uint32_t OV2640_SetImageSizeOut(uint16_t width, uint16_t height)
{
    uint8_t temp;

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xFF, 0x00);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x04);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xC0, (width >> 3) & 0xFF);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xC1, (height >> 3) & 0xFF);

    temp = ((width & 0x07) << 3);
    temp |= (height & 0x07);
    temp |= ((width >> 4) & 0x80);

    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0x8C, temp);
    OV2640_SCCB_WriteRegCallback(ov2640Info.addr, 0xE0, 0x00);

    return OV2640_STATUS_OK;
}

/**
 * @brief   OV2640 delay
 *
 * @param   delay: Delay in ms
 *
 * @retval  None
 */
__weak void OV2640_DelayCallback(uint32_t delay)
{
    UNUSED(delay);
}

/**
 * @brief   Initializes SCCB
 *
 * @param   None
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t OV2640_SCCB_InitCallback(void)
{
    return 0;
}

/**
 * @brief   De-initializes SCCB
 *
 * @param   None
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t OV2640_SCCB_UninitCallback(void)
{
    return 0;
}

/**
 * @brief   Read a register
 *
 * @param   addr: SCCB address
 *
 * @param   regAddr: Register address
 *
 * @param   regVal: Pointer to the register value
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t OV2640_SCCB_ReadRegCallback(uint8_t addr, uint8_t regAddr, uint8_t *regVal)
{
    UNUSED(addr);
    UNUSED(regAddr);
    UNUSED(regVal);

    return 0;
}

/**
 * @brief   Write a value to a register
 *
 * @param   addr: SCCB address
 *
 * @param   regAddr: Register address
 *
 * @param   regVal: Value to be written
 *
 * @retval  0 if OK, -1 if ERROR
 */
__weak int32_t OV2640_SCCB_WriteRegCallback(uint8_t addr, uint8_t regAddr, uint8_t regVal)
{
    UNUSED(addr);
    UNUSED(regAddr);
    UNUSED(regVal);

    return 0;
}
