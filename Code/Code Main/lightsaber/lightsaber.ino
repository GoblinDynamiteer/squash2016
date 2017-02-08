
#include <SoftwareSerial.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_Soundboard.h>

/*   Led-strip   */
#define NUMPIXELS 54
#define DATAPIN 4
#define CLOCKPIN 5
#define MAX_STR 250
#define LED_BRIGHTNESS_MAX 255
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

/*  Adafruit Audio FX pins  */
#define SFX_TX 8
#define SFX_RX 7
#define SFX_RST 9

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

bool humRise = 0;
bool on = 0;
short ledGreen = 0;
short ledRed = 255;
short ledBlue = 0;
short LEDBrightness = 10;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

/*   Software serial communication with Soundboard.  */
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

/* Init soundboard as "sfx"  */
Adafruit_Soundboard sfx = Adafruit_Soundboard(
  &ss,
  NULL,
  SFX_RST
);

void fadeLedGreen(short fadeSpeed);
void fadeRedGreen(short fadeSpeed);
void fadeBlueGreen(short fadeSpeed);
void setLedWhite(void);
void clashLed(void);
void startUpLed(void);

void setup(){
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1);
  #endif

  /*    For communication with soundboard */
  ss.begin(9600);
  sfx.reset();

  /*   For debug  */
  Serial.begin(115200);

  /* Start Led-strip  */
  strip.begin();
  //strip.show();
  startUpLed();
}

void loop(){
  ledGreen = ledGreen > 0 ? ledGreen - 1 : 0;
  ledBlue = ledBlue > 0 ? ledBlue - 1 : 0;
  //ledRed = ledRed > 0 ? ledRed - 1 : 0;

  /*  Set color of LED-strip   */
  for(int i = 0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
  }

  if(LEDBrightness > LED_BRIGHTNESS_HUM_MAX){
    LEDBrightness -= 1;
  }
  else{
    if(humRise == 1){
      LEDBrightness += 1;
      Serial.println("Brightn++");
    }
    else{
      Serial.println("Brightn--");
      LEDBrightness -= 1;
    }
  }
  if((LEDBrightness < LED_BRIGHTNESS_HUM_MIN ||
    LEDBrightness > LED_BRIGHTNESS_HUM_MAX)){
    humRise = !humRise;
  }
  strip.setBrightness(LEDBrightness);
  strip.show();

  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  Serial.print("Vibration Data: ");
  Serial.println(vibrationData);

  if(vibrationData > VIBRATION_TRIGGER){
    Serial.println("Trigg!");
    clashLed();
    sfx.playTrack(2);
    //delay(200);
  }
}

/*   Set led strip to white */
void setLedWhite(void){
  ledRed = ledBlue = ledGreen = 255;
}

/* Set LED-strip to white at impact  */
void clashLed(void){
  setLedWhite();
  humRise = 0;
  LEDBrightness = LED_BRIGHTNESS_MAX;
  strip.setBrightness(LEDBrightness);
  for(int i = 0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
  }
  strip.show();
}

/*  Led animation at startup   */
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
