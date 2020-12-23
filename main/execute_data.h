#ifndef Execute_Data_h
#define Execute_Data_h

#define BRUSHLESS_SPEED 160
#define TIME_CUTTER 5000
extern uint8_t Appl_SystemState_xdu8;
extern bool Appl_NoMaterial_xdu;
extern bool Appl_EmergencyHold_xdu;

bool Appl_FinishTransfer_xdu = false;

int Execute_Manual(String Data_Input);
void EVxecute_Auto(String Data_Input);
void Execute_String(String Data_Input);
void Execute_Forward(String Data_Input);
void Execute_Cut(String Data_Input);
void Execute_Cut_First_End(String Data_Input, bool type);
void Start();
void Pause();
void Emergency();
void Button_Press_Buzzer();
void return0index();
void Forward_1_Hole();
void Moveto_0Hole();
void check_and_return();
void check_and_break();
void process_with_pause();
void process_with_material();

int Execute_Manual(String Data_Input)
{
  static uint32_t Length_Data;
  Length_Data = Data_Input.length();
  String Command = Data_Input.substring(0, 2);
  String Data = Data_Input.substring(3, Length_Data);
  if (Command == "HA") // MOVE_HOMA
  {
    Home_Move_A();
    return 1;
  }
  else if (Command == "HM") // HOME ALL
  {
    Serial2.println("LOG Home machine. Send 6 to PC");
    delay(200);
    Serial2.println(6);
    home_done = false;
    Home_All();
    if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
    {
      return;
    }
    Serial2.println(1);
    home_done = true;
    return 1;
  }
  else if (Command == "CN") // KẾT NỐI
  {
    Serial2.println(10);
    Serial2.println("LOG Send Buzzer status");
    if (buzzer_needed == true)
    {
      Serial2.println("BZON");
    }
    else
    {
      Serial2.println("BZOF");
    }
    Serial2.println("LOG Send Home status");
    if (home_done == true)
    {
      Serial2.println("HDONE");
    }
    else
    {
      Serial2.println("HFAIL");
    }
    return 1;
  }
  else if (Command == "ST") // STOP
  {
    Serial2.println("LOG STOP");

    return 1;
  }
  else if (Command == "PS") // PAUSE
  {
    Serial2.println("LOG PAUSE");
    return 1;
  }
  else if (Command == "HB") //MOVE_HOMB
  {
    Serial2.println("LOG home-b");
    Home_Move_B();
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "HC") //HOME_CUT
  {
    Serial2.println("LOG home-cut");
    Cutter_Backward(); // Forward Cut
    delay(200);
    Home_Cutter();
    if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
    {
      return 0;
      return;
    }
    delay(100);
    Center_Stepper_Cutter();
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "CF") //CUTTER FW
  {
    Serial2.println("LOG CUTTER FW");
    Cutter_Forward();
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "CB") //CUTTER BW
  {
    Serial2.println("LOG CUTTER BW");
    Cutter_Backward();
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "CU") //CUTTER UP
  {
    Serial2.println("LOG CUTTER UP");
    digitalWrite(SOL_LIFTER_PIN, LOW); // Nang dao
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "CD") //CUTTER DW
  {
    Serial2.println("LOG CUTTER DW");
    digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
    Serial2.println("HFAIL");
    home_done = false;                                   // digitalWrite(SOL_CLAMPER_PIN,HIGH);   // Ha dao
    return 1;
  }
  else if (Command == "MO") //MOTOR ON
  {
    Serial2.println("LOG MOTOR ON");
    Brushless_Run(BRUSHLESS_SPEED);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "MX") //MOTOR OFF
  {
    Serial2.println("LOG MOTOR OFF");    
    Brushless_Off();
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "BR") //MOTOR OFF
  {
    Serial2.println("LOG BUZZER RESET");
    digitalWrite(MATERIAL_STATUS, LOW);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "KP") //KEP PHOI
  {
    Serial2.println("LOG KEP PHOI");    
    digitalWrite(SOL_CLAMPER_PIN, HIGH);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "NP") //NHẢ PHOI
  {
    Serial2.println("LOG KEP PHOI");    
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "AR") //ANGLE CUT RIGHT
  {
    Serial2.println("LOG ANGLE CUT RIGHT");    
    Angle_Cut(float(Data.toFloat() * 120));
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "AL") //ANGLE CUT LEFT
  {
    Serial2.println("LOG ANGLE CUT LEFT ");    
    Angle_Cut(-float(Data.toFloat() * 120));
    Serial2.println("HFAIL");
    home_done = false;;
    return 1;
  }
  else if (Command == "MB") //MOVE BACK
  {
    Serial2.println("LOG Move back");
    Execute_Forward(Data);     
    Serial2.println("XB" + Data);    
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "MF" and Data.toFloat() > 0) //MOVE FORWARD
  {
    
    Serial2.println("LOG Move forward");
    Appl_StartRunning_xdu = true;
    Execute_Forward(Data);    
    Serial2.println("XF" + Data);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "AC") //ANGLE CUT
  {
    Serial2.println("LOG ANGLE CUT");
    Execute_Cut(Data); //Execute_Cut
    Serial2.println("XC" + Data);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "AI") //ANGLE CUT IN
  {
    Serial2.println("LOG ANGLE CUT IN");
    Execute_Cut("-10");
    Serial2.println("HFAIL");
    home_done = false;
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    return 1;
  }
  else if (Command == "LP") //ANGLE CUT
  {
    Serial2.println("LOG Set Length on pulse");
    Serial2.println(Data.toFloat());
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "MK") //KEP MOVE
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Command == "MN") //NHA MOVE
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
    Serial2.println("HFAIL");
    home_done = false;
    return 1;
  }
  else if (Data_Input == "BZON")
  {
    Serial2.println("LOG Allow buzzer");
    buzzer_needed = true;
    digitalWrite(MATERIAL_STATUS, HIGH);
    delay(1200);
    digitalWrite(MATERIAL_STATUS, LOW);
    return 1;
  }
  else if (Data_Input == "BZOF")
  {
    Serial2.println("LOG Not allow buzzer");
    buzzer_needed = false;
    digitalWrite(MATERIAL_STATUS, HIGH);
    delay(500);
    digitalWrite(MATERIAL_STATUS, LOW);

    return 1;
  }
  else
  {
    return 0;
  }
}
void Execute_String(String Data_Input)
{
  uint32_t Length_Data;
  Length_Data = Data_Input.length() - 1;
  String Command = Data_Input.substring(0, 1);
  String Data_Command = Data_Input.substring(1, Length_Data);
  display_string = "LOG Command + Data_Command :" + Command + Data_Command;
  Serial2.println(display_string);
  if (Command == "F")
  {
    Serial2.println(Command + Data_Command);
    Serial2.println("LOG Execute_Forward");
    if (Data_Command.toFloat() > 0)
    {
      Execute_Forward(Data_Command);
      Serial2.println("LOG Send finished signal " + Data_Input );
      Serial2.println("X"+Data_Input);   
    }    
  }
  else if (Command == "C")
  {
    Serial2.println(Command + Data_Command);
    Serial2.println("LOG Execute_Cut");
    Execute_Cut(Data_Command);
    Serial2.println("LOG Send finished signal " + Data_Input );
    Serial2.println("X"+Data_Input);
  }
  else if (Command == "S")
  {
    Serial2.println("LOG Execute_Cut First");
    Serial2.println(Data_Input);
    Execute_Cut_First_End(Data_Command, EXECUTE_FIRST);
    Serial2.println("LOG Send finished signal " + Data_Input );
    Serial2.println("X"+Data_Input);
  }
  else if (Command == "T")
  {
    Serial2.println("LOG Starting");
    Appl_StartRunning_xdu = true;
    Appl_ButtonStopPress_xdu = false;
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
  }
  else if (Command == "E")
  {
    Serial2.println(Data_Input);
    Serial2.println("LOG Execute_Cut End");
    Execute_Cut_First_End(Data_Command, EXECUTE_END);
    Serial2.println("LOG Send finished signal " + Data_Input );
    Serial2.println("X"+Data_Input);
  }
  else if (Command == "W")
  {
    Serial2.println(66);
    Serial2.println(Command);
    Appl_SystemState_xdu8 = FINISH_LETTER_STATE;
    Serial2.println("LOG FINISH LETTER");
  }
  else if (Command == "Z")
  {
    Serial2.println("Z");
    Appl_SystemState_xdu8 = FINISH_STATE;
    //Appl_FinishTransfer_xdu = true;
    delay(100);
    //Serial2.println(8);
    Serial2.println("LOG FINISH ALL LETTER");
  }
  else
  {
    Serial2.println("LOG _False");
  }
}

