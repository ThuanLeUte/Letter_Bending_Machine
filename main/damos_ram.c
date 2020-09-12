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

/* Includes ----------------------------------------------------------- */
#include "damos_ram.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
float Appl_LengthToAlarm_B_fdu32    = 0;
float Appl_LengthToAlarm_A_fdu32    = 0;
float Appl_LengthToAlarm_fdu32      = 0;

uint32_t Appl_FisrtPulse_xdu8       = 0;
uint32_t Appl_LastPulse_xdu8        = 0;
uint32_t NumHolesAlreadyRun_xdu32  = 0;
uint32_t Holes_HaveToRun_xdu32      = 0;

uint8_t Appl_NumHolesFromAToB_xdu8  = 0;
uint8_t Appl_SystemState_xdu8       = 0;
uint8_t Appl_Second_xdu8            = 0;

bool Appl_NoMaterialTriger_xdu      = false;
bool Appl_StartRunning_xdu          = false;
bool Appl_ButtonStartPress_xdu      = false;
bool Appl_ButtonPausePress_xdu      = false;
bool Appl_ButtonPausePress_1_xdu    = false;
bool Appl_ButtonStopPress_xdu       = false;
bool Appl_FinishStateFirstCall_xdu  = true;

bool Appl_DataLengthIsRemain_xdu    = false;
bool Appl_NoMaterial_xdu            = false;

bool Appl_CutterBackwardTrigger_xdu = false;
bool Appl_PauseTrigger_xdu          = false;
bool Appl_Forward_Trigger_xdu       = false;

bool Appl_FinishTransfer_xdu        = false;
bool Appl_NoMaterialFirstCallCapture_xdu = false;

/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
