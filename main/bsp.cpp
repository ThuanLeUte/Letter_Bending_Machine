/**
 * @file       bsp.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Board Support Package (BSP)
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"
#include "bsp.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
String g_uart_data_receive = "";

/* Private variables -------------------------------------------------- */
static boolean m_uart_string_complete = false;

/* Private function prototypes ---------------------------------------- */
static void m_timer_init(void);
static void m_interrupt_init(void);

/* Function definitions ----------------------------------------------- */
void bsp_init(void)
{
  // Init board
  board_define_init();

  // Init Serial communication
  Serial.begin(115200);     // Monitor
  Serial3.begin(115200);    // PC communication
}

 void bsp_uart_receive()
{
  while (Serial.available()) // Receive data from computer
  {
    char data = (char)Serial.read();
    g_uart_data_receive += data;

    if (data == '\n')
    {
      m_uart_string_complete = true;
    }

    if (m_uart_string_complete)
    {
      DATA_SEND_TO_PC(g_uart_data_receive);
      m_uart_string_complete = false;
      g_uart_data_receive = "";
    }
  }
}

/* Private functions definitions -------------------------------------- */
/* End of file -------------------------------------------------------- */