void Execute_Forward(String Data_Input)
{
  static float leng_move;
  static int32_t holes;
  static int32_t Step_truoc;
  static int32_t Step_sau;
  static int32_t Step;
  uint16_t prev_holes;
  leng_move = Data_Input.toFloat();
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial2.println(6);
    Serial2.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
    STEPPER_MOVE.stop();
    return;
  }  
  Serial2.println("LOG leng move: " +String(leng_move));
  if (digitalRead(SS1_MOVE_HOME_A_PIN) == 1) // đang ở Home A, dời lên lỗ 0
  {    
    return0index();   
  }
  if (digitalRead(SOL_CLAMP_FEEDER_PIN) == 0)
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    delay(300);
  }
  check_and_return();
  if (digitalRead(SOL_CLAMPER_PIN) == 1)
  {
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    delay(300);
  }
  else if (hole_index == 43) // lên lỗ đầu tiên và reset hole_index
  {
    return0index();
    
    if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
    {
      return;
    }
  }
  if (leng_move < (delta - du_truoc))
  {
    Serial2.println("LOG Step 2 : leng move nho hon delta. move length ");
    Step = leng_move / leng_per_step;
    STEPPER_MOVE.setSpeed(2000);
    uint16_t last_pos = STEPPER_MOVE.currentPosition();  
    Serial2.println("LOG Current pos: " + STEPPER_MOVE.currentPosition());
    while (STEPPER_MOVE.currentPosition() - last_pos < Step)
    {
      STEPPER_MOVE.runSpeed();
      if (digitalRead(BUTTON_STOP_PIN) == 1)
      {
        Serial2.println("LOG Stop press");
        Serial2.println(2); // nhấn stop
        STEPPER_MOVE.stop();
        Appl_ButtonStopPress_xdu = true;
        Serial2.println(6);
      }
    }    
    STEPPER_MOVE.stop();
    Serial2.println("LOG Pos after move: " + STEPPER_MOVE.currentPosition());
    du_truoc += leng_move;
    check_and_return();
    if (digitalRead(SOL_CLAMPER_PIN) == 1)
    {
      digitalWrite(SOL_CLAMPER_PIN, LOW);
      delay(300);
    }
    check_and_return();
    
  }
  else
  {
    if (hole_index == 43 and du_truoc > 0)
    {
      Serial2.println("LOG hole_index = 43. Moving to 0 index");
      return0index();
      if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
      {
        return;
      }
    }
    // move phan dư truoc toi lo tiep
    if (du_truoc > 0)
    {
      leng_move -= delta - du_truoc;
      Step_truoc = (delta - du_truoc) / leng_per_step;
      Serial2.println("LOG Doan move con lai sau khi tru du truoc : " + String(delta - du_truoc));
      Serial2.println("LOG Du truoc: " + String(du_truoc));
      Serial2.println("LOG Step du truoc: " + String(Step_truoc));
      STEPPER_MOVE.setSpeed(3000);
      uint16_t last_pos = STEPPER_MOVE.currentPosition();      
      Serial2.println("LOG Current pos: " + String(STEPPER_MOVE.currentPosition()));
      while (STEPPER_MOVE.currentPosition() - last_pos < Step_truoc)
      {
        STEPPER_MOVE.runSpeed();
        if (digitalRead(BUTTON_STOP_PIN) == 1)
        {
          Serial2.println(2); // nhấn stop
          Serial2.println("LOG Stop press");
          STEPPER_MOVE.stop();
          Appl_ButtonStopPress_xdu = true;
          Serial2.println(6);
        }
      }
      Serial2.println("LOG Possition after move: " + String(STEPPER_MOVE.currentPosition()));
      hole_index += 1;
    }
    holes = leng_move / delta;
    du_sau = leng_move - delta * holes;
    Step_sau = du_sau / leng_per_step;
    Serial2.println("LOG Step 3: Holes phai move: " + String(holes));
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      STEPPER_MOVE.stop();
      
      Serial2.println(6);
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu == true;
      return;
    }
    //xong move phan du
    prev_holes = holes;
    while (holes > 0 and Appl_ButtonStopPress_xdu == false)
    {
      if (hole_index == 43 or digitalRead(SS1_MOVE_HOME_A_PIN) == 1)
      {
        STEPPER_MOVE.stop();
        delay(50);
        return0index();       
        if (Appl_ButtonStopPress_xdu == true)
        {
          STEPPER_MOVE.stop();
          return;
        }
      }
      while (digitalRead(SS6_MATERIAL_PIN) == 0) // check material
      {
        process_with_material();
        if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
        {
          break;
        }
      }
      if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
      {
        return;
      }
      if (digitalRead(BUTTON_PAUSE_PIN) == 0) // check pause 
      {
        process_with_pause();
        if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
        {
          return;
        }
      }
      if (holes == 1 or hole_index == prev_holes or hole_index == 42 or hole_index == 0)
      {
        STEPPER_MOVE.setSpeed(4000);
      }
      else
      {
        STEPPER_MOVE.setSpeed(9000);
      }
      Forward_1_Hole();
      if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
      {
        return;
      }
      holes -= 1;
      hole_index += 1;
    }
    if (Appl_ButtonStopPress_xdu == true)
    {
      STEPPER_MOVE.stop();
      return;
    }
    Serial2.println("LOG hole index: " + String(hole_index));
    Serial2.println("LOG Du sau: " + String(du_sau));
    Serial2.println("LOG Step_sau: " +String(Step_sau));
    uint16_t last_pos = STEPPER_MOVE.currentPosition();    
    Serial2.println("LOG current: " + String(STEPPER_MOVE.currentPosition()));
    STEPPER_MOVE.setSpeed(3000);
    while (STEPPER_MOVE.currentPosition() - last_pos < Step_sau)
    {
      STEPPER_MOVE.runSpeed();
      if (digitalRead(BUTTON_STOP_PIN) == 1)
      {        
        Serial2.println("LOG Stop press");
        STEPPER_MOVE.stop();
        Appl_ButtonStopPress_xdu = true;
        Serial2.println(6);
        Serial2.println(2); // nhấn stop
      }
    }
    Serial2.println("LOG After move: " + String(STEPPER_MOVE.currentPosition()));
    du_truoc = du_sau;
    if (Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    Serial2.println("LOG Ket thuc move");
  }
}
void return0index()
{
  digitalWrite(SOL_CLAMPER_PIN, HIGH);
  delay(300);
  digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
  delay(200);
  if (digitalRead(SS1_MOVE_HOME_A_PIN) == 0)
  {
    Serial2.println("LOG Dang khong o vi tri Home A, Ve Home A...");
    Home_Move_A();
    if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
    {
      return;
    }
    else
    {
      Serial2.println("LOG Dang o vi tri Home A. Doi len vi tri 0 index...");
      delay(200);
    }
  }
  Moveto_0Hole();
  Moveto_0Hole(); // move 0 hole 2 lần vì lỗ đầu bị sai
  if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
  {
    STEPPER_MOVE.stop();
    return;
  }
  STEPPER_MOVE.stop();
  Serial2.println("LOG Da o vi tri 0 index");
  digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
  delay(300);
  digitalWrite(SOL_CLAMPER_PIN, LOW);
  delay(200);
  hole_index = 0;
  du_truoc = 0;
}

