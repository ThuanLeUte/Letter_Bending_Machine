/**
 * @file       damos_ram.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Damos Ram
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DAMOS_RAM_H
#define __DAMOS_RAM_H

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */
extern float Appl_LengthToAlarm_B_fdu32;
extern float Appl_LengthToAlarm_A_fdu32;
extern float Appl_LengthToAlarm_fdu32;

extern uint32_t Appl_FisrtPulse_xdu8;
extern uint32_t Appl_LastPulse_xdu8;
extern uint32_t NumHolesAlreadyRun_xdu32;
extern uint32_t Holes_HaveToRun_xdu32;

extern uint8_t Appl_NumHolesFromAToB_xdu8;
extern uint8_t Appl_SystemState_xdu8;
extern uint8_t Appl_Second_xdu8;

extern bool Appl_NoMaterialTriger_xdu;
extern bool Appl_StartRunning_xdu;
extern bool Appl_ButtonStartPress_xdu;
extern bool Appl_ButtonPausePress_xdu;
extern bool Appl_ButtonPausePress_1_xdu;
extern bool Appl_ButtonStopPress_xdu;
extern bool Appl_FinishStateFirstCall_xdu;
extern bool Appl_NoMaterialFirstCallCapture_xdu;

extern bool Appl_DataLengthIsRemain_xdu;
extern bool Appl_NoMaterial_xdu;

extern bool Appl_CutterBackwardTrigger_xdu;
extern bool Appl_PauseTrigger_xdu;
extern bool Appl_Forward_Trigger_xdu;
extern bool Appl_FinishTransfer_xdu;

/* Public function definition ----------------------------------------- */

#endif // __DAMOS_RAM_H

/* End of file -------------------------------------------------------- */
