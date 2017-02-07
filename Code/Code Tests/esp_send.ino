#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <Ticker.h>  // ptova ta bort?
#include <Arduino.h>
/* *************************************
 *  VÄRDEN SOM ÄNDRAS
 ************************************** */
const char* ssid = "Dennis";             // NAMN PÅ WIFI
const char* password = "hejhej12";   // PASSWORD TILL WIFI
IPAddress ipClient(192, 168, 137, 1);  //IP ADRESS CLIENT (EJ TVÅNG)

/* *************************************
 *  FASTA VÄRDEN
 ************************************** */
char dataToSend[2];
WiFiUDP Udp;
unsigned int localPort = 9999;        // PORT SOM SERVER LÄSER AV
IPAddress ipServer(192, 168, 137, 1);   // IP ADRESS TILL SERVER
IPAddress Subnet(255, 255, 255, 0);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  WiFi.config(ipClient, ipServer, Subnet);
  Udp.begin(localPort);
  delay(2000);
  while (WiFi.status() != WL_CONNECTED) {     // KONTROLLERAR SÅ ESP ANSLUTER
    delay(500);
    Serial.print("\n.");
  }
  delay(100);
  Serial.println("Connected to WiFi");
}

void loop() {
    for (int t=0;t<10;t++) {  // SKICKAR DATA 10GGR FÖR UDP ÄR INTE SÄKERT :)
      Udp.beginPacket(ipServer,9999);
      Udp.write(dataToSend);
      Udp.endPacket();
      //Serial.println(packetBuffer);
      delay(1);
    }
    Serial.println(dataToSend);
  }

