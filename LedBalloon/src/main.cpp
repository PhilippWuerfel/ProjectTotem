#include <Arduino.h>

#include "communication/infrared.h"
#include "lighting/ledstripe.h"

Infrared infrared;
Ledstripe ledstripe;

void setup() {

  Serial.begin(9600);  // Initialize serial port for debugging.
  
  delay(750);  // Soft startup to ease the flow of electrons.
  
  infrared.begin();
  
  ledstripe.setBrightness(128);   
  ledstripe.setMaxPowerInVoltsAndMilliamps(5, 1700);

  ledstripe.currentPalette = RainbowColors_p; // you can change this later
  ledstripe.currentBlending = LINEARBLEND;

  infrared.currentmode = 0;
  
} // setup()

void loop() {

  if (!infrared.receiving()) {
          // experiment with lightshows here
          if(infrared.currentmode == 1){
            ledstripe.ChangePalettePeriodically();        
            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed            
            ledstripe.FillLEDsFromPaletteColors(startIndex);            
            ledstripe.show();
            ledstripe.delay(1000 / UPDATES_PER_SECOND);
          }
          if(infrared.currentmode == 2){
            ledstripe.rainbow_march(200, 10);
            ledstripe.show();
          }
          if(infrared.currentmode == 3){ 
            ledstripe.RainbowPalette();
            static uint8_t startIndex3 = 0;
            startIndex3 = startIndex3 + 1; // motion speed            
            ledstripe.FillLEDsFromPaletteColors( startIndex3);            
            ledstripe.show();
            ledstripe.delay(1000 / UPDATES_PER_SECOND);
          }
          if(infrared.currentmode == 4){ 
            ledstripe.fadeToBlack(NUM_LEDS, 30);
            int pos = random16(NUM_LEDS);
            ledstripe.leds[pos] = CHSV(135,80,200);
            ledstripe.show();
          }
          if(infrared.currentmode == 5){
            ledstripe.sawtooth();
            ledstripe.show();
          }
          if(infrared.currentmode == 6){
            ledstripe.blendwave();
            ledstripe.show();
          }
          if(infrared.currentmode == 7){
            ledstripe.rainbow_beat();
            ledstripe.show();
          }
          if(infrared.currentmode == 8){
              ledstripe.inoise8_fire();
              ledstripe.show();
          }
          if(infrared.currentmode == 9){
              ledstripe.sawtoothBlue();
              ledstripe.show();
          }
          if(infrared.currentmode == 10){
            ledstripe.inoise8_icefire();
            ledstripe.show();
          }
          if(infrared.currentmode == 11){
            ledstripe.fill(NUM_LEDS, CRGB::Blue);
            ledstripe.show();
          }
          if(infrared.currentmode == 12){
            ledstripe.fill(NUM_LEDS, CRGB::Purple);
            ledstripe.show();
          }
          if(infrared.currentmode == 13){
            ledstripe.fill(NUM_LEDS, CRGB::Green);
            ledstripe.show();
          }
          if(infrared.currentmode == 14){
            ledstripe.fill(NUM_LEDS, CRGB::Orange);
            ledstripe.show();
          }
          if(infrared.currentmode == 15){
            ledstripe.fill(NUM_LEDS, CRGB::White);
            ledstripe.show();
          }
          if(infrared.currentmode == 21){
            ledstripe.fill(NUM_LEDS, CRGB::Black);
            ledstripe.show();
          }
  }
  
  infrared.irlcall();
  
} // loop()