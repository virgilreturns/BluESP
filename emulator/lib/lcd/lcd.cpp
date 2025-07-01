#include "lcd.h"
#include <stdint.h>
#include "TFT_eSPI.h"

#define disp_max_w 480 // 480 x 320
#define disp_max_h 320
#define frame_w  288;


typedef uint32_t color_t;

void lcd_init(){
    TFT_eSPI tft = TFT_eSPI();
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    tft.fillRect((disp_max_w - 288)/2, 0, 288, 320, TFT_RED); // or any color you like
    
}
