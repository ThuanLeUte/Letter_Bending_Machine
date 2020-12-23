#ifndef Stepper_Control_h
#define Stepper_Control_h

#include "main.h"
#include <AccelStepper.h>
#include "Board_Define.h"
#define DATA_STEPPER
#define SS1_MOVE_HOME_A_PIN SS1
#define SS2_MOVE_HOME_B_PIN SS3
#define SS5_CUT_HOME_PIN SS5
#define SS3_END_STROKE_FRONT_PIN SS2
#define SS4_END_STROKE_BACK_PIN SS4
#define SS6_MATERIAL_PIN SS6
#define SS7_HOLES_PIN SS7



#define BUTTON_START_PIN BT1
#define BUTTON_PAUSE_PIN BT2
#define BUTTON_STOP_PIN BT3
#define BUTTON_EMERGENCY_PIN BT4

#define SOL_CLAMP_FEEDER_PIN OUT_2
#define SOL_SLIDE_FORWARD_PIN OUT_3
#define SOL_LIFTER_PIN OUT_4
#define SOL_CLAMPER_PIN OUT_5
#define MATERIAL_STATUS OUT_6

#define STEPPER_MOVE_ENA_PIN OP1
#define STEPPER_MOVE_DIR_PIN OP2
#define STEPPER_MOVE_STEP_PIN OP3

#define STEPPER_CUT_ENA_PIN OP5
#define STEPPER_CUT_DIR_PIN OP6
#define STEPPER_CUT_STEP_PIN OP4

#define BRUSHLESS_ENA_PIN OP7
#define BRUSHLESS_SPEED_PIN OP8

#define MOVE_STEPPER_SPEED 10000
#define MOVE_STEPPER_SPEED_HOME 15000

AccelStepper STEPPER_MOVE(AccelStepper::DRIVER, STEPPER_MOVE_STEP_PIN, STEPPER_MOVE_DIR_PIN);
AccelStepper STEPPER_CUT(AccelStepper::DRIVER, STEPPER_CUT_STEP_PIN, STEPPER_CUT_DIR_PIN);

void Home_Move_A();
void Home_Move_B();
void Home_All();
void Home_And_Center_Cutter();
void Home_Cutter();
void Cutter_Forward();
void Cutter_Backward();
void Center_Stepper_Cutter();
void Angle_Cut(int16_t Step_Remain);

void Brushless_Run(int Speed);
void Brushless_Off();
void Forward_Move(unsigned long Step_Remain);
void toi_cat_trong();
void lui_cat_trong();
int Forward_Move_Holes(int Holes);
unsigned long Backward_Move(unsigned long Step_Remain);

