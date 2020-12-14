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
#define CUTTER_STEP_PER_DEGREE (240)
#define LENGTH_OF_ONE_HOLE (4.998)
//#define LENGTH_OF_ONE_STEP          (0.011090301)
#define LENGTH_OF_ONE_STEP (0.01118976558)

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
static inline void m_wait_for_cutter_go_out(void);
static inline void trigger_change(void);
static inline void m_wait_to_cutter_go_midle(void);
static inline void m_wait_to_cutter_go_in(void);
static inline void m_button_stop_delay(uint16_t ms);
static inline void m_move_home_a(void);
static inline void m_move_less_than_5cm(unsigned long step);

static void m_cutter_cut_out(int32_t step);
static void m_cutter_cut_in(void);

/* Function definitions ----------------------------------------------- */
int Execute_Manual(String Data_Input)
{
  static uint32_t Length_Data;

  // Analyze data receive from computer software
  Length_Data = Data_Input.length();
  String Command = Data_Input.substring(0, 2);
  String Data = Data_Input.substring(3, Length_Data);

  if (Command == "HA") // Home move A
  {
    Home_Move_A();
    return 1;
  }
  else if (Command == "HM") // Home All
  {
    Appl_ButtonStopPress_xdu = true;
    return 1;
  }
  else if (Command == "CN") // Connection
  {
    DELAY(100);
    DATA_SEND_TO_PC(RES_CONNECTED);
    return 1;
  }
  else if (Command == "HB") // Home move B
  {
    Home_Move_B();
    return 1;
  }
  else if (Command == "HC") // Home cutter
  {
    Home_And_Center_Cutter();
    return 1;
  }
  else if (Command == "CF") // Cutter forward
  {
    Cutter_Forward();
    return 1;
  }
  else if (Command == "CB") // Cutter backward
  {
    Cutter_Backward();
    return 1;
  }
  else if (Command == "CU") // Cutter up
  {
    GPIO_SET(SOL_LIFTER_PIN, LOW);
    return 1;
  }
  else if (Command == "CD") // Cutter down
  {
    GPIO_SET(SOL_LIFTER_PIN, HIGH);
    return 1;
  }
  else if (Command == "MO") // Motor on
  {
    Brushless_Run(BRUSHLESS_SPEED);
    return 1;
  }
  else if (Command == "MX") // Motor off
  {
    Brushless_Off();
    return 1;
  }
  else if (Command == "KP") //
  {
    GPIO_SET(SOL_CLAMPER_PIN, HIGH);
    return 1;
  }
  else if (Command == "NP") //
  {
    GPIO_SET(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "AR") // Anger cut right
  {
    Angle_Cut(float(Data.toFloat() * 120));
    return 1;
  }
  else if (Command == "AL") // Anger cut left
  {
    Angle_Cut(-float(Data.toFloat() * 120));
    return 1;
  }
  else if (Command == "MB") // Move back
  {
    Execute_Move(Data);
    return 1;
  }
  else if (Command == "MF") // Move forward
  {
    Appl_StartRunning_xdu = true;
    Execute_Move(Data);
    return 1;
  }
  else if (Command == "AC") // Angle cut
  {
    Execute_Cut(Data);
    GPIO_SET(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "AI") // Angle cut in
  {
    Execute_Cut("-10");
    GPIO_SET(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "MK") // Move clamp push
  {
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
    return 1;
  }
  else if (Command == "MN") // Move clamp release
  {
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
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
  SERIAL_DATA_MONITOR(Data_Input);

  // Check and excecute manual data
  if (Execute_Manual(Data_Input) == 1)
  {
    return;
  }

  // Analyze data
  for (i = 0; i <= 2; i++)
  {
    // If data is correct
    if ((Data_Input.substring(i, i + 1) == "F") || (Data_Input.substring(i, i + 1) == "C") || (Data_Input.substring(i, i + 1) == "W") ||
        (Data_Input.substring(i, i + 1) == "S") || (Data_Input.substring(i, i + 1) == "E") || (Data_Input.substring(i, i + 1) == "T") ||
        (Data_Input.substring(i, i + 1) == "Z"))
    {
      count++;
      LOG("Count: %d", count);

      // Loop for the command
      for (j = i + 1; j < j + 10; j++)
      {
        if (Data_Input.substring(j, j + 1) == "_")
        {
          String Data_Command = Data_Input.substring(i + 1, j);
          String Command = Data_Input.substring(i, i + 1);

          // Check command and excecute
          if (Command == "F")
          {
            DATA_SEND_TO_PC(Command + Data_Command);
            LOG("Execute_Move");
            Execute_Move(Data_Command);
          }
          else if (Command == "C")
          {
            DATA_SEND_TO_PC(Command + Data_Command);
            LOG("Execute_Cut");
            Execute_Cut(Data_Command);
          }
          else if (Command == "S")
          {
            DATA_SEND_TO_PC(Command);
            LOG("Execute_Cut First");
            Execute_Cut_First_End(Data_Command, EXECUTE_FIRST);
          }
          else if (Command == "T")
          {
            LOG("Starting");
            Appl_ButtonStopPress_xdu = false;
            Appl_StartRunning_xdu = true;
            GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
          }
          else if (Command == "E")
          {
            DATA_SEND_TO_PC(Command);
            LOG("Execute_Cut End");
            Execute_Cut_First_End(Data_Command, EXECUTE_END);
          }
          else if (Command == "W")
          {
            DATA_SEND_TO_PC(Command);
            Appl_SystemState_xdu8 = SYS_FINISH_LETTER_STATE;
            LOG("FINISH LETTER");
          }
          else if (Command == "Z")
          {
            Appl_FinishTransfer_xdu = true;
            LOG("FINISH ALL LETTER");
          }
          else
          {
            //Do nothing
          }
          SERIAL_DATA_MONITOR(Data_Input.substring(i, j));
          break;
        }
      }
    }
    else
    {
      // LOG("Data is incorrect");
    }
  }
}

void Execute_Move(String Data_Input)
{
  static float Appl_DataLengthFloat_fdu32;
  static float Appl_DataLengthFloatRemainEnd_fdu32;
  static unsigned long Step;
  static unsigned long StepForSmallMove;
  // disable motor cắt
  GPIO_SET(STEPPER_CUT_ENA_PIN, LOW);
  Appl_Forward_Trigger_xdu = true;

  Appl_FisrtPulse_xdu8 = 0;
  Appl_LastPulse_xdu8 = 0;
  Appl_DataLengthFloat_fdu32 = Data_Input.toFloat();
  NumHolesAlreadyRun_xdu32 = 0;

  Holes_HaveToRun_xdu32 = Appl_DataLengthFloat_fdu32 / (float)LENGTH_OF_ONE_HOLE;
  Appl_DataLengthFloatRemainEnd_fdu32 = Appl_DataLengthFloat_fdu32 - (Holes_HaveToRun_xdu32 * (float)LENGTH_OF_ONE_HOLE);

  Step = (Appl_DataLengthFloatRemainEnd_fdu32 / (float)LENGTH_OF_ONE_STEP);
  StepForSmallMove = (Appl_DataLengthFloat_fdu32 / (float)LENGTH_OF_ONE_STEP);
  LOG("length: %d", Appl_DataLengthFloat_fdu32);
  SERIAL_DATA_MONITOR(Appl_DataLengthFloat_fdu32);
  LOG("Holes have to run: %d", Holes_HaveToRun_xdu32);
  LOG("Remain end :");
  SERIAL_DATA_MONITOR(Appl_DataLengthFloatRemainEnd_fdu32);
  LOG("NumHolesFromAToB_xdu8: %d", Appl_NumHolesFromAToB_xdu8);

  // Excecute forward
  if (Appl_DataLengthFloat_fdu32 > 0)
  {
    // Data is remain or Home A
    if ((Appl_DataLengthIsRemain_xdu) or           // Check data is remain
        (IS_SENSOR_DETECTED(SS1_MOVE_HOME_A_PIN))) // Sensor is not in hole
    {
      if (Appl_NumHolesFromAToB_xdu8 >= 43)
      {
        m_move_home_a();
      }
      else
      {
        // Length move is less than 5cm
        if ((Appl_DataLengthFloat_fdu32 > 0) and 
            (Appl_DataLengthFloat_fdu32 < 50) and 
           ((Appl_NumHolesFromAToB_xdu8 + Holes_HaveToRun_xdu32) < 43))
        {
          m_move_less_than_5cm(StepForSmallMove);
          return;
        }
        else // Length move is more than 5cm
        {
          GPIO_SET(SOL_CLAMPER_PIN, HIGH);      // Kep cat
          m_button_stop_delay(300);

          GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);  // Tha phoi
          m_button_stop_delay(500);

          Forward_Move_1Step();
          Appl_NumHolesFromAToB_xdu8++;
          LOG("Forward starting");
          
          GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
          m_button_stop_delay(500);

          GPIO_SET(SOL_CLAMPER_PIN, LOW);       // Tha cat
          m_button_stop_delay(300);
        }
      }
    }
    else  // Data is not remain
    {
      // Length move is less than 5cm
      if ((Appl_DataLengthFloat_fdu32 > 0) and 
          (Appl_DataLengthFloat_fdu32 < 50) and 
         ((Appl_NumHolesFromAToB_xdu8 + Holes_HaveToRun_xdu32) < 43))
      {
        m_move_less_than_5cm(StepForSmallMove);
        return;
      }
      GPIO_SET(SOL_CLAMPER_PIN, LOW); // Tha cat
      m_button_stop_delay(300);
    }

    // Run all number of holes have to run
    while ((NumHolesAlreadyRun_xdu32 < Holes_HaveToRun_xdu32) and
           (Appl_ButtonStopPress_xdu == false))
    {
      if (Appl_NumHolesFromAToB_xdu8 >= 44 or IS_SENSOR_DETECTED(SS2_MOVE_HOME_B_PIN))
      {
        m_move_home_a();
      }
      else
      {
        if (NumHolesAlreadyRun_xdu32 < Holes_HaveToRun_xdu32)
        {
          NumHolesAlreadyRun_xdu32 += Forward_Move_Holes(Holes_HaveToRun_xdu32 - NumHolesAlreadyRun_xdu32);
        }
        else
        {
          // Do notthing
        }
      }
    }

    LOG("Holes HaveToRun : %d", Holes_HaveToRun_xdu32);
    LOG("NumHoles AlreadyRun : %d", NumHolesAlreadyRun_xdu32);

    if (Step != 0)
    {
      Appl_DataLengthIsRemain_xdu = true;
    }
    else
    {
      Appl_DataLengthIsRemain_xdu = false;
    }

    if (Appl_NumHolesFromAToB_xdu8 >= 45 or (IS_SENSOR_DETECTED(SS2_MOVE_HOME_B_PIN)))
    {
      m_move_home_a();
    }

    // Move step remand
    Forward_Move(Step);
    LOG("Execute_Move Done");
    GPIO_SET(SOL_CLAMPER_PIN, LOW);
  }
  else // Excecute backward
  {
    static unsigned long Step_Remain;
    static unsigned long Step;
    Step = -float(Appl_DataLengthFloat_fdu32 / 0.011090301);
    LOG("Backward starting");
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
    m_button_stop_delay(1000);

    GPIO_SET(SOL_CLAMPER_PIN, LOW);
    m_button_stop_delay(300);

    Step_Remain = Backward_Move(Step);
    LOG("Xung con lai: %d", Step_Remain);
    if (Step_Remain == 0)
    {
      LOG("Execute_Backward Done");
      GPIO_SET(SOL_CLAMPER_PIN, LOW);
    }
    else
    {
      while (Step_Remain != 0)
      {
        GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
        DELAY(1000);
        Home_Move_B();
        GPIO_SET(SOL_CLAMPER_PIN, LOW);
        GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH);
        m_button_stop_delay(1000);
        Step_Remain = Backward_Move(Step_Remain);
        LOG("Xung con lai: %d", Step_Remain);
        if (Appl_ButtonStopPress_xdu == true)
        {
          return;
        }
      }
      LOG("Execute_Backward Done");
      GPIO_SET(SOL_CLAMPER_PIN, LOW);
    }
  }
  Appl_Forward_Trigger_xdu = false;
  //enable motor cắt
  GPIO_SET(STEPPER_CUT_ENA_PIN, HIGH);
}

void Execute_Cut(String Data_Input)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int32_t Step;
  GPIO_SET(STEPPER_CUT_ENA_PIN, HIGH);
  Data_Angle_Float_Raw = (Data_Input.toFloat());

  // Calculate Angle cut
  if (Data_Angle_Float_Raw <= 45 and Data_Angle_Float_Raw >= 0)
  {
    Data_Angle_Float = 0;
  }
  else if (Data_Angle_Float_Raw > 45)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);

    // Limit angle cut to 40 degrre
    if (Data_Angle_Float >= 37)
    {
      Data_Angle_Float = 37;
    }
  }
  else
  {
    Data_Angle_Float = Data_Angle_Float_Raw;
  }

  LOG("Angle cut: ");
  SERIAL_DATA_MONITOR(Data_Angle_Float);

  // Calculate step
  Step = float(Data_Angle_Float * CUTTER_STEP_PER_DEGREE);

  if ((Step >= 0) and (Appl_ButtonStopPress_xdu == false))
  {
    LOG("CUT OUT");
    LOG("Step cut out: %d", Step);
    m_cutter_cut_out(Step);
  }
  else if ((Step < 0) and (Appl_ButtonStopPress_xdu == false))
  {
    LOG("CUT IN");
    LOG("Step cut in: %d", Step);
    m_cutter_cut_in();
  }
  else
  {
    // Do nothing
  }
  GPIO_SET(STEPPER_CUT_ENA_PIN, LOW);
}

