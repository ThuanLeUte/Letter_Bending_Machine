/**
 * @file       uart_communication.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Uart communication
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "uart_communication.h"
#include "execute_data.h"
#include "damos_ram.h"
#include "main.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
String data_software = "";

/* Private variables -------------------------------------------------- */
static boolean string_complete = false;

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void receive_data()
{
  while (Serial.available()) // gửi data từ C# qua
  {
    char data = (char)Serial.read();
    data_software += data;

    if (data == '\n')
    {
      string_complete = true;
    }

    if (string_complete)
    {
      string_complete = false;
      Serial3.println("RUNNING");
      Execute_String(data_software);
      if (Appl_SystemState_xdu8 == FINISH_LETTER_STATE)
      {
        // Do nothing
      }
      else
      {
        Serial.println(1);
      }

      data_software = "";
    }
  }
}

/* End of file -------------------------------------------------------- */
