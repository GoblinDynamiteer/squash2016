/*   Squash-racket hit game  */

#include <Arduino.h>
#include <Adafruit_DotStar.h>

/*   Led-strip   */
#define NUMPIXELS 50
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
#define HIT_DELAY 200

/*   "Game" works for 60 seconds  */
#define PLAY_TIME 60000

bool idle = 1;
int hits = 0;
short LEDBrightness = 250;

//GRB - Starts at RED
uint32_t ledColor = 0x00FF00;

unsigned long timer = 0;
unsigned long startTime = 0;


/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void startUpLed(void);
void setLEDsOff(int count);
bool sensorTrigger(void);
bool checkHitDelay(void);

void setLEDs(int count){
    setLEDsOff(count);
    strip.setBrightness(LED_BRIGHTNESS_MAX);
    for(int i = 0; i < count && i < NUMPIXELS; i++){
        strip.setPixelColor(i, ledColor);
    }
}

void setLEDsOff(int count){
    strip.setBrightness(0);
    for(int i = 0; i < count && i < NUMPIXELS; i++){
        strip.setPixelColor(i, 0, 0, 0);
    }
}

void setLEDCount(int hits){
    if(hits > 0){
      ledColor = 0xFF0000; //GREEN
      setLEDs(hits);
    }
    if(hits > NUMPIXELS){
      ledColor = 0x0000FF; //BLUE
      setLEDs(hits - NUMPIXELS);
    }
    if(hits > (NUMPIXELS * 2)){
      ledColor = 0x00FF00; //RED
      setLEDs(hits - (NUMPIXELS * 2));
    }
    if(hits > (NUMPIXELS * 3)){
      ledColor = 0xA5FF00; //ORANGE rgb(255,165,0) 	#FFA500
      setLEDs(hits - (NUMPIXELS * 3));
    }
}

void setup(){
  /* Start Led-strip  */
  strip.begin();
  startUpLed();
}



void loop(){
  /*   Vibration sensor detection    */
  if(sensorTrigger() == 1 && checkHitDelay() == 1){
    setLEDCount(hits++);
    strip.show();
    timer = millis();
  }
}

/*  LED-strip animation at startup   */
void startUpLed(void){
  short start = 0;
  short end = NUMPIXELS;
  while(start <= end){
    strip.setPixelColor(start++, ledColor);
    strip.setPixelColor(end--, ledColor);
    strip.show();
    delay(20);
  }
}

bool sensorTrigger(void){
  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  return (vibrationData > VIBRATION_TRIGGER);
}

bool checkHitDelay(void){
  unsigned long offset = millis() - timer;
  return (offset > HIT_DELAY);
}