void Execute_Cut_First_End(String Data_Input, execute_type_t type)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int32_t Step;
  GPIO_SET(OUT_8, HIGH);
  delay(300);
  GPIO_SET(STEPPER_CUT_ENA_PIN, HIGH);
  Data_Angle_Float_Raw = (Data_Input.toFloat());

  // Calculate angle cut
  if (Data_Angle_Float_Raw <= 45 and Data_Angle_Float_Raw >= 0)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
  }

  else if (Data_Angle_Float_Raw > 45)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);

    // Limit angle cut to 40 degrre
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

  LOG("Angle cut: ");
  SERIAL_DATA_MONITOR(Data_Angle_Float);

  // Calculate angle cut
  Step = float(Data_Angle_Float * CUTTER_STEP_PER_DEGREE);

  if (Appl_ButtonStopPress_xdu == false)
  {
    GPIO_SET(SOL_CLAMPER_PIN, HIGH); // Kep phoi

    m_button_stop_delay(800);

    if (type == EXECUTE_FIRST)
    {
      Angle_Cut(Step); // Xoay dao
    }
    else
    {
      Angle_Cut(-Step); // Xoay dao
    }

    Brushless_Run(BRUSHLESS_SPEED); // Brushless quay

    GPIO_SET(SOL_LIFTER_PIN, HIGH); // Ha dao

    m_button_stop_delay(1500);

    Cutter_Backward(); // Backward Cut

    trigger_change();

    m_wait_for_cutter_go_out(); // Wait to cutter go out

    m_button_stop_delay(TIME_CUTTER);

    Cutter_Forward(); // Forward Cut

    delay(500);

    m_wait_to_cutter_go_in(); // Wait to cutter go in

    Brushless_Off(); // Brushless off

    if (type == EXECUTE_FIRST)
    {
      Angle_Cut(-Step); // Xoay dao
    }
    else
    {
      Angle_Cut(Step); // Xoay dao
    }

    if (Appl_ButtonStopPress_xdu == true)
    {
      GPIO_SET(SOL_CLAMPER_PIN, HIGH); // Kep  phoi
    }
    else
    {
      // Do nothing
    }
  }
  else
  {
    // Do nothing
  }
  GPIO_SET(OUT_8, LOW);
  delay(300);
  GPIO_SET(STEPPER_CUT_ENA_PIN, LOW);
}

