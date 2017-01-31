
//#include <SoftwareSerial.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 54
#define DATAPIN 4
#define CLOCKPIN 5
#define LED_PIN 3
#define MAX_STR 250
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

#define LED_BRIGHTNESS_MAX 255
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

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

void setup(){
  //Serial.begin(115200);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1);
  #endif

  strip.begin();
  strip.show();
  startUpLed();
}

void loop(){
  for(int i = 0; i < 54; i++){
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
    Serial.println("Triggar!");
    setLedWhite();
    humRise = 0;
    LEDBrightness = LED_BRIGHTNESS_MAX;
    strip.setBrightness(LEDBrightness);
    for(int i = 0; i < 54; i++){
      strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
    }
    strip.show();
    delay(200);
  }
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

void startUpLed(void){
  ledGreen = ledBlue = 0;
  ledRed = LED_BRIGHTNESS_MAX;
  short start = 0;
  short end = NUMPIXELS;
  while(start <= end){
    strip.setPixelColor(start++, ledGreen, ledRed, ledBlue);
    strip.setPixelColor(end--, ledGreen, ledRed, ledBlue);
    strip.show();
    delay(100);
  }
}
