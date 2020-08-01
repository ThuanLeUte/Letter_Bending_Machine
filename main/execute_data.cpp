/**
 * @file       execute_data.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Execute Data
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "execute_data.h"
#include "damos_ram.h"
#include "main.h"
#include "stepper_control.h"
#include "bsp.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
int Execute_Manual(String Data_Input)
{
  static uint32_t Length_Data;
  Length_Data = Data_Input.length();
  String Command = Data_Input.substring(0, 2);
  String Data = Data_Input.substring(3, Length_Data);

  if (Command == "HA") // MOVE_HOMA
  {
    Serial.println("home-a");
    Home_Move_A();
    return 1;
  }
  else if (Command == "HM") // HOME ALL
  {
    Serial.println("home-all");
    Appl_ButtonStopPress_xdu = true;
    return 1;
  }
  else if (Command == "CN") // KẾT NỐI
  {
    delay(100);
    Serial2.println(10);
    Serial.println("KET NOI");
    return 1;
  }
  else if (Command == "ST") // STOP
  {
    Serial.println("STOP");
    Stop();
    return 1;
  }
  else if (Command == "PS") // PAUSE
  {
    Serial.println("PAUSE");
    Pause();
    return 1;
  }
  else if (Command == "HB") //MOVE_HOMB
  {
    Serial.println("home-b");
    Home_Move_B();
    return 1;
  }
  else if (Command == "HC") //HOME_CUT
  {
    Serial.println("home-cut");
    Home_And_Center_Cutter();
    return 1;
  }
  else if (Command == "CF") //CUTTER FW
  {
    Serial.println("CUTTER FW");
    Cutter_Forward();
    return 1;
  }
  else if (Command == "CB") //CUTTER BW
  {
    Serial.println("CUTTER BW");
    Cutter_Backward();
    return 1;
  }
  else if (Command == "CU") //CUTTER UP
  {
    Serial.println("CUTTER UP");
    digitalWrite(SOL_LIFTER_PIN, LOW); // Nang dao
                                       // digitalWrite(SOL_CLAMPER_PIN,LOW);   // Nang dao

    return 1;
  }
  else if (Command == "CD") //CUTTER DW
  {
    Serial.println("CUTTER DW");
    digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
                                        // digitalWrite(SOL_CLAMPER_PIN,HIGH);   // Ha dao
    return 1;
  }
  else if (Command == "MO") //MOTOR ON
  {
    Serial.println("MOTOR ON");
    Brushless_Run(BRUSHLESS_SPEED);
    return 1;
  }
  else if (Command == "MX") //MOTOR OFF
  {
    Serial.println("MOTOR OFF");
    Brushless_Off();
    return 1;
  }
  else if (Command == "BR") //MOTOR OFF
  {
    Serial.println("BUZZER RESET");
    digitalWrite(MATERIAL_STATUS, LOW);
    return 1;
  }
  else if (Command == "KP") //KEP PHOI
  {
    Serial.println("KEP PHOI");
    digitalWrite(SOL_CLAMPER_PIN, HIGH);
    return 1;
  }
  else if (Command == "NP") //NHẢ PHOI
  {
    Serial.println("KEP PHOI");
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "AR") //ANGLE CUT RIGHT
  {
    Serial.println("ANGLE CUT RIGHT");
    Angle_Cut(float(Data.toFloat() * 120));
    return 1;
  }
  else if (Command == "AL") //ANGLE CUT LEFT
  {
    Serial.println("ANGLE CUT LEFT ");
    Angle_Cut(-float(Data.toFloat() * 120));
    return 1;
  }
  else if (Command == "MB") //MOVE BACK
  {
    Serial.println("Move back");
    Execute_Forward(Data);
    Serial.println(Data);
    return 1;
  }
  else if (Command == "MF") //MOVE FORWARD
  {
    Serial.println("Move forward");
    Appl_StartRunning_xdu = true;
    Execute_Forward(Data);
    Serial.println(Data);

    return 1;
  }
  else if (Command == "AC") //ANGLE CUT
  {
    Serial.println("ANGLE CUT");
    Execute_Cut(Data);
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    Serial.println(Data);
    return 1;
  }
  else if (Command == "AI") //ANGLE CUT IN
  {
    Serial.println("ANGLE CUT IN");
    Execute_Cut("-10");
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "LP") //ANGLE CUT
  {
    Serial.println("Set Length on pulse");
    Serial.println(Data.toFloat());
    return 1;
  }
  else if (Command == "MK") //KEP MOVE
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    return 1;
  }
  else if (Command == "MN") //NHA MOVE
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
    return 1;
  }
  else
  {
    return 0;
  }
}

void Execute_String(String Data_Input)
{
  static uint32_t Length_Data;
  static uint32_t i;
  static uint32_t j;
  static uint32_t count;
  Length_Data = Data_Input.length();
  Serial.println(Data_Input);

  if (Execute_Manual(Data_Input) == 1)
  {
    return;
  }

  for (i = 0; i <= 2; i++)
  {
    if (Data_Input.substring(i, i + 1) == "F" || Data_Input.substring(i, i + 1) == "C" || Data_Input.substring(i, i + 1) == "W" ||
        Data_Input.substring(i, i + 1) == "S" || Data_Input.substring(i, i + 1) == "E" || Data_Input.substring(i, i + 1) == "T" || Data_Input.substring(i, i + 1) == "Z")
    {
      count++;
      Serial.println(count);
      for (j = i + 1; j < j + 10; j++)
      {
        if (Data_Input.substring(j, j + 1) == "_")
        {
          String Data_Command = Data_Input.substring(i + 1, j);
          String Command = Data_Input.substring(i, i + 1);
          if (Command == "F")
          {
            Serial2.println(Command + Data_Command);
            Serial.println("Execute_Forward");
            Execute_Forward(Data_Command);
          }
          else if (Command == "C")
          {
            Serial2.println(Command + Data_Command);
            Serial.println("Execute_Cut");
            Execute_Cut(Data_Command);
          }
          else if (Command == "S")
          {
            Serial.println("Execute_Cut First");
            Serial2.println(Command);
            Execute_Cut_First_End(Data_Command, EXECUTE_FIRST);
          }
          else if (Command == "T")
          {
            Serial.println("Starting");
            Appl_StartRunning_xdu = true;
            Appl_ButtonStopPress_xdu = false;
            digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
          }
          else if (Command == "E")
          {
            Serial2.println(Command);
            Serial.println("Execute_Cut End");
            Execute_Cut_First_End(Data_Command, EXECUTE_END);
          }
          else if (Command == "W")
          {
            Serial2.println(Command);
            Appl_SystemState_xdu8 = FINISH_LETTER_STATE;
            Serial.println("FINISH LETTER");
          }
          else if (Command == "Z")
          {
            //Serial2.println(Command);
            Appl_FinishTransfer_xdu = true;
            Serial.println("FINISH ALL LETTER");
          }
          else
          {
            //Do nothing
          }
          Serial.println(Data_Input.substring(i, j));
          break;
        }
      }
    }
    else
    {
      Serial.println("_False");
    }
  }
}

void Execute_Forward(String Data_Input)
{
  static float Appl_DataLengthFloat_fdu32;
  static float Appl_DataLengthFloatRemainEnd_fdu32;
  static unsigned long Step;
  static unsigned long StepForSmallMove;

  Appl_Forward_Trigger_xdu = true;
  Appl_FisrtPulse_xdu8 = 0;
  Appl_LastPulse_xdu8 = 0;
  Appl_DataLengthFloat_fdu32 = Data_Input.toFloat();
  NumHoles_AlreadyRun_xdu32 = 0;

  Holes_HaveToRun_xdu32 = Appl_DataLengthFloat_fdu32 / 5.002;
  Appl_DataLengthFloatRemainEnd_fdu32 = Appl_DataLengthFloat_fdu32 - Holes_HaveToRun_xdu32 * 5.002;

  Step = (Appl_DataLengthFloatRemainEnd_fdu32 / 0.011090301);
  StepForSmallMove = (Appl_DataLengthFloat_fdu32 / 0.011090301);

  Serial.print("Holes HaveToRun : ");
  Serial.println(Holes_HaveToRun_xdu32);
  Serial.print("Remain end: ");
  Serial.println(Appl_DataLengthFloatRemainEnd_fdu32);
  Serial.print("NumHolesFromAToB_xdu8: ");
  Serial.println(Appl_NumHolesFromAToB_xdu8);

  if (Appl_DataLengthFloat_fdu32 > 0)
  {
    if (Appl_DataLengthIsRemain_xdu or digitalRead(SS1_MOVE_HOME_A_PIN) == 1) // Sensor bị che=> lưng chừng
    {
      if (Appl_NumHolesFromAToB_xdu8 >= 43)
      {
        digitalWrite(SOL_CLAMPER_PIN, HIGH); //kẹp cắt
        delay(1000);
        digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW); //nhả kẹp kéo
        delay(500);
        Home_Move_A();

        Appl_NumHolesFromAToB_xdu8 = 0;
        Forward_Move_First();
        Appl_NumHolesFromAToB_xdu8++;

        Serial.println("Forward starting");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(500);
        }
        digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(300);
        }
      }
      else
      {
        if (Appl_DataLengthFloat_fdu32 > 0 and Appl_DataLengthFloat_fdu32 < 50 and (Appl_NumHolesFromAToB_xdu8 + Holes_HaveToRun_xdu32) < 43)
        {
          Serial.println("Forward starting");
          digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(500);
          }
          digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(300);
          }
          Forward_Move(StepForSmallMove);
          return;
        }
        else
        {
          digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep cat
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(300);
          }
          digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW); // Tha phoi
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(500);
          }
          Forward_Move_1Step();
          Appl_NumHolesFromAToB_xdu8++;
          Serial.println("Forward starting");
          digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(500);
          }
          digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
          if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
          {
            delay(300);
          }
        }
      }
    }
    else
    {
      if (Appl_DataLengthFloat_fdu32 > 0 and Appl_DataLengthFloat_fdu32 < 50 and (Appl_NumHolesFromAToB_xdu8 + Holes_HaveToRun_xdu32) < 43)
      {
        Serial.println("Forward starting");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(500);
        }
        digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(300);
        }
        Forward_Move(StepForSmallMove);
        return;
      }
      digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
      if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
      {
        delay(300);
      }
    }

    while (NumHoles_AlreadyRun_xdu32 < Holes_HaveToRun_xdu32 and Appl_ButtonStopPress_xdu == false and (digitalRead(BUTTON_EMERGENCY_PIN) == 0))
    {
      if (Appl_NumHolesFromAToB_xdu8 >= 44 or digitalRead(SS2_MOVE_HOME_B_PIN) == 1)
      {
        delay(500);
        digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep cat
        delay(500);
        digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW); // Tha phoi
        delay(300);
        Home_Move_A();
        delay(300);

        Appl_NumHolesFromAToB_xdu8 = 0;
        Forward_Move_First();
        Appl_NumHolesFromAToB_xdu8++;
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi

        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(500);
        }
        digitalWrite(SOL_CLAMPER_PIN, LOW); // Tha cat
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(300);
        }
      }
      else
      {
        if (NumHoles_AlreadyRun_xdu32 < Holes_HaveToRun_xdu32)
        {
          NumHoles_AlreadyRun_xdu32 += Forward_Move_Holes(Holes_HaveToRun_xdu32 - NumHoles_AlreadyRun_xdu32);
          //        Serial.print("Holes HaveToRun : ");
          //        Serial.println(Holes_HaveToRun_xdu32);
          //        Serial.print("NumHoles AlreadyRun : ");
          //        Serial.println(NumHoles_AlreadyRun_xdu32);
        }
        else
        {
          // Do notthing
        }
      }
    }
    Serial.print("Holes HaveToRun : ");
    Serial.println(Holes_HaveToRun_xdu32);
    Serial.print("NumHoles AlreadyRun : ");
    Serial.println(NumHoles_AlreadyRun_xdu32);

    if (Step != 0)
    {
      Appl_DataLengthIsRemain_xdu = true;
    }
    else
    {
      Appl_DataLengthIsRemain_xdu = false;
    }
    if (Appl_NumHolesFromAToB_xdu8 >= 45 or digitalRead(SS2_MOVE_HOME_B_PIN) == 1)
    {
      digitalWrite(SOL_CLAMPER_PIN, HIGH);
      delay(1000);
      digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
      delay(300);
      Home_Move_A();
      delay(300);
      Appl_NumHolesFromAToB_xdu8 = 0;
      Forward_Move_First();
      Appl_NumHolesFromAToB_xdu8++;
      delay(500);
      digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
      if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
      {
        delay(500);
      }
      digitalWrite(SOL_CLAMPER_PIN, LOW);
      if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
      {
        delay(300);
      }
    }
    Forward_Move(Step);
    Serial.println("Execute_Forward Done");
    digitalWrite(SOL_CLAMPER_PIN, LOW);
  }
  else
  {
    static unsigned long Step_Remain;
    static unsigned long Step;
    Step = -float(Appl_DataLengthFloat_fdu32 / 0.011090301);
    Serial.println("Backward starting");
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(1000);
    }
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(300);
    }
    Step_Remain = Backward_Move(Step);
    Serial.print("Xung con lai: ");
    Serial.println(Step_Remain);
    if (Step_Remain == 0)
    {
      Serial.println("Execute_Backward Done");
      digitalWrite(SOL_CLAMPER_PIN, LOW);
    }
    else
    {
      while (Step_Remain != 0 and (digitalRead(BUTTON_EMERGENCY_PIN) == 0))
      {
        digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
        delay(1000);
        Home_Move_B();
        digitalWrite(SOL_CLAMPER_PIN, LOW);
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
        if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
        {
          delay(1000);
        }
        Step_Remain = Backward_Move(Step_Remain);
        Serial.print("Xung con lai: ");
        Serial.println(Step_Remain);
        if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true)
        {
          return;
        }
      }
      Serial.println("Execute_Backward Done");
      digitalWrite(SOL_CLAMPER_PIN, LOW);
    }
  }
  Appl_Forward_Trigger_xdu = false;
}

void Execute_Cut(String Data_Input)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int Step;
  Data_Angle_Float_Raw = (Data_Input.toFloat());
  Serial.println(Data_Angle_Float_Raw);
  if (Data_Angle_Float_Raw <= 45 and Data_Angle_Float_Raw >= 0)
  {
    Data_Angle_Float = 0;
  }
  else if (Data_Angle_Float_Raw > 45)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
    if (Data_Angle_Float >= 40)
    {
      Data_Angle_Float = 40;
    }
  }
  else
  {
    Data_Angle_Float = Data_Angle_Float_Raw;
  }
  Serial.print("Goc cat: ");
  Serial.println(Data_Angle_Float);
  Step = float(Data_Angle_Float * 120);

  if (Step >= 0 and Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(1000);
    }
    Angle_Cut(Step);                    // Xoay dao
    Brushless_Run(BRUSHLESS_SPEED);     // Brushless quay
    digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(2000);
    }
    Cutter_Backward(); // Backward Cut
    while (digitalRead(SS7_END_STROKE_FRONT_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == INIT_STATE)
      {
        return;
      }
    }
    Appl_CutterBackwardTrigger_xdu = false;
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(TIME_CUTTER);
    }
    Angle_Cut(-2 * Step); // Xoay dao
    Cutter_Forward();     // Forward Cut
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == INIT_STATE)
      {
        return;
      }
    }
    Brushless_Off();
    Angle_Cut(Step); // Xoay dao

    if (Appl_ButtonStopPress_xdu == true)
    {
      digitalWrite(SOL_CLAMPER_PIN, HIGH); // Keop  phoi
    }
    else
    {
      // digitalWrite(SOL_CLAMPER_PIN,LOW);   // Tha phoi
    }
  }
  else if (Step < 0 and Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(1000);
    }
    Brushless_Run(BRUSHLESS_SPEED);     // Brushless quay
    digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(2000);
    }
    Cutter_Backward(); // Backward Cut
    while (digitalRead(SS7_END_STROKE_FRONT_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == INIT_STATE)
      {
        return;
      }
    }
    Appl_CutterBackwardTrigger_xdu = false;
    Cutter_Forward(); // Forward Cut
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true)
      {
        return;
      }
    }
    Brushless_Off(); // Brushless off
    if (Appl_ButtonStopPress_xdu == true)
    {
      digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep  phoi
    }
    else
    {
    }
  }
}

void Execute_Cut_First_End(String Data_Input, bool type)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int Step;
  Data_Angle_Float_Raw = (Data_Input.toFloat());
  Serial.println(Data_Angle_Float_Raw);

  if (Data_Angle_Float_Raw <= 45 and Data_Angle_Float_Raw >= 0)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
    //Data_Angle_Float= (Data_Angle_Float_Raw/2);
    //Data_Angle_Float  = 22.5 - Data_Angle_Float;
  }

  else if (Data_Angle_Float_Raw > 45)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
    //Data_Angle_Float  = 22.5 - Data_Angle_Float;
    if (Data_Angle_Float >= 40)
    {
      Data_Angle_Float = 40;
    }
  }
  else
  {
    Data_Angle_Float_Raw = 0;
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
  }
  Serial.print("Goc cat: ");
  Serial.println(Data_Angle_Float);
  Step = float(Data_Angle_Float * 120);

  if (/*Step>=0 and */ Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(1000);
    }
    if (type == EXECUTE_FIRST)
    {
      Angle_Cut(-Step); // Xoay dao
    }
    else
    {
      Angle_Cut(Step); // Xoay dao
    }
    Brushless_Run(BRUSHLESS_SPEED);     // Brushless quay
    digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(2000);
    }
    Cutter_Backward(); // Backward Cut
    while (digitalRead(SS7_END_STROKE_FRONT_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == INIT_STATE)
      {
        return;
      }
    }
    Appl_CutterBackwardTrigger_xdu = false;
    if (Appl_ButtonStopPress_xdu == false and digitalRead(BUTTON_EMERGENCY_PIN) == 0)
    {
      delay(TIME_CUTTER);
    }
    Cutter_Forward(); // Forward Cut
    while (digitalRead(SS4_END_STROKE_BACK_PIN) == 0)
    {
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1 or Appl_ButtonStopPress_xdu == true)
      {
        return;
      }
    }
    Brushless_Off();
    if (type == EXECUTE_FIRST)
    {
      Angle_Cut(Step); // Xoay dao
    }
    else
    {
      Angle_Cut(-Step); // Xoay dao
    }
    if (Appl_ButtonStopPress_xdu == true)
    {
      digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep  phoi
    }
    else
    {
      // digitalWrite(SOL_CLAMPER_PIN,LOW);   // Tha phoi
    }
  }
}

void Stop()
{
  Brushless_Off();
  Cutter_Forward_Normal(); // Forward Cut
  Appl_Second_xdu8 = 0;
  Appl_CutterBackwardTrigger_xdu = false;
  Serial2.println(2);
}

void Start()
{
  Serial2.println(4);
}

void Pause()
{
  Serial2.println(3);
  Appl_SystemState_xdu8 = 1; //RECIEVE_AND_RUNNING_STATE
}

void Emergency()
{
  Brushless_Off();
  digitalWrite(STEPPER_MOVE_ENA_PIN, LOW);
  digitalWrite(STEPPER_CUT_ENA_PIN, LOW);
  digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
  digitalWrite(SOL_SLIDE_BACKWARD_PIN, LOW);
  digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
  digitalWrite(SOL_LIFTER_PIN, LOW);
  Serial2.println(5);
}

void Button_Press_Buzzer()
{
  digitalWrite(MATERIAL_STATUS, HIGH);
  delay(4000);
  digitalWrite(MATERIAL_STATUS, LOW);
}

/* End of file -------------------------------------------------------- */
