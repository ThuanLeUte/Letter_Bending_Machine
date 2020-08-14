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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BOARD_DEFINE_H
#define __BOARD_DEFINE_H

/* Includes ----------------------------------------------------------- */

/* Public defines ----------------------------------------------------- */
#define OUT_1                   (61)
#define OUT_2                   (60)
#define OUT_3                   (59)
#define OUT_4                   (58)
#define OUT_5                   (57)
#define OUT_6                   (56)
#define OUT_7                   (55)
#define OUT_8                   (54)

#define SS1                     (4)
#define SS2                     (26)
#define SS3                     (6)
#define SS4                     (7)
#define SS5                     (8)
#define SS6                     (9)
#define SS7                     (10)
#define SS8                     (11)

#define BT1                     (18)
#define BT2                     (19)
#define BT3                     (20)
#define BT4                     (21)
#define BT5                     (2)

#define OP1                     (34)
#define OP2                     (35)
#define OP3                     (36)
#define OP4                     (37)
#define OP5                     (40)
#define OP6                     (41)
#define OP7                     (38)
#define OP8                     (5)
#define OP9                     (31)
#define OP10                    (32)
#define BUZZER                  (30)

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function definition ----------------------------------------- */
void board_setup();

#endif // __BOARD_DEFINE_H

/* End of file -------------------------------------------------------- */
