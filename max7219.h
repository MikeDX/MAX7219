class max7219Class
{
  private:
    byte max7219_reg_noop        = 0x00;
    byte max7219_reg_digit0      = 0x01;
    byte max7219_reg_digit1      = 0x02;
    byte max7219_reg_digit2      = 0x03;
    byte max7219_reg_digit3      = 0x04;
    byte max7219_reg_digit4      = 0x05;
    byte max7219_reg_digit5      = 0x06;
    byte max7219_reg_digit6      = 0x07;
    byte max7219_reg_digit7      = 0x08;
    byte max7219_reg_decodeMode  = 0x09;
    byte max7219_reg_intensity   = 0x0a;
    byte max7219_reg_scanLimit   = 0x0b;
    byte max7219_reg_shutdown    = 0x0c;
    byte max7219_reg_displayTest = 0x0f;
    uint8_t clock;
    uint8_t dataIn;
    uint8_t load;
    int e;
    byte _rotate(byte c, byte j);
  protected:
    byte circle;
  public:

    uint8_t maxInUse;
    uint8_t font = 0;
    uint8_t rot = 0;
    void putByte(byte);
    void begin(uint8_t dataIn, uint8_t clock, uint8_t load, uint8_t maxInUse);
    void maxOne(byte, byte, byte);
    void maxAll(byte, byte);
    void maxSingle(byte, byte);
    void drawChar(byte);
    void drawChar(byte, byte);

    void print(String , int);
    void scroll(String);

    void scroll(String, int);
  private:
};

extern max7219Class max7219;


