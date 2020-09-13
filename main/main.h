#ifndef main_h
#define main_h

enum StateMachine
{
  INIT_STATE = 0,
  RECIEVE_AND_RUNNING_STATE,
  CONFIG_STATE,
  FINISH_LETTER_STATE,
  FINISH_STATE,
  STOP_BUTTON_PRESS_STATE,
  EMERGENCY_STATE
};

enum execute
{
  EXECUTE_FIRST,
  EXECUTE_END
};

float Appl_LengthToAlarm_B_fdu32 = 0;
float Appl_LengthToAlarm_A_fdu32 = 0;
float Appl_LengthToAlarm_fdu32 = 0;

uint32_t Appl_FisrtPulse_xdu8 = 0;
uint32_t Appl_LastPulse_xdu8 = 0;
uint32_t NumHoles_AlreadyRun_xdu32 = 0;
uint32_t Holes_HaveToRun_xdu32 = 0;

uint8_t Appl_NumHolesFromAToB_xdu8 = 0;
uint8_t Appl_SystemState_xdu8 = 0;
uint8_t Appl_Second_xdu8 = 0;

bool Appl_NoMaterialTriger_xdu = false;
bool Appl_StartRunning_xdu = false;
bool Appl_ButtonStartPress_xdu = false;
bool Appl_ButtonPausePress_xdu = false;
bool Appl_ButtonPausePress_1_xdu = false;
bool Appl_ButtonStopPress_xdu = false;
bool Appl_FinishStateFirstCall_xdu = true;
bool Appl_EmergencyHold_xdu = false;
bool Appl_NoMaterialFirstCallCapture_xdu = false;

bool Appl_DataLengthIsRemain_xdu = false;
bool Appl_NoMaterial_xdu = false;

bool Appl_CutterBackwardTrigger_xdu = false;
bool Appl_PauseTrigger_xdu = false;
bool Appl_Forward_Trigger_xdu = false;

#endif
