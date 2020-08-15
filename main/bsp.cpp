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
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
static void m_timer_init(void);
static void m_interrupt_init(void);

/* Function definitions ----------------------------------------------- */
void bsp_init(void)
{
  // Init board
  board_define_init();

  // Init Serial communication
  Serial.begin(115200);
  Serial3.begin(115200);

  // Init timer 1
  m_timer_init();

  // Init interrupt
  m_interrupt_init();
}

/* Private functions definitions -------------------------------------- */
/**
 * @brief         Timer init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
static void m_timer_init(void)
{
  // initialize Timer1
  cli();      // disable global interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624;

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);

  // Set CS10 and CS12 bits for 1024 prescaler:
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12);

  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);

  // enable global interrupts:
  sei();
}

/**
 * @brief         Interrupts init
 * @param[in]     None
 * @attention     None
 * @return        None
 */
static void m_interrupt_init(void)
{
  attachInterrupt(2, bsp_pause_push, FALLING);    // Pin 21 Push go to LOW
  attachInterrupt(3, bsp_emergency_push, RISING); // Pin 20 Push go to HIGH
  attachInterrupt(4, bsp_stop_push, RISING);      // Pin 19 Push go to HIGH
  attachInterrupt(5, bsp_start_push, FALLING);    // Pin 18 Push go to LOW
}

/* End of file -------------------------------------------------------- */
