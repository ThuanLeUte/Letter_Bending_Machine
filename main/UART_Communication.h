/**
 * @file       uart_communication.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Uart communication
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __UART_COMMUNICATION_H
#define __UART_COMMUNICATION_H

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */
extern String data_software;

/* Public function definition ----------------------------------------- */
void receive_data();

#endif // __UART_COMMUNICATION_H

/* End of file -------------------------------------------------------- */
