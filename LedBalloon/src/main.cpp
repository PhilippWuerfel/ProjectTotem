#include <Arduino.h>

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED     Use 3.2
#include "IRLremote.h"                                        // https://github.com/NicoHood/IRLremote  Use 2.0.2

#include "commands.h"                                         // The IR commands.

#if IRL_VERSION < 202
#error "Requires IRLRemote 2.0.2 or later; check github for latest code."
#endif

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// choose a valid PinInterrupt pin of your Arduino board
#define pinIR 2                                               // I'm using pin D2
#define IRL_BLOCKING true

CNec IRLremote;

// Fixed definitions cannot change on the fly.
#define LED_DT 6                                             // Data pin to connect to the strip.
//#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 30                                           // Number of LED's.

struct CRGB leds[NUM_LEDS];

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS];

CRGB clr1;
CRGB clr2;
uint8_t speed;
uint8_t loc1;
uint8_t loc2;
uint8_t ran1;
uint8_t ran2;

//for fire
uint32_t xscale = 20;                                          // How far apart they are
uint32_t yscale = 3;                                           // How fast they move
uint8_t index = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

#define UPDATES_PER_SECOND 100

// Global variables can be changed on the fly.
uint8_t max_bright = 128;                                      // Overall brightness.

int currentmode;


////////////////////////////////////////////// GETTING CODE RUN CLEAN UP LATER /////////////////////////////////////////////////////
void change_mode(int newmode) {
  
  Serial.print("New mode: ");
  Serial.println(newmode);
  currentmode = newmode;
  
} // change_mode()

void irlcall() {                                      // This is the built-in IR function that just selects a mode.


  if (IRLremote.available()) {
    
    auto data = IRLremote.read();           // Get the new data from the remote

//    Serial.print(F("Address: "));           // Print the protocol data. Note that there's also 65535, which we don't use.
//    Serial.println(data.address);
    Serial.print(F("Command: "));
    Serial.println(data.command);
    Serial.println();

    if (data.address == IR_ADD) {     
           
      switch(data.command) {

      case IR_A1:  change_mode(1);  break;          //a1 - 
      case IR_A2:  change_mode(2);  break;          //a2 - 
      case IR_A3:  change_mode(3);  break;          //a3 - 
      case IR_A4:  change_mode(4);  break;          //a4 - 

      case IR_B1:  change_mode(5);   break;          //b1 - 
      case IR_B2:  change_mode(6);   break;          //b2 - 
      case IR_B3:  change_mode(7);   break;          //b3 - 
      case IR_B4:  change_mode(8);   break;          //b4 - 

      case IR_C1:  change_mode(9);   break;          //c1 - 
      case IR_C2:  change_mode(10);  break;          //c2 - 
      case IR_C3:  change_mode(11);  break;          //c3 - 
      case IR_C4:  change_mode(12);  break;          //c4 -

      case IR_D1:  change_mode(13);  break;          //d1 -
      case IR_D2:  change_mode(14);  break;          //d2 - 
      case IR_D3:  change_mode(15);  break;          //d3 - 
      case IR_D4:  change_mode(16);  break;          //d4 -

      case IR_E1:  change_mode(17);  break;          //e1 -
      case IR_E2:  change_mode(18);  break;          //e2 - 
      case IR_E3:  change_mode(19);  break;          //e3 - 
      case IR_E4:  change_mode(20);  break;          //e4 -

      case IR_F1:  change_mode(21);  break;          //f1 -
      //case IR_F2:  change_mode(22);  break;          //f2 - 
      //case IR_F3:  change_mode(98);  break;          //f3 - 
      //case IR_F4:  change_mode(99);  break;          //f4 -

      default: break;                                // We could do something by default
      
      } // switch IRCommand

    } // if IR_ADD
  } // if IRLRemote


} // irltest()

void rainbow_beat() {
  
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.

} // rainbow_beat()


void rainbow_march(uint8_t thisdelay, uint8_t deltahue) {     // The fill_rainbow call doesn't support brightness levels.

  uint8_t thishue = millis()*(255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
  
// thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
// thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
  
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.

}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void RainbowPalette()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;   }
        if( secondHand == 30)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 35)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;   }
        if( secondHand == 45)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;; }
        if( secondHand == 50)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND;  }
        if( secondHand == 55)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;   }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



void sawtooth() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;                                // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);     // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(leds, NUM_LEDS, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending);    // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()


void blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()

void inoise8_fire() {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, min(i*(index)>>6, 255), i*255/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                      // Also, the higher the value of i => the brighter the LED.
} // inoise8_fire()

void sawtoothBlue() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;                                // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);     // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(leds, NUM_LEDS, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 127, 250, LINEARBLEND);    // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()

void inoise8_icefire() {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, 127 , i*455/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                      // Also, the higher the value of i => the brighter the LED.
// min(i*(index)>>6, 255)
} // inoise8_fire()

////////////////////////////////////////////// GETTING CODE RUN CLEAN UP LATER /////////////////////////////////////////////////////



