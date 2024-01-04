#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10
#define PIN 7
#define NUMPIXELS 7 
#define BRAKE_ACCEL 5
#define DEBUG_SERIAL true
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, PIN, NEO_GRB + NEO_KHZ800);
long lastSerialLog = 0;
long lastAccelTime = 0;
int qtyBrakeEvents = 0;


void setup() {
  if(DEBUG_SERIAL){

    Serial.begin(115200);
    while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
    Serial.println("LIS3DH test!");
  }
  if (! lis.begin(0x18)) {
      if(DEBUG_SERIAL){ 
        Serial.println("Couldnt start");
      }
      while (1) yield();
  }
  if(DEBUG_SERIAL){
    Serial.println("LIS3DH found!");

    Serial.print("Range = "); Serial.print(2 << lis.getRange());
    Serial.println("G");
    Serial.print("Data rate set to: ");
    switch (lis.getDataRate()) {
      case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
      case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
      case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
      case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
      case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
      case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
      case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;
      case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
      case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
      case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("16 Khz Low Power"); break;
    }
  }
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  lis.read();     
  if(DEBUG_SERIAL){
    //Serial.print("X:  "); Serial.print(lis.x);
   // Serial.print("  Y:  "); Serial.print(lis.y);
   // Serial.print("  Z:  "); Serial.print(lis.z);
  }
  /* Or....get a new sensor event, normalized */
  sensors_event_t event;
  lis.getEvent(&event);
  if(DEBUG_SERIAL){
    if(millis()-lastSerialLog > 100){

      Serial.print(" | X: "); Serial.print(event.acceleration.x);
      Serial.print(" Y: "); Serial.print(event.acceleration.y);
      Serial.print(" Z: "); Serial.print(event.acceleration.z);
      Serial.print(" m/s^2 ");

      Serial.println();
      lastSerialLog = millis();

    }
  }

  bool doBrakeLight = false;
  if(event.acceleration.y > BRAKE_ACCEL){
    Serial.print("brake!");
    Serial.print("qty: "); Serial.print(qtyBrakeEvents);
    Serial.println();
    qtyBrakeEvents++;
  }else{
    qtyBrakeEvents = 0;
  }
  if(qtyBrakeEvents > 5){
    doBrakeLight = true;

  }

  if(doBrakeLight){
    for(int i=0; i<NUMPIXELS; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show(); 
    }
    delay(1000);
  }else{
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show(); 
    }
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