/* Private function definitions ---------------------------------------- */
static inline void m_wait_for_cutter_go_out(void)
{
  while (1) // Wait for cutter go out
  {
    if ((IS_SENSOR_DETECTED(SS4_END_STROKE_BACK_PIN)) and   // Sensor back is detected
        (IS_SENSOR_DETECTED(SS7_END_STROKE_FRONT_PIN)))         // Sensor front is detected
    {
      break;
    }
    if (Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == SYS_INIT_STATE)
    {
      return;
    }
  }
}
static inline void trigger_change(void)
{
  while (1) // Wait for cutter go out of 2 sensors
  {
     if ((IS_SENSOR_NOT_DETECTED(SS4_END_STROKE_BACK_PIN)) and     // Sensor back is detected
        (IS_SENSOR_NOT_DETECTED(SS7_END_STROKE_FRONT_PIN)))   // Sensor front is not detected
     {
      break;
      }
    if (Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == SYS_INIT_STATE)
    {
      return;
    }
  }
}
static inline void m_wait_to_cutter_go_midle(void)
{
  while (1) // Wait for cutter go midle
  {
    if (IS_SENSOR_DETECTED(SS7_END_STROKE_FRONT_PIN))   // Sensor FRONT is detected
    {
      break;
    }
    if (Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == SYS_INIT_STATE)
    {
      return;
    }
  }
}