void setup() {

  Serial.begin(9600);                                        // Initialize serial port for debugging.
  
  delay(750);                                                // Soft startup to ease the flow of electrons.
  
  if (!IRLremote.begin(pinIR))
    Serial.println(F("You did not choose a valid pin."));
  
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B - Does not work due to 3 pin configuration.
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801

  FastLED.setBrightness(max_bright);
  
  set_max_power_in_volts_and_milliamps(5, 1700);               // FastLED Power management set at 5V, 500mA.

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  currentmode = 0;
  
} // setup()



void loop() {

  if (!IRLremote.receiving()) {

          if(currentmode == 1){
            ChangePalettePeriodically();        
            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed            
            FillLEDsFromPaletteColors( startIndex);            
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
          }
          if(currentmode == 2){
            rainbow_march(200, 10);
            FastLED.show();
          }
          if(currentmode == 3){ 
            RainbowPalette();
            static uint8_t startIndex3 = 0;
            startIndex3 = startIndex3 + 1; // motion speed            
            FillLEDsFromPaletteColors( startIndex3);            
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
          }
          if(currentmode == 4){ 
            fadeToBlackBy( leds, NUM_LEDS, 30);
            int pos = random16(NUM_LEDS);
            leds[pos] = CHSV(135,80,200);
            FastLED.show();
          }
          if(currentmode == 5){
            sawtooth();
            FastLED.show();
          }
          if(currentmode == 6){
            blendwave();
            FastLED.show();

          }
          if(currentmode == 7){
            rainbow_beat();
            FastLED.show();
          }
          if(currentmode == 8){
              inoise8_fire();                                               // I am the god of hell fire and I bring you . . .
              FastLED.show();
          }
          if(currentmode == 9){
              sawtoothBlue();
              FastLED.show();
          }
          if(currentmode == 10){
            inoise8_icefire();
            FastLED.show();
          }
          if(currentmode == 11){
            fill_solid(leds, NUM_LEDS, CRGB::Blue);
            FastLED.show();
          }
          if(currentmode == 12){
            fill_solid(leds, NUM_LEDS, CRGB::Purple);
            FastLED.show();
          }
          if(currentmode == 13){
            fill_solid(leds, NUM_LEDS, CRGB::Green);
            FastLED.show();
          }
          if(currentmode == 14){
            fill_solid(leds, NUM_LEDS, CRGB::Orange);
            FastLED.show();
          }
          if(currentmode == 15){
            fill_solid(leds, NUM_LEDS, CRGB::White);
            FastLED.show();
          }
          if(currentmode == 21){
            fill_solid(leds, NUM_LEDS, CRGB::Black);
            FastLED.show();
          }
          /*
          case 7: break;
          case 8: break;
          case 9: break;
          case 10: break;
          case 11: break;
          case 12: break;
          case 13: break;
          case 14: break;
          case 15: break;
          case 16: break;
          case 17: break;
          case 18: break;
          case 19: break;
          case 20: break;
          case 21: break; 
          */      
        
        /*
        switch(currentmode){
          case 1:
            //RainbowPalette();
            ChangePalettePeriodically();        
            static uint8_t startIndex = 0;
            startIndex = startIndex + 1; // motion speed            
            FillLEDsFromPaletteColors( startIndex);            
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
            break;
          case 2:
//            fadeToBlackBy( leds, NUM_LEDS, 30);
//            int pos = random16(NUM_LEDS);
//            leds[pos] = CHSV(135,80,200);
//            FastLED.show();
            rainbow_march(200, 10);
            FastLED.show();
            break;
          case 3: 
            RainbowPalette();
            static uint8_t startIndex3 = 0;
            startIndex = startIndex3 + 1; // motion speed            
            FillLEDsFromPaletteColors( startIndex3);            
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
            break;
          case 4: 
            animationRed();                                               // render the first animation into leds2   
            animationGreen();                                               // render the second animation into leds3
          
            uint8_t ratio = beatsin8(2);                                // Alternate between 0 and 255 every minute
            
            for (int i = 0; i < NUM_LEDS; i++) {                        // mix the 2 arrays together
              leds1[i] = blend( leds2[i], leds3[i], ratio );
            }
          
            FastLED.show();
            break;
          case 5:
            ChangePalettePeriodically();        
            static uint8_t startIndex5 = 0;
            startIndex = startIndex5 + 1; // motion speed            
            FillLEDsFromPaletteColors( startIndex5);            
            FastLED.show();
            FastLED.delay(1000 / UPDATES_PER_SECOND);
            break;
          case 6:

            break;
          case 7: break;
          case 8: break;
          case 9: break;
          case 10: break;
          case 11: break;
          case 12: break;
          case 13: break;
          case 14: break;
          case 15: break;
          case 16: break;
          case 17: break;
          case 18: break;
          case 19: break;
          case 20: break;
          case 21: break;       
        }
        */
  }
  
  irlcall();
  
} // loop()