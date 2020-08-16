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
static void init_variables(void);

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
  Serial3.println("--------------SystemState moved to RECIEVE_STATE----------------------------");
    Home_All(); //Home Cut and Move
    init_variables();

    Appl_SystemState_xdu8 = SYS_RECIEVE_AND_RUNNING_STATE;

    Serial.println(7);
    Serial3.println("--------------SystemState moved to RECIEVE_STATE----------------------------");
    
    break;
  case SYS_RECIEVE_AND_RUNNING_STATE:
    bsp_uart_receive();

    if (Appl_FinishTransfer_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_FINISH_STATE;
      Serial.println(8);
      Serial3.println("--------------SystemState moved to SYS_FINISH_STATE---------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_FINISH_LETTER_STATE:
    digitalWrite(MATERIAL_STATUS, HIGH);

    if (digitalRead(BUTTON_START_PIN) == 0)
    {
      digitalWrite(MATERIAL_STATUS, LOW);
      Serial3.println("start press");
      Execute_Forward("-200");
      Serial.println(1);
      Appl_SystemState_xdu8 = SYS_RECIEVE_AND_RUNNING_STATE;
      Serial3.println("--------------SystemState moved to RECIEVE_STATE--------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_FINISH_STATE:
    if (Appl_FinishStateFirstCall_xdu == true)
    {
      digitalWrite(MATERIAL_STATUS, HIGH);
      delay(2000);
      digitalWrite(MATERIAL_STATUS, LOW);
      Appl_FinishStateFirstCall_xdu = false;
    }

    if (digitalRead(BUTTON_START_PIN) == 0)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_SystemState_xdu8 = SYS_INIT_STATE;
      Appl_FinishTransfer_xdu = false;
      Serial.println(6);
      Serial3.println("--------------SystemState moved to INIT STATE-----------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      Appl_SystemState_xdu8 = SYS_STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to SYS_STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case SYS_STOP_BUTTON_PRESS_STATE:
    delay(1000);
    Appl_SystemState_xdu8 = SYS_INIT_STATE;

    Serial.println(6);
    Serial3.println("--------------SystemState moved to INIT STATE-------------------------------");

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
      Serial3.println("Machine not start");
    }
  }
  else
  {
    digitalWrite(MATERIAL_STATUS, LOW);
    digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
    Serial3.println("Tắt còi");
  }
}

void bsp_start_push(void)
{
  if (Appl_ButtonPausePress_xdu == true)
  {
    Appl_ButtonStartPress_xdu = true;
    if (Appl_Forward_Trigger_xdu == true)
    {
      Serial.println(13);
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
    Serial3.println("Pause not press");
  }
}

/* Private function  -------------------------------------------------- */
/**
 * @brief         Init variables
 * @param[in]     None
 * @attention     None
 * @return        None
 */
static void init_variables(void)
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

/**
 * @brief         Timer 1 interrupt
 * @param[in]     None
 * @attention     None
 * @return        None
 */
ISR(TIMER1_COMPA_vect)
{
  if (Appl_CutterBackwardTrigger_xdu == true)
  {
    Appl_Second_xdu8++;
    if (Appl_Second_xdu8 >= 8 and Appl_CutterBackwardTrigger_xdu == true)
    {
      Appl_SystemState_xdu8 = SYS_INIT_STATE;
      Brushless_Off();
      Serial.println(12); // Send Error to PC
      Serial.println(2);  // Send Stop to PC
      Serial.println(6);  // Homing to PC
      Appl_Second_xdu8 = 0;
      Appl_CutterBackwardTrigger_xdu = false;
    }
  }
  else
  {
    Appl_Second_xdu8 = 0;
  }
}

/* End of file -------------------------------------------------------- */