static inline void m_wait_to_cutter_go_in(void)
{
  while (1) // Wait for cutter go in
  {
    if (IS_SENSOR_DETECTED(SS4_END_STROKE_BACK_PIN))   // Sensor back is detected
     {
      break;
    }
    if (Appl_ButtonStopPress_xdu == true or Appl_SystemState_xdu8 == SYS_INIT_STATE)
    {
      return;
    }
  }
}

static void m_cutter_cut_out(int32_t step)
{

  GPIO_SET(SOL_CLAMPER_PIN, HIGH);  // Kep phoi

  Brushless_Run(BRUSHLESS_SPEED);   // Brushless quay

  m_button_stop_delay(1000);

  Angle_Cut(step);                  // Xoay dao

  GPIO_SET(SOL_LIFTER_PIN, HIGH);   // Ha dao
  if (step < 400)
  {
    m_button_stop_delay(1500);
  }
  else
  {
    m_button_stop_delay(1000);
  }

  Cutter_Backward();                  // Backward Cut

  m_wait_for_cutter_go_out();         // Wait to cutter go out to end stroke (detect 2 sensors)

  m_button_stop_delay(TIME_CUTTER);

  Angle_Cut(-2 * step);               // Xoay dao

  m_button_stop_delay(400);

  Cutter_Forward();                   // Forward Cut - Set pin

  trigger_change();                   // Trigger 2 sensor OFF
  m_wait_to_cutter_go_in();           // Wait to cutter go in - detect BACK sensor

  Brushless_Off();                    // Brushless off

  Angle_Cut(step);                    // Xoay dao

  if (Appl_ButtonStopPress_xdu == true)
  {
    GPIO_SET(SOL_CLAMPER_PIN, HIGH);  // Kep phoi
  }
  else
  {
    // Do nothing
  }
}

