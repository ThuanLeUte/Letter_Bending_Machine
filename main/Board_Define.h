#ifndef Board_Define_h
#define Board_Define_h

#define OUT_1 54
#define OUT_2 55
#define OUT_3 56
#define OUT_4 57
#define OUT_5 58
#define OUT_6 59
#define OUT_7 60
#define OUT_8 61

#define OUT_9 69
#define OUT_10 68
#define OUT_11 67
#define OUT_12 66
#define OUT_13 65
#define OUT_14 64
#define OUT_15 63
#define OUT_16 62

#define SS1 4
#define SS2 26
#define SS3 6
#define SS4 7
#define SS5 8
#define SS6 9
#define SS7 10
#define SS8 11

#define BT1 18
#define BT2 19
#define BT3 20
#define BT4 21
#define BT5 13

#define IP14 2
#define IP15 3
#define IP16 12
#define IP17 25
#define IP18 23
#define IP19 24

#define IO1 49
#define IO2 48
#define IO3 47
#define IO4 46
#define IO5 42
#define IO6 43
#define IO7 44
#define IO8 45

#define OP1 38
#define OP2 41
#define OP3 40
#define OP4 37
#define OP5 36
#define OP6 35
#define OP7 34
#define OP8 33
#define OP9 32
#define OP10 31
#define OP11 30
#define OP12 39
#define OP13 29
#define OP14 28
#define OP15 27
#define OP16 5

#define UART2_TX 16
#define UART2_RX 17

#define UART0_TX 1
#define UART0_RX 0

void Board_Setup()
{
  pinMode(UART2_TX, OUTPUT);
  pinMode(UART2_RX, OUTPUT);
  pinMode(UART0_TX, OUTPUT);
  pinMode(UART0_RX, OUTPUT);

  digitalWrite(UART2_TX, HIGH);
  digitalWrite(UART2_RX, HIGH);
  digitalWrite(UART0_TX, HIGH);
  digitalWrite(UART0_RX, HIGH);

  pinMode(OUT_1, OUTPUT);
  pinMode(OUT_2, OUTPUT);
  pinMode(OUT_3, OUTPUT);
  pinMode(OUT_4, OUTPUT);
  pinMode(OUT_5, OUTPUT);
  pinMode(OUT_6, OUTPUT);
  pinMode(OUT_7, OUTPUT);
  pinMode(OUT_8, OUTPUT);
  pinMode(OUT_9, OUTPUT);
  pinMode(OUT_10, OUTPUT);
  pinMode(OUT_11, OUTPUT);
  pinMode(OUT_12, OUTPUT);
  pinMode(OUT_13, OUTPUT);
  pinMode(OUT_14, OUTPUT);
  pinMode(OUT_15, OUTPUT);
  pinMode(OUT_16, OUTPUT);

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
  pinMode(OP11, OUTPUT);
  pinMode(OP12, OUTPUT);
  pinMode(OP13, OUTPUT);
  pinMode(OP14, OUTPUT);
  pinMode(OP15, OUTPUT);
  pinMode(OP16, OUTPUT);

  pinMode(IO1, OUTPUT);
  pinMode(IO2, OUTPUT);
  pinMode(IO3, OUTPUT);
  pinMode(IO4, OUTPUT);
  pinMode(IO5, OUTPUT);
  pinMode(IO6, OUTPUT);
  pinMode(IO7, OUTPUT);
  pinMode(IO8, OUTPUT);

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

  pinMode(IP14, INPUT);
  pinMode(IP15, INPUT_PULLUP);
  pinMode(IP16, INPUT);
  pinMode(IP17, INPUT);
  pinMode(IP18, INPUT);
  pinMode(IP19, INPUT);
}

void Out_Put_Off()
{
  digitalWrite(OUT_1, LOW);
  digitalWrite(OUT_2, LOW);
  digitalWrite(OUT_3, LOW);
  digitalWrite(OUT_4, LOW);
  digitalWrite(OUT_5, LOW);
  digitalWrite(OUT_6, LOW);
  digitalWrite(OUT_7, LOW);
  digitalWrite(OUT_8, LOW);
  digitalWrite(OUT_9, LOW);
  digitalWrite(OUT_10, LOW);
  digitalWrite(OUT_11, LOW);
  digitalWrite(OUT_12, LOW);
  digitalWrite(OUT_13, LOW);
  digitalWrite(OUT_14, LOW);
  digitalWrite(OUT_15, LOW);
  digitalWrite(OUT_16, LOW);

  digitalWrite(OP1, LOW);
  digitalWrite(OP2, LOW);
  digitalWrite(OP3, LOW);
  digitalWrite(OP4, LOW);
  digitalWrite(OP5, LOW);
  digitalWrite(OP6, LOW);
  digitalWrite(OP7, LOW);
  digitalWrite(OP8, LOW);
  digitalWrite(OP9, LOW);
  digitalWrite(OP10, LOW);
  digitalWrite(OP11, LOW);
  digitalWrite(OP12, LOW);
  digitalWrite(OP13, LOW);
  digitalWrite(OP14, LOW);
  digitalWrite(OP15, LOW);
  digitalWrite(OP16, LOW);
}

#endif
