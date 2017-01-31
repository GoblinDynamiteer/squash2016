/* Kjell.com - Hur funkar Arduino?, Sida 206 

Anslut till wifi-nätverk med WiFi-modul ESP8266

VCC & CH_PD till 3.3V
TX/RX till 6/7

*/

#include <SoftwareSerial.h>
#include <WiFiEsp.h>

SoftwareSerial Serial1(6,7);

//SSID & lösenord för WiFi-modul
char ssid[] = "ssid";
char pw[] = "password";

int status = WL_IDLE_STATUS;

void printWiFiStatus();

void setup(){
	//Serial för debug och info
	Serial1.begin(9600);
	Serial.begin(115200);
	
	WiFi.init(&Serial1);
	
	if(WiFi.status() == WL_NO_SHIELD){
		Serial.println("WiFi Shield not present!");
		while(1);
	}
	
	while(status != WL_CONNECTED){
		Serial.print("Connecting to SSID: ");
		Serial.println(ssid);
		
		status = WiFi.begin(ssid, pw);
	}
	
	Serial.println("Connection successful!");
	printWiFiStatus();
}

void loop(){

}

void printWiFiStatus(){
		Serial.print("SSID: ");
		Serial.println(WiFi.SSID());
		IPAddress ip = WiFi.localIP();
		Serial.print("Local IP: ");
		Serial.println(ip);
}