void toi_cat_trong()
{
    uint16_t count = 0;
    digitalWrite(SOL_SLIDE_FORWARD_PIN, HIGH);
    // wait xy lanh move ra khoi end stroke
    while (digitalRead(SS3_END_STROKE_FRONT_PIN) == 1 and digitalRead(BUTTON_EMERGENCY_PIN) == 1 
      and (digitalRead(BUTTON_STOP_PIN) == 0) and count < 5005)
    {
      delay(1);
      count +=1;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      STEPPER_MOVE.stop();
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (count > 5000)
    {
      STEPPER_MOVE.stop();
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
}
void lui_cat_trong()
{
  if (Appl_ButtonStopPress_xdu == false and Appl_EmergencyHold_xdu == false)
  {
    digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
    // wait xy lanh move vao
    uint16_t count = 0;
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 1 and digitalRead(BUTTON_EMERGENCY_PIN) == 1 
      and (digitalRead(BUTTON_STOP_PIN) == 0) and count <5005)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      STEPPER_MOVE.stop();
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (count >= 5000)
    {
      STEPPER_MOVE.stop();
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
  }
}
void Home_Move_A()
{
  STEPPER_MOVE.setSpeed(-800);
  //STEPPER_MOVE.setMaxSpeed(-35000);
  //STEPPER_MOVE.moveTo(-50000);
  //STEPPER_MOVE.runSpeed();
//  long current_pos = 0;
  uint16_t current_pos = STEPPER_MOVE.currentPosition();
  Serial.println("LOG Dang move ve A");
  while ((digitalRead(SS1_MOVE_HOME_A_PIN) == 1) and (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
  and (digitalRead(BUTTON_STOP_PIN) == 0) and STEPPER_MOVE.currentPosition() - current_pos >= -50000 )
  {
    STEPPER_MOVE.runSpeed();
    if (STEPPER_MOVE.speed() > -70000)
    {
      STEPPER_MOVE.setSpeed(STEPPER_MOVE.speed() - accel_factor);
    }
  }
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
  {
    STEPPER_MOVE.stop();
    Appl_EmergencyHold_xdu = true;
    return;
  }
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial.println(2); // nhấn stop
    STEPPER_MOVE.stop();
    Appl_ButtonStopPress_xdu = true;
    return;
  }
  STEPPER_MOVE.stop();
  if (STEPPER_MOVE.currentPosition() - current_pos >= 50000) 
  {
    Serial.println(13);
    Appl_ButtonStopPress_xdu = true;
    return;
  }
  Serial.println("LOG Home Move A speed :" + String(STEPPER_MOVE.speed()));
  STEPPER_MOVE.setCurrentPosition(0);
  Serial.println("LOG Xong move A");
  hole_index = 0;
  du_truoc = 0;
}
void Home_Move_B()
{
  static bool Flag_Pre;
  STEPPER_MOVE.moveTo(50000);
  STEPPER_MOVE.setSpeed(5000);
  while ((digitalRead(SS2_MOVE_HOME_B_PIN) == 1) and (digitalRead(BUTTON_EMERGENCY_PIN) == 1) 
    and (digitalRead(BUTTON_STOP_PIN) == 0))
  {
    STEPPER_MOVE.runSpeed();
    if (STEPPER_MOVE.speed() < 70000)
    {
      STEPPER_MOVE.setSpeed(STEPPER_MOVE.speed() + accel_factor);
    }
  }
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
    STEPPER_MOVE.stop();
    return;
  }
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
  {
    Appl_EmergencyHold_xdu = true;
    return;
  }
  STEPPER_MOVE.stop();
  Serial.println("LOG Finished Home Move B");
}
void Cutter_Backward()
{
  if (Appl_ButtonStopPress_xdu == false and Appl_EmergencyHold_xdu == false)
  {
    uint16_t count = 0;
    Serial.println("LOG Bat dau lui dao");
    digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
    while ((digitalRead(SS3_END_STROKE_FRONT_PIN) == 0) and (SS4_END_STROKE_BACK_PIN) == 0
      and count <=5000 and digitalRead(BUTTON_EMERGENCY_PIN) ==1 and digitalRead(BUTTON_STOP_PIN) == 0)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (count >= 5000)
    {
      STEPPER_MOVE.stop();
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    delay(500);
    count = 0;
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 1)
    {
    delay(1);
    count += 1;
    }
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
    return;
    }
    if (count >= 5000)
    {
      STEPPER_MOVE.stop();
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    } 
  }
}
void Cutter_Forward()
{
  uint16_t count = 0;
  if (Appl_EmergencyHold_xdu == false and Appl_EmergencyHold_xdu == false)
  {
    digitalWrite(SOL_SLIDE_FORWARD_PIN, HIGH);
    // wait xy lanh move ra khoi end stroke
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 0 and digitalRead(BUTTON_STOP_PIN) == 0
      and digitalRead(BUTTON_EMERGENCY_PIN) == 1 and count <= 5000)
    {      
      delay(1);
      count += 1;
    }
    Serial.println("LOG xy lanh da move khoi diem trong cung");
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {      
      Serial.println(2); // nhấn stop
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (count >=5000)
    {
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    //delay(300);
    count = 0;
    // trigger tin hieu sensor sau bang 1
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 1 
      and digitalRead(BUTTON_STOP_PIN) == 0 and digitalRead(BUTTON_EMERGENCY_PIN) == 1 and count <= 5000)
    {
     delay(1);
     count += 1;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (count >=5000)
    {
      Serial.println(12);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    Serial.println("LOG xy lanh da toi diem ngoai cung");
    delay(300);
   }
}

void Center_Stepper_Cutter()
{
  STEPPER_CUT.moveTo(-9840);
  STEPPER_CUT.setSpeed(-4000);
  while (STEPPER_CUT.currentPosition() != -9840 and (digitalRead(BUTTON_EMERGENCY_PIN) == 1))
  {
    STEPPER_CUT.runSpeed();
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      STEPPER_CUT.stop();
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Appl_EmergencyHold_xdu = true;
      STEPPER_CUT.stop();
      break;
    }
  }
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  STEPPER_CUT.stop();
  Serial2.println(STEPPER_CUT.currentPosition());
  STEPPER_CUT.setCurrentPosition(0);
}
void Angle_Cut(int16_t Step_Remain)
{
  static int Speed;
  if (Step_Remain > 0)
  {
    Speed = 8000;
  }
  else
  {
    Speed = -8000;
  }

  STEPPER_CUT.moveTo(Step_Remain);
  while (STEPPER_CUT.currentPosition() != Step_Remain and (digitalRead(BUTTON_STOP_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 1) and Appl_ButtonStopPress_xdu == false)
  {
    STEPPER_CUT.setSpeed(Speed);
    STEPPER_CUT.runSpeed();
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      STEPPER_CUT.stop();
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      STEPPER_CUT.stop();
      break;
    }
  }
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  STEPPER_CUT.stop();
  STEPPER_CUT.setCurrentPosition(0);
}

void Brushless_Run(int Speed)
{
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 and (digitalRead(BUTTON_STOP_PIN) == 0))

  {
    analogWrite(BRUSHLESS_SPEED_PIN, Speed);
    digitalWrite(BRUSHLESS_ENA_PIN, LOW);
  }
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
    digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
  }
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
  {
    Appl_EmergencyHold_xdu = true;
    digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
  }
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
}

