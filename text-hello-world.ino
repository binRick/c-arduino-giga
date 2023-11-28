#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"

Arduino_H7_Video Display(800, 480, GigaDisplayShield);

void setup() {
  Display.begin();
  Display.clear();
  Display.beginDraw();
  Display.textFont(Font_5x7);
  Display.stroke(255, 255, 255);
  Display.text("Hello world!", 50, 50);
  Display.endDraw();
}

void loop() {}
