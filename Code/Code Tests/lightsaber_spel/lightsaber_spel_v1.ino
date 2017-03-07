#include <Adafruit_DotStar.h>

/*   Led-strip   */
#define NUMPIXELS 60
#define DATAPIN 4
#define CLOCKPIN 5
#define LED_BRIGHTNESS_MAX 255

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

bool on = 0;
short ledGreen = 0;
short ledRed = 255;
short ledBlue = 0;
short LEDBrightness = 10;

int counter = 0;

/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void countLed(void){
   /*  Set color of LED-strip   */
   for(int i = 0; i < counter && counter <= 50; i++){
     strip.setPixelColor(i, 255, 0, 255);
   }
   /*  Set brightness of LED-strip   */
   strip.setBrightness(LEDBrightness);

   if (counter > 50) {
      Serial.println("fdsfafdsdfa");

      for(int i = 0; i < 5; i++){
        strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
        strip.setBrightness(LEDBrightness);
        Serial.println("-------------");
      }

      // /*  Set color of LED-strip   */
      // for(int i = 0; i < counter-50 && counter < 100; i++){
      //   strip.setPixelColor(i, ledGreen, ledRed, ledBlue);
      //   strip.setBrightness(LEDBrightness);
      // }
   }
   strip.show();
}

void setup(){
   /* Start Led-strip  */
   strip.begin();
   Serial.begin(9600);
}

void loop(){
  float vibrationData =
    (float)analogRead(VIB_SENSOR_PIN) * 0.2;
  //Serial.print("Vibration Data: ");
  //Serial.println(vibrationData);

  if(vibrationData > VIBRATION_TRIGGER){
    //Serial.println("Trigg!");
    counter++;
    countLed();
    delay(200);
  }
  /*  Do something smart to loop hum sound: */
  /*  Modulus of ticker?   */
  //sfx.playTrack(1);
}
