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
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function definition ----------------------------------------- */
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Home_Move_A();

/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Home_Move_B();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Home_All();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
 void Init_Home();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Home_And_Center_Cutter();

/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Cutter_Forward();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Cutter_Backward();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Cutter_Forward_Normal();

/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Cutter_Backward_Normal();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Home_Stepper_Cutter();

/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Center_Stepper_Cutter();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Angle_Cut(int Step_Remain);
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Brushless_Run(int Speed);
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Brushless_Off();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Forward_Move_First();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Forward_Move(unsigned long Step_Remain);
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
int Forward_Move_Holes(int Holes);
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
unsigned long Backward_Move(unsigned long Step_Remain);
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void Forward_Move_1Step();
/**
 * @brief         Board init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void stepper_setup(void);

#endif // __STEPPER_CONTROL_H

/* End of file -------------------------------------------------------- */
