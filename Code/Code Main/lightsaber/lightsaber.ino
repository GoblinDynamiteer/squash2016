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
#define LED_BRIGHTNESS_MAX 220
#define LED_BRIGHTNESS_MIN 5

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

/*   Delay between hit triggers  */
#define HIT_DELAY 60
#define FADE_DELAY 4
#define IDLE_DELAY 40

/*   Predefined colors  */
uint32_t ledColor[NUMCOLORS] = {
  0x00FF00, 0x0000FF, 0xFF0000, 0xA5FF00,
  0x00FFFF, 0xCCFFCC
};

/*  Indexes for ledColor array   */
enum{RED, BLUE, GREEN, YELLOW, PURPLE, WHITE};

int colorIndex = RED;

uint32_t chosenColor = ledColor[colorIndex];
uint8_t brightness = LED_BRIGHTNESS_MIN;

int idleModeChaserPos = 0;
int idleModeChaserPos2 = NUMPIXELS/3;
int idleModeChaserPos3 = NUMPIXELS - (NUMPIXELS/3);

/*  Timers   */
unsigned long timer = 0;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );


void setLEDsOff(int count);
bool sensorTrigger(void);
bool checkHitDelay(void);
void clashHit(void);
void fadeMode();
void idleMode(void);
void loopColor(void);
void startUp(void);

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
  startUp();
  timer = millis();
}

void loop(){
  chosenColor = ledColor[colorIndex];
  /*   Vibration sensor detection    */
  if(sensorTrigger() == 1 && checkHitDelay() == 1){
    clashHit();
    loopColor();
    timer = millis();
  }
  if(brightness != LED_BRIGHTNESS_MIN){
    fadeMode();
  }
  else{
    idleMode();
  }
}

void startUp(void){
  int start = 0;
  int end = NUMPIXELS;
  strip.setBrightness(250);
  while(start <= end){
    strip.setPixelColor(start++, chosenColor);
    strip.setPixelColor(end--, chosenColor);
    strip.show();
    delay(35);
  }
}

void loopColor(void){
  colorIndex++;
  if(colorIndex == NUMCOLORS){
    colorIndex = 0;
  }
  chosenColor = ledColor[colorIndex];
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
  if(millis() % IDLE_DELAY == 0){
    idleModeChaserPos++;
    idleModeChaserPos2++;
    idleModeChaserPos3++;
    if(idleModeChaserPos == NUMPIXELS){
      idleModeChaserPos = 0;
    }
    if(idleModeChaserPos2 == NUMPIXELS){
      idleModeChaserPos2 = 0;
    }
    if(idleModeChaserPos3 == NUMPIXELS){
      idleModeChaserPos3 = 0;
    }
    strip.clear();
    strip.setBrightness(150);
    strip.setPixelColor(idleModeChaserPos, chosenColor);
    strip.setPixelColor(idleModeChaserPos2, chosenColor);
    strip.setPixelColor(idleModeChaserPos3, chosenColor);
    strip.show();
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
