/**
 * @file       stepper_control.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Stepper control
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include <AccelStepper.h>
#include "bsp.h"
#include "stepper_control.h"
#include "damos_ram.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
AccelStepper STEPPER_MOVE(AccelStepper::DRIVER, STEPPER_MOVE_STEP_PIN, STEPPER_MOVE_DIR_PIN);
AccelStepper STEPPER_CUT(AccelStepper::DRIVER, STEPPER_CUT_STEP_PIN, STEPPER_CUT_DIR_PIN);

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void Home_Move_A_First()
{
  STEPPER_MOVE.moveTo(-1000);
  while (STEPPER_MOVE.currentPosition() != -1000 and (digitalRead(SS1_MOVE_HOME_A_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0))
  {
    STEPPER_MOVE.setSpeed(-3000);
    STEPPER_MOVE.runSpeed();
  }
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
}

void Home_Move_A()
{
  STEPPER_MOVE.moveTo(-50000);
  while (STEPPER_MOVE.currentPosition() != -50000 and (digitalRead(SS1_MOVE_HOME_A_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0))
  {
    STEPPER_MOVE.setSpeed(-MOVE_STEPPER_SPEED_HOME);
    STEPPER_MOVE.runSpeed();
  }
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
  Appl_FisrtPulse_xdu8 = 0;
  Appl_LastPulse_xdu8 = 0;
}

void Home_Move_B()
{
  static bool Flag_Pre;
  STEPPER_MOVE.moveTo(50000);
  while (STEPPER_MOVE.currentPosition() != 50000 and (digitalRead(SS2_MOVE_HOME_B_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0))
  {
    STEPPER_MOVE.setSpeed(MOVE_STEPPER_SPEED);
    STEPPER_MOVE.runSpeed();
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {
        num_holes_already_run_xdu32++;
        Flag_Pre = 0;
      }
    }
  }
  STEPPER_MOVE.stop();
  Serial3.println(STEPPER_MOVE.currentPosition());
  STEPPER_MOVE.setCurrentPosition(0);
  Serial.print("Số lỗ: ");
  Serial3.println(num_holes_already_run_xdu32);
  num_holes_already_run_xdu32 = 0;
}

void Forward_Move(unsigned long Step_Remain)
{
  static bool Flag_Pre;
  while (STEPPER_MOVE.currentPosition() != Step_Remain and (digitalRead(SS2_MOVE_HOME_B_PIN) == 0) and (Appl_NoMaterial_xdu == false) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    STEPPER_MOVE.setSpeed(4000);
    STEPPER_MOVE.runSpeed();
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {
        Flag_Pre = 0;
        Appl_NumHolesFromAToB_xdu8++;
        Serial.print("NumHolesFromAToB_xdu8 forward: ");
        Serial3.println(Appl_NumHolesFromAToB_xdu8);
      }
    }
  }
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
  Serial.print("Step_Remain: ");
  Serial3.println(Step_Remain);
  Serial3.println(STEPPER_MOVE.currentPosition());
}

void Forward_Move_First()
{
  static bool Flag_Pre;
  static int NumHole_Internal;
  NumHole_Internal = 0;
  while (STEPPER_MOVE.currentPosition() != 100000 and (digitalRead(SS2_MOVE_HOME_B_PIN) == 0) and (Appl_NoMaterial_xdu == false) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false and NumHole_Internal != 1)
  {
    STEPPER_MOVE.setSpeed(3000);
    STEPPER_MOVE.runSpeed();
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {
        if ((digitalRead(SS6_HOLES_PIN) == 0))
        {
          if (STEPPER_MOVE.currentPosition() >= 400)
          {
            NumHole_Internal++;
            Flag_Pre = 0;
            Serial.print("First Holes : ");
            Serial3.println(NumHole_Internal);
          }
          else
          {
            Serial3.println("Loi cam bien");
          }
        }
      }
    }
  }
  STEPPER_MOVE.setCurrentPosition(0);
  STEPPER_MOVE.stop();
}

void Forward_Move_1Step()
{
  static bool Flag_Pre;
  static int NumHole_Internal;
  NumHole_Internal = 0;
  while (STEPPER_MOVE.currentPosition() != 100000 and (digitalRead(SS2_MOVE_HOME_B_PIN) == 0) and (Appl_NoMaterial_xdu == false) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false and NumHole_Internal != 1)
  {
    STEPPER_MOVE.setSpeed(3000);
    STEPPER_MOVE.runSpeed();
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {
        delay(20);
        if ((digitalRead(SS6_HOLES_PIN) == 0))
        {
          NumHole_Internal++;
          Flag_Pre = 0;
          Serial.print("First Holes : ");
          Serial3.println(NumHole_Internal);
        }
      }
    }
  }
  STEPPER_MOVE.setCurrentPosition(0);
  STEPPER_MOVE.stop();
}

int Forward_Move_Holes(int Holes)
{
  static bool Flag_Pre;
  static int NumHole_Internal;
  static int Speed = 1000;
  static int Appl_NumHolesFromAToB_Pre_xdu8;
  NumHole_Internal = 0;
  while (STEPPER_MOVE.currentPosition() != 100000 and (digitalRead(SS2_MOVE_HOME_B_PIN) == 0) and (Appl_ButtonPausePress_1_xdu == false) and (Appl_NoMaterial_xdu == false) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false and NumHole_Internal != Holes and Appl_NumHolesFromAToB_xdu8 < 44)
  {
    if (Appl_NumHolesFromAToB_xdu8 == 42 or (NumHole_Internal == (Holes - 2)) or Appl_NumHolesFromAToB_xdu8 == 1)
    {
      STEPPER_MOVE.setSpeed(6000); // số chuẩn
      //STEPPER_MOVE.setSpeed(7000); //nghiệm phụ
      STEPPER_MOVE.runSpeed();
    }
    else if ((Appl_NumHolesFromAToB_xdu8 >= 43 or (NumHole_Internal == (Holes - 1))))
    {
      STEPPER_MOVE.setSpeed(4000); // số chuẩn
      //STEPPER_MOVE.setSpeed(5000); nghiệm phụ
      STEPPER_MOVE.runSpeed();
    }
    else if ((NumHole_Internal == 0) or (NumHole_Internal == 1) or (NumHole_Internal == 2) or (NumHole_Internal == 3))
    {
      if (NumHole_Internal == 0)
      {
        STEPPER_MOVE.setSpeed(3000);
        STEPPER_MOVE.runSpeed();
      }
      else if (NumHole_Internal == 1)
      {
        STEPPER_MOVE.setSpeed(5000);
        STEPPER_MOVE.runSpeed();
      }
      else if (NumHole_Internal == 2)
      {
        STEPPER_MOVE.setSpeed(7000);
        STEPPER_MOVE.runSpeed();
      }
      else
      {
        STEPPER_MOVE.setSpeed(9000);
        STEPPER_MOVE.runSpeed();
      }
    }
    else
    {
      STEPPER_MOVE.setSpeed(MOVE_STEPPER_SPEED);
      STEPPER_MOVE.runSpeed();
    }
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {
        if ((digitalRead(SS6_HOLES_PIN) == 0))
        {
          Appl_FisrtPulse_xdu8 = STEPPER_MOVE.currentPosition();
          if (Appl_FisrtPulse_xdu8 - Appl_LastPulse_xdu8 >= 400)
          {
            NumHole_Internal++;
            Appl_NumHolesFromAToB_xdu8++;
            //Serial.print("NumHolesFromAToB_xdu8: ");
            //Serial3.println(Appl_NumHolesFromAToB_xdu8);
            //Serial.print("Step_Remain: ");
            //Serial3.println(STEPPER_MOVE.currentPosition());
            Appl_LastPulse_xdu8 = STEPPER_MOVE.currentPosition();
            Flag_Pre = 0;
          }
          else
          {
            if (Appl_NoMaterialTriger_xdu == true or Appl_PauseTrigger_xdu == true)
            {
              NumHole_Internal++;
              Appl_NumHolesFromAToB_xdu8++;
              Appl_LastPulse_xdu8 = STEPPER_MOVE.currentPosition();
              Flag_Pre = 0;
              Appl_NoMaterialTriger_xdu = false;
              Appl_PauseTrigger_xdu = false;
            }
            else
            {
              Serial3.println("Loi cam bien");
            }
          }
        }
      }
    }

    if (digitalRead(SS8_MATERIAL_BACK_PIN) == 0)
    {
      if (Appl_NoMaterialFirstCallCapture_xdu == 0)
      {
        Appl_LengthToAlarm_A_fdu32 = abs(STEPPER_MOVE.currentPosition()) * 0.011090301;
        Appl_NoMaterialFirstCallCapture_xdu = 1;
        Serial.print("Length: ");
        Serial3.println(Appl_LengthToAlarm_A_fdu32);
      }
      if (Appl_NoMaterial_xdu == false and Appl_NoMaterialFirstCallCapture_xdu == 1)
      {
        Appl_LengthToAlarm_B_fdu32 = abs(STEPPER_MOVE.currentPosition()) * 0.011090301;
        Appl_LengthToAlarm_fdu32 = Appl_LengthToAlarm_B_fdu32 - Appl_LengthToAlarm_A_fdu32;
        if (Appl_LengthToAlarm_fdu32 >= 100 or Appl_NumHolesFromAToB_xdu8 >= 43)
        {
          Serial.print("Length: ");
          Serial3.println(Appl_LengthToAlarm_fdu32);
          Serial.print("currentPosition: ");
          Serial3.println(STEPPER_MOVE.currentPosition());

          Serial.println(11);
          digitalWrite(MATERIAL_STATUS, HIGH);
          Appl_NoMaterial_xdu = true;
          Appl_NoMaterialTriger_xdu = true;
        }
      }
    }
  }
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);

  if (digitalRead(SS2_MOVE_HOME_B_PIN) == 1)
  {
    if (digitalRead(SS8_MATERIAL_BACK_PIN) == 0)
    {
      if (Appl_NoMaterial_xdu == false)
      {
        Serial.println(11);
        digitalWrite(MATERIAL_STATUS, HIGH);
        Appl_NoMaterial_xdu = true;
      }
    }
  }
  if (Appl_NoMaterial_xdu == true)
  {
    if (digitalRead(SS8_MATERIAL_BACK_PIN) == 1)
    {
      if (digitalRead(BUTTON_START_PIN) == 0)
      {
        Serial3.println("start press");
        Appl_NoMaterial_xdu = false;
        Appl_NoMaterialFirstCallCapture_xdu = 0;
        Serial.println(7);
        digitalWrite(MATERIAL_STATUS, LOW);
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
        delay(1000);
        return NumHole_Internal;
      }
      digitalWrite(MATERIAL_STATUS, LOW);
    }
    else
    {
      if (digitalRead(BUTTON_START_PIN) == 0)
      {
        Serial3.println("Het phoi");
        digitalWrite(MATERIAL_STATUS, HIGH);
      }
    }
  }
  else if (Appl_ButtonPausePress_1_xdu == true)
  {
    if (Appl_ButtonStartPress_xdu == true)
    {
      Appl_ButtonPausePress_1_xdu = false;
      Appl_ButtonStartPress_xdu = false;
      Appl_StartRunning_xdu = true;
      Serial3.println("start press");
      return NumHole_Internal;
    }
  }
  else
  {
  }
  return NumHole_Internal;
}

unsigned long Backward_Move(unsigned long Step_Remain)
{
  static bool Flag_Pre;
  Serial3.println(Step_Remain);
  STEPPER_MOVE.moveTo(-Step_Remain);
  while (STEPPER_MOVE.currentPosition() != -Step_Remain and (digitalRead(SS1_MOVE_HOME_A_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    STEPPER_MOVE.setSpeed(-10000);
    STEPPER_MOVE.runSpeed();
    if (digitalRead(SS6_HOLES_PIN) == 1)
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (digitalRead(SS6_HOLES_PIN) == 0))
      {

        if (Appl_NumHolesFromAToB_xdu8 <= 0)
        {
          Appl_NumHolesFromAToB_xdu8 = 45;
        }
        Appl_NumHolesFromAToB_xdu8--;
        Serial.print("NumHolesFromAToB_xdu8: ");
        Serial3.println(Appl_NumHolesFromAToB_xdu8);
        Flag_Pre = 0;
      }
    }
  }
  STEPPER_MOVE.stop();
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
  {
    return 0;
  }
  digitalWrite(SOL_CLAMPER_PIN, HIGH);

  if (abs(Step_Remain) == abs(STEPPER_MOVE.currentPosition()))
  {
    STEPPER_MOVE.setCurrentPosition(0);
    return 0;
  }
  else
  {
    static long Current_Position;
    Current_Position = abs(STEPPER_MOVE.currentPosition());
    STEPPER_MOVE.setCurrentPosition(0);
    return (Step_Remain - Current_Position);
  }
  STEPPER_MOVE.setCurrentPosition(0);
}

void Cutter_Forward()
{
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_SLIDE_FORWARD_PIN, HIGH);
    digitalWrite(SOL_SLIDE_BACKWARD_PIN, LOW);
  }
}

void Cutter_Backward()
{
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_SLIDE_BACKWARD_PIN, HIGH);
    digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
  }
  Appl_CutterBackwardTrigger_xdu = true;
}

void Cutter_Forward_Normal()
{
  digitalWrite(SOL_SLIDE_FORWARD_PIN, HIGH);
  digitalWrite(SOL_SLIDE_BACKWARD_PIN, LOW);
}

void Cutter_Backward_Normal()
{
  digitalWrite(SOL_SLIDE_BACKWARD_PIN, HIGH);
  digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
  
  Appl_CutterBackwardTrigger_xdu = true;
}

void Home_Stepper_Cutter()
{
  STEPPER_CUT.moveTo(10800);
  while (STEPPER_CUT.currentPosition() != 10800 and (digitalRead(SS3_CUT_HOME_PIN) == 0) /* and (digitalRead(BUTTON_STOP_PIN)==0) */ and (digitalRead(BUTTON_EMERGENCY_PIN) == 0))
  {
    STEPPER_CUT.setSpeed(2000);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  Serial3.println(STEPPER_CUT.currentPosition());
  STEPPER_CUT.setCurrentPosition(0);
}

