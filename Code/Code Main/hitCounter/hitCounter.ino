#include <Adafruit_DotStar.h>

/*   Led-strip   */
#define NUMPIXELS 50
#define DATAPIN 4
#define CLOCKPIN 5
#define MAX_STR 250
#define LED_BRIGHTNESS_MAX 255
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
#define HIT_DELAY 50
const int VIB_SENSOR_PIN = A0;

bool humRise = 0;
bool idle = 1;
int hits = 0;
short ledGreen = 255;
short ledRed = 255;
short ledBlue = 255;
short LEDBrightness = HIT_DELAY;

int hitDelay = 50;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void startUpLed(void);

void setLEDs(int count){
    for(int i = 0; i < count; i++){
        strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
        strip.setBrightness(LEDBrightness);
    }
     strip.show();
}

void setLEDCount(int hits){
    if(hits <= 50){
        ledGreen = 255;
        ledBlue = 0;
        ledRed = 0;
        setLEDs(hits);
    }
}

void setup(){
  startUpLed();
}



void loop(){
  /*  Set color of LED-strip   */
  setLEDCount(hits);



  /*   Vibratin sensor detection    */
  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  if((vibrationData > VIBRATION_TRIGGER) && !hitDelay){
    Serial.println("Trigg!");
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
