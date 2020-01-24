#include <FastLED.h>
#include "FastLED_RGBW.h"
#include <Wire.h>
#include "Nunchuk.h"

#define NUM_LEDS 12
#define DATA_PIN 6
#define BRIGHTNESS  50

//define colors for use
#define BLACK 0
#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4

CRGBW leds[NUM_LEDS];
CRGB *ledsRGB = (CRGB *) &leds[0];

CRGB colors[] = {0x000000,0xFFFFFF,0xFF0000,0x008000,0x0000FF}; // black, white,r,g,b

int backcolor=0;
int frontcolor=0;
uint8_t current_C_BUTTON_STATUS=0;
uint8_t current_Z_BUTTON_STATUS=0;

void setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(ledsRGB, getRGBWsize(NUM_LEDS));
    FastLED.setBrightness(BRIGHTNESS );

    Serial.begin(9600);
    Wire.begin();

     // Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
    Wire.setClock(400000);

    // nunchuk_init_power(); // A1 and A2 is power supply
    nunchuk_init();
    uint8_t current_C_BUTTON_STATUS=0;

    //Test all leds 
    //Red

    for (size_t i = 0; i < 59; i++)
    {
      for (size_t j = 0; j < 4; j++)
      {
        stripsolidcolor(j);
      }
    }

    stripsolidcolor(RED);
    delay(100);
    stripsolidcolor(GREEN);
    delay(100);
    stripsolidcolor(BLUE);
    delay(100);
    stripsolidcolor(WHITE);
    delay(100);
    stripsolidcolor(RED);
    delay(100);
    stripsolidcolor(GREEN);
    delay(100);
    stripsolidcolor(BLUE);
    delay(100);
    stripsolidcolor(WHITE);
    delay(100);

    stripwipecolor(RED);
    delay(100);
    stripwipecolor(GREEN);
    delay(100);
    stripwipecolor(BLUE);
    delay(100);
    stripwipecolor(WHITE);
    delay(100);
    stripwipecolor(RED);
    delay(100);
    stripwipecolor(GREEN);
    delay(100);
    stripwipecolor(BLUE);
    delay(100);
    stripwipecolor(WHITE);
    delay(100);


}

void loop() {
    //for  
      //leds[0] = CRGB::Black;
    //attempt to read nunchuck if sucessful execute code if not blink red
    if (nunchuk_read()) {
      
      //Get C button status
      uint8_t new_C_BUTTON_STATUS = nunchuk_buttonC();

      if (new_C_BUTTON_STATUS != current_C_BUTTON_STATUS ) {
       // detected button pressed, we don't care about released
       delay (30);  // debounce by waiting a while and re testing
       if (new_C_BUTTON_STATUS == 1) {
            // still low so the button has been pressed
            if (backcolor == 4) {
                backcolor  = 0;
            } else {
                ++backcolor ;
            }
        }
        current_C_BUTTON_STATUS = new_C_BUTTON_STATUS;
      }

      //Get Z button status
      uint8_t new_Z_BUTTON_STATUS = nunchuk_buttonZ();

      if (new_Z_BUTTON_STATUS != current_Z_BUTTON_STATUS ) {
       // detected button pressed, we don't care about released
       delay (30);  // debounce by waiting a while and re testing
       if (new_Z_BUTTON_STATUS == 1) {
            // still low so the button has been pressed
            if (frontcolor == 4) {
                frontcolor  = 0;
            } else {
                ++frontcolor ;
            }
        }
        current_Z_BUTTON_STATUS = new_Z_BUTTON_STATUS;
      }
      
      
      int16_t JOYSTICK_X = nunchuk_joystickX();  
      int16_t JOYSTICK_Y = nunchuk_joystickY(); 

      

      const float DEG2RAD = PI / 180.0f;
      const float RAD2DEG = 180.0f / PI;

      float rad = atan2(JOYSTICK_X, JOYSTICK_Y);
      float angle = atan2(JOYSTICK_X, JOYSTICK_Y) * RAD2DEG;

      if (angle < 0) {
        angle = angle + 360;
      }

      int angle_leds = map(angle, 0, 359, 0, 12);

      // Serial.print("Angle=");
      // Serial.println(angle);
      // Serial.print(" - ");
      // Serial.print(angle_leds);
      // Serial.print("\n");

      Serial.print("C Button Status- ");
      Serial.println(new_C_BUTTON_STATUS);
      Serial.print("; Color - ");
      Serial.print(backcolor);
      Serial.print("\n");

      for (size_t i = 0; i < 12; i++)
      {
        if (i == angle_leds)
        {
          leds[i] = colors[frontcolor];
        } else
        {
          leds[i] = colors[backcolor];
        }
        FastLED.show();
      }  
    }
} 

void stripsolidcolor(int color){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = colors[color];
      FastLED.show();
    }
}

void stripwipecolor(int color){
    for (size_t i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = colors[color];
      FastLED.show();

      delay(100);
    }
}
