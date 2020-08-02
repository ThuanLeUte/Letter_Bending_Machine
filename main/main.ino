#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "stepper_control.h"
#include "execute_data.h"
#include "uart_communication.h"
#include "main.h"
#include "damos_ram.h"
#include "board_define.h"
#include "bsp.h"

//-------------------STEPPER_MOVE (11)--------------//
/*

  Micro-Stepp:800 step/vòng
  Speed: 7000=> 17.5 vòng/giây.
  Ti so: 1/18 => Speed = 0.972 vong/giay
  Run: 7200 step => 1 vòng
*/

//-------------------STEPPER_CUT (12)--------------//
/*
  Micro-Stepp:400 step/vòng
  1000=> 2.5 vong/s
  ti so => 0.07 vong
  0.7 vong/s
  Speed: 7000=> 17.5 vòng/giây.
  Ti so: 1/36 => Speed = 0.486 vong/giay
  Run: 14400 => 1 vòng =>360
  Run 43200 > 1 vong lon =>360
  1 do => 120 step
  Center to Home: 42 do 
*/

bool Emergency_Check();

void setup()
{
  Board_Setup();

  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

//  stepper_setup();

  attachInterrupt(2, Pause_Push, FALLING);    // Pin 21 Push go to LOW
  attachInterrupt(3, Emergency_Push, RISING); // Pin 20 Push go to HIGH
  attachInterrupt(4, Stop_Push, RISING);      // Pin 19 Push go to HIGH
  attachInterrupt(5, Start_Push, FALLING);    // Pin 18 Push go to LOW

  // initialize Timer1
  cli();      // disable global interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;
  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);
  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);
  // enable global interrupts:
  sei();
}

void InitVariables()
{
  NumHoles_AlreadyRun_xdu32 = 0;
  Appl_NumHolesFromAToB_xdu8 = 0;
  data_software = "";
  Appl_NoMaterial_xdu = false;
  Appl_NoMaterialFirstCallCapture_xdu = false;
  Appl_Second_xdu8 = 0;
  Appl_CutterBackwardTrigger_xdu = false;
  Appl_Forward_Trigger_xdu = false;
}

void loop()
{
  switch (Appl_SystemState_xdu8)
  {
  case INIT_STATE:
    Home_All(); //Home Cut and Move
    InitVariables();

    Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE;

    Serial.println(7);
    Serial3.println("--------------SystemState moved to RECIEVE_STATE----------------------------");
    
    break;
  case RECIEVE_AND_RUNNING_STATE:
    receive_data();

    if (Emergency_Check() == true)
    {
      Appl_SystemState_xdu8 = EMERGENCY_STATE;
      Serial.println(5);
      Serial3.println("--------------SystemState moved to EMERGENCY_STATE------------------------");
    }

    if (Appl_FinishTransfer_xdu == true and Appl_EmergencyHold_xdu == false)
    {
      Appl_SystemState_xdu8 = FINISH_STATE;
      Serial.println(8);
      Serial3.println("--------------SystemState moved to FINISH_STATE---------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case FINISH_LETTER_STATE:
    digitalWrite(MATERIAL_STATUS, HIGH);

    if (digitalRead(BUTTON_START_PIN) == 0)
    {
      digitalWrite(MATERIAL_STATUS, LOW);
      Serial3.println("start press");
      Execute_Forward("-200");
      Serial.println(1);
      Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE;
      Serial3.println("--------------SystemState moved to RECIEVE_STATE--------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case FINISH_STATE:
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
      Appl_SystemState_xdu8 = INIT_STATE;
      Appl_FinishTransfer_xdu = false;
      Serial.println(6);
      Serial3.println("--------------SystemState moved to INIT STATE-----------------------------");
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;
      Serial3.println("--------------SystemState moved to STOP_BUTTON_PRESS_STATE----------------");
    }

    break;
  case STOP_BUTTON_PRESS_STATE:
    delay(1000);
    Appl_SystemState_xdu8 = INIT_STATE;

    Serial.println(6);
    Serial3.println("--------------SystemState moved to INIT STATE-------------------------------");

    break;
  case EMERGENCY_STATE:
    if (Emergency_Check() == false)
    {
      Appl_SystemState_xdu8 = INIT_STATE;
      delay(1000);
      Serial.println(6);
      Serial3.println("--------------SystemState moved to INIT_STATE-----------------------------");
    }

    break;
  default:
    break;
  }
}

void Stop_Push()
{
  NumHoles_AlreadyRun_xdu32 = 0;
  if (Appl_EmergencyHold_xdu == true)
  {
    Serial3.println("Emergency_Push");
  }
  else
  {
    Stop();
    Appl_ButtonStopPress_xdu = true;
    Appl_ButtonStartPress_xdu = false;
    Appl_StartRunning_xdu = false;
  }
}

void Emergency_Push()
{
  Emergency();
}

void Pause_Push()
{
  if (Appl_EmergencyHold_xdu == true)
  {
    Serial3.println("Emergency_Push");
  }
  else
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
}

void Start_Push()
{
  if (Appl_EmergencyHold_xdu == true)
  {
    Serial3.println("Emergency_Push");
  }
  else
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
}

bool Emergency_Check()
{
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
  {
    Appl_EmergencyHold_xdu = true;
    Brushless_Off();
    digitalWrite(STEPPER_MOVE_ENA_PIN, LOW);
    digitalWrite(STEPPER_CUT_ENA_PIN, LOW);
    return true;
  }
  else
  {
    Appl_EmergencyHold_xdu = false;
    return false;
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (Appl_CutterBackwardTrigger_xdu == true)
  {
    Appl_Second_xdu8++;
    if (Appl_Second_xdu8 >= 8 and Appl_CutterBackwardTrigger_xdu == true)
    {
      Appl_SystemState_xdu8 = INIT_STATE;
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
