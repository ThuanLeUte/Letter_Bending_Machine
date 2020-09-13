#ifndef UART_Communication_h
#define UART_Communication_h

String data_software = "";
boolean stringComplete = false;

void Recive_Data()
{
  while (Serial2.available()) // gửi data từ C# qua
  {
    char data = (char)Serial2.read();
    data_software += data;
    if (data == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete)
    {
      stringComplete = false;
      Serial.println("RUNNING");
      Execute_String(data_software);
      if (Appl_SystemState_xdu8 == FINISH_LETTER_STATE)
      {
        // Do nothing
      }
      else
      {
        Serial2.println(1);
      }
      data_software = "";
    }
  }
}

#endif
