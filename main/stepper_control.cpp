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
//#define MOVE_STEPPER_SPEED_HOME     (10000)
#define MOVE_STEPPER_SPEED_HOME     (30000)
#define MOVE_STEPPER_SPEED          (10000)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
AccelStepper STEPPER_MOVE(AccelStepper::DRIVER, STEPPER_MOVE_STEP_PIN, STEPPER_MOVE_DIR_PIN);
AccelStepper STEPPER_CUT(AccelStepper::DRIVER, STEPPER_CUT_STEP_PIN, STEPPER_CUT_DIR_PIN);

/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
void Home_Move_A()
{
  STEPPER_MOVE.moveTo(-50000);

  // Wait untill sensor A detected or button stop pressed
  while (STEPPER_MOVE.currentPosition() != -50000     and 
        (IS_SENSOR_NOT_DETECTED(SS1_MOVE_HOME_A_PIN)) and 
        (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)))
  {
    STEPPER_MOVE.setSpeed(-MOVE_STEPPER_SPEED_HOME);
    STEPPER_MOVE.runSpeed();
  }

  // Stop stepper and reset position
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
  
  // Reset numbers of pulse
  Appl_FisrtPulse_xdu8 = 0;
  Appl_LastPulse_xdu8 = 0;
}

void Home_Move_B()
{
  static bool Flag_Pre;
  STEPPER_MOVE.moveTo(50000);
  
  // Wait untill sensor B detected or button stop pressed
  while (STEPPER_MOVE.currentPosition() != 50000      and 
        (IS_SENSOR_NOT_DETECTED(SS2_MOVE_HOME_B_PIN)) and 
        (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)))
  {
    STEPPER_MOVE.setMaxSpeed(MOVE_STEPPER_SPEED_HOME);
    STEPPER_MOVE.moveTo(20000);

    // Increase number of Holes already run
    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        NumHolesAlreadyRun_xdu32++;
        Flag_Pre = 0;
      }
    }
  }

  // Stop stepper and reset positon
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
  NumHolesAlreadyRun_xdu32 = 0;
}

void Forward_Move(unsigned long Step_Remain)
{
  static bool Flag_Pre;

  // Wait untill reach positin and button stop is pressed
  while ((STEPPER_MOVE.currentPosition() != Step_Remain)  and
        (IS_SENSOR_NOT_DETECTED(SS2_MOVE_HOME_B_PIN))     and
        (Appl_NoMaterial_xdu == false)                    and
        (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN))          and
        (Appl_ButtonStopPress_xdu == false))
  {
    STEPPER_MOVE.setSpeed(4000);
    STEPPER_MOVE.runSpeed();

    // Increase number of Holes already run
    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        Flag_Pre = 0;
        Appl_NumHolesFromAToB_xdu8++;
      }
    }
  }

  // Stop stepper and reset positon
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);
}

