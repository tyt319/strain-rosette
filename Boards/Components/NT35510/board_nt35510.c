/**
 * @file        board_nt35510.c
 *
 * @brief       This file provides firmware functions to manage NT35510
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

/* Includes ***************************************************************/
#include "board_nt35510.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief  Initialize the NT35510
 *
 * @param  None
 *
 * @retval NT35510 status
 */
uint32_t NT35510_Init(void)
{
    NT35510_DelayCallback(100);

    /* Initialize NT35510 low level bus layer ----------------------------------*/
    if (NT35510_IO_InitCallback() != 0)
    {
        return NT35510_STATUS_ERROR;
    }

    return NT35510_STATUS_OK;
}

/**
 * @brief  DeInitialize the NT35510
 *
 * @param  None
 *
 * @retval NT35510 status
 */
uint32_t NT35510_DeInit(void)
{

    return NT35510_STATUS_OK;
}

/**
 * @brief  Configure the NT35510 registers
 * 
 * @param  None
 * 
 * @retval NT35510 status
 */
uint32_t NT35510_RegConfig(void)
{
    /* Initialize NT35510 */
    /* Page 1 */
    NT35510_WriteCmdDataCallback(0xF000, 0x0055);
    NT35510_WriteCmdDataCallback(0xF001, 0x00AA);
    NT35510_WriteCmdDataCallback(0xF002, 0x0052);
    NT35510_WriteCmdDataCallback(0xF003, 0x0008);
    NT35510_WriteCmdDataCallback(0xF004, 0x0001);

    /* Set AVDD Voltage to 5.2V */
    NT35510_WriteCmdDataCallback(0xB000, 0x000D);
    NT35510_WriteCmdDataCallback(0xB001, 0x000D);
    NT35510_WriteCmdDataCallback(0xB002, 0x000D);

    /* Set AVEE Voltage to 5.2V */
    NT35510_WriteCmdDataCallback(0xB100, 0x000D);
    NT35510_WriteCmdDataCallback(0xB101, 0x000D);
    NT35510_WriteCmdDataCallback(0xB102, 0x000D);

    /* Set VCL Voltage to -2.5V */
    NT35510_WriteCmdDataCallback(0xB200, 0x0000);
    NT35510_WriteCmdDataCallback(0xB201, 0x0000);
    NT35510_WriteCmdDataCallback(0xB202, 0x0000);

    /* Set AVDD Ratio */
    NT35510_WriteCmdDataCallback(0xB600, 0x0044);
    NT35510_WriteCmdDataCallback(0xB601, 0x0044);
    NT35510_WriteCmdDataCallback(0xB602, 0x0044);

    /* Set AVEE Ratio */
    NT35510_WriteCmdDataCallback(0xB700, 0x0034);
    NT35510_WriteCmdDataCallback(0xB701, 0x0034);
    NT35510_WriteCmdDataCallback(0xB702, 0x0034);

    /* Set VCL Voltage to -2.5V */
    NT35510_WriteCmdDataCallback(0xB800, 0x0034);
    NT35510_WriteCmdDataCallback(0xB801, 0x0034);
    NT35510_WriteCmdDataCallback(0xB802, 0x0034);

    /* Control VGH booster voltage rang */
    NT35510_WriteCmdDataCallback(0xBF00, 0x0001);

    /* Set VGH Voltage to 15V */
    NT35510_WriteCmdDataCallback(0xB300, 0x000F);
    NT35510_WriteCmdDataCallback(0xB301, 0x000F);
    NT35510_WriteCmdDataCallback(0xB302, 0x000F);

    /* Set VGH Ratio */
    NT35510_WriteCmdDataCallback(0xB900, 0x0034);
    NT35510_WriteCmdDataCallback(0xB901, 0x0034);
    NT35510_WriteCmdDataCallback(0xB902, 0x0034);

    /* Set VGL_REG Voltage to -10V */
    NT35510_WriteCmdDataCallback(0xB500, 0x0008);
    NT35510_WriteCmdDataCallback(0xB501, 0x0008);
    NT35510_WriteCmdDataCallback(0xB502, 0x0008);

    NT35510_WriteCmdDataCallback(0xC200, 0x0003);

    /* Set VGLX Ratio */
    NT35510_WriteCmdDataCallback(0xBA00, 0x0034);
    NT35510_WriteCmdDataCallback(0xBA01, 0x0034);
    NT35510_WriteCmdDataCallback(0xBA02, 0x0034);

    /* Set VGMP and VGSP Voltage */
    NT35510_WriteCmdDataCallback(0xBC00, 0x0000);
    NT35510_WriteCmdDataCallback(0xBC01, 0x0078);
    NT35510_WriteCmdDataCallback(0xBC02, 0x0000);

    /* Set VGMN and VGSN Voltage */
    NT35510_WriteCmdDataCallback(0xBD00, 0x0000);
    NT35510_WriteCmdDataCallback(0xBD01, 0x0078);
    NT35510_WriteCmdDataCallback(0xBD02, 0x0000);

    /* Set VCOM Offset Voltage */
    NT35510_WriteCmdDataCallback(0xBE00, 0x0000);
    NT35510_WriteCmdDataCallback(0xBE01, 0x0064);

    /* Set Gamma (R+) */
    NT35510_WriteCmdDataCallback(0xD100, 0x0000);
    NT35510_WriteCmdDataCallback(0xD101, 0x0033);
    NT35510_WriteCmdDataCallback(0xD102, 0x0000);
    NT35510_WriteCmdDataCallback(0xD103, 0x0034);
    NT35510_WriteCmdDataCallback(0xD104, 0x0000);
    NT35510_WriteCmdDataCallback(0xD105, 0x003A);
    NT35510_WriteCmdDataCallback(0xD106, 0x0000);
    NT35510_WriteCmdDataCallback(0xD107, 0x004A);
    NT35510_WriteCmdDataCallback(0xD108, 0x0000);
    NT35510_WriteCmdDataCallback(0xD109, 0x005C);
    NT35510_WriteCmdDataCallback(0xD10A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD10B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD10C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD10D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD10E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD10F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD110, 0x0001);
    NT35510_WriteCmdDataCallback(0xD111, 0x0013);
    NT35510_WriteCmdDataCallback(0xD112, 0x0001);
    NT35510_WriteCmdDataCallback(0xD113, 0x0054);
    NT35510_WriteCmdDataCallback(0xD114, 0x0001);
    NT35510_WriteCmdDataCallback(0xD115, 0x0082);
    NT35510_WriteCmdDataCallback(0xD116, 0x0001);
    NT35510_WriteCmdDataCallback(0xD117, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD118, 0x0002);
    NT35510_WriteCmdDataCallback(0xD119, 0x0000);
    NT35510_WriteCmdDataCallback(0xD11A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD11B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD11C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD11D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD11E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD11F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD120, 0x0002);
    NT35510_WriteCmdDataCallback(0xD121, 0x0084);
    NT35510_WriteCmdDataCallback(0xD122, 0x0002);
    NT35510_WriteCmdDataCallback(0xD123, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD124, 0x0002);
    NT35510_WriteCmdDataCallback(0xD125, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD126, 0x0002);
    NT35510_WriteCmdDataCallback(0xD127, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD128, 0x0002);
    NT35510_WriteCmdDataCallback(0xD129, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD12A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD12B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD12C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD12D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD12E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD12F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD130, 0x0003);
    NT35510_WriteCmdDataCallback(0xD131, 0x0033);
    NT35510_WriteCmdDataCallback(0xD132, 0x0003);
    NT35510_WriteCmdDataCallback(0xD133, 0x006D);

    /* Set Gamma (G+) */
    NT35510_WriteCmdDataCallback(0xD200, 0x0000);
    NT35510_WriteCmdDataCallback(0xD201, 0x0033);
    NT35510_WriteCmdDataCallback(0xD202, 0x0000);
    NT35510_WriteCmdDataCallback(0xD203, 0x0034);
    NT35510_WriteCmdDataCallback(0xD204, 0x0000);
    NT35510_WriteCmdDataCallback(0xD205, 0x003A);
    NT35510_WriteCmdDataCallback(0xD206, 0x0000);
    NT35510_WriteCmdDataCallback(0xD207, 0x004A);
    NT35510_WriteCmdDataCallback(0xD208, 0x0000);
    NT35510_WriteCmdDataCallback(0xD209, 0x005C);
    NT35510_WriteCmdDataCallback(0xD20A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD20B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD20C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD20D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD20E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD20F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD210, 0x0001);
    NT35510_WriteCmdDataCallback(0xD211, 0x0013);
    NT35510_WriteCmdDataCallback(0xD212, 0x0001);
    NT35510_WriteCmdDataCallback(0xD213, 0x0054);
    NT35510_WriteCmdDataCallback(0xD214, 0x0001);
    NT35510_WriteCmdDataCallback(0xD215, 0x0082);
    NT35510_WriteCmdDataCallback(0xD216, 0x0001);
    NT35510_WriteCmdDataCallback(0xD217, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD218, 0x0002);
    NT35510_WriteCmdDataCallback(0xD219, 0x0000);
    NT35510_WriteCmdDataCallback(0xD21A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD21B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD21C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD21D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD21E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD21F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD220, 0x0002);
    NT35510_WriteCmdDataCallback(0xD221, 0x0084);
    NT35510_WriteCmdDataCallback(0xD222, 0x0002);
    NT35510_WriteCmdDataCallback(0xD223, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD224, 0x0002);
    NT35510_WriteCmdDataCallback(0xD225, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD226, 0x0002);
    NT35510_WriteCmdDataCallback(0xD227, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD228, 0x0002);
    NT35510_WriteCmdDataCallback(0xD229, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD22A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD22B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD22C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD22D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD22E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD22F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD230, 0x0003);
    NT35510_WriteCmdDataCallback(0xD231, 0x0033);
    NT35510_WriteCmdDataCallback(0xD232, 0x0003);
    NT35510_WriteCmdDataCallback(0xD233, 0x006D);

    /* Set Gamma (B+) */
    NT35510_WriteCmdDataCallback(0xD300, 0x0000);
    NT35510_WriteCmdDataCallback(0xD301, 0x0033);
    NT35510_WriteCmdDataCallback(0xD302, 0x0000);
    NT35510_WriteCmdDataCallback(0xD303, 0x0034);
    NT35510_WriteCmdDataCallback(0xD304, 0x0000);
    NT35510_WriteCmdDataCallback(0xD305, 0x003A);
    NT35510_WriteCmdDataCallback(0xD306, 0x0000);
    NT35510_WriteCmdDataCallback(0xD307, 0x004A);
    NT35510_WriteCmdDataCallback(0xD308, 0x0000);
    NT35510_WriteCmdDataCallback(0xD309, 0x005C);
    NT35510_WriteCmdDataCallback(0xD30A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD30B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD30C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD30D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD30E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD30F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD310, 0x0001);
    NT35510_WriteCmdDataCallback(0xD311, 0x0013);
    NT35510_WriteCmdDataCallback(0xD312, 0x0001);
    NT35510_WriteCmdDataCallback(0xD313, 0x0054);
    NT35510_WriteCmdDataCallback(0xD314, 0x0001);
    NT35510_WriteCmdDataCallback(0xD315, 0x0082);
    NT35510_WriteCmdDataCallback(0xD316, 0x0001);
    NT35510_WriteCmdDataCallback(0xD317, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD318, 0x0002);
    NT35510_WriteCmdDataCallback(0xD319, 0x0000);
    NT35510_WriteCmdDataCallback(0xD31A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD31B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD31C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD31D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD31E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD31F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD320, 0x0002);
    NT35510_WriteCmdDataCallback(0xD321, 0x0084);
    NT35510_WriteCmdDataCallback(0xD322, 0x0002);
    NT35510_WriteCmdDataCallback(0xD323, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD324, 0x0002);
    NT35510_WriteCmdDataCallback(0xD325, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD326, 0x0002);
    NT35510_WriteCmdDataCallback(0xD327, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD328, 0x0002);
    NT35510_WriteCmdDataCallback(0xD329, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD32A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD32B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD32C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD32D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD32E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD32F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD330, 0x0003);
    NT35510_WriteCmdDataCallback(0xD331, 0x0033);
    NT35510_WriteCmdDataCallback(0xD332, 0x0003);
    NT35510_WriteCmdDataCallback(0xD333, 0x006D);

    /* Set Gamma (R-) */
    NT35510_WriteCmdDataCallback(0xD400, 0x0000);
    NT35510_WriteCmdDataCallback(0xD401, 0x0033);
    NT35510_WriteCmdDataCallback(0xD402, 0x0000);
    NT35510_WriteCmdDataCallback(0xD403, 0x0034);
    NT35510_WriteCmdDataCallback(0xD404, 0x0000);
    NT35510_WriteCmdDataCallback(0xD405, 0x003A);
    NT35510_WriteCmdDataCallback(0xD406, 0x0000);
    NT35510_WriteCmdDataCallback(0xD407, 0x004A);
    NT35510_WriteCmdDataCallback(0xD408, 0x0000);
    NT35510_WriteCmdDataCallback(0xD409, 0x005C);
    NT35510_WriteCmdDataCallback(0xD40A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD40B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD40C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD40D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD40E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD40F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD410, 0x0001);
    NT35510_WriteCmdDataCallback(0xD411, 0x0013);
    NT35510_WriteCmdDataCallback(0xD412, 0x0001);
    NT35510_WriteCmdDataCallback(0xD413, 0x0054);
    NT35510_WriteCmdDataCallback(0xD414, 0x0001);
    NT35510_WriteCmdDataCallback(0xD415, 0x0082);
    NT35510_WriteCmdDataCallback(0xD416, 0x0001);
    NT35510_WriteCmdDataCallback(0xD417, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD418, 0x0002);
    NT35510_WriteCmdDataCallback(0xD419, 0x0000);
    NT35510_WriteCmdDataCallback(0xD41A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD41B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD41C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD41D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD41E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD41F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD420, 0x0002);
    NT35510_WriteCmdDataCallback(0xD421, 0x0084);
    NT35510_WriteCmdDataCallback(0xD422, 0x0002);
    NT35510_WriteCmdDataCallback(0xD423, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD424, 0x0002);
    NT35510_WriteCmdDataCallback(0xD425, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD426, 0x0002);
    NT35510_WriteCmdDataCallback(0xD427, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD428, 0x0002);
    NT35510_WriteCmdDataCallback(0xD429, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD42A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD42B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD42C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD42D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD42E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD42F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD430, 0x0003);
    NT35510_WriteCmdDataCallback(0xD431, 0x0033);
    NT35510_WriteCmdDataCallback(0xD432, 0x0003);
    NT35510_WriteCmdDataCallback(0xD433, 0x006D);

    /* Set Gamma (G-) */
    NT35510_WriteCmdDataCallback(0xD500, 0x0000);
    NT35510_WriteCmdDataCallback(0xD501, 0x0033);
    NT35510_WriteCmdDataCallback(0xD502, 0x0000);
    NT35510_WriteCmdDataCallback(0xD503, 0x0034);
    NT35510_WriteCmdDataCallback(0xD504, 0x0000);
    NT35510_WriteCmdDataCallback(0xD505, 0x003A);
    NT35510_WriteCmdDataCallback(0xD506, 0x0000);
    NT35510_WriteCmdDataCallback(0xD507, 0x004A);
    NT35510_WriteCmdDataCallback(0xD508, 0x0000);
    NT35510_WriteCmdDataCallback(0xD509, 0x005C);
    NT35510_WriteCmdDataCallback(0xD50A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD50B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD50C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD50D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD50E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD50F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD510, 0x0001);
    NT35510_WriteCmdDataCallback(0xD511, 0x0013);
    NT35510_WriteCmdDataCallback(0xD512, 0x0001);
    NT35510_WriteCmdDataCallback(0xD513, 0x0054);
    NT35510_WriteCmdDataCallback(0xD514, 0x0001);
    NT35510_WriteCmdDataCallback(0xD515, 0x0082);
    NT35510_WriteCmdDataCallback(0xD516, 0x0001);
    NT35510_WriteCmdDataCallback(0xD517, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD518, 0x0002);
    NT35510_WriteCmdDataCallback(0xD519, 0x0000);
    NT35510_WriteCmdDataCallback(0xD51A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD51B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD51C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD51D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD51E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD51F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD520, 0x0002);
    NT35510_WriteCmdDataCallback(0xD521, 0x0084);
    NT35510_WriteCmdDataCallback(0xD522, 0x0002);
    NT35510_WriteCmdDataCallback(0xD523, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD524, 0x0002);
    NT35510_WriteCmdDataCallback(0xD525, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD526, 0x0002);
    NT35510_WriteCmdDataCallback(0xD527, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD528, 0x0002);
    NT35510_WriteCmdDataCallback(0xD529, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD52A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD52B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD52C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD52D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD52E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD52F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD530, 0x0003);
    NT35510_WriteCmdDataCallback(0xD531, 0x0033);
    NT35510_WriteCmdDataCallback(0xD532, 0x0003);
    NT35510_WriteCmdDataCallback(0xD533, 0x006D);

    /* Set Gamma (B-) */
    NT35510_WriteCmdDataCallback(0xD600, 0x0000);
    NT35510_WriteCmdDataCallback(0xD601, 0x0033);
    NT35510_WriteCmdDataCallback(0xD602, 0x0000);
    NT35510_WriteCmdDataCallback(0xD603, 0x0034);
    NT35510_WriteCmdDataCallback(0xD604, 0x0000);
    NT35510_WriteCmdDataCallback(0xD605, 0x003A);
    NT35510_WriteCmdDataCallback(0xD606, 0x0000);
    NT35510_WriteCmdDataCallback(0xD607, 0x004A);
    NT35510_WriteCmdDataCallback(0xD608, 0x0000);
    NT35510_WriteCmdDataCallback(0xD609, 0x005C);
    NT35510_WriteCmdDataCallback(0xD60A, 0x0000);
    NT35510_WriteCmdDataCallback(0xD60B, 0x0081);
    NT35510_WriteCmdDataCallback(0xD60C, 0x0000);
    NT35510_WriteCmdDataCallback(0xD60D, 0x00A6);
    NT35510_WriteCmdDataCallback(0xD60E, 0x0000);
    NT35510_WriteCmdDataCallback(0xD60F, 0x00E5);
    NT35510_WriteCmdDataCallback(0xD610, 0x0001);
    NT35510_WriteCmdDataCallback(0xD611, 0x0013);
    NT35510_WriteCmdDataCallback(0xD612, 0x0001);
    NT35510_WriteCmdDataCallback(0xD613, 0x0054);
    NT35510_WriteCmdDataCallback(0xD614, 0x0001);
    NT35510_WriteCmdDataCallback(0xD615, 0x0082);
    NT35510_WriteCmdDataCallback(0xD616, 0x0001);
    NT35510_WriteCmdDataCallback(0xD617, 0x00CA);
    NT35510_WriteCmdDataCallback(0xD618, 0x0002);
    NT35510_WriteCmdDataCallback(0xD619, 0x0000);
    NT35510_WriteCmdDataCallback(0xD61A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD61B, 0x0001);
    NT35510_WriteCmdDataCallback(0xD61C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD61D, 0x0034);
    NT35510_WriteCmdDataCallback(0xD61E, 0x0002);
    NT35510_WriteCmdDataCallback(0xD61F, 0x0067);
    NT35510_WriteCmdDataCallback(0xD620, 0x0002);
    NT35510_WriteCmdDataCallback(0xD621, 0x0084);
    NT35510_WriteCmdDataCallback(0xD622, 0x0002);
    NT35510_WriteCmdDataCallback(0xD623, 0x00A4);
    NT35510_WriteCmdDataCallback(0xD624, 0x0002);
    NT35510_WriteCmdDataCallback(0xD625, 0x00B7);
    NT35510_WriteCmdDataCallback(0xD626, 0x0002);
    NT35510_WriteCmdDataCallback(0xD627, 0x00CF);
    NT35510_WriteCmdDataCallback(0xD628, 0x0002);
    NT35510_WriteCmdDataCallback(0xD629, 0x00DE);
    NT35510_WriteCmdDataCallback(0xD62A, 0x0002);
    NT35510_WriteCmdDataCallback(0xD62B, 0x00F2);
    NT35510_WriteCmdDataCallback(0xD62C, 0x0002);
    NT35510_WriteCmdDataCallback(0xD62D, 0x00FE);
    NT35510_WriteCmdDataCallback(0xD62E, 0x0003);
    NT35510_WriteCmdDataCallback(0xD62F, 0x0010);
    NT35510_WriteCmdDataCallback(0xD630, 0x0003);
    NT35510_WriteCmdDataCallback(0xD631, 0x0033);
    NT35510_WriteCmdDataCallback(0xD632, 0x0003);
    NT35510_WriteCmdDataCallback(0xD633, 0x006D);

    /* Set Page 0 */
    NT35510_WriteCmdDataCallback(0xF000, 0x0055);
    NT35510_WriteCmdDataCallback(0xF001, 0x00AA);
    NT35510_WriteCmdDataCallback(0xF002, 0x0052);
    NT35510_WriteCmdDataCallback(0xF003, 0x0008);
    NT35510_WriteCmdDataCallback(0xF004, 0x0000);

    /* Set pixel to 480x800 */
    NT35510_WriteCmdDataCallback(0xB500, 0x0050);

    /* Dispay control */
    NT35510_WriteCmdDataCallback(0xB100, 0x00CC);
    NT35510_WriteCmdDataCallback(0xB101, 0x0000);

    /* Source hold time */
    NT35510_WriteCmdDataCallback(0xB600, 0x0005);

    /* Gate EQ control */
    NT35510_WriteCmdDataCallback(0xB700, 0x0077);
    NT35510_WriteCmdDataCallback(0xB701, 0x0077);

    /* Source EQ control */
    NT35510_WriteCmdDataCallback(0xB800, 0x0001);
    NT35510_WriteCmdDataCallback(0xB801, 0x0003);
    NT35510_WriteCmdDataCallback(0xB802, 0x0003);
    NT35510_WriteCmdDataCallback(0xB803, 0x0003);

    /* Inversion mode */
    NT35510_WriteCmdDataCallback(0xBC00, 0x0002);
    NT35510_WriteCmdDataCallback(0xBC01, 0x0000);
    NT35510_WriteCmdDataCallback(0xBC02, 0x0000);

    /* Frame rate */
    NT35510_WriteCmdDataCallback(0xBD00, 0x0001);
    NT35510_WriteCmdDataCallback(0xBD01, 0x0084);
    NT35510_WriteCmdDataCallback(0xBD02, 0x001c);
    NT35510_WriteCmdDataCallback(0xBD03, 0x001c);
    NT35510_WriteCmdDataCallback(0xBD04, 0x0000);

    /* LGD timing control */
    NT35510_WriteCmdDataCallback(0xC900, 0x00D0);
    NT35510_WriteCmdDataCallback(0xC901, 0x0002);
    NT35510_WriteCmdDataCallback(0xC902, 0x0050);
    NT35510_WriteCmdDataCallback(0xC903, 0x0050);
    NT35510_WriteCmdDataCallback(0xC904, 0x0050);

    NT35510_WriteCmdDataCallback(0x3600, 0x0000);
    NT35510_WriteCmdDataCallback(0x3500, 0x0000);

    NT35510_WriteCmdDataCallback(0xFF00, 0x00AA);
    NT35510_WriteCmdDataCallback(0xFF01, 0x0055);
    NT35510_WriteCmdDataCallback(0xFF02, 0x0025);
    NT35510_WriteCmdDataCallback(0xFF03, 0x0001);

    NT35510_WriteCmdDataCallback(0xFC00, 0x0016);
    NT35510_WriteCmdDataCallback(0xFC01, 0x00A2);
    NT35510_WriteCmdDataCallback(0xFC02, 0x0026);
    NT35510_WriteCmdDataCallback(0x3A00, 0x0006);

    NT35510_WriteCmdDataCallback(0x3A00, 0x0055);

    NT35510_WriteCmdCallback(0x1100);

    NT35510_DelayCallback(500);

    NT35510_WriteCmdCallback(0x2900);

    return NT35510_STATUS_OK;
}

