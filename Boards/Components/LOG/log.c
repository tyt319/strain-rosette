/**
  *
  * @file    log.c
  * @brief   LOG module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the LOG component.
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
  *
  */

/* Includes ------------------------------------------------------------------*/
#include "log.h"
#include <stdio.h>

#if (USE_LOG_COMPONENT == 1U)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static LOG_FUNC_T logCallback = &vprintf;

/* Exported functions --------------------------------------------------------*/

/**
 * @brief   Set log callback function
 *
 * @param   callback: Log callback function
 *
 * @retval  None
 */
void LOG_SetCallback(LOG_FUNC_T callback)
{
    logCallback = callback;
}

/**
 * @brief   Print message into log
 *
 * @param   format: Print message format
 * 
 * @retval  None
 */
void LOG_Print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    (*logCallback)(format, args);
    va_end(args);
    fflush(stdout);
}

#endif /* USE_LOG_COMPONENT */
