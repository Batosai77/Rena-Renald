#ifndef ICONS_H
#define ICONS_H
#include <stdint.h>
#include <pgmspace.h>

extern const unsigned char lion_of_informatics[] PROGMEM;
//*************************Biometric Icons*********************************
#define Wifi_start_width 54
#define Wifi_start_height 49
extern const uint8_t Wifi_start_bits[] PROGMEM;

#define Wifi_connected_width 63
#define Wifi_connected_height 49
extern const uint8_t Wifi_connected_bits[] PROGMEM;

//*************************Biometric Icons*********************************
#define FinPr_start_width 64
#define FinPr_start_height 64
extern const uint8_t FinPr_start_bits[] PROGMEM;
//---------------------------------------------------------------
#define FinPr_valid_width 64
#define FinPr_valid_height 64
extern const uint8_t FinPr_valid_bits[] PROGMEM;

//---------------------------------------------------------------
#define FinPr_invalid_width 64
#define FinPr_invalid_height 64
extern const uint8_t FinPr_invalid_bits[] PROGMEM ;
//---------------------------------------------------------------
#define FinPr_failed_width 64
#define FinPr_failed_height 64
extern const uint8_t FinPr_failed_bits[] PROGMEM;
//---------------------------------------------------------------
#define FinPr_scan_width 64
#define FinPr_scan_height 64
extern const uint8_t FinPr_scan_bits[] PROGMEM;

#endif