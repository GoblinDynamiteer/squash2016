/*   Squash-racket "lightup" mode

      LED strip on racket light up at ball hit trigger.
      Then fades to zero brightness.

      For project Squash SM 2017
      Members:
      Dylan Saleh
      Dennis Bunne
      Johan Kämpe

      Mölk Utbildning
      Mjukvaruutvecklare inbyggda system

      Code by Johan Kämpe
 */

#include <Arduino.h>
#include <Adafruit_DotStar.h>

/*   Led-strip   */
#define NUMPIXELS 50
#define NUMCOLORS 6
#define DATAPIN 4
#define CLOCKPIN 5
#define LED_MAX_STR 250
#define LED_BRIGHTNESS_MAX 200
#define LED_BRIGHTNESS_MIN 0

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

/*   Delay between hit triggers  */
#define HIT_DELAY 60
#define FADE_DELAY 4

/*   Predefined colors  */
uint32_t ledColor[NUMCOLORS] = {
  0x00FF00, 0x0000FF, 0xFF0000, 0xA5FF00,
  0x00FFFF, 0xCCFFCC
};

/*  Indexes for ledColor array   */
enum{RED, BLUE, GREEN, YELLOW, PURPLE, WHITE};

#define CHOSENCOLOR GREEN

uint32_t chosenColor = ledColor[CHOSENCOLOR];
uint8_t brightness = LED_BRIGHTNESS_MIN;



/*  Timers   */
unsigned long timer = 0;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void idleMode(void);
void setLEDsOff(int count);
bool sensorTrigger(void);
bool checkHitDelay(void);
void clashHit(void);
void fadeMode();

/*  Sets pixel colors  */
void setLEDs(int count, uint32_t color, uint8_t b){
    setLEDsOff(count);
    strip.setBrightness(b);
    for(int i = 0; i < count && i < NUMPIXELS; i++){
        strip.setPixelColor(i, color);
    }
}

/*   Turn off pixels  */
void setLEDsOff(int count){
    strip.setBrightness(0);
    for(int i = 0; i < count && i < NUMPIXELS; i++){
        strip.setPixelColor(i, 0, 0, 0);
    }
}

void clashHit(void){
  brightness = LED_BRIGHTNESS_MAX;
  setLEDs(NUMPIXELS, chosenColor, brightness);
  strip.show();
}

void setup(){
  /* Start Led-strip  */
  strip.begin();
  /*  Start idle mode, ends if hit is detectled   */
  idleMode();
  clashHit();
  timer = millis();
}

void loop(){
  /*   Vibration sensor detection    */
  if(sensorTrigger() == 1 && checkHitDelay() == 1){
    clashHit();
    timer = millis();
  }
  if(brightness != LED_BRIGHTNESS_MIN){
    fadeMode();
  }
}

void fadeMode(){
  if(millis() % FADE_DELAY == 0){
    brightness--;
    if(brightness < LED_BRIGHTNESS_MIN){
      brightness = LED_BRIGHTNESS_MIN;
    }
    setLEDs(NUMPIXELS, chosenColor, brightness);
    strip.show();
  }
}

/*  LED-strip animation at startup   */
void idleMode(void){
  while(1){
    if(sensorTrigger() == 1){
      return;
    }
  }
}

/*  Detects sensor trigger treshold   */
bool sensorTrigger(void){
  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  return (vibrationData > VIBRATION_TRIGGER);
}

/*   Checks delay between hits  */
bool checkHitDelay(void){
  unsigned long offset = millis() - timer;
  return (offset > HIT_DELAY);
}
