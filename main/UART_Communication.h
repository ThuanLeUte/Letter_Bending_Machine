#ifndef UART_Communication_h
#define UART_Communication_h

boolean stringComplete = false;
char data;
void Recive_Data()
{
  while (Serial2.available()) // gửi data từ C# qua
  {
    data =  (char)Serial2.read();
    delay(10);
    data_software += data;     
    last_minute = millis(); //khởi động tính thời gian timeout connection
    if (info2 == false or data_software !="" )
    {
      Serial2.println("LOG Receiving data...");       
      info2 = true;
    }
    if (data == 'n')
    {
      last_index = true; 
      auto_receiving = false;
    }
    else 
    {
      Serial2.println("N");    
    }
  }  
  
  data_software.trim();
  if ( millis()-last_minute >=1800 and auto_receiving == true) //sau khi cho 1.8s ma ko nhan dc data moi thi xoa data cu
  {
    data_software ="";
    Serial2.println("N");
    if (info1 == false)
    {
      data_software !="";    
      Serial2.println("LOG Timeout connection. Reset data va chuan bi nhan data moi");
      info1=true;
      info2 = false; 
      info_receive = false;
    }    
  }
  else
  {
    info1=false;
  }
  if (data_software == "AUT") // nhận và gửi phản hồi về data
  {
    if (info == false)
    {
      Serial2.println("LOG Receiving AUTO data...");
      info = true;   
      info2 = false;   
      Serial2.println("N"); // gửi N để next value trong chuỗi auto   
      info_receive = false;
      last_minute = millis(); //khởi động tính thời gian timeout connection 
    }
    auto_receiving = true;   
   }
  if (last_index == true)
    {
      Serial2.println("LOG Data full received. Checking data...");      
      excute_string = data_software;
      delay(200);
      Serial2.println(data_software);
      Serial2.println(data_software);
      data_software ="";
      last_index = false;
      info2 = false;       
      info_receive = false;
    }    
  if (data_software == "BEGIN")
  {
    delay(100);
    Serial2.println("LOG Nhan duoc tin hieu BEGIN. Gui NEXT len PC");
    delay(200);
    Serial2.println("NEXT");
    data_software = "";
    delay(300);
    Serial2.println("N");
    info_receive= false;
    return;
  }
  if (data_software == "DATA NG")
  {
    auto_data_received = false;
    Serial2.println("Retry");
    Serial2.println("LOG Data NG. Retry... "); 
    Serial2.println(data_software); 
    data_software = "";   
    last_index = false;
    info = false;
    delay(300);
    Serial2.println("N");    
    last_minute = millis(); //khởi động tính thời gian timeout connection
    info2 = false; 
    auto_receiving = true;
    info_receive = false;
  }
  else if (data_software == "DATA OK")
  {    
    auto_data_received = true;
    data_index = 0;
    data_software = "";
    Serial2.println("LOG Data is ok. Pass to excute string: " + excute_string); 
    excute_string = excute_string.substring(0,excute_string.length()-1);    
    info = false;
    last_index = false;
    info2 = false; 
    info_receive = false;
    data_index = 0;
    //return;
  }
  if (auto_receiving == false and data_software != "" )
  {
    String temp = data_software;
    if (Execute_Manual(temp) == 1)
    {
    Serial2.println("LOG Done Excute Manual: " + data_software);   
    data_software ="";
    auto_data_received = false;
    data_index =0;
    info = false;
    last_index = false;
    info2 = false;
    info_receive = false;
    
    //return;
    }
    else if (Appl_EmergencyHold_xdu == true or Appl_ButtonStopPress_xdu == true)
    {
      
      info_receive = false;
      //return;
    }
  }
  return;
}

void cut_and_excute()
{
  info_receive = true;
  int last_index;
  String data_trim = "";
  while (data_index <= excute_string.length())
  {
    Serial2.println("LOG Bat dau vong lap");
    while (excute_string.substring(data_index,data_index + 1) != "S" && excute_string.substring(data_index,data_index + 1) != "F" && 
      excute_string.substring(data_index,data_index + 1) != "C" && excute_string.substring(data_index,data_index + 1) != "E" &&
     excute_string.substring(data_index,data_index + 1) != "Z"  && excute_string.substring(data_index,data_index + 1) != "W" 
      && data_index <= excute_string.length())
      {
        data_index += 1;
      }
      last_index = data_index + 1;
    //cắt xong command
    while (excute_string.substring(last_index,last_index + 1) != "_" && last_index <= excute_string.length())
    {
      last_index += 1;
    }
    data_trim = excute_string.substring(data_index,last_index) + "_";      
    data_index = last_index;
    Serial2.println("LOG Data trim : " + data_trim);
    if (last_index != data_index+1)
    {
      Execute_String(data_trim);
    }
    if (Appl_ButtonStopPress_xdu == true or Appl_EmergencyHold_xdu == true)
    {
      break;
    }
    Serial2.println("LOG Send 1 len PC");
    Serial2.println(1);
  }
  auto_data_received = false;
  excute_string ="";
  data_index = 0;
  data_software = "";
  if (Appl_ButtonStopPress_xdu ==true or Appl_EmergencyHold_xdu == true)
  {
    return;
  }
  Serial2.println("LOG Thuc hien xong chu");
}
#endif
