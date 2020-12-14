/**
 * @file       main.c
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-14
 * @author     Thuan Le
 * @brief      Main file
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "bsp.h"
#include "stepper_control.h"
#include "execute_data.h"
#include "main.h"
#include "damos_ram.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
#define FSM_UPDATE_STATE(new_state)           \
do                                            \
{                                             \
  if (new_state < SYS_STATE_CNT){             \
    Appl_SystemState_xdu8 = new_state;        \
    LOG("[FSM] new state: %s", #new_state);   \
  }                                           \
} while (0);                                  \

/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
static inline void init_variables(void);
static inline void Stop();
static inline void Start();
static inline void Pause();

/* Function definitions ----------------------------------------------- */
void setup()
{
  bsp_init();        // Board support package init
  stepper_setup();   // Stepper setup
  GPIO_SET(BRUSHLESS_ENA_PIN , HIGH);
  
}

void loop()
{

  
  switch (Appl_SystemState_xdu8)
  {
  case SYS_INIT_STATE:
  {
    static bool first_call = true;
    if (first_call == true)
    {
      Init_Home();
      first_call = false;
    }
    else
    {
      Home_All(); // Home Cut and Move
    }

    init_variables();               // Init variables
    DATA_SEND_TO_PC(RES_READY_RECEIVE);
    FSM_UPDATE_STATE(SYS_RECIEVE_AND_RUNNING_STATE);
    break;
  }

  case SYS_RECIEVE_AND_RUNNING_STATE:
  {
    bsp_uart_receive();             // Wait data from PC

    // Finish of transfer data from PC to device
    if (Appl_FinishTransfer_xdu == true)
    {
      DATA_SEND_TO_PC(RES_LETTER_FINISHED);
      FSM_UPDATE_STATE(SYS_FINISH_STATE);
    }

    // Button stop pressed
    if (Appl_ButtonStopPress_xdu == true)
    {
      FSM_UPDATE_STATE(SYS_STOP_BUTTON_PRESS_STATE)
    }
    break;
  }

  case SYS_FINISH_LETTER_STATE:
  {
    GPIO_SET(MATERIAL_STATUS, HIGH);    // Buzzer on

    // Button start pressed
    if (IS_BUTTON_PRESSED(BUTTON_START_PIN))
    {
      GPIO_SET(MATERIAL_STATUS, LOW);
      LOG("Start press");
      Execute_Move("-200");
      DATA_SEND_TO_PC(RES_EXCECUTE_SUCCESS);
      FSM_UPDATE_STATE(SYS_RECIEVE_AND_RUNNING_STATE);
    }

    // Button stop pressed
    if (Appl_ButtonStopPress_xdu == true)
    {
      FSM_UPDATE_STATE(SYS_STOP_BUTTON_PRESS_STATE);
    }

    break;
  }
  case SYS_FINISH_STATE:
  {
    if (Appl_FinishStateFirstCall_xdu == true)
    {
      GPIO_SET(MATERIAL_STATUS, HIGH);
      DELAY(2000);
      GPIO_SET(MATERIAL_STATUS, LOW);
      Appl_FinishStateFirstCall_xdu = false;
    }

    if (IS_BUTTON_PRESSED(BUTTON_START_PIN))
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      DATA_SEND_TO_PC(RES_COMMAND_HOMING);
      FSM_UPDATE_STATE(SYS_INIT_STATE);
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      FSM_UPDATE_STATE(SYS_STOP_BUTTON_PRESS_STATE);
    }

    break;
  }
  case SYS_STOP_BUTTON_PRESS_STATE:
  {
    DELAY(1000);
    DATA_SEND_TO_PC(RES_COMMAND_HOMING);
    FSM_UPDATE_STATE(SYS_INIT_STATE);

    break;
  }
  
  default:
    break;
  }
}

void bsp_stop_push(void)
{
  NumHolesAlreadyRun_xdu32 = 0;
  Stop();
  Appl_ButtonStopPress_xdu = true;
  Appl_ButtonStartPress_xdu = false;
  Appl_StartRunning_xdu = false;
}

void bsp_pause_push(void)
{
  if (Appl_NoMaterial_xdu == false)
  {
    if (Appl_StartRunning_xdu == true and Appl_ButtonStopPress_xdu == false)
    {
      Pause();
      Appl_ButtonPausePress_xdu = true;
      Appl_ButtonPausePress_1_xdu = true;
      Appl_StartRunning_xdu = false;
      Appl_PauseTrigger_xdu = true;
    }
    else
    {
      LOG("Machine not start");
    }
  }
  else
  {
    GPIO_SET(MATERIAL_STATUS, LOW);
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
    LOG("Tắt còi");
  }
}

void bsp_start_push(void)
{
  LOG("Nhan nut Start");
  if (Appl_ButtonPausePress_xdu == true)
  {
    Appl_ButtonStartPress_xdu = true;
    if (Appl_Forward_Trigger_xdu == true)
    {
      DATA_SEND_TO_PC(RES_START);
    }
    else
    {
      Start();
    }
    Appl_ButtonPausePress_xdu = false;
    Appl_ButtonStopPress_xdu = false;
  }
  else
  {
    LOG("Pause not press");
  }
}

/* Private function  -------------------------------------------------- */
/**
 * @brief         Init variables
 * @param[in]     None
 * @attention     None
 * @return        None
 */
static inline void init_variables(void)
{
  NumHolesAlreadyRun_xdu32 = 0;
  Appl_NumHolesFromAToB_xdu8 = 0;
  g_uart_data_receive = "";
  Appl_NoMaterial_xdu = false;
  Appl_NoMaterialFirstCallCapture_xdu = false;
  Appl_Second_xdu8 = 0;
  Appl_CutterBackwardTrigger_xdu = false;
  Appl_Forward_Trigger_xdu = false;
}

void Stop()
{
  Brushless_Off();
  Cutter_Forward_Normal(); // Forward Cut
  Appl_Second_xdu8 = 0;
  Appl_CutterBackwardTrigger_xdu = false;
  DATA_SEND_TO_PC(RES_COMMAND_STOP);
}

void Start()
{
  DATA_SEND_TO_PC(RES_COMMAND_START);
}

void Pause()
{
  DATA_SEND_TO_PC(RES_COMMAND_PAUSE);
  Appl_SystemState_xdu8 = 1; //SYS_RECIEVE_AND_RUNNING_STATE
}
/**
 * @brief         Timer 1 interrupt
 * @param[in]     None
 * @attention     None
 * @return        None
 */
ISR(TIMER1_COMPA_vect)
{
 
}

/* End of file -------------------------------------------------------- */