void Brushless_Off()
{
  digitalWrite(BRUSHLESS_SPEED_PIN, LOW);
  digitalWrite(BRUSHLESS_ENA_PIN, HIGH);
}
void Home_Cutter()
{
  Serial.println("LOG Bat dau home Xoay dao");
  Serial2.println(digitalRead(SS5_CUT_HOME_PIN));
  STEPPER_CUT.setSpeed(5000);
  uint16_t current_pos = STEPPER_CUT.currentPosition();
  while (digitalRead(SS5_CUT_HOME_PIN) == 1 and STEPPER_CUT.currentPosition() - current_pos <21000)
  {
    STEPPER_CUT.runSpeed();
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      Appl_EmergencyHold_xdu = true;
      break;
    }
  }
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  STEPPER_CUT.stop();
  if (STEPPER_CUT.currentPosition() - current_pos >21000)
  {
    Serial.println(14);
    Appl_ButtonStopPress_xdu = true;
    return;
  }
  STEPPER_CUT.setCurrentPosition(0);
  delay(200);
  Serial.println("LOG Xong home Xoay dao");
}
void Home_All()
{
  Serial.println("LOG Bat dau Home All");
  delay(200);
  Serial.println(6);
  digitalWrite(MATERIAL_STATUS, LOW);
  STEPPER_MOVE.setEnablePin(STEPPER_MOVE_ENA_PIN);
  STEPPER_CUT.setEnablePin(STEPPER_CUT_ENA_PIN);
  digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
  digitalWrite(SOL_CLAMPER_PIN, LOW);
  Home_Move_A();
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  Cutter_Backward(); // Forward Cut
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  Serial.println("LOG Finished Backward");
  delay(200);
  Home_Cutter();
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  Serial.println("LOG Finished Home Cutter");
  Center_Stepper_Cutter();
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  Serial.println("LOG Finished Center Cutter");
  Appl_ButtonStopPress_xdu = false;
  Appl_FinishStateFirstCall_xdu = true;
  if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
  {
    return;
  }
  else
  {
    delay(100);
    Serial.println(7);
  }
}

#endif
