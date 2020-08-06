#include "ledstripe.h"


Ledstripe::Ledstripe(){
    LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);      // For WS2812B - Does not work due to 3 pin configuration.
//  LEDS.addLeds<LED_TYPE, LED_DT, LED_CK, COLOR_ORDER>(leds, NUM_LEDS);   // For APA102 or WS2801
    FastLED.setBrightness(max_bright);
};

Ledstripe::~Ledstripe(){
};

void Ledstripe::setBrightness(uint8_t brightness){
    FastLED.setBrightness(max_bright);
} // setBrightness()

void Ledstripe::setMaxPowerInVoltsAndMilliamps(int voltage, int mamp){
    set_max_power_in_volts_and_milliamps(voltage, mamp);  // FastLED Power management set at 5V, 500mA.
}

void Ledstripe::show(){
    FastLED.show();
}

void Ledstripe::delay(int val){
    //FastLED.delay(val / UPDATES_PER_SECOND);
    FastLED.delay(val);
}

void Ledstripe::rainbow_beat() {  
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.
} // rainbow_beat()


void Ledstripe::rainbow_march(uint8_t thisdelay, uint8_t deltahue) {
    // The fill_rainbow call doesn't support brightness levels.
    uint8_t thishue = millis()*(255-thisdelay)/255;             // To change the rate, add a beat or something to the result. 'thisdelay' must be a fixed value.
    // thishue = beat8(50);                                       // This uses a FastLED sawtooth generator. Again, the '50' should not change on the fly.
    // thishue = beatsin8(50,0,255);                              // This can change speeds on the fly. You can also add these to each other.
    
    fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
}

void Ledstripe::FillLEDsFromPaletteColors( uint8_t colorIndex)
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

void Ledstripe::RainbowPalette()
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
void Ledstripe::SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void Ledstripe::SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;    
}

void Ledstripe::SetupRainbowStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette = RainbowStripeColors_p;
    currentBlending = LINEARBLEND;    
}

// This function sets up a palette of purple and green stripes.
void Ledstripe::SetupPurpleAndGreenPalette()
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

// This function sets up a palette of purple and green stripes.
void Ledstripe::SetupPurpleAndBlackStripedPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   purple,  purple,  black,  black,
                                   purple, purple, black,  black,
                                   purple,  purple,  black,  black,
                                   purple, purple, black,  black );
}

void Ledstripe::ChangePalettePeriodically()
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
} // ChangePalettePeriodically()

void Ledstripe::sawtooth() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;  // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);  // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(leds, NUM_LEDS, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 0, 255, currentBlending);  // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()

void Ledstripe::blendwave() {

  speed = beatsin8(6,0,255);

  clr1 = blend(CHSV(beatsin8(3,0,255),255,255), CHSV(beatsin8(4,0,255),255,255), speed);
  clr2 = blend(CHSV(beatsin8(4,0,255),255,255), CHSV(beatsin8(3,0,255),255,255), speed);

  loc1 = beatsin8(10,0,NUM_LEDS-1);
  
  fill_gradient_RGB(leds, 0, clr2, loc1, clr1);
  fill_gradient_RGB(leds, loc1, clr2, NUM_LEDS-1, clr1);

} // blendwave()

void Ledstripe::inoise8_fire() {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, min(i*(index)>>6, 255), i*255/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                      // Also, the higher the value of i => the brighter the LED.
} // inoise8_fire()

void Ledstripe::sawtoothBlue() {

  int bpm = 60;
  int ms_per_beat = 60000/bpm;  // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/bpm/NUM_LEDS;

  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(NUM_LEDS);  // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  if (cur_led == 0)
   fill_solid(leds, NUM_LEDS, CRGB::Black);
  else
    leds[cur_led] = ColorFromPalette(currentPalette, 127, 250, LINEARBLEND);  // I prefer to use palettes instead of CHSV or CRGB assignments.

} // sawtooth()

void Ledstripe::inoise8_icefire() {
  
  for(int i = 0; i < NUM_LEDS; i++) {
    index = inoise8(i*xscale,millis()*yscale*NUM_LEDS/255);                                           // X location is constant, but we move along the Y at the rate of millis()
    leds[i] = ColorFromPalette(currentPalette, 127 , i*455/NUM_LEDS, LINEARBLEND);  // With that value, look up the 8 bit colour palette value and assign it to the current LED.
  }                                                                                                   // The higher the value of i => the higher up the palette index (see palette definition).
                                                                                                      // Also, the higher the value of i => the brighter the LED.
// min(i*(index)>>6, 255)
} // inoise8_fire()

void Ledstripe::fadeToBlack(uint16_t numLeds, uint8_t fadeBy){
    fadeToBlackBy(leds, numLeds, fadeBy);  // Use FastLED's fadeToBlackBy function.
} // fadeToBlack()

void Ledstripe::fill(uint16_t numLeds, CRGB crgb){
    fill_solid(leds, numLeds, crgb);  // Use FastLED's fadeToBlackBy function.
} // fadeToBlack
