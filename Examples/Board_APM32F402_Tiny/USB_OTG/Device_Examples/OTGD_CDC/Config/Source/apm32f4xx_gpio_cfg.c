/**
 * @file        apm32f4xx_gpio_cfg.c
 *
 * @brief       This file provides configuration support for GPIO
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
#include "apm32f4xx_gpio_cfg.h"

/* Private includes *******************************************************/

/* Private macro **********************************************************/

/* Private typedef ********************************************************/

/* Private variables ******************************************************/

/* Private function prototypes ********************************************/

/* External variables *****************************************************/

/* External functions *****************************************************/

/**
 * @brief   GPIO configuration
 *
 * @param   None
 *
 * @retval  None
 */
void DAL_GPIO_Config(void)
{

    // 1. ���� AFIO ʱ�ӣ������һ����
    __DAL_RCM_AFIO_CLK_ENABLE();

    // 2. �����İ�ȫ������������ JTAG��ǿ�Ʊ��� SWD
    //    �궨��˵����AFIO_REMAP1_SWJ_CFG_JTAGDISABLE = �ر� JTAG������ SWD
    AFIO->REMAP1 = (AFIO->REMAP1 & ~AFIO_REMAP1_SWJ_CFG_Msk) | AFIO_REMAP1_SWJ_CFG_JTAGDISABLE;

    GPIO_InitTypeDef  GPIO_InitStruct = {0U};

    /* Configure the spowen pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_10;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
    
    /* Configure the led pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_15;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
    
    GPIO_InitStruct.Pin     = GPIO_PIN_2;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

    /* Configure the CS pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_1;              //CS7
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
    DAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�

    GPIO_InitStruct.Pin     = GPIO_PIN_11;              //CS8
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�

    GPIO_InitStruct.Pin     = GPIO_PIN_0;              //CS9
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�

    GPIO_InitStruct.Pin     = GPIO_PIN_12;              //CS10
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�

    // GPIO_InitStruct.Pin     = GPIO_PIN_10;              //CS11
    // GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    // GPIO_InitStruct.Pull    = GPIO_NOPULL;
    // GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    // DAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    // DAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�

    // GPIO_InitStruct.Pin     = GPIO_PIN_3;              //CS12
    // GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    // GPIO_InitStruct.Pull    = GPIO_NOPULL;
    // GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    // DAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    // DAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET); // ��ʼ״̬�����ߣ���ѡ�У�



    /* Configure the DRDY pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_0;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;         // ����ģʽ
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure the sync pin */
    GPIO_InitStruct.Pin     = GPIO_PIN_1;              //sync
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;     // �������
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FAST;
    DAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    DAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // ��ʼ״̬������
}
