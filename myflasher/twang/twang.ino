/*
 LEDC Software Fade

 This example shows how to software fade LED
 using the ledcWrite function.

 Code adapted from original Arduino Fade example:
 https://www.arduino.cc/en/Tutorial/Fade

 This example code is in the public domain.
 */

#define LEDC_CHANNEL_0     0
#define LEDC_CHANNEL_1     1

#define LEDC_TIMER_13_BIT  13

#define LEDC_BASE_FREQ     5000

#define LED_PIN_RED            21 // red
#define LED_PIN_WHITE            22 // white

uint32_t brightness = 0x0000;
uint32_t* speed = 0;

uint32_t fadeAmount = 1;
//uint32_t fadeAmount = 0x0fff;    // how many points to fade the LED by

void setup() {
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN_RED, LEDC_CHANNEL_0);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_13_BIT);
  ledcAttachPin(LED_PIN_WHITE, LEDC_CHANNEL_1);

  Serial.begin(115200);

  Serial.printf("hellworld\n");

//  uint32_t p = 8;
//  uint32_t c0 = (0x00000001 << p) * 2;
//  uint32_t c2 = (0x00000001 << p) / 6; //0x0000002a;
//  Serial.printf("%x %x\n",c0,c2);

//  for (uint32_t n=0; n<= 512;n+=16) {    
//    float p=0.00390625;
//    float err = p*(myexp(n)+0.5) - exp(n*p);
//    Serial.printf("% 4x % 4x % 9f % 9f % 9f % 9f\n",n, myexp(n), n*p,p*(0.5+myexp(n)), exp(n*p), err);
//  }

//  Serial.printf("%x %f\n",myexp(0x1fff),myexp(0x1fff)/256.0);
//  Serial.printf("%x %f %f\n",myexp(0x1000), myexp(0x100 * 8)/256.0, exp(8.0));

//  Serial.printf("%x\n",mymul(0x80, 0x80));
//  Serial.printf("%x\n",mymul(0x100, 0x80));
//  Serial.printf("%x\n",mymul(0x80, 0x100));
//  Serial.printf("%x\n",mymul(0xff, 0xff));
//  Serial.printf("%x\n",mymul(0x180, 0x180));
//  Serial.printf("%x\n",mymul(0x200, 0x200));

  for (uint32_t n=0; n<= 0x32<<8; n+=128) {
    Serial.printf("% 9f % 9f % 9f % 9f % 9f\n",n,n/256.0, myexp(n)/256.0, exp(n/256.0), myexp(n)/256.0/ exp(n/256.0));
  }

}

uint32_t fixed8exp(uint32_t r) {
  uint32_t p = 8;
  uint32_t c0 = (0x00000001 << p) * 2;
  uint32_t c2 = (0x00000001 << p) / 6; //0x0000002a;
  
  uint32_t r2 = (r*r)>>p;

  uint32_t remez = c0 + ((r2 * c2)>>p); // R(r^2)

  uint32_t den = remez - r;
  // 1 + r * 2 / (R(r^2) - r)
  return (0x00000001<<p) + (r << (p+1)) / den;
}

uint32_t mymul(uint32_t a, uint32_t b) {
  uint32_t p = 8;
  uint32_t m = (0x1<<p)-1;
  uint32_t ai = a >> p;
  uint32_t af = a & m;
  uint32_t bi = b >> p;
  uint32_t bf = b & m;
  return a*bi + ai*bf + ((af*bf)>>p);
}


uint32_t myexp(uint32_t r) {
  uint32_t p = 8;
  uint32_t thresh = (0x00000001 << (p-1));
  if (r <= thresh) {
    return fixed8exp(r);
  } else {
    return mymul(myexp(r>>1), myexp((r+1)>>1));
  }
  
}

uint32_t looplim = 4096*2;

uint32_t ledtime(uint32_t x) {
//  return (((myexp(x)-0x00000100)) * 0x00000099) >> 8;
//  return (((myexp(x)-0x00000100)) * 0x00000099) >> 8;
//  return uint32_t(8192 * (exp(x*16.0/(looplim>>1))-1.0) / (exp(16.0) - 1.0));
//  return uint32_t((myexp(x)-256.0) * 32.0 / (exp(16.0) - 1.0));
//  return uint32_t((myexp(x) * 32.0 / (exp(16.0) - 1.0) - 256.0 * 32.0 / (exp(16.0) - 1.0)) * 524288 / 524288);
//  return uint32_t((myexp(x) * 32.0 / (exp(16.0) - 1.0) - 256.0 * 32.0 / (exp(16.0) - 1.0)) * 262144 / 262144); 

  return (myexp(x) - 0x000000f2) >> 18;
}

void loop() {

//  float compbright = ((myexp(brightness>>3)-0x00000100)) * 2048.0 / (exp(8.0) - 1.0);
//  float compbright = ((myexp(brightness)-0x00000100)) * 2048.0 / (exp(1.0) - 1.0);
//  float compbright = ((myexp(brightness)-0x00000100)) * 32.0  / (exp(4.0) - 1.0);
//  uint32_t compbright = (((myexp(brightness)-0x00000100)) * 0x00000099) >> 8;

  uint32_t bb1 = (brightness <= (looplim>>1)) ? brightness : looplim-brightness;
  uint32_t bb2 = (looplim>>1)-bb1;

  uint32_t compbright1 = ledtime(bb1);
  uint32_t compbright2 = ledtime(bb2);

//  uint32_t compbright1 = (((myexp(bb1)-0x00000100)) * 0x00000099) >> 8;
//  uint32_t compbright2 = (((myexp(bb2)-0x00000100)) * 0x00000099) >> 8;

//  if (brightness % 100 == 0)
//    Serial.printf("%d %d % 9d\n", brightness, bb1, uint32_t(compbright1));

  ledcWrite(LEDC_CHANNEL_0, compbright1);
  ledcWrite(LEDC_CHANNEL_1, compbright2);

  brightness = (brightness + fadeAmount) & (looplim-1);
  
  //delay(1);
//  delayMicroseconds(100);
  delayMicroseconds(200);
//  delayMicroseconds(1000);
}
