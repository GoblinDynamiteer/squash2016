#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Adafruit_Soundboard.h>

#define SFX_TX 5
#define SFX_RX 6

#define SFX_RST 4

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

const int PIEZO_PIN = A0; // Piezo output
const byte LED_PIN = 3; // LED output

void setup()
{
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
}

void loop()
{
  // Read Piezo ADC value in, and convert it to a voltage
  int piezoADC = analogRead(PIEZO_PIN);
  float piezoV = piezoADC / 1023.0 * 200.0;

  if(piezoV > 10){
    digitalWrite(LED_PIN, HIGH);
    sfx.playTrack(2);
  }
  digitalWrite(LED_PIN, LOW);
}

