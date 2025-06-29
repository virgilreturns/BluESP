/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

#include <SPI.h>

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

const int scale = 2;
const int offsetX = 80;
const int offsetY = 16;

const int disp_max_w = 480; // 480 x 320
const int disp_max_h = 320;

// must center 288 x 320 w x h

const int frame_w = 288;
const int frame_h = 320;



void setup(void) {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  
  tft.fillRect((disp_max_w-frame_w)/2, 0, 288, 320, TFT_RED); // or any color you like

}

void loop() {

}



