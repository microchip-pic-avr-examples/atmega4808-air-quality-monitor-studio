#ifndef OLED_H
#define	OLED_H


#include <stdint.h>


#define OLED_WIDTH             (96)
#define OLED_HEIGHT            (39)

#define SSD1306_DISPLAYOFF          (0xAE)
#define SSD1306_SETDISPLAYCLOCKDIV  (0xD5)
#define SSD1306_SETMULTIPLEX        (0xA8)
#define SSD1306_SETDISPLAYOFFSET    (0xD3)
#define SSD1306_SETSTARTLINE        (0x40)
#define SSD1306_CHARGEPUMP          (0x8D)

#define SSD1306_SETSEGMENTREMAP     (0xA1)
#define SSD1306_SEGREMAP            (0xA0)

#define SSD1306_COMSCANDEC          (0xC8)
#define SSD1306_SETCOMPINS          (0xDA)
#define SSD1306_SETCONTRAST         (0x81)
#define SSD1306_SETPRECHARGE        (0xD9)
#define SSD1306_SETVCOMDETECT       (0xDB)
#define SSD1306_DISPLAYALLON_RESUME (0xA4)
#define SSD1306_NORMALDISPLAY       (0xA6)
#define SSD1306_DISPLAYON           (0xAF)

#define SSD1306_DISPLAYALLON        (0xA5)
#define SSD1306_INVERTDISPLAY       (0xA7)
#define SSD1306_SETLOWCOLUMN        (0x00)
#define SSD1306_SETHIGHCOLUMN       (0x10)
#define SSD1306_MEMORYMODE          (0x20)
#define SSD1306_COLUMNADDR          (0x21)
#define SSD1306_PAGEADDR            (0x22)
#define SSD1306_COMSCANINC          (0xC0)
#define SSD1306_SEGREMAP            (0xA0)
#define SSD1306_EXTERNALVCC         (0x1)
#define SSD1306_SWITCHCAPVCC        (0x2)

#define SSD1306_ACTIVATE_SCROLL                         (0x2F)
#define SSD1306_DEACTIVATE_SCROLL                       (0x2E)
#define SSD1306_SET_VERTICAL_SCROLL_AREA                (0xA3)
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 (0x26)
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  (0x27)
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    (0x29)
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     (0x2A)

#define OLED_TIMEOUT    (8)

extern uint8_t OLED_Timer;

void OLED_Command(uint8_t);

void OLED_Data(uint8_t);

void OLED_Init(void);

// Set row
void OLED_SetRow(uint8_t);

// Set column
void OLED_SetColumn(uint8_t);

// Set scaling factors
void OLED_SetScale(uint8_t, uint8_t);

// Display picture form array in memory
void OLED_PutPicture(const uint8_t *);

// control contrast in steps  from 1 to 256
void OLED_SetContrast(uint8_t);

void OLED_PutChar(char ch);

void OLED_PutString(char x, char y, char *s);

void OLED_Clear(void);

void OLED_PutStringByColumn(char x, char y, char *s);

#endif	/* OLED_H */