#include <Arduino.h>

extern const uint8_t font_data[768][8];

class max7219Class
{
  private:
    uint8_t max7219_reg_noop        = 0x00;
    uint8_t max7219_reg_digit0      = 0x01;
    uint8_t max7219_reg_digit1      = 0x02;
    uint8_t max7219_reg_digit2      = 0x03;
    uint8_t max7219_reg_digit3      = 0x04;
    uint8_t max7219_reg_digit4      = 0x05;
    uint8_t max7219_reg_digit5      = 0x06;
    uint8_t max7219_reg_digit6      = 0x07;
    uint8_t max7219_reg_digit7      = 0x08;
    uint8_t max7219_reg_decodeMode  = 0x09;
    uint8_t max7219_reg_intensity   = 0x0a;
    uint8_t max7219_reg_scanLimit   = 0x0b;
    uint8_t max7219_reg_shutdown    = 0x0c;
    uint8_t max7219_reg_displayTest = 0x0f;
    uint8_t _clock;
    uint8_t _dataIn;
    uint8_t _load;
//    int e;
    uint8_t _rotate(uint8_t c, uint8_t j);
  protected:
//    uint8_t _circle;

  public:
    uint8_t maxInUse;
    uint8_t font = 0;
    uint8_t rot = 0;
    max7219Class(uint8_t data_pin, uint8_t clock_pin, uint8_t load_pin, int maxinuse);
    void begin(uint8_t dataIn, uint8_t clock, uint8_t load, uint8_t maxInUse);
    void maxOne(uint8_t, uint8_t, uint8_t);
    void maxAll(uint8_t, uint8_t);
    void maxSingle(uint8_t, uint8_t);
    void drawChar(uint8_t);
    void drawChar(uint8_t, uint8_t);

    void print(String , int);
    void scroll(String s);

    void scroll(String s, int speed);

  private:
    void _putByte(uint8_t data);

};

//extern max7219Class max7219;


