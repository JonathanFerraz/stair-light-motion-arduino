// DIGITAL MOTION SENSING STAIR LIGHTS
// Jonathan Ferraz
// 2022

// This system utilizes an Atmel Atmega 328p MCU along with 2 PIR (Pyroelectric infrared) Sensors (HC-SR501) and a Digital RGB LED strip (WS2811).
// One sensor placed at the bottom of the stairs (or where ever) and one at the top. Each sensor triggers a different animation (walking up or walking down) along with a shut down warning animation.
// Just modify the function calls in the void loop() and the actual functins towards the bottom area to change the animations, color, etc. colorWipe for example. 
// The four parameters it takes are the RGB pixels followed by the speed at which you wish to run the animations. 0, 0, 0 would mean the LEDS are off and 255, 255, 255 means that they are all white/full brightness.

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
// NEO_RGB     Pixels are wired for RGB bitstream
// NEO_GRB     Pixels are wired for GRB bitstream
// NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
// NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(150, 6, NEO_GRB + NEO_KHZ800);

int motionPin = 8;                                                                               // PIR Input pin 8, UPSTAIRS
int motionPin2 = 7;                                                                              // PIR 2 Input pin 7, DOWNSTAIRS
int senseMotion = 0;                                                                             // variable to hold current state of motion detector
int senseMotion2 = 0;                                                                            // 2nd variable for 2nd sensor

void setup() {
  pinMode(motionPin, INPUT);                                                                     //PIRs declared as inputs
  pinMode(motionPin2, INPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  rainbow (30000);
  rainbowCycle (30000);
  strip.show();
}

void loop() {
  senseMotion = digitalRead (motionPin);                                                       //set variables equal to what our sensors are reading (on or off)
  senseMotion2 = digitalRead (motionPin2);
    
  if (senseMotion == HIGH || senseMotion2 == HIGH) {                                       // Testing both sensors at same time, if high, go to apropriate line.
      if (senseMotion2 == HIGH) {                                                            // If Upstairs motion sensor is triggered...
      colorWipe (strip.Color (30, 50, 255), 5);                                              // White/blue color sweep going up stairs (Towards the arrow on the strip)
      strip.show();                                                                          // This being enabled or not, doesnt seem to make a difference. NVM it looks like its more responsive when trying to activate the PIRs one after another
      delay (10000);                                                                         // Wait so someone can get to the bottom or top of stairs, will remove this when 2nd sensor is implemented with an interrupt
      //colorChase (strip.Color(255, 25, 25), 5);                                            // Color chase animations, red, green, blue indicating that the light are about to shut down
      //colorChase2(strip.Color(25, 255, 25), 5);
      //colorChase (strip.Color(25, 25, 255), 5);
      scanner(200, 0, 255, 1);
      colorWipe (strip.Color (0, 0, 0), 0);                                                  // This is needed for turn everything off, clear all pixels. More efficient with this here
      strip.show();
    }                        
        
    if (senseMotion == HIGH) {                                                                 // If motion sensor 2 (Downstairs) if high go to next line
      colorWipe2 (strip.Color(30, 50, 255), 5);
      strip.show();
      delay(10000);
      scanner(0, 200, 250, 1);
      //colorChase (strip.Color(255, 25, 25), 5);
      //colorChase2 (strip.Color(25, 255, 25), 5);
      //colorChase (strip.Color(25, 25, 255), 5);
      colorWipe2 (strip.Color(0, 0, 0), 0);
      strip.show();
      }  
    } else {                                                                                    // no motion = no lights or animate til off
        digitalWrite (motionPin, LOW);                                                        //turning motion pins low, doesnt seem to make a difference either.
        digitalWrite (motionPin2, LOW);
        //colorWipe (strip.Color(0, 0, 0), 30);
        strip.show();
    }
  }

//Function time!

// LED animation fuction, Fill the dots one after the other with a color

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
                                                                                                                        
// LED animate the other direction                                         
                                         
void colorWipe2(uint32_t c, uint8_t wait) {
  for(uint16_t i=149; i<strip.numPixels(); i--) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
                                          
// Every other LED is on (from arduino forum)

void everyOther(uint32_t c, uint8_t wait) {
  for(uint16_t k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor(2*k, c); 
    strip.show();
    delay(wait);    
  }
}

//Opposite from above function (from arduino forum)

void everyOther2(uint32_t c, uint8_t wait) {
  for(uint16_t k=0; k<strip.numPixels(); k++) {
    strip.setPixelColor((2*k+1), c); 
    strip.show();
    delay(wait);    
  }
}

//Rainbow fade effect (from adafruit)

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Chase one dot down the full strip.

void colorChase(uint32_t c, uint8_t wait) {
   int i;

    // Start by turning all pixels off:
    for(i=0; i<strip.numPixels(); i++)
      strip.setPixelColor(i, 0);

    // Then display one pixel at a time:
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);                // Set new pixel 'on'
      strip.show();                             // Refresh LED states
      strip.setPixelColor(i, 0);                // Erase pixel, but don't refresh!
      delay(wait);
    }

  strip.show();                                 // Refresh to turn off last pixel
}
                                            
// Chase one dot down the full strip, opposite direction.

void colorChase2(uint32_t c, uint8_t wait) {
   int i;

    // Start by turning all pixels off:
    for(i=0; i<strip.numPixels(); i++)
      strip.setPixelColor(i, 0);

   // Then display one pixel at a time:
    for(i=59; i<strip.numPixels(); i--) {
      strip.setPixelColor(i, c);                // Set new pixel 'on'
      strip.show();                             // Refresh LED states
      strip.setPixelColor(i, 0);                // Erase pixel, but don't refresh!
      delay(wait);
    }
  strip.show();                                 // Refresh to turn off last pixel
}                                            

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// "Larson scanner" = Cylon/KITT bouncing light effect

void scanner(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  int i, j, pos, dir;

  pos = 0;
  dir = 1;

  for(i=0; i<((strip.numPixels()-1) * 4); i++) {
    
    // Draw 5 pixels centered on pos.  setPixelColor() will clip
    // any pixels off the ends of the strip, no worries there.
    // we'll make the colors dimmer at the edges for a nice pulse
    // look
    
    strip.setPixelColor(pos - 2, strip.Color(r/4, g/4, b/4));
    strip.setPixelColor(pos - 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos, strip.Color(r, g, b));
    strip.setPixelColor(pos + 1, strip.Color(r/2, g/2, b/2));
    strip.setPixelColor(pos + 2, strip.Color(r/4, g/4, b/4));

    strip.show();
    delay(wait);
    
    // If we wanted to be sneaky we could erase just the tail end
    // pixel, but it's much easier just to erase the whole thing
    // and draw a new one next time.
    
    for(j=-2; j<= 2; j++) 
      strip.setPixelColor(pos+j, strip.Color(0,0,0));
      
    // Bounce off ends of strip
    
    pos += dir;
    if(pos < 0) {
      pos = 1;
      dir = -dir;
    } 
    else if(pos >= strip.numPixels()) {
      pos = strip.numPixels() - 2;
      dir = -dir;
    }
  }
}