void Moveto_0Hole()
{
  STEPPER_MOVE.setSpeed(3000);
  STEPPER_MOVE.runSpeed();
  while (digitalRead(SS7_HOLES_PIN) == 1)
  {
    STEPPER_MOVE.runSpeed();
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      STEPPER_MOVE.stop();
      Appl_ButtonStopPress_xdu = true;
      Serial2.println(6);
      return;
    }
  }
  while (digitalRead(SS7_HOLES_PIN) == 0)
  {
    STEPPER_MOVE.runSpeed();
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      STEPPER_MOVE.stop();
      Appl_ButtonStopPress_xdu = true;
      Serial2.println(6);
      return;
    }
  }
}
void Forward_1_Hole()
{
  int past_pos;
  int delta_move;
  int noise = 0;
  STEPPER_MOVE.runSpeed();
  past_pos = STEPPER_MOVE.currentPosition();
  delta_move = STEPPER_MOVE.currentPosition() - past_pos;
  //while (digitalRead(SS7_HOLES_PIN) == 1 and  delta_move<200) //sensor nam giua lo,chua bi che
  while (digitalRead(BUTTON_STOP_PIN) == 0)
  {
    STEPPER_MOVE.runSpeed();
    delta_move = STEPPER_MOVE.currentPosition() - past_pos;
    if (delta_move >= 400 and digitalRead(SS7_HOLES_PIN) == 1)
    {
      break;
    }
  }
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial2.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
    STEPPER_MOVE.stop();
    Serial2.println(6);
    return;
  }
  Serial2.println("LOG delta: " + String(delta_move));
  past_pos = STEPPER_MOVE.currentPosition();
  delta_move = STEPPER_MOVE.currentPosition() - past_pos;
}
void check_and_break()
{
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial2.println(6);
    Serial2.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
  }
}
void check_and_return()
{
  Serial2.println("LOG checking stop to return ");
  if (digitalRead(BUTTON_STOP_PIN) == 1)
  {
    Serial2.println("LOG stop pressed while moving ");
    Serial2.println(6);
    Serial2.println(2); // nhấn stop
    Appl_ButtonStopPress_xdu = true;
    return;
  }
}
void process_with_pause()
{
  Serial2.println(3);
  STEPPER_MOVE.stop();
  Serial2.println("LOG PAUSE button pressed. Wait to be relased. Doing nothing...");
  while (digitalRead(BUTTON_PAUSE_PIN) == 0) // 0 là mức press
  {
    // waiting . do nothing
  }
  Serial2.println("LOG Pause released. Wait START button pressed...");
  while (digitalRead(BUTTON_START_PIN) == 1)
  {
    if (digitalRead(BUTTON_PAUSE_PIN) == 0) //pause pressed
    {
      if (digitalRead(SOL_CLAMP_FEEDER_PIN) == 1)
      {
        Serial2.println("LOG PAUSE button pressed. Release FEEDER CLAMPER. Wait PAUSE released.Doing nothing...");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
        while (digitalRead(BUTTON_PAUSE_PIN) == 0)
        {
          // waiting . do nothing
        }
      }
      else
      {
        Serial2.println("LOG PAUSE button pressed. Active FEEDER CLAMPER. Wait PAUSE released. Doing nothing...");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
        while (digitalRead(BUTTON_PAUSE_PIN) == 0)
        {
          // waiting. doing nothing
        }
      }
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(6);
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      break;
    }
  }
  if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
  {
    return;
  }
  Serial2.println(4); // đã nhấn start
  if (digitalRead(SOL_CLAMP_FEEDER_PIN) == 0)
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    delay(500);
  }
  Serial2.println("LOG Pause pressed. Continue to run...");
}
void process_with_material()
{
  STEPPER_MOVE.stop();
  Serial2.println(11);
  Serial2.println("LOG Missing material.Turn on Buzzer. Wait material...");
  digitalWrite(MATERIAL_STATUS, HIGH);
  while (digitalRead(SS6_MATERIAL_PIN) == 0)
  {
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(6);
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      break;
    }
    if (digitalRead(BUTTON_PAUSE_PIN) == 0) //pause pressed
    {
      Serial2.println(3);
      digitalWrite(MATERIAL_STATUS, LOW);
      Serial2.println("LOG Turn off buzzer. Still waiting for material....");
      bool info8 = false;
      Serial2.println("LOG Wait for PAUSE release. Still waiting for material....");
      while (digitalRead(BUTTON_PAUSE_PIN) == 0)
      {
        // doing nothing
      }
    }
    bool start_press = false;
    while (digitalRead(BUTTON_START_PIN) == 0)
    {
      // phat hien nhan start, doi nha ra
      start_press = true;
    }
    if (start_press == true)
    {
      Serial2.println("LOG Start pressed while missing. send signal to pc.Turn on Buzzer");
      digitalWrite(MATERIAL_STATUS, HIGH);
      Serial2.println(11);
    }
  }
  if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
  {
    return;
  }
  if (digitalRead(MATERIAL_STATUS) == 1)
  {
    Serial2.println("LOG Turn off buzzer. Waiting press START.Meanwhile can clamp/release by pressing PAUSE button....");
    digitalWrite(MATERIAL_STATUS, LOW);
  }
  Serial2.println("LOG Waiting press START.Meanwhile can clamp/release by pressing PAUSE button....");
  digitalWrite(MATERIAL_STATUS, LOW);
  while (digitalRead(BUTTON_START_PIN) == 1)
  {
    if (digitalRead(BUTTON_PAUSE_PIN) == 0)
    {
      if (digitalRead(SOL_CLAMPER_PIN) == 0)
      {
        digitalWrite(SOL_CLAMPER_PIN, HIGH);
        delay(400);
      }
      if (digitalRead(SOL_CLAMP_FEEDER_PIN) == 1)
      {
        Serial2.println("LOG Release Feeder Clamp");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, LOW);
        delay(400);
      }
      else
      {
        Serial2.println("LOG Active Feeder Clamp");
        digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
        delay(400);
      }
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2);
      Appl_ButtonStopPress_xdu = true;
      Serial2.println(6);
      break;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      break;
    }
  }
  
