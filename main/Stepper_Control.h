/**
 * @file       stepper_control.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Stepper control
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __STEPPER_CONTROL_H
#define __STEPPER_CONTROL_H

/* Includes ----------------------------------------------------------- */
/* Public defines ----------------------------------------------------- */
#define MOVE_STEPPER_SPEED          (10000)
#define MOVE_STEPPER_SPEED_HOME     (15000)

/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function definition ----------------------------------------- */
void Home_Move_A();
void Home_Move_B();
void Home_All();
void Home_And_Center_Cutter();
void Cutter_Forward();
void Cutter_Backward();
void Cutter_Forward_Normal();
void Cutter_Backward_Normal();

void Home_Stepper_Cutter();
void Center_Stepper_Cutter();
void Angle_Cut(int Step_Remain);

void Brushless_Run(int Speed);
void Brushless_Off();
void Home_Move_A_First();
void Forward_Move_First();
void Forward_Move(unsigned long Step_Remain);

int Forward_Move_Holes(int Holes);
unsigned long Backward_Move(unsigned long Step_Remain);
void Forward_Move_1Step();
void stepper_setup(void);

#endif // __STEPPER_CONTROL_H

/* End of file -------------------------------------------------------- */
