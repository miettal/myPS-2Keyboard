/*
 * File:   mykeyboard.c
 * Author: miettal
 *
 * Created on 2012/02/17, 17:51
 */
#include <htc.h>

__CONFIG(FOSC_HS&WDTE_OFF&LVP_OFF);

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

enum Data_byte { NONE, LED, SCAN_CODE };
enum Data_byte data_byte = NONE;

unsigned char queue[32];
unsigned char start = 0;
unsigned char end = 0;
unsigned char size = 0;

unsigned char keys[14] = { 0 };
unsigned char old_keys[14] = { 0 };

const unsigned char table0[][6] = {
  /* Key番号 makeキー 0x00  */
  /* Esc */
  { 1, 0x00, 0x01, 0x76, 0x00, 0x00 },
  /* E/J */
  { 1, 0x00, 0x01, 0x0e, 0x00, 0x00 },
  /* Del */
  /* { 1, 0x00, 0x0c, 0x */
  /* Back Space */
  { 1, 0x00, 0x0d, 0x66, 0x00, 0x00 },

  /* finish */
  { 0, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

const unsigned char table1[][6] = {
  /* 1-9 0*/
  { 1, 0x01, 0x01, 0x16, 0x00, 0x00 },
  { 1, 0x01, 0x02, 0x1e, 0x00, 0x00 },
  { 1, 0x01, 0x03, 0x26, 0x00, 0x00 },
  { 1, 0x01, 0x04, 0x25, 0x00, 0x00 },
  { 1, 0x01, 0x05, 0x2e, 0x00, 0x00 },
  { 1, 0x01, 0x06, 0x36, 0x00, 0x00 },
  { 1, 0x01, 0x07, 0x3d, 0x00, 0x00 },
  { 1, 0x01, 0x08, 0x3e, 0x00, 0x00 },
  { 1, 0x01, 0x09, 0x46, 0x00, 0x00 },
  { 1, 0x01, 0x0a, 0x45, 0x00, 0x00 },
  /* - */
  { 1, 0x01, 0x0b, 0x4e, 0x00, 0x00 },
  /* ^ */
  { 1, 0x01, 0x0c, 0x55, 0x00, 0x00 },
  /* ¥ */
  { 1, 0x01, 0x0d, 0x6a, 0x00, 0x00 },

  /* finish */
  { 0, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

const unsigned char table2[][6] = {
  /* Tab */
  { 1, 0x02, 0x00, 0x0d, 0x00, 0x00 },
  /* qwertyuiop */
  { 1, 0x02, 0x01, 0x15, 0x00, 0x00 },
  { 1, 0x02, 0x02, 0x1d, 0x00, 0x00 },
  { 1, 0x02, 0x03, 0x24, 0x00, 0x00 },
  { 1, 0x02, 0x04, 0x2d, 0x00, 0x00 },
  { 1, 0x02, 0x05, 0x2c, 0x00, 0x00 },
  { 1, 0x02, 0x06, 0x35, 0x00, 0x00 },
  { 1, 0x02, 0x07, 0x3c, 0x00, 0x00 },
  { 1, 0x02, 0x08, 0x43, 0x00, 0x00 },
  { 1, 0x02, 0x09, 0x44, 0x00, 0x00 },
  { 1, 0x02, 0x0a, 0x4d, 0x00, 0x00 },
  /* @ */
  { 1, 0x02, 0x0b, 0x54, 0x00, 0x00 },
  /* [ */
  { 1, 0x02, 0x0c, 0x5b, 0x00, 0x00 },
  /* Enter */
  { 1, 0x02, 0x0d, 0x5a, 0x00, 0x00 },

  /* finish */
  { 0, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

const unsigned char table3[][6] = {
  /* Caps Lock */
  { 1, 0x03, 0x00, 0x58, 0x00, 0x00 },
  /* asdfghjkl */
  { 1, 0x03, 0x01, 0x1c, 0x00, 0x00 },
  { 1, 0x03, 0x02, 0x1b, 0x00, 0x00 },
  { 1, 0x03, 0x03, 0x23, 0x00, 0x00 },
  { 1, 0x03, 0x04, 0x2b, 0x00, 0x00 },
  { 1, 0x03, 0x05, 0x34, 0x00, 0x00 },
  { 1, 0x03, 0x06, 0x33, 0x00, 0x00 },
  { 1, 0x03, 0x07, 0x3b, 0x00, 0x00 },
  { 1, 0x03, 0x08, 0x42, 0x00, 0x00 },
  { 1, 0x03, 0x09, 0x4b, 0x00, 0x00 },
  /* ; */
  { 1, 0x03, 0x0a, 0x4c, 0x00, 0x00 },
  /* : */
  { 1, 0x03, 0x0b, 0x52, 0x00, 0x00 },
  /* ] */
  { 1, 0x03, 0x0c, 0x5d, 0x00, 0x00 },

  /* finish */
  { 0, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

const unsigned char table4[][6] = {
  /* Shift */
  { 1, 0x04, 0x00, 0x12, 0x00, 0x00 },
  /* zxcvbnm */
  { 1, 0x04, 0x01, 0x1a, 0x00, 0x00 },
  { 1, 0x04, 0x02, 0x22, 0x00, 0x00 },
  { 1, 0x04, 0x03, 0x21, 0x00, 0x00 },
  { 1, 0x04, 0x04, 0x2a, 0x00, 0x00 },
  { 1, 0x04, 0x05, 0x32, 0x00, 0x00 },
  { 1, 0x04, 0x06, 0x31, 0x00, 0x00 },
  { 1, 0x04, 0x07, 0x3a, 0x00, 0x00 },
  /* , */
  { 1, 0x04, 0x08, 0x41, 0x00, 0x00 },
  /* . */
  { 1, 0x04, 0x09, 0x49, 0x00, 0x00 },
  /* / */
  { 1, 0x04, 0x0a, 0x4a, 0x00, 0x00 },
  /* _ */
  { 1, 0x04, 0x0b, 0x51, 0x00, 0x00 },
  /* Shift */
  { 1, 0x04, 0x0c, 0x59, 0x00, 0x00 },

  /* Ctrl */
  { 1, 0x00, 0x00, 0x14, 0x00, 0x00 },
  /* Alt */
  { 1, 0x00, 0x02, 0x11, 0x00, 0x00 },
  /* Space */
  { 1, 0x00, 0x06, 0x29, 0x00, 0x00 },

  /* finish */
  { 0, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

int enqueue(unsigned char d)
{
  while(size==32){
  }

  /* 割り込みと競合するので，割り込みOFF */
  /* --ここから-- */
  TMR0IE = 0;
  start--;
  start %= 32;

  queue[start] = d;

  size++;

  TMR0IE = 1;
  /* --ここまで-- */

  return 0;
}

int get_queue()
{
  if(size == 0){
    return -1;
  }

  return queue[(end-1)%32];
}

int dequeue()
{
  int d;

  if(size == 0){
    return -1;
  }

  end--;
  end %= 32;

  d = queue[end];

  size--;

  return d;
}

void set_key(int line, int col, int value)
{
  if (value == 1) {
    keys[col] |= 1<<line;
  } else {
    keys[col] &= ~(1<<line);
  }
}

int get_key(int line, int col)
{
  return (keys[col]>>line) & 1;
}

void set_old_key(int line, int col, int value)
{
  if (value == 1) {
    old_keys[col] |= 1<<line;
  } else {
    old_keys[col] &= ~(1<<line);
  }
}

int get_old_key(int line, int col)
{
  return (old_keys[col]>>line) & 1;
}

int clock(void)
{
  if(RA0 == 0){
    RA2 = 0;
    return -1;
  }

  RA1 = 1;
  __delay_us(40);
  RA1 = 0;
  __delay_us(40);

  return 0;
}

int recv_ps_2(void)
{
  int i;
  int data;

  /* 開始ビットを読む */
  if(RB5 != 0){ return -1; }
  if(-1 == clock()){ return -1; }

  data = 0;
  for(i=0;i<8;i++){
    /* データビットを読む */
    data += RB5<<i;
    if(-1 == clock()){ return -1; }
  }

  /* パリティビットを読む */
  if(-1 == clock()){ return -1; }

  /* 終了ビットを読む */
  if(RB5 != 1){ return -1; }
  /* 応答ビットを書き込む */
  RA2 = 1;
  if(-1 == clock()){ return -1; }
  RA2 = 0;

  return data;
}

int send_ps_2(unsigned char data5)
{
  int i;
  int parity;

  /* 開始ビットを書き込む */
  RA2 = 1;
  if(-1 == clock()){ return -1; }

  parity = 0;
  for(i=0;i<8;i++){
    /* データビットを書き込む */
    RA2 = !((data5>>i)&1);
    parity += (data5>>i)&1;
    if(-1 == clock()){ return -1; }
  }

  /* パリティビットを書き込む */
  RA2 = parity%2;
  if(-1 == clock()){ return -1; }

  /* 終了ビットを書き込む */
  RA2 = 0;
  if(-1 == clock()){ return -1; }

  return 0;
}

void interrupt func()
{
  if(TMR0IF==1){
    TMR0IE = 0;

    if(RA0 == 1){
      int data;
      if(RB5 == 1){
        data = get_queue();
        if(-1 != data){
          int result;
          result = send_ps_2(data);
          if(result != -1){
            dequeue();
          }
        }
      }else{
        data = recv_ps_2();
        if(data == 0xed){ /* LED */
          data_byte = LED;
          send_ps_2(0xfa);
        }else if(data == 0xee){
          send_ps_2(0xee);
        }else if(data == 0xf0){ /* スキャンコード選択 */
          data_byte = SCAN_CODE;
          send_ps_2(0xfa);
        }else if(data == 0xfa){ /* オートリピート有効化 */
          send_ps_2(0xfa);
        }else if(data == 0xff){ /* リセット */
          send_ps_2(0xaa);
        }else if(0x00 <= data && data <= 0x0f){
          if(data_byte == SCAN_CODE){
            if(data_byte == 0x00){
              send_ps_2(0x02);
              send_ps_2(0xfa);
            }else if(data_byte == 0x01){
              send_ps_2(0xfa);
            }else if(data_byte == 0x02){
              send_ps_2(0xfa);
            }else{
              send_ps_2(0xfa);
            }
          }
          data_byte = NONE;
        }else if(data == -1){ /* 受信失敗，再送要求 */
          send_ps_2(0xfe);
        }else{
          send_ps_2(0xfa);
          volatile int i;
          i = 30;
          i *= 5;
        }
      }
    }

    TMR0IE = 1;
    TMR0IF = 0;
    TMR0 = 156;
  }
}

void main(void)
{
  int i, j, k, l;


  // キーマトリクス
  // select pin
  // B1~B5
  // input pin
  // C4~C7
  // D4~D7
  // A3~A5
  // E0~E2
  //
  // ps/2
  // Ver1
  // A0 data 読み取り
  // B5 clock読み取り
  // A2 data 書き込み
  // A1 clock書き込み
  // Ver2
  // A0 clock読み取り
  // B5 data 読み取り
  // A2 data 書き込み
  // A1 clock書き込み
  TRISA=0b00111001;
  TRISB=0b00100000;
  TRISC=0b11110000;
  TRISD=0b11110000;
  TRISE=0b00000111;

  RA2 = 0;
  RA1 = 0;

  ADCON1 = 0x07;

  T0CS = 0;
  PSA = 0;
  PS2 = 1;
  PS1 = 0;
  PS0 = 0;
  TMR0 = 0;
  TMR0IF = 0;
  TMR0IE = 1;
  GIE = 1;

  while(1){
    for(i=0;i<5;i++){
      PORTB = 1<<i;
      set_key(i,  0, RA3);
      set_key(i,  1, RA4);
      set_key(i,  2, RA5);
      set_key(i,  3, RE0);
      set_key(i,  4, RE1);
      set_key(i,  5, RE2);
      set_key(i,  6, RC7);
      set_key(i,  7, RC6);
      set_key(i,  8, RC5);
      set_key(i,  9, RC4);
      set_key(i, 10, RD4);
      set_key(i, 11, RD5);
      set_key(i, 12, RD6);
      set_key(i, 13, RD7);
    }

      for(j=0;j<14;j++){
        for(k=0;table0[k][0];k++){
          if(table0[k][1] == 0 && table0[k][2] == j){
            if(get_key(0, j) != get_old_key(0, j)){
              if(get_key(0, j) == 0){
                enqueue(0xf0);
              }
              for(l=0;table0[k][3+l];l++){
                enqueue(table0[k][3+l]);
              }
            }
            break;
          }
        }
        set_old_key(0, j, get_key(0, j));
        for(k=0;table1[k][0];k++){
          if(table1[k][1] == 1 && table1[k][2] == j){
            if(get_key(1, j) != get_old_key(1, j)){
              if(get_key(1, j) == 0){
                enqueue(0xf0);
              }
              for(l=0;table1[k][3+l];l++){
                enqueue(table1[k][3+l]);
              }
            }
            break;
          }
        }
        set_old_key(1, j, get_key(1, j));
        for(k=0;table2[k][0];k++){
          if(table2[k][1] == 2 && table2[k][2] == j){
            if(get_key(2, j) != get_old_key(2, j)){
              if(get_key(2, j) == 0){
                enqueue(0xf0);
              }
              for(l=0;table2[k][3+l];l++){
                enqueue(table2[k][3+l]);
              }
            }
            break;
          }
        }
        set_old_key(2, j, get_key(2, j));
        for(k=0;table3[k][0];k++){
          if(table3[k][1] == 3 && table3[k][2] == j){
            if(get_key(3, j) != get_old_key(3, j)){
              if(get_key(3, j) == 0){
                enqueue(0xf0);
              }
              for(l=0;table3[k][3+l];l++){
                enqueue(table3[k][3+l]);
              }
            }
            break;
          }
        }
        set_old_key(3, j, get_key(3, j));
        for(k=0;table4[k][0];k++){
          if(table4[k][1] == 4 && table4[k][2] == j){
            if(get_key(4, j) != get_old_key(4, j)){
              if(get_key(4, j) == 0){
                enqueue(0xf0);
              }
              for(l=0;table4[k][3+l];l++){
                enqueue(table4[k][3+l]);
              }
            }
            break;
          }
        }
        set_old_key(4, j, get_key(4, j));
      }
  }
}
