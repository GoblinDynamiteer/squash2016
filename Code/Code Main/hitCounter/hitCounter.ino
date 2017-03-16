/*   Squash-racket hit game

    Get as many hits you can in one minute!

    scoring guide:
      STRIP COLOR:
      GREEN : 0 + Pixel Position
      RED: 	50 + Pixel Position
      BLUE: 	100 + Pixel Position
      YELLOW: 150 + Pixel Position
      PURPLE: 200 + Pixel Position

      White pixels are shown at every 10th pixel to help with count!

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
#define LED_BRIGHTNESS_MAX 250
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

/*   Delay between hit triggers  */
#define HIT_DELAY 120

/*   "Game" works for 60 seconds  */
#define PLAY_TIME 60000


/*  Counts hits on racket   */
int hits = 0;

/*   Predefined colors  */
uint32_t ledColor[NUMCOLORS] = {
  0x00FF00, 0x0000FF, 0xFF0000, 0xA5FF00,
  0x00FFFF, 0xCCFFCC
};

/*  Indexes for ledColor array   */
enum{RED, BLUE, GREEN, YELLOW, PURPLE, WHITE};

/*  Timers   */
unsigned long timer = 0;
unsigned long startTime = 0;


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
void setScore(void);

/*  Sets pixel colors  */
void setLEDs(int count, uint32_t color){
    setLEDsOff(count);
    strip.setBrightness(LED_BRIGHTNESS_MAX);
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

/*   Set LEDs to indicate hits  */
void setLEDCount(int hits){
    if(hits > 0)
      setLEDs(hits, ledColor[GREEN]);
    if(hits > NUMPIXELS)
      setLEDs(hits - NUMPIXELS, ledColor[RED]);
    if(hits > (NUMPIXELS * 2))
      setLEDs(hits - (NUMPIXELS * 2), ledColor[BLUE]);
    if(hits > (NUMPIXELS * 3))
      setLEDs(hits - (NUMPIXELS * 3), ledColor[YELLOW]);
    if(hits > (NUMPIXELS * 4))
      setLEDs(hits - (NUMPIXELS * 4), ledColor[PURPLE]);
}

void setup(){
  /* Start Led-strip  */
  strip.begin();
  /*  Start idle mode, ends if hit is detectled   */
  idleMode();
  /*  Turn off all LEDs   */
  setLEDs(NUMPIXELS, 0x000000);
  strip.show();
  /*   Begin hit counter and timers  */
  setLEDCount(++hits);
  strip.show();
  timer = millis();
  startTime = millis();
}

void loop(){
  /*   Vibration sensor detection    */
  if(sensorTrigger() == 1 && checkHitDelay() == 1){
    setLEDCount(++hits);
    strip.show();
    timer = millis();
  }
  /*   Game over, set score!   */
  if(millis() - startTime > PLAY_TIME){
    setScore();
    strip.show();
    /*  Infinite loop - restart racket to reset   */
    while(1){
      delay(1000);
    }
  }
}

void setScore(void){
  strip.clear();
  int position = 0;
  int score = 0;
  if(hits > (NUMPIXELS * 4)){
    position = hits - (NUMPIXELS * 4);
    setLEDs(position, ledColor[PURPLE]);
  }
  else if(hits > (NUMPIXELS * 3)){
    position = hits - (NUMPIXELS * 3);
    setLEDs(position, ledColor[YELLOW]);
  }
  else if(hits > (NUMPIXELS * 2)){
    position = hits - (NUMPIXELS * 2);
    setLEDs(position, ledColor[BLUE]);
  }
  else if(hits > NUMPIXELS){
    position = hits - NUMPIXELS;
    setLEDs(position, ledColor[RED]);
  }
  else if(hits > 0){
    position = hits;
    setLEDs(position, ledColor[GREEN]);
  }
  int setTenMarker = 9;
  while(position > 10){
    strip.setPixelColor(setTenMarker, 0xFFFFFF);
    position -= 10;
    setTenMarker += 10;
  }
}

/*  LED-strip animation at startup   */
void idleMode(void){
  int cIndex = 0;
  while(1){
    if(++cIndex == NUMCOLORS){
      cIndex = 0;
    }
    uint32_t color = ledColor[cIndex];
    for(int i = 0; i < NUMPIXELS; i++){
      strip.setPixelColor(i, color);
      strip.show();
      timer = millis();
      while(millis() - timer < 30){
        if(sensorTrigger() == 1){
          return;
        }
      }
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
