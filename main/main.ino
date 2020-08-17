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
  // Board support package init
  bsp_init();

  // Stepper setup
   stepper_setup();
}

void loop()
{
  switch (Appl_SystemState_xdu8)
  {
  case SYS_INIT_STATE:
    Home_All(); // Home Cut and Move
    init_variables();

    Appl_SystemState_xdu8 = SYS_RECIEVE_AND_RUNNING_STATE;

    SERIAL_DATA_SEND(7);
    SERIAL_DATA_MONITOR("--------------SystemState moved to RECIEVE_STATE----------------------------");
    
    break;
  case SYS_RECIEVE_AND_RUNNING_STATE:
    bsp_uart_receive();

    if (Appl_FinishTransfer_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_FINISH_STATE;
      SERIAL_DATA_SEND(8);
      SERIAL_DATA_MONITOR("--------------SystemState moved to SYS_FINISH_STATE---------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      SERIAL_DATA_MONITOR("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_FINISH_LETTER_STATE:
    GPIO_SET(MATERIAL_STATUS, HIGH);

    if (IS_BUTTON_NOT_PRESSED(BUTTON_START_PIN))
    {
      GPIO_SET(MATERIAL_STATUS, LOW);
      SERIAL_DATA_MONITOR("start press");
      Execute_Move("-200");
      SERIAL_DATA_SEND(1);
      Appl_SystemState_xdu8 = SYS_RECIEVE_AND_RUNNING_STATE;
      SERIAL_DATA_MONITOR("--------------SystemState moved to RECIEVE_STATE--------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      SERIAL_DATA_MONITOR("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_FINISH_STATE:
    if (Appl_FinishStateFirstCall_xdu == true)
    {
      GPIO_SET(MATERIAL_STATUS, HIGH);
      DELAY(2000);
      GPIO_SET(MATERIAL_STATUS, LOW);
      Appl_FinishStateFirstCall_xdu = false;
    }

    if (IS_BUTTON_NOT_PRESSED(BUTTON_START_PIN))
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_SystemState_xdu8 = SYS_INIT_STATE;
      Appl_FinishTransfer_xdu = false;
      SERIAL_DATA_SEND(6);
      SERIAL_DATA_MONITOR("--------------SystemState moved to INIT STATE-----------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      SERIAL_DATA_MONITOR("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_STOP_BUTTON_PRESS_STATE:
    DELAY(1000);
    Appl_SystemState_xdu8 = SYS_INIT_STATE;

    SERIAL_DATA_SEND(6);
    SERIAL_DATA_MONITOR("--------------SystemState moved to INIT STATE-------------------------------");

    break;
  
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
      SERIAL_DATA_MONITOR("Machine not start");
    }
  }
  else
  {
    GPIO_SET(MATERIAL_STATUS, LOW);
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
    SERIAL_DATA_MONITOR("Tắt còi");
  }
}

void bsp_start_push(void)
{
  if (Appl_ButtonPausePress_xdu == true)
  {
    Appl_ButtonStartPress_xdu = true;
    if (Appl_Forward_Trigger_xdu == true)
    {
      SERIAL_DATA_SEND(13);
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
    SERIAL_DATA_MONITOR("Pause not press");
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
  SERIAL_DATA_SEND(2);
}

void Start()
{
  SERIAL_DATA_SEND(4);
}

void Pause()
{
  SERIAL_DATA_SEND(3);
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
