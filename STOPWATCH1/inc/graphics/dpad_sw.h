#define dpad_sw_width 16
#define dpad_sw_height 16
static SI_SEGMENT_VARIABLE(dpad_sw_bits[], const uint8_t, SI_SEG_CODE) = {
  0x00, 0x02, 0x00, 0x07, 0x00, 0x0E, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};