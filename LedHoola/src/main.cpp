#include <Arduino.h>

#include "communication/infrared.h"
#include "lighting/ledstripe.h"

Infrared infrared;
Ledstripe ledstripe;

void setup() {

  // Serial.begin(9600);  // Initialize serial port for debugging.
  
  delay(750);  // Soft startup to ease the flow of electrons.
  
  infrared.begin();
  
  ledstripe.setBrightness(70); // 70 // 128   
  ledstripe.setMaxPowerInVoltsAndMilliamps(5, 1000);

  ledstripe.currentPalette = RainbowColors_p; // you can change this later
  ledstripe.currentBlending = LINEARBLEND;

  infrared.currentmode = 0;
  
} // setup()

void loop() {

  if (!infrared.receiving()) {
          // experiment with lightshows here
          if(infrared.currentmode == 1){ 
            // Button: Power
            ledstripe.fadeToBlack(NUM_LEDS, 30);
            int pos = random16(NUM_LEDS);
            ledstripe.leds[pos] = CHSV(135,80,200);
            ledstripe.show();
          }
          if(infrared.currentmode == 2){
            // Button: Plau/Pause
            ledstripe.rainbow_march(200, 10);
            ledstripe.show();
          }
          if(infrared.currentmode == 3){
            // Button: Vol +
            ledstripe.RainbowPalette(); // SetupRainbowStripedPalette();
            static uint8_t startIndex3 = 0;
            startIndex3 = startIndex3 + 1; // motion speed            
            ledstripe.FillLEDsFromPaletteColors( startIndex3);            
            ledstripe.show();
            ledstripe.delay(1000 / UPDATES_PER_SECOND);
          }
          if(infrared.currentmode == 4){
            // Button: Vol -
            ledstripe.ChangePalettePeriodically();        
            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed            
            ledstripe.FillLEDsFromPaletteColors(startIndex);            
            ledstripe.show();
            ledstripe.delay(1000 / UPDATES_PER_SECOND);
          }
          if(infrared.currentmode == 5){
            // Button: Next
            ledstripe.sawtooth();
            ledstripe.show();
          }
          if(infrared.currentmode == 6){
            // Button: Prev
            ledstripe.sawtoothBlue();
            ledstripe.show();
          }
          if(infrared.currentmode == 7){
            // Button: 3D
            ledstripe.rainbow_beat();
            ledstripe.show();
          }
  }
  
  infrared.irlcall();
  
} // loop()