void Center_Stepper_Cutter()
{
  STEPPER_CUT.moveTo(-5040);
  while (STEPPER_CUT.currentPosition() != -5040 and (digitalRead(BUTTON_EMERGENCY_PIN) == 0))
  {
    STEPPER_CUT.setSpeed(-2000);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  Serial3.println(STEPPER_CUT.currentPosition());
  STEPPER_CUT.setCurrentPosition(0);
}

void Angle_Cut(int Step_Remain)
{
  static int Speed;
  if (Step_Remain > 0)
  {
    Speed = 3000;
  }
  else
  {
    Speed = -3000;
  }

  STEPPER_CUT.moveTo(Step_Remain);
  while (STEPPER_CUT.currentPosition() != Step_Remain and (digitalRead(BUTTON_STOP_PIN) == 0) and (digitalRead(BUTTON_EMERGENCY_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    STEPPER_CUT.setSpeed(Speed);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  STEPPER_CUT.setCurrentPosition(0);
}

void Brushless_Run(int Speed)
{
  if (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and (digitalRead(BUTTON_STOP_PIN) == 0) and Appl_ButtonStopPress_xdu == false)
  {
    analogWrite(BRUSHLESS_SPEED_PIN, Speed);
  }
}

void Brushless_Off()
{
  digitalWrite(BRUSHLESS_SPEED_PIN, LOW);
}

void home_all()
{
  digitalWrite(MATERIAL_STATUS, LOW);
  STEPPER_MOVE.setEnablePin(STEPPER_MOVE_ENA_PIN);
  STEPPER_CUT.setEnablePin(STEPPER_CUT_ENA_PIN);
  digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
  Home_Move_A_First();
  Home_Move_A();
  Cutter_Forward(); // Forward Cut
  while (digitalRead(SS4_END_STROKE_BACK_PIN) == 0)  // Wait to cutter go in
  {
  }
  Home_Stepper_Cutter();
  delay(200);
  Center_Stepper_Cutter();
  digitalWrite(SOL_CLAMPER_PIN, LOW);
  Appl_ButtonStopPress_xdu = false;
  Appl_FinishStateFirstCall_xdu = true;
}

void Home_And_Center_Cutter()
{
  Home_Stepper_Cutter();
  delay(200);
  Center_Stepper_Cutter();
}

void stepper_setup(void)
{
  STEPPER_MOVE.setEnablePin(STEPPER_MOVE_ENA_PIN);
  STEPPER_MOVE.setMaxSpeed(100000);

  STEPPER_CUT.setEnablePin(STEPPER_CUT_ENA_PIN);
  STEPPER_CUT.setMaxSpeed(100000);
}

/* End of file -------------------------------------------------------- */
