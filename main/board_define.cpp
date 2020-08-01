/**
 * @file       board_define.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-08-01
 * @author     ThuanLe
 * @brief      Board define
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "Arduino.h"
#include "board_define.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */

void Board_Setup()
{
  pinMode(OUT_1, OUTPUT);
  pinMode(OUT_2, OUTPUT);
  pinMode(OUT_3, OUTPUT);
  pinMode(OUT_4, OUTPUT);
  pinMode(OUT_5, OUTPUT);
  pinMode(OUT_6, OUTPUT);
  pinMode(OUT_7, OUTPUT);
  pinMode(OUT_8, OUTPUT);

  pinMode(OP1, OUTPUT);
  pinMode(OP2, OUTPUT);
  pinMode(OP3, OUTPUT);
  pinMode(OP4, OUTPUT);
  pinMode(OP5, OUTPUT);
  pinMode(OP6, OUTPUT);
  pinMode(OP7, OUTPUT);
  pinMode(OP8, OUTPUT);
  pinMode(OP9, OUTPUT);
  pinMode(OP10, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(SS1, INPUT);
  pinMode(SS2, INPUT);
  pinMode(SS3, INPUT);
  pinMode(SS4, INPUT);
  pinMode(SS5, INPUT);
  pinMode(SS6, INPUT);
  pinMode(SS7, INPUT);
  pinMode(SS8, INPUT);

  pinMode(BT1, INPUT);
  pinMode(BT2, INPUT);
  pinMode(BT3, INPUT);
  pinMode(BT4, INPUT);
  pinMode(BT5, INPUT);
}
/* End of file -------------------------------------------------------- */