static void m_cutter_cut_in(void)
{
  GPIO_SET(SOL_CLAMPER_PIN, HIGH);    // Kep phoi

  GPIO_SET(SOL_LIFTER_PIN, HIGH);     // Ha dao

  Brushless_Run(BRUSHLESS_SPEED);     // Brushless quay

  m_button_stop_delay(2000);

  Cutter_Backward();                  // Backward Cut
  m_wait_to_cutter_go_midle();        // Wait to cutter go midle - detect sensor FRONT
  Cutter_Forward();                   // Forward Cut - SET PIN
  m_wait_to_cutter_go_in();           // Wait to cutter go in

  Brushless_Off();                    // Brushless off

  if (Appl_ButtonStopPress_xdu == true)
  {
    GPIO_SET(SOL_CLAMPER_PIN, HIGH);  // Kep phoi
  }
  else
  {
    // Do nothing
  }
}

static inline void m_button_stop_delay(uint16_t ms)
{
  if (false == Appl_ButtonStopPress_xdu)
  {
    DELAY(ms);
  }
}

static void m_excecute_forward(void)
{
}

static void m_excecute_backward(void)
{
}

static inline void m_move_home_a(void)
{
  GPIO_SET(SOL_CLAMPER_PIN, HIGH);      // Kẹp cắt

  DELAY(1000);

  GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);  // Nhả kẹp kéo

  DELAY(500);

  Home_Move_A();                        // Home A

  m_button_stop_delay(300);

  Appl_NumHolesFromAToB_xdu8 = 0;
  Forward_Move_First();
  Appl_NumHolesFromAToB_xdu8++;

  LOG("Forward starting");

  GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
  m_button_stop_delay(500);

  GPIO_SET(SOL_CLAMPER_PIN, LOW);       // Tha cat
  m_button_stop_delay(300);
}

static inline void m_move_less_than_5cm(unsigned long step)
{
  Appl_DataLengthIsRemain_xdu = true;
  LOG("Forward starting");
  if (Appl_NumHolesFromAToB_xdu8 == 0)
  {
    GPIO_SET(SOL_CLAMP_FEEDER_PIN, LOW);
    delay(200);
    Forward_Move_First();
  }
  GPIO_SET(SOL_CLAMP_FEEDER_PIN, HIGH); // Kep phoi
  m_button_stop_delay(300);

  GPIO_SET(SOL_CLAMPER_PIN, LOW);       // Tha cat
  m_button_stop_delay(300);

  Forward_Move(step);
}

/* End of file -------------------------------------------------------- */
