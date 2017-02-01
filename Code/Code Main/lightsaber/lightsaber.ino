
#include <SoftwareSerial.h>
#include <Adafruit_DotStar.h>
//#include <Adafruit_Soundboard.h>

/*   Led-strip   */
#define NUMPIXELS 54
#define DATAPIN 4
#define CLOCKPIN 5
#define MAX_STR 250
#define LED_BRIGHTNESS_MAX 255
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

/*  Adafruit Audio FX pins  */
#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

bool humRise = 0;
bool on = 0;
short ledGreen = 0;
short ledRed = 0;
short ledBlue = 0;
short LEDBrightness = 10;


Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

/*   Software serial communication with Soundboard.  */
//SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
// Adafruit_Soundboard sfx = Adafruit_Soundboard(
//   &ss,
//   NULL,
//   SFX_RST
// );


void setup(){
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1);
  #endif

  /*    For communication with soundboard */
  //ss.begin(9600);
  //sfx.reset();

  strip.begin();
  strip.show();
  startUpLed();
}

void loop(){
  for(int i = 0; i < NUMPIXELS; i++){
    strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
  }

  fadeLedBlue();
  fadeLedGreen();

  if(LEDBrightness > LED_BRIGHTNESS_HUM_MAX){
    LEDBrightness--;
  }
  else{
    if(humRise){
      LEDBrightness++;
    }
    else{
      LEDBrightness--;
    }
  }
  if(LEDBrightness == LED_BRIGHTNESS_HUM_MIN ||
    LEDBrightness == LED_BRIGHTNESS_HUM_MAX){
    humRise = !humRise;
  }
  strip.setBrightness(LEDBrightness);
  strip.show();
  float vibrationData = (float)analogRead(VIB_SENSOR_PIN) / 1023.0 * 200.0;
  if(vibrationData > VIBRATION_TRIGGER){
    //Serial.println("Triggar!");
    clashLed();
    //sfx.playTrack(2);
    delay(200);
  }
  strip.show();
}

/*   Set led strip to white */
void setLedWhite(void){
  ledRed = ledBlue = ledGreen = 255;
}

/*    Decreases Blue color */
void fadeLedBlue(void){
  if(ledBlue){
    ledBlue--;
  }
}

/*    Decreases Green color */
void fadeLedGreen(void){
  if(ledGreen){
    ledGreen--;
  }
}

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
