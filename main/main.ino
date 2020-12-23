

#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#
#include "Stepper_Control.h"
#include "Execute_Data.h"
#include "UART_Communication.h"
#include "main.h"

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

void setup()
{
  Board_Setup();
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

  STEPPER_MOVE.setEnablePin(STEPPER_MOVE_ENA_PIN);
  STEPPER_MOVE.setMaxSpeed(100000);

  STEPPER_CUT.setEnablePin(STEPPER_CUT_ENA_PIN);
  STEPPER_CUT.setMaxSpeed(100000);
  digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
  //
  //  attachInterrupt(2, Pause_Push, FALLING);    // Pin 21 Push go to LOW
  //  attachInterrupt(3, Emergency_Push, RISING); // Pin 20 Push go to HIGH
  //  attachInterrupt(4, Stop_Push, RISING);      // Pin 19 Push go to HIGH
  //  attachInterrupt(5, Start_Push, FALLING);    // Pin 18 Push go to LOW

  // initialize Timer1
  //  cli();      // disable global interrupts
  //  TCCR1A = 0; // set entire TCCR1A register to 0
  //  TCCR1B = 0; // same for TCCR1B
  //
  //  // set compare match register to desired timer count:
  //  OCR1A = 15624;
  //  // turn on CTC mode:
  //  TCCR1B |= (1 << WGM12);
  //  // Set CS10 and CS12 bits for 1024 prescaler:
  //  TCCR1B |= (1 << CS10);
  //  TCCR1B |= (1 << CS12);
  //  // enable timer compare interrupt:
  //  TIMSK1 |= (1 << OCIE1A);
  //  // enable global interrupts:
  //  sei();
  Appl_SystemState_xdu8 = INIT_STATE;
}
void InitVariables()
{
  delay(20);
  digitalWrite(OP4, HIGH);
  Appl_NumHolesFromAToB_xdu8 = 0;
  data_software = "";
  Appl_NoMaterial_xdu = false;
  Appl_NoMaterialFirstCallCapture_xdu = false;
  Appl_Second_xdu8 = 0;
  Appl_CutterBackwardTrigger_xdu = false;
  Appl_Forward_Trigger_xdu = false;
  hole_index = 0;
  du_truoc = 0;
  data_index = 0;
  auto_data_received = false;
  Appl_ButtonStopPress_xdu = false;
  Appl_EmergencyHold_xdu = false;
  auto_receiving = false;
}
void loop()
{
  if (Appl_SystemState_xdu8 == INIT_STATE)
  {
    Serial.println("LOG --------------Turned to INIT STATE...--------------");
    InitVariables();
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    delay(400);
    digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
    delay(400);
    digitalWrite(SOL_CLAMPER_PIN, HIGH);
    delay(400);
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    delay(400);
    digitalWrite(BRUSHLESS_SPEED_PIN, LOW);
    digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
    Serial.println("LOG Initialize Done");
    delay(100);
    Serial.println(7);
    info_receive = false;
    home_done = false;
    Serial.println("HFAIL");
    Serial.println("LOG Home not yet");
    Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE;
  }
  else if (Appl_SystemState_xdu8 == RECIEVE_AND_RUNNING_STATE)
  {
    if (info_receive == false)
    {
      Serial.println("LOG ------ Turned to RECEIVE STATE.WAIT TO RECEIVE DATA........ ---------------");
      info_receive = true;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      info_stop = true;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Appl_EmergencyHold_xdu = true;
      Serial.println(5);      
    }
    Recive_Data();
    if (auto_data_received == true)
    {
      Serial.println("LOG Starting");
      Appl_StartRunning_xdu = true;
      Appl_ButtonStopPress_xdu = false;
      digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
      Serial.println("LOG Excuting data auto");
      cut_and_excute();
      auto_data_received = false;
    }
    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;    
    }
    else if (Appl_EmergencyHold_xdu == true)
    {
      Appl_SystemState_xdu8 = EMERGENCY_STATE;
      Serial.println(5);
    }
  }
  else if (Appl_SystemState_xdu8 == FINISH_LETTER_STATE)
  {
    info_receive = false;
    Serial.println("LOG --------------Turned to FINISH LETTER STATE...--------------");    
    if (buzzer_needed == true)
    {
      digitalWrite(MATERIAL_STATUS, HIGH);
      delay(1500);
      digitalWrite(MATERIAL_STATUS, LOW);
    }
    else
    {
      delay(1000);
    }
    home_done = false;
    Home_All();
    if (Appl_EmergencyHold_xdu == false or Appl_ButtonStopPress_xdu == false)
    {
      home_done = true;
    }    
    delay(500);
    Serial.println("LOG Send waiting START signal state to machine"); // gửi trạng thái chờ lên máy
    Serial.println(55); // gửi trạng thái chờ lên máy
    bool info3 = false;
    while (digitalRead(BUTTON_START_PIN) == 0)
    {
      if (info3 == false)
      {
        Serial.println("LOG Wait press Start...");
        info3 = true;
      }
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
      {
        STEPPER_MOVE.stop();
        Appl_EmergencyHold_xdu = true;  
        break;
      }
      if (digitalRead(BUTTON_STOP_PIN) == 1)
      {
        Serial.println(2); // nhấn stop
        STEPPER_MOVE.stop();
        Appl_ButtonStopPress_xdu = true;
        Serial.println(6);
        break;
      }
    }
    if (Appl_ButtonStopPress_xdu == false and Appl_EmergencyHold_xdu == false)
    {
      Serial.println(4); // gửi tín hiệu nhấn start lên
      Serial.println("LOG Gui NEXT len PC");
      delay(100);
      Serial.println("NEXT");
      delay(100);
      Serial.println("N");
      Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE; 
    }
    else if (Appl_ButtonStopPress_xdu == true)
    {
//      Serial.println(2); // nhấn stop
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;      
    }
    else if (Appl_EmergencyHold_xdu == true)
    {
      Appl_SystemState_xdu8 = EMERGENCY_STATE;
      Serial.println(5); // nhấn emg
    }
  }
  else if (Appl_SystemState_xdu8 == FINISH_STATE)
  {
    info_receive = false;
    Serial.println("LOG --------------Turned to FINISH ALL LETTER STATE--------------");
    
    if (Appl_ButtonStopPress_xdu == true)
    {
      Appl_FinishStateFirstCall_xdu = false;
      Appl_FinishTransfer_xdu = false;
      Appl_SystemState_xdu8 = STOP_BUTTON_PRESS_STATE;
    }
    else if (Appl_EmergencyHold_xdu == true)
    {
      Appl_SystemState_xdu8 = EMERGENCY_STATE;
      Serial.println(5);
    }
    else
    {
      Serial.println("LOG Send finished letter state to PC");
      Serial.println(8);
      if (buzzer_needed == true)
      {
        digitalWrite(MATERIAL_STATUS, HIGH);
        delay(1500);
        digitalWrite(MATERIAL_STATUS, LOW);
      }
      home_done = false;
      Home_All();     
      if (Appl_EmergencyHold_xdu == false or Appl_ButtonStopPress_xdu == false)
      {
        home_done = true;
      }
      Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE;
    }
  }
  else if (Appl_SystemState_xdu8 == STOP_BUTTON_PRESS_STATE)
  {
    info_receive = false;
    Serial.println("LOG --------------Turned to STOP STATE...--------------");
    Appl_ButtonStopPress_xdu = false;
    Appl_SystemState_xdu8 = false;
    //Serial.println(2);
    STEPPER_MOVE.stop();
    STEPPER_CUT.stop();
    digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
    //Serial.println(2);
    bool info5 = false;
    while (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      if (info5 == false)
      {
        Serial.println("LOG Wait button Stop release...");
        info5 = true;
      }
    }
    info5 = false;
    delay(300);
    while (digitalRead(BUTTON_STOP_PIN) == 0)
    {
      if (info5 == false)
      {
        Serial.println("LOG Stop released. Wait press again...");
        info5 = true;
      }
      Serial.println(28);
      delay(50);
    }
    info5 = false;
    while (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      if (info5 == false)
      {
        Serial.println("LOG Wait button Stop release again...");
        info5 = true;
      }
    }
    info5 = false;
    home_done = false;
    Home_All();
    if (Appl_EmergencyHold_xdu == false or Appl_ButtonStopPress_xdu == false)
    {
      home_done = true;
      Serial.println(7);
      if (buzzer_needed == true)
      {
        Serial.println("BZON");
      }
      else
      {
        Serial.println("BZOF");
      }
    }
    delay(20);
    Appl_SystemState_xdu8 = RECIEVE_AND_RUNNING_STATE;
  }
  else if (Appl_SystemState_xdu8 == EMERGENCY_STATE)
  {
    info_receive = false;
    Serial.println("LOG --------------Turned to EMERGENCY STATE--------------");
    while (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Serial.println(5);
      delay(300);
    }
    Appl_SystemState_xdu8 = INIT_STATE;
  }
  else
  {
    Serial.println("LOG ENUM UNEXPECTED. TURNING TO INIT STATE....");
    Appl_SystemState_xdu8 = INIT_STATE;
  }
}

ISR(TIMER1_COMPA_vect)
{
  Appl_CutterBackwardTrigger_xdu = false;
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
