#ifndef Board_Define_h
#define Board_Define_h

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

#define BT1                     (18)    // Interrupt 5
#define BT2                     (19)    // Interrupt 4
#define BT3                     (20)    // Interrupt 3
#define BT4                     (21)    // Interrupt 2
#define BT5                     (2)     // Interrupt 0

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

//#define UART2_TX 16
//#define UART2_RX 17
//
//#define UART0_TX 1
//#define UART0_RX 0

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



#endif