//  if (digitalRead(MATERIAL_STATUS) == 0)
//  {
//    Serial2.println("LOG Still missing. send signal to pc");
//    Serial2.println(11);
//  }
  if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
  {
    return;
  }
  if (digitalRead(SOL_CLAMP_FEEDER_PIN) == 0)
  {
    digitalWrite(SOL_CLAMP_FEEDER_PIN, HIGH);
    delay(500);
  }
  if (digitalRead(SOL_CLAMPER_PIN) == 1)
  {
    digitalWrite(SOL_CLAMPER_PIN, LOW);
    delay(400);
  }
  
  Serial2.println("LOG Material availabled. Start pressed. Continue to run...");
  Serial2.println(88);
}
void Execute_Cut(String Data_Input)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int16_t Step;
  Data_Angle_Float_Raw = (Data_Input.toFloat());  
  if (Data_Angle_Float_Raw <= 45 and Data_Angle_Float_Raw >= 0)
  {
    Data_Angle_Float = 0;
  }
  else if (Data_Angle_Float_Raw > 45)
  {
    Data_Angle_Float = ((Data_Angle_Float_Raw - 45) / 2);
    if (Data_Angle_Float >= 37)
    {
      Data_Angle_Float = 37;
    }
  }
  else
  {
    Data_Angle_Float = Data_Angle_Float_Raw;
    
  }
  Serial2.println("LOG Angle xoay dao: " + String(Data_Angle_Float));
  Step = float(Data_Angle_Float * cutter_step_per_degree);
  if (Data_Angle_Float <0)
  {
      Step = -1;
  }
  Serial2.println("LOG step xoay dao: " + String(Step));
  if (Step >= 0 and Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    Brushless_Run(BRUSHLESS_SPEED);      // Brushless quay
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    digitalWrite(SOL_LIFTER_PIN, HIGH);  // Ha dao
    int count = 0;
    Angle_Cut(Step); // Xoay dao
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
      // delay(2000);
      while (digitalRead(BUTTON_STOP_PIN) == 0 and digitalRead(BUTTON_EMERGENCY_PIN) == 0
      and count <= 2000)
      {
        delay(1);
        count += 1;
      }
      if (digitalRead(BUTTON_STOP_PIN) == 1)
      {
        Serial2.println(2); // nhấn stop
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_ButtonStopPress_xdu = true;
      return;
      }
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
      {
        digitalWrite(SOL_LIFTER_PIN, LOW);
        Appl_EmergencyHold_xdu = true;
        return;
      }
    // delay(2000);
    Cutter_Forward(); // Tien Cut
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    // delay(5000);
    count = 0;
    while (digitalRead(BUTTON_STOP_PIN) == 0 and digitalRead(BUTTON_EMERGENCY_PIN) == 0
      and count <= 5000)
    {
        delay(1);
        count += 1;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
        Serial2.println(2); // nhấn stop
        digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
        Appl_ButtonStopPress_xdu = true;
        return;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      return;
    }
    // \delay(400);
    Angle_Cut(-2 * Step); // Xoay dao
    // delay(500);
    count = 0;
    while (digitalRead(BUTTON_STOP_PIN) == 0 and digitalRead(BUTTON_EMERGENCY_PIN) == 0
      and count <= 500)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_ButtonStopPress_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      digitalWrite(SOL_LIFTER_PIN, LOW);
      Appl_EmergencyHold_xdu = true;
      return;
    }
    // \delay(500);
    Cutter_Backward();                  // Lui dao
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    digitalWrite(SOL_CLAMPER_PIN, LOW); // Nha kep  phoi
    Brushless_Off();
    Angle_Cut(Step); // Xoay dao
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
  }
  else if (Step < 0 and Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    int count = 0;
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    Brushless_Run(BRUSHLESS_SPEED);      // Brushless quay
    digitalWrite(SOL_LIFTER_PIN, HIGH);  // Ha dao
    //delay(2000);
      count = 0;
      while (digitalRead(BUTTON_STOP_PIN) == 0 and digitalRead(BUTTON_EMERGENCY_PIN) == 0
      and count <= 2000)
      {
        delay(1);
        count += 1;
      }
      if (digitalRead(BUTTON_STOP_PIN) == 1)
      {
        Serial2.println(2); // nhấn stop
      digitalWrite(SOL_SLIDE_FORWARD_PIN, LOW);
      Appl_ButtonStopPress_xdu = true;
      return;
      }
      if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
      {
        digitalWrite(SOL_LIFTER_PIN, LOW);
        Appl_EmergencyHold_xdu = true;
        return;
      } 
    // \delay(2000);
    toi_cat_trong(); // toi Cut
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    lui_cat_trong();
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    Appl_CutterBackwardTrigger_xdu = false;
    Brushless_Off();                    // Brushless off
    digitalWrite(SOL_CLAMPER_PIN, LOW); //Nhả kẹp  phoi
    delay(400);
  }
}
void Execute_Cut_First_End(String Data_Input, bool type)
{
  static float Data_Angle_Float_Raw;
  static float Data_Angle_Float;
  static int Step;
  Data_Angle_Float_Raw = (Data_Input.toFloat());
  Serial2.println("LOG Data angle : " + String(Data_Angle_Float_Raw));
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
  Serial2.println("LOG Goc cat: " + String(Data_Angle_Float));
  Step = float(Data_Angle_Float * cutter_step_per_degree);
  Brushless_Run(BRUSHLESS_SPEED);     // Brushless quay
  digitalWrite(SOL_LIFTER_PIN, HIGH); // Ha dao
  if (/*Step>=0 and */ Appl_EmergencyHold_xdu == false and Appl_ButtonStopPress_xdu == false)
  {
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep phoi
    if (type == EXECUTE_FIRST)
    {
      Angle_Cut(-Step); // Xoay dao
    }
    else
    {
      Angle_Cut(Step); // Xoay dao
    }
    int count = 0;
    // delay 1.5s
    while (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and digitalRead(BUTTON_STOP_PIN) == 0 and count <= 1500)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      return;
    }  // end delay
    Cutter_Forward();
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
    }
    count = 0;
    // delay 0.5s    
    while (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and digitalRead(BUTTON_STOP_PIN) == 0 
      and count <= 5000)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      return;
    } // end delay
    Cutter_Backward(); // Forward Cut
    if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      return;
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
    digitalWrite(SOL_CLAMPER_PIN, HIGH); // Kep  phoi
    delay(400);
    count = 0;
    // delay 0.5s    
    while (digitalRead(BUTTON_EMERGENCY_PIN) == 0 and digitalRead(BUTTON_STOP_PIN) == 0 and count <= 500)
    {
      delay(1);
      count += 1;
    }
    if (digitalRead(BUTTON_EMERGENCY_PIN) == 1)
    {
      Appl_EmergencyHold_xdu = true;
      return;
    }
    if (digitalRead(BUTTON_STOP_PIN) == 1)
    {
      Serial2.println(2); // nhấn stop
      Appl_ButtonStopPress_xdu = true;
      return;
    } // end delay
  }
}

#endif