/**
 * @brief  NT35510 delay
 * 
 * @param  delay: Delay in ms
 * 
 * @retval None
 */
__weak void NT35510_DelayCallback(uint32_t delay)
{
    UNUSED(delay);
}

/**
 * @brief  Initialize the NT35510 interface
 *
 * @param  None
 *
 * @retval 0 if OK, -1 if ERROR
 */
__weak int32_t NT35510_IO_InitCallback(void)
{
    return 0;
}

/**
 * @brief  De-initialize the NT35510 interface
 *
 * @param  None
 *
 * @retval 0 if OK, -1 if ERROR
 */
__weak int32_t NT35510_IO_UninitCallback(void)
{
    return 0;
}

/**
 * @brief  Write command to the NT35510
 * 
 * @param  cmd: Command to write
 * 
 * @retval None
 */
__weak void NT35510_WriteCmdCallback(uint16_t cmd)
{
    UNUSED(cmd);
}

/**
 * @brief  Write data to the NT35510
 * 
 * @param  data: Data to write
 * 
 * @retval None
 */
__weak void NT35510_WriteDataCallback(uint16_t data)
{
    UNUSED(data);
}

/**
 * @brief  Write command and data to the NT35510
 * 
 * @param  cmd: Command to write
 * 
 * @param  data: Data to write
 * 
 * @retval None
 */
__weak void NT35510_WriteCmdDataCallback(uint16_t cmd, uint16_t data)
{
    UNUSED(cmd);
    UNUSED(data);
}

/**
 * @brief  Read data from the NT35510
 * 
 * @param  None
 * 
 * @retval Read data
 */
__weak uint16_t NT35510_ReadDataCallback(void)
{
    return 0;
}
