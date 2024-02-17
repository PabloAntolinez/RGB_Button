// Application to choose randomly between Red, Green and Blue on button press
// the neopixels is used to show the color
// very useful with 3 kids to decide who's going first to bathroom when watching Pokemon :)
// using Entropy library to generate as random as possible numbers https://code.google.com/archive/p/avr-hardware-random-number-generation/

#include <Adafruit_NeoPixel.h>
#include <Entropy.h>

#define BUTTON_PIN 12  // Pin connected to the button. It's a Pullup input, trig on a high -> low -> high
#define PIXEL_PIN 10   // Pin connected to the NeoPixels.
#define PIXEL_COUNT 1  // Number of Pixels to change color

#define NB_SUSPENS_LOOP 25  //fancy loop for suspens :)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int firstColor = -1;
int secondColor = -1;
int newColor = 0;
long superSeed = 0;
long lapse = 0;
long curMicros = 0;
int loopCounter = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.setPixelColor(0, strip.Color(255, 255, 255));  // green
  strip.show();
  Entropy.initialize();
}

void loop() {

  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);
  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      // Wait until button HIGH again (release)
      while (digitalRead(BUTTON_PIN) != HIGH)
        delay(10);
      // Suspens loop with color changing
      for (int iLoop = 0; iLoop < NB_SUSPENS_LOOP; iLoop++) {
        strip.clear();
        strip.setPixelColor(0, strip.Color(random(256), random(256), random(256)));  // green
        strip.show();
        delay((NB_SUSPENS_LOOP - iLoop) * 10);
      }
      // switch off the strip
      strip.clear();
      strip.setPixelColor(0, strip.Color(0, 0, 0));  // green
      strip.show();
      delay(1500);

      // select a color different from the previous one
      if (loopCounter == 0) {
        newColor = Entropy.random(3);
        firstColor = newColor;
      }
      else if(loopCounter == 1) {
        do {
          newColor = Entropy.random(3);
        } while (newColor == firstColor);
        secondColor = newColor; 
      }
      else if(loopCounter = 2) {
        do {
          newColor = Entropy.random(3);
        } while (newColor == firstColor || newColor == secondColor);
      }
      else
      {
        newColor = Entropy.random(3);
      }
      colorShow(newColor);
      delay(5000);
      strip.setPixelColor(0, strip.Color(255, 255, 255));  // green
      strip.show();
      loopCounter++;
    }
  }

  // Set the last button state to the old state.
  oldState = newState;
}

void colorShow(int i) {
  switch (i) {
    case 0:
      {
        strip.clear();
        strip.setPixelColor(0, strip.Color(0, 255, 0));  // green
        strip.show();
      }
      break;
    case 1:
      {
        strip.clear();
        strip.setPixelColor(0, strip.Color(255, 0, 0));  // red
        strip.show();
      }
      break;
    case 2:
      {
        strip.clear();
        strip.setPixelColor(0, strip.Color(0, 0, 255));  // blue
        strip.show();
      }
      break;
  }
}
