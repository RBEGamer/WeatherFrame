//DEMO RAM
#ifndef DEMO_RAM
#define DEMO_RAM
#define DEMO_RAM_SIZE 2048
#define DEMO_RAM_INIT_CHAR 0
#define DEMO_RAM_ERROR_CHAR 0
unsigned char demo_ram[DEMO_RAM_SIZE];
inline void write_to_ram(unsigned int _addr, unsigned char _value) {
  if (_addr > DEMO_RAM_SIZE - 1) {
#ifdef _SER_DEBUG_
    Serial.println(F("DEMO RAM - OUT OF RANGE"));
#endif
  }
  else {
    demo_ram[_addr] = _value;
  }
  }
inline unsigned char read_from_ram(unsigned int _addr) {
  if (_addr > DEMO_RAM_SIZE - 1) {
#ifdef _SER_DEBUG_
    Serial.println(F("DEMO RAM - OUT OF RANGE"));
#endif
    return DEMO_RAM_ERROR_CHAR;
  }
  return demo_ram[_addr];
}
void init_ram() {
  for (int i = 0; i < DEMO_RAM_SIZE; i++) {
    demo_ram[i] = DEMO_RAM_INIT_CHAR;
  }
}
#endif // !DEMO_RAM
