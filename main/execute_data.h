/**
 * @file       execute_data.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Execute Data
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __EXECUTE_DATA_H
#define __EXECUTE_DATA_H

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"
#include "stepper_control.h"

/* Public defines ----------------------------------------------------- */
#define BRUSHLESS_SPEED     (40)
#define TIME_CUTTER         (1000)

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Execute enum
 */
typedef enum
{
  EXECUTE_FIRST,
  EXECUTE_END
}
execute_type_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function definition ----------------------------------------- */
/**
 * @brief         Execute manual command send from computer software
 * @param[in]     Data_Input      Data
 * @attention     None
 * @return        true of false
 */
int  Execute_Manual(String Data_Input);
/**
 * @brief         Excecute data send from computer software
 * @param[in]     Data_Input      Data
 * @attention     None
 * @return        None
 */
void Execute_String(String Data_Input);
/**
 * @brief         Excecute stepper move
 * @param[in]     Data_Input      Data
 * @attention     None
 * @return        None
 */
void Execute_Move(String Data_Input);
/**
 * @brief         Excecute cutter 
 * @param[in]     Data_Input      Data
 * @attention     None
 * @return        None
 */
void Execute_Cut(String Data_Input);
/**
 * @brief         Excecute cutter first or end letter
 * @param[in]     type            First of End letter
 * @attention     None
 * @return        None
 */
void Execute_Cut_First_End(String Data_Input, execute_type_t type);

#endif // __EXECUTE_DATA_H

/* End of file -------------------------------------------------------- */
