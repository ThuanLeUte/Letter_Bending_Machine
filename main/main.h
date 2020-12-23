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
uint32_t hole_index = 0;

float du_truoc = 0;
float du_sau = 0;
//float delta = 4.998;
float delta = 5.0;
float leng_per_step =  0.01118976558;

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
String data_software ;
String excute_string = "";
int data_index = 0;
bool auto_data_received = false;
float accel_factor = 200;
int cutter_step_per_degree = 240;
bool info = false;
bool info1 = false;
bool info2 = false;
int data_retry = 0;
bool last_index = false;
String buffer_display = "";
unsigned long last_minute = 0;
bool need_check_connection = false;
bool auto_receiving = false;
bool buzzer_needed = true;
bool info_receive = false;
bool info_stop = false;
bool info_emg = false;
bool info_finished = false;
bool info_init = false;
String display_string ="";
bool home_done = false;
#endif
