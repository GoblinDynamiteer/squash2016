#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>
#include <Adafruit_DotStar.h>


#define SFX_TX 5
#define SFX_RX 6

#define SFX_RST 4


#define NUMPIXELS 6 // Number of LEDs in strip


#define DATAPIN    7
#define CLOCKPIN   8
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

const int PIEZO_PIN = A0; // Piezo output
const byte LED_PIN = 3; // LED output

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Adafruit Sound Board!");

  // softwareserial at 9600 baud
  ss.begin(9600);
  // can also do Serial1.begin(9600)

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  strip.setBrightness(10);
  
}

void loop()
{
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoADC = analogRead(PIEZO_PIN);
  float piezoV = piezoADC / 1023.0 * 200.0;

  if(piezoV > 15){
    digitalWrite(LED_PIN, HIGH);
    sfx.playTrack(2);

    strip.setPixelColor(0, 255, 0, 255);
    strip.setPixelColor(1, 255, 0, 255);
    strip.setPixelColor(2, 255, 0, 255);
    strip.setPixelColor(3, 255, 0, 255);
    strip.setPixelColor(4, 255, 0, 255);
    strip.setPixelColor(5, 255, 0, 255);
    strip.setPixelColor(6, 255, 0, 255);

    strip.setBrightness(30);
    strip.show();
    delay(200);
    strip.setBrightness(0);
    strip.show();       
  }
  digitalWrite(LED_PIN, LOW);
}

