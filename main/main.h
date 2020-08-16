/**
 * @file       main.h
 * @copyright  Copyright (C) 2020 Thuan Le. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    major.minor.patch
 * @date       2020-08-16
 * @author     Thuan Le
 * @brief      Main file
 * @note       None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ----------------------------------------------------------- */
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief State Machine enum
 */
enum StateMachine
{
  SYS_INIT_STATE = 0,
  SYS_RECIEVE_AND_RUNNING_STATE,
  SYS_FINISH_LETTER_STATE,
  SYS_FINISH_STATE,
  SYS_STOP_BUTTON_PRESS_STATE,
  SYS_EMERGENCY_STATE,
  SYS_STATE_CNT
};

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */

#endif // __MAIN_H

/* End of file -------------------------------------------------------- */
