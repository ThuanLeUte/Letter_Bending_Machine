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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_H
#define __BSP_H

/* Includes ----------------------------------------------------------- */
#include "board_define.h"

/* Public defines ----------------------------------------------------- */
#define SS1_MOVE_HOME_A_PIN               (SS1)
#define SS2_MOVE_HOME_B_PIN               (SS2)

#define SS7_END_STROKE_FRONT_PIN          (SS3)
#define SS4_END_STROKE_BACK_PIN           (SS4)

#define SS3_CUT_HOME_PIN                  (SS5)
#define SS8_MATERIAL_BACK_PIN             (SS6)
#define SS6_HOLES_PIN                     (SS7)

#define BUTTON_START_PIN                  (BT1)
#define BUTTON_PAUSE_PIN                  (BT2)
#define BUTTON_STOP_PIN                   (BT3)

#define SOL_CLAMP_FEEDER_PIN              (OUT_2)
#define SOL_SLIDE_BACKWARD_PIN            (OUT_3)
#define SOL_LIFTER_PIN                    (OUT_4)
#define SOL_CLAMPER_PIN                   (OUT_5)

#define SOL_SLIDE_FORWARD_PIN             (OUT_5)
#define MATERIAL_STATUS                   (OUT_6)

#define STEPPER_MOVE_ENA_PIN              (OP1)
#define STEPPER_MOVE_DIR_PIN              (OP2)
#define STEPPER_MOVE_STEP_PIN             (OP3)

#define STEPPER_CUT_ENA_PIN               (OP5)
#define STEPPER_CUT_DIR_PIN               (OP6)
#define STEPPER_CUT_STEP_PIN              (OP7)

#define BRUSHLESS_SPEED_PIN               (OP8)

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  RES_EXCECUTE_SUCCESS = 1,
  RES_COMMAND_STOP     = 2,
  RES_COMMAND_PAUSE    = 3,
  RES_COMMAND_START    = 4,
  RES_COMMAND_HOMING   = 6,
  RES_READY_RECEIVE    = 7,
  RES_LETTER_FINISHED  = 8,
  RES_CONNECTED        = 10,
  RES_NO_MATERIAL      = 11,
  RES_START            = 13
}
data_respond_pc_t;

/* Public macros ------------------------------------------------------ */
#define IS_SENSOR_DETECTED(sensor)            (digitalRead(sensor) == 0)
#define IS_SENSOR_NOT_DETECTED(sensor)        (digitalRead(sensor) == 1)

#define IS_BUTTON_PRESSED(button)             (digitalRead(button) == 1)
#define IS_BUTTON_NOT_PRESSED(button)         (digitalRead(button) == 0)

#define DATA_SEND_TO_PC(data)                 (Serial.println(data))
#define SERIAL_DATA_MONITOR(data)             (Serial3.println(data))

#define GPIO_SET(pin, level)                  (digitalWrite(pin, level))
#define DELAY(ms)                             (delay(ms))

#define LOG(...)                               bsp_log_data(__VA_ARGS__)

/* Public variables --------------------------------------------------- */
extern String g_uart_data_receive;

/* Public function prototypes ----------------------------------------- */
/**
 * @brief         Board Support Package Init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_init(void);

/**
 * @brief         Stop button interrupt
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_stop_push(void);

/**
 * @brief         Pause button interrupt
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_pause_push(void);

/**
 * @brief         Start button interrupt
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_start_push(void);

/**
 * @brief         Receive date from uart
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_uart_receive(void);
/**
 * @brief         Log data
 * @param[in]     None
 * @attention     None
 * @return        None
 */
void bsp_log_data(const char *format, ...);

#endif // __BSP_H

/* End of file -------------------------------------------------------- */
