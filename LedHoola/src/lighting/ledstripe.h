/**
 * ** GNU General Public License Usage
 ** This file may be used under the terms of the GNU
 ** General Public License version 3 or (at your option) any later version
 ** approved by the KDE Free Qt Foundation. The licenses are as published by
 ** the Free Software Foundation and appearing in the file LICENSE.GPL3
 ** included in the packaging of this file. Please review the following
 ** information to ensure the GNU General Public License requirements will
 ** be met: https://www.gnu.org/licenses/gpl-3.0.html.* 
 * 
 * description     :Handles the control of the LED-stripe
 *                  It uses the wonderfull FastLED library - THANKS!
 *                  A lot of sketches are with high inspiration from there!
 * 
 * variables       :
 *                 tbd
 * 
 * methods         :
 *                 tbd
 *                  
 * author          :Philipp Wuerfel
 * date            :2020-05-06
 * version         :1.0
 * notes           :Still running tests in prototype environment
 * language        :C++
*/

#ifndef LEDSTRIPE_H
#define LEDSTRIPE_H

#include "FastLED.h"                                          // https://github.com/FastLED/FastLED     Use 3.2

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define LED_DT 6                                             // Data pin to connect to the strip.
//#define LED_CK 11                                             // Clock pin for WS2801 or APA102.
#define COLOR_ORDER GRB                                       // It's GRB for WS2812 and BGR for APA102.
#define LED_TYPE WS2812                                       // Using APA102, WS2812, WS2801. Don't forget to modify LEDS.addLeds to suit.
#define NUM_LEDS 90                                           // Number of LED's.
#define UPDATES_PER_SECOND 100

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


class Ledstripe{
    private:
        uint8_t max_bright = 128;   // Overall brightness.

    public:
        Ledstripe();
        ~Ledstripe();

        // Global variables can be changed on the fly.  
        CRGBPalette16 currentPalette;
        TBlendType    currentBlending;     

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
        uint32_t xscale = 20;   // How far apart they are
        uint32_t yscale = 3;    // How fast they move
        uint8_t index = 0;

        // setter
        void setBrightness(uint8_t max_bright);
        void setMaxPowerInVoltsAndMilliamps(int voltage, int mamp);

        void show();
        void delay(int val);

        // light shows
        void rainbow_beat();
        void rainbow_march(uint8_t thisdelay, uint8_t deltahue);
        void FillLEDsFromPaletteColors( uint8_t colorIndex);
        void RainbowPalette();
        void SetupTotallyRandomPalette();
        void SetupBlackAndWhiteStripedPalette();
        void SetupPurpleAndBlackStripedPalette();
        void SetupRainbowStripedPalette();
        void SetupPurpleAndGreenPalette();
        void ChangePalettePeriodically();
        void sawtooth();
        void blendwave();
        void inoise8_fire();
        void sawtoothBlue();
        void inoise8_icefire();

        // using methods which are already available in FastLED
        void fadeToBlack(uint16_t numLeds, uint8_t fadeBy);
        void fill(uint16_t numLeds, CRGB crgb);
};

#endif