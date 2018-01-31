/** MAX7219 8x8 GRID CLASS by MikeDX
 *
 *
 *
 * Thrown together, probablly wrong
 * Who cares.
 *
 * MIT / CC-SHAREALIKE
 *
 */
#include <Arduino.h>
#include "max7219.h"
#include "vincent.h"
void max7219Class::scroll(String s) {
  scroll(s, 100);
}
void max7219Class::scroll(String s, int speed) {
  for (int a = 0; a < maxInUse; a++) {
    s = " " + s;
  }
  s += " ";

  for (int x =  0; x <= (s.length() - 1) * 8; x ++) {
    max7219.print(s, x);
    delay(speed);
  }

}
void max7219Class::begin(uint8_t d, uint8_t c, uint8_t l, uint8_t m) {
  dataIn = d;
  clock = c;
  load = l;
  maxInUse = m;
  pinMode(dataIn, OUTPUT);
  pinMode(clock,  OUTPUT);
  pinMode(load,   OUTPUT);
  maxAll(max7219_reg_scanLimit, 0x07);
  maxAll(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
  maxAll(max7219_reg_shutdown, 0x01);    // not in shutdown mode
  maxAll(max7219_reg_displayTest, 0x00); // no display test
  maxAll(max7219_reg_intensity, 0x01);

  // delay(2000);
  for (e = 1; e <= 8; e++) { // empty registers, turn all LEDs off
    maxAll(e, 0);
  }
  maxAll(max7219_reg_intensity, 0x01);    // the first 0x0f is the value you can set

  drawChar(1);
}

void max7219Class::putByte(byte data) {
  byte i = 8;
  byte mask;
  while (i > 0) {
    mask = 0x01 << (i - 1);      // get bitmask
    digitalWrite( clock, LOW);   // tick
    if (data & mask) {           // choose bit
      digitalWrite(dataIn, HIGH);// send 1
    } else {
      digitalWrite(dataIn, LOW); // send 0
    }
    digitalWrite(clock, HIGH);   // tock
    --i;                         // move to lesser bit
  }
}


void max7219Class::maxSingle( byte reg, byte col) {
  //maxSingle is the "easy"  function to use for a     //single max7219

  digitalWrite(load, LOW);       // begin
  putByte(reg);                  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  digitalWrite(load, LOW);       // and load da shit
  digitalWrite(load, HIGH);
}

void max7219Class::maxAll (byte reg, byte col) {    // initialize  all  MAX7219's in the system
  int c = 0;
  digitalWrite(load, LOW);  // begin
  for ( c = 1; c <= maxInUse; c++) {
    putByte(reg);  // specify register
    putByte(col);//((data & 0x01) * 256) + data >> 1); // put data
  }
  digitalWrite(load, LOW);
  digitalWrite(load, HIGH);
}

void max7219Class::maxOne(byte maxNr, byte reg, byte col) {
  //maxOne is for adressing different MAX7219's,
  //whilele having a couple of them cascaded

  int c = 0;
  digitalWrite(load, LOW);  // begin

  for ( c = maxInUse; c > maxNr; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  putByte(reg);  // specify register
  putByte(col);//((data & 0x01) * 256) + data >> 1); // put data

  for ( c = maxNr - 1; c >= 1; c--) {
    putByte(0);    // means no operation
    putByte(0);    // means no operation
  }

  digitalWrite(load, LOW); // and load da shit
  digitalWrite(load, HIGH);
}


void max7219Class::drawChar(byte c) {
  for (int b = 0; b < 8; b++) {
    maxAll (b + 1, _rotate(c, b));
  }
}
void max7219Class::drawChar(byte c, byte num) {
  for (int b = 0; b < 8; b++) {
    max7219.maxOne(num, b + 1, _rotate(c, b));
  }
}

byte max7219Class::_rotate(byte c, byte j) {
  char currentcharbit = 0;
  byte outputbyte = 0;//font_data[ic][j];
  byte b;
  int ic = c + font * 256;
  switch (rot) {

    case 0:
      // verbatim
      outputbyte = font_data[ic][j];
      break;

    case 1:
      for (int k = 7; k >= 0; k--) // Copy over data for 8 columns to current row and send it to current display
      {
        // This byte is the bitmap of the current character for the current row
        byte currentcharbits = font_data[ic][7 - k]; //g[k];//Font8x5[((curchar - 32) * 8) + j];

        if (currentcharbits & (1 << (7 - j)))
          outputbyte |= (1 << (7 - k));
      }
      break;

    case 2:
      b = font_data[ic][7 - j];
      b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
      b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
      b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
      outputbyte = b;
      break;

    case 3:
      for (int k = 7; k >= 0; k--) // Copy over data for 8 columns to current row and send it to current display
      {
        // This byte is the bitmap of the current character for the current row
        byte currentcharbits = font_data[ic][k]; //g[k];//Font8x5[((curchar - 32) * 8) + j];

        if (currentcharbits & (1 << j))
          outputbyte |= (1 << (7 - k));
      }    // advance the current character bit of current character
      break;

  }
  return outputbyte;
}

void max7219Class::print(String st, int x) {
  uint8_t cc;
  int pos = (int)x / 8;
  int p = x - (pos * 8);

  uint8_t len = maxInUse;
  while (len > 0) {
    char c = st[pos + len - 1];
    char c2 = st[pos + len];

    //if you use just one MAX7219 it should look like this
    switch (rot) {
      case 0: // CORRECT!
        for (int b = 0; b < 8; b++) {
          byte b1 = _rotate(c, b);
          byte b2 = _rotate(c2, b);

          cc = ((b1 << p & 0xFF) + (b2 >> (8 - p) & 0XFF));
          //      cc = (font_data[ic][b] << p & 0xFF) + (font_data[ic2][b] >> (8 - p) & 0xFF);
          maxOne(maxInUse - (len - 1), b + 1, cc);

        }
        break;
      case 1: // CORRECT!
        for (int b = 0; b < 8; b++) {
          if (b - p >= 0)
            cc = _rotate(c, ( 7 - (b - p)));
          else
            cc = _rotate(c2,  7 - (b - p + 8));

          maxOne(len, (7 - b) + 1, cc);
        }
        break;
      case 2: // CORRECT
        for (int b = 0; b < 8; b++) {
          byte b2 = _rotate(c, b);
          byte b1 = _rotate(c2, b);

          cc = ((b1 << (8 - p) & 0xFF) + (b2 >> p) & 0XFF);
          //      cc = (font_data[ic][b] << p & 0xFF) + (font_data[ic2][b] >> (8 - p) & 0xFF);
          maxOne(len, b + 1, cc);

        }
        break;


      case 3: // CORRECT!
        for (int b = 0; b < 8; b++) {

          if (b - p >= 0)
            cc = _rotate(c, ( (b - p)));
          else
            cc = _rotate(c2,  (b - p + 8));

          maxOne(maxInUse - (len - 1), b + 1, cc);

        }
        break;

    }
    len--;
  }
}


max7219Class max7219;

