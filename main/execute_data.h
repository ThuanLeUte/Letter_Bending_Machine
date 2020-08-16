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
#define BRUSHLESS_SPEED     (160)
#define TIME_CUTTER         (5000)

/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Execute enum
 */
enum execute
{
  EXECUTE_FIRST,
  EXECUTE_END
};

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function definition ----------------------------------------- */
int  Execute_Manual(String Data_Input);
void EVxecute_Auto(String Data_Input);
void Execute_String(String Data_Input);
void Execute_Forward(String Data_Input);
void Execute_Cut(String Data_Input);
void Execute_Cut_First_End(String Data_Input, bool type);
void Stop();
void Start();
void Pause();
void Emergency();
void Button_Press_Buzzer();

#endif // __EXECUTE_DATA_H

/* End of file -------------------------------------------------------- */
