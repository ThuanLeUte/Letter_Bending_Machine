/**
 * @file       main.c
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-14
 * @author     Thuan Le
 * @brief      Main file
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp.h"
#include "main.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void setup()
{
  bsp_init();        // Board support package init
}

void loop()
{
  if (IS_SENSOR_DETECTED(SS1) || IS_SENSOR_DETECTED(SS2) ||
      IS_SENSOR_DETECTED(SS3) || IS_SENSOR_DETECTED(SS4) ||
      IS_SENSOR_DETECTED(SS5) || IS_SENSOR_DETECTED(SS6) ||
      IS_SENSOR_DETECTED(SS7) || IS_SENSOR_DETECTED(SS8) ||
      IS_BUTTON_PRESSED(BT1)  || IS_BUTTON_PRESSED(BT2)  ||
      IS_BUTTON_PRESSED(BT3)  || IS_BUTTON_PRESSED(BT4)  ||
      IS_BUTTON_PRESSED(BT5))
  {
    GPIO_SET(OUT_1, 1);
    GPIO_SET(OUT_2, 1);
    GPIO_SET(OUT_3, 1);
    GPIO_SET(OUT_4, 1);
    GPIO_SET(OUT_5, 1);
    GPIO_SET(OUT_6, 1);
    GPIO_SET(OUT_7, 1);
    GPIO_SET(OUT_8, 1);

    GPIO_SET(OP1, 1);
    GPIO_SET(OP2, 1);
    GPIO_SET(OP3, 1);
    GPIO_SET(OP4, 1);
    GPIO_SET(OP5, 1);
    GPIO_SET(OP6, 1);
    GPIO_SET(OP7, 1);
    GPIO_SET(OP8, 1);
    GPIO_SET(OP9, 1);
    GPIO_SET(OP10, 1);

    DATA_SEND_TO_PC("DATA_SEND_TO_PC");
    SERIAL_DATA_MONITOR("SERIAL_DATA_MONITOR");
  }
  else
  {
    GPIO_SET(OUT_1, 0);
    GPIO_SET(OUT_2, 0);
    GPIO_SET(OUT_3, 0);
    GPIO_SET(OUT_4, 0);
    GPIO_SET(OUT_5, 0);
    GPIO_SET(OUT_6, 0);
    GPIO_SET(OUT_7, 0);
    GPIO_SET(OUT_8, 0);

    GPIO_SET(OP1, 0);
    GPIO_SET(OP2, 0);
    GPIO_SET(OP3, 0);
    GPIO_SET(OP4, 0);
    GPIO_SET(OP5, 0);
    GPIO_SET(OP6, 0);
    GPIO_SET(OP7, 0);
    GPIO_SET(OP8, 0);
    GPIO_SET(OP9, 0);
    GPIO_SET(OP10, 0);
  }

  bsp_uart_receive();
}

/* End of file -------------------------------------------------------- */
