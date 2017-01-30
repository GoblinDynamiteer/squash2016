
#include <SoftwareSerial.h>
#include <Adafruit_DotStar.h>

#define NUMPIXELS 54
#define DATAPIN 7
#define CLOCKPIN 8
#define LED_PIN 3
#define MAX_STR 250
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

void setup(){
  Serial.begin(115200);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

void loop(){
  strip.setBrightness(10);
  strip.show();
  // Read Piezo ADC value in, and convert it to a voltage
  float vibrationData = (float)analogRead(VIB_SENSOR_PIN) / 1023.0 * 200.0;
  if(vibrationData > VIBRATION_TRIGGER){
    Serial.println("Triggar!");
    strip.setBrightness(200);
    for(int i = 0; i < 54; i++){
      strip.setPixelColor(i, 255, 0, 255);
    }
    strip.show();
    delay(200);
  }

}
