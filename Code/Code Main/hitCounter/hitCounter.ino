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
#define HIT_DELAY 50
const int VIB_SENSOR_PIN = A0;

bool idle = 1;
int hits = 0;
short ledGreen = LED_MAX_STR;
short ledRed = LED_MAX_STR;
short ledBlue = LED_MAX_STR;
short LEDBrightness = 250;

int hitDelay = HIT_DELAY;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void startUpLed(void);

void setLEDs(int count){
    strip.setBrightness(LEDBrightness);
    for(int i = 0; i < count; i++){
        strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
    }
     strip.show();
}

void setLEDCount(int hits){
    if((hits <= 50)  && (hits != 0)){
        ledGreen = 255;
        ledBlue = 0;
        ledRed = 0;
        setLEDs(hits);
    }
    return;
}

void setup(){
  /* Start Led-strip  */
  strip.begin();
  startUpLed();
}



void loop(){
    hitDelay--;
  /*  Set color of LED-strip   */
  setLEDCount(hits);

  /*   Vibration sensor detection    */
  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  if((vibrationData > VIBRATION_TRIGGER) && (hitDelay < 0)){
    hits++;
    hitDelay = HIT_DELAY;
  }
}

/*  LED-strip animation at startup   */
void startUpLed(void){
  ledGreen = ledBlue = 0;
  ledRed = LED_BRIGHTNESS_MAX;
  short start = 0;
  short end = NUMPIXELS;
  while(start <= end){
    strip.setPixelColor(start++, ledGreen, ledRed, ledBlue);
    strip.setPixelColor(end--, ledGreen, ledRed, ledBlue);
    strip.show();
    delay(20);
  }
}
