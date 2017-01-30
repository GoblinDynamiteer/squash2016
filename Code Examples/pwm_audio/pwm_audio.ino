

#define SPEAKER_PIN 5
#define MAX_STR 3

void setup(){
		pinMode(SPEAKER_PIN, OUTPUT);
}

void loop(){
	for(int i = 0; i < MAX_STR; i++){
		analogWrite(SPEAKER_PIN, i);
		delay(100);
	}
  for(int i = MAX_STR; i > 0; i--){
		analogWrite(SPEAKER_PIN, i);
		delay(100);
	}
}
