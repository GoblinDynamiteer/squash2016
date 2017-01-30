
#include <SoftwareSerial.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 54
#define DATAPIN 4
#define CLOCKPIN 5
#define LED_PIN 3
#define MAX_STR 250
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;
int LEDBrightness = 10;
#define LED_BRIGHTNESS_TRIGGER 255
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 50

bool humRise = 0;

Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup(){
  Serial.begin(115200);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

void loop(){
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
  // Read Piezo ADC value in, and convert it to a voltage
  float vibrationData = (float)analogRead(VIB_SENSOR_PIN) / 1023.0 * 200.0;
  if(vibrationData > VIBRATION_TRIGGER){
    Serial.println("Triggar!");
    humRise = 0;
    LEDBrightness = LED_BRIGHTNESS_TRIGGER;
    strip.setBrightness(LEDBrightness);
    for(int i = 0; i < 54; i++){
      /*  ?? RED ??   */
      strip.setPixelColor(i, 0, 255, 0);
    }
    strip.show();
    delay(200);
  }

}
