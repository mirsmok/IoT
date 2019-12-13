const char activeSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00011000,
    B00100100,
    B01000010,
    B01000010,
    B00100100,
    B00011000
};

const char inactiveSymbol[] PROGMEM = {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00011000,
    B00011000,
    B00000000,
    B00000000
};
#define wifi_width 25
#define wifi_height 14
static char wifi_bits[] = {
  0x00, 0x38, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x00, 0xC0, 0x01, 0x07, 0x00, 
  0x60, 0x00, 0x0C, 0x00, 0x20, 0xFE, 0x08, 0x00, 0x80, 0xC7, 0x03, 0x00, 
  0x80, 0x01, 0x03, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 
  0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, };


