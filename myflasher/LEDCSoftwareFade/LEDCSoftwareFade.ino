/*
 LEDC Software Fade

 This example shows how to software fade LED
 using the ledcWrite function.

 Code adapted from original Arduino Fade example:
 https://www.arduino.cc/en/Tutorial/Fade

 This example code is in the public domain.
 */

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 13 bit precision for LEDC timer
#define LEDC_TIMER_13_BIT  13

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
//#define LED_PIN            21 // red
#define LED_PIN            22 // white

uint32_t brightness = 0;    // how bright the LED is
uint32_t fadeAmount = 1;    // how many points to fade the LED by
//uint32_t fadeAmount = 0x0fff;    // how many points to fade the LED by

void setup() {
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
}

uint32_t integerSqrt(uint32_t n) {
  if(n < 2) 
  {
    return n;
  } else {
    uint32_t smallCandidate = integerSqrt(n >> 2) << 1;
    uint32_t largeCandidate = smallCandidate + 1;
    return (largeCandidate*largeCandidate > n) ?
            smallCandidate :largeCandidate;
  }
}

void loop() {

//  ledcWrite(LEDC_CHANNEL_0, integerSqrt(brightness));
//  ledcWrite(LEDC_CHANNEL_0, brightness>>13);
  ledcWrite(LEDC_CHANNEL_0, brightness*brightness >> 13);

  brightness = (brightness + fadeAmount) & 0x1fff;
//  brightness = (brightness + fadeAmount) & 0x0fff;
//  brightness = (brightness + fadeAmount) & 0x03ffffff;


  //delay(1);
  delayMicroseconds(100);
}