void Forward_Move_First()
{
  static bool Flag_Pre;
  static int NumHole_Internal;
  NumHole_Internal = 0;

  while ((STEPPER_MOVE.currentPosition() != 100000)          and 
         (IS_SENSOR_NOT_DETECTED(SS2_MOVE_HOME_B_PIN))       and 
         (Appl_NoMaterial_xdu == false)                      and 
         (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN))            and 
         (Appl_ButtonStopPress_xdu == false)                 and 
         (NumHole_Internal != 1))
  {
    STEPPER_MOVE.setSpeed(3000);
    STEPPER_MOVE.runSpeed();
    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        if ((IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
        {
         if (STEPPER_MOVE.currentPosition() >= 400 )
          {
            NumHole_Internal++;
            Flag_Pre = 0;
            LOG(NumHole_Internal);
          }
          else
          {
            LOG("Loi cam bien 2");
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
  LOG("Forward_Move_1Step");
  NumHole_Internal = 0;

  while (STEPPER_MOVE.currentPosition() != 100000       and
        (IS_SENSOR_NOT_DETECTED(SS2_MOVE_HOME_B_PIN))   and
        (Appl_NoMaterial_xdu == false)                  and
        (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN))        and
        (Appl_ButtonStopPress_xdu == false)             and
        (NumHole_Internal != 1))
  {
    STEPPER_MOVE.setSpeed(3000);
    STEPPER_MOVE.runSpeed();
    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        DELAY(20);
        if ((IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
        {
          NumHole_Internal++;
          Flag_Pre = 0;
          LOG("First Holes : %d", NumHole_Internal);
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
  int32_t timer1;
  int32_t timercount;
  NumHole_Internal = 0;
  
  while ((STEPPER_MOVE.currentPosition() != 100000)       and
         (IS_SENSOR_NOT_DETECTED(SS2_MOVE_HOME_B_PIN))    and
         (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN))         and
         (Appl_ButtonPausePress_1_xdu == false)           and
         (Appl_NoMaterial_xdu == false)                   and
         (Appl_ButtonStopPress_xdu == false)              and
         (NumHole_Internal != Holes)                      and
         (Appl_NumHolesFromAToB_xdu8 < 44))
  {
    if (Appl_NumHolesFromAToB_xdu8 == 42 or (NumHole_Internal == (Holes - 2)) or Appl_NumHolesFromAToB_xdu8 == 1)
    {
      STEPPER_MOVE.setSpeed(6000); // số chuẩn
      STEPPER_MOVE.runSpeed();
    }
    else if ((Appl_NumHolesFromAToB_xdu8 >= 43 or (NumHole_Internal == (Holes - 1))))
    {
      STEPPER_MOVE.setSpeed(4000); // số chuẩn
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

    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      STEPPER_MOVE.runSpeed();
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        if ((IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
        {
          Appl_FisrtPulse_xdu8 = STEPPER_MOVE.currentPosition();
          if (Appl_FisrtPulse_xdu8 - Appl_LastPulse_xdu8 >= 400) 
          {
            NumHole_Internal++;
            Appl_NumHolesFromAToB_xdu8++;
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
              LOG("Loi cam bien 1");
            }
          }
        }
      }
    }

    if (IS_MATERIAL_NOT_DETECTED(SS8_MATERIAL_BACK_PIN))
    {
      if (Appl_NoMaterialFirstCallCapture_xdu == 0)
      {
        Appl_LengthToAlarm_A_fdu32 = abs(STEPPER_MOVE.currentPosition()) * 0.011090301;
        Appl_NoMaterialFirstCallCapture_xdu = 1;
        LOG("Length: %f", Appl_LengthToAlarm_A_fdu32);
      }
      if (Appl_NoMaterial_xdu == false and Appl_NoMaterialFirstCallCapture_xdu == 1)
      {
        Appl_LengthToAlarm_B_fdu32 = abs(STEPPER_MOVE.currentPosition()) * 0.011090301;
        Appl_LengthToAlarm_fdu32 = Appl_LengthToAlarm_B_fdu32 - Appl_LengthToAlarm_A_fdu32;
        if (Appl_LengthToAlarm_fdu32 >= 100 or Appl_NumHolesFromAToB_xdu8 >= 43)
        {
          LOG("Length: %f", Appl_LengthToAlarm_fdu32);
          LOG("CurrentPosition: %d", STEPPER_MOVE.currentPosition());

          DATA_SEND_TO_PC(RES_NO_MATERIAL);
          GPIO_SET(MATERIAL_STATUS, HIGH);
          Appl_NoMaterial_xdu = true;
          Appl_NoMaterialTriger_xdu = true;
        }
      }
    }
  }
  STEPPER_MOVE.stop();
  STEPPER_MOVE.setCurrentPosition(0);

  if (IS_SENSOR_DETECTED(SS2_MOVE_HOME_B_PIN))
  {
    if (IS_MATERIAL_NOT_DETECTED(SS8_MATERIAL_BACK_PIN))
    {
      if (Appl_NoMaterial_xdu == false)
      {
        DATA_SEND_TO_PC(RES_NO_MATERIAL);
        GPIO_SET(MATERIAL_STATUS, HIGH);
        Appl_NoMaterial_xdu = true;
      }
    }
  }
  if (Appl_NoMaterial_xdu == true)
  {
    if (IS_MATERIAL_DETECTED(SS8_MATERIAL_BACK_PIN))
    {
      if (IS_BUTTON_PRESSED(BUTTON_START_PIN))
      {
        LOG("start press");
        Appl_NoMaterial_xdu = false;
        Appl_NoMaterialFirstCallCapture_xdu = 0;
        DATA_SEND_TO_PC(RES_READY_RECEIVE);
        GPIO_SET(MATERIAL_STATUS, LOW);
        GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
        DELAY(1000);
        return NumHole_Internal;
      }
      GPIO_SET(MATERIAL_STATUS, LOW);
    }
    else
    {
      if (IS_BUTTON_NOT_PRESSED(BUTTON_START_PIN))
      {
        LOG("Het phoi");
        GPIO_SET(MATERIAL_STATUS, HIGH);
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
      LOG("start press");
      return NumHole_Internal;
    }
  }
  else
  {
    // Do nothing
  }
  return NumHole_Internal;
}

unsigned long Backward_Move(unsigned long Step_Remain)
{
  static bool Flag_Pre;
  LOG(Step_Remain);
  STEPPER_MOVE.moveTo(-Step_Remain);

  while ((STEPPER_MOVE.currentPosition() != -Step_Remain)   and
         (IS_SENSOR_NOT_DETECTED(SS1_MOVE_HOME_A_PIN))      and
         (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN))           and
         (Appl_ButtonStopPress_xdu == false))
  {
    //STEPPER_MOVE.setSpeed(-10000);
    STEPPER_MOVE.setSpeed(-18000);
    STEPPER_MOVE.runSpeed();
    if (IS_SENSOR_DETECTED(SS6_HOLES_PIN))
    {
      Flag_Pre = 1;
    }
    else
    {
      if ((Flag_Pre == 1) and (IS_SENSOR_NOT_DETECTED(SS6_HOLES_PIN)))
      {
        if (Appl_NumHolesFromAToB_xdu8 <= 0)
        {
          Appl_NumHolesFromAToB_xdu8 = 45;
        }
        Appl_NumHolesFromAToB_xdu8--;
        LOG("NumHolesFromAToB_xdu8: %d", Appl_NumHolesFromAToB_xdu8);
        Flag_Pre = 0;
      }
    }
  }
  STEPPER_MOVE.stop();

  GPIO_SET(SOL_CLAMPER_PIN, HIGH);

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
  if ((IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)) and Appl_ButtonStopPress_xdu == false)
  {
    GPIO_SET(SOL_SLIDE_BACKWARD_PIN, LOW);
  }
}

void Cutter_Backward()
{
  if ((IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)) and Appl_ButtonStopPress_xdu == false)
  {
    GPIO_SET(SOL_SLIDE_BACKWARD_PIN, HIGH);
  }
  Appl_CutterBackwardTrigger_xdu = true;
}

void Cutter_Forward_Normal()
{
  GPIO_SET(SOL_SLIDE_FORWARD_PIN, HIGH);
  GPIO_SET(SOL_SLIDE_BACKWARD_PIN, LOW);
}

void Cutter_Backward_Normal()
{
  GPIO_SET(SOL_SLIDE_BACKWARD_PIN, HIGH);
  GPIO_SET(SOL_SLIDE_FORWARD_PIN, LOW);

  Appl_CutterBackwardTrigger_xdu = true;
}

void Home_Stepper_Cutter()
{
  STEPPER_CUT.moveTo(15000);
  while (STEPPER_CUT.currentPosition() != 15000 and (IS_SENSOR_NOT_DETECTED(SS3_CUT_HOME_PIN)))
  {
    STEPPER_CUT.setSpeed(2000);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  STEPPER_CUT.setCurrentPosition(0);
}

void Center_Stepper_Cutter()
{
  STEPPER_CUT.moveTo(-9040);
  while (STEPPER_CUT.currentPosition() != -9040)
  {
    STEPPER_CUT.setSpeed(-3000);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  STEPPER_CUT.setCurrentPosition(0);
}

void Angle_Cut(int Step_Remain)
{
  static int Speed;
  if (Step_Remain > 0)
  {
   // Speed = 2000;
   Speed = 12000;
  }
  else
  {
   // Speed = -2000;
    Speed = -12000;
  }

  STEPPER_CUT.moveTo(Step_Remain);
  while (STEPPER_CUT.currentPosition() != Step_Remain /*and (IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)) and Appl_ButtonStopPress_xdu == false */)
  {
    STEPPER_CUT.setSpeed(Speed);
    STEPPER_CUT.runSpeed();
  }
  STEPPER_CUT.stop();
  STEPPER_CUT.setCurrentPosition(0);
}

void Brushless_Run(int Speed)
{
  if ((IS_BUTTON_NOT_PRESSED(BUTTON_STOP_PIN)) and Appl_ButtonStopPress_xdu == false)
  {
    analogWrite(BRUSHLESS_SPEED_PIN, Speed);
    GPIO_SET(BRUSHLESS_ENA_PIN , LOW);
  }
}

void Brushless_Off()
{
  GPIO_SET(BRUSHLESS_SPEED_PIN, LOW);
  GPIO_SET(BRUSHLESS_ENA_PIN , HIGH);
}
void Init_Home()
{
  GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
  delay(500);
  GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
  delay(500);
  GPIO_SET(SOL_CLAMPER_PIN, HIGH);
  delay(500);
  GPIO_SET(SOL_CLAMPER_PIN, LOW);
  Appl_ButtonStopPress_xdu = false;
  Appl_FinishStateFirstCall_xdu = true;
}
void Home_All()
{
  GPIO_SET(MATERIAL_STATUS, LOW);
  GPIO_SET(OUT_8,HIGH);
  delay(100);
  STEPPER_MOVE.setEnablePin(STEPPER_MOVE_ENA_PIN);
  //STEPPER_CUT.setEnablePin(STEPPER_CUT_ENA_PIN);
  GPIO_SET(STEPPER_CUT_ENA_PIN,HIGH);
  GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
  Home_Move_A();
  Cutter_Forward();                                         // Forward Cut
  while (IS_SENSOR_NOT_DETECTED(SS4_END_STROKE_BACK_PIN))   // Wait to cutter go in
  {
  }
  Home_Stepper_Cutter();
  DELAY(200);
  Center_Stepper_Cutter();
  GPIO_SET(STEPPER_CUT_ENA_PIN,LOW);
  GPIO_SET(SOL_CLAMPER_PIN, LOW);
  Appl_ButtonStopPress_xdu = false;
  Appl_FinishStateFirstCall_xdu = true;
  GPIO_SET(OUT_8,LOW);
}

void Home_And_Center_Cutter()
{
  Home_Stepper_Cutter();
  DELAY(200);
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
