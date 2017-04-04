// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.
// Modified by Omar Lara.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SimpleDHT.h>


#define FIREBASE_HOST "databaseName.firebaseio.com"
#define FIREBASE_AUTH "[FIREBASE_AUTH_CODE]"
#define WIFI_SSID "[WIFI_SSID_]"
#define WIFI_PASSWORD "[WIFI_PASSWORD]"

int pinDHT11 = 2;
SimpleDHT11 dht11;

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
  
void loop() {
  
  Firebase.setString("message", "Hi Omar my sensor is ready!!!");

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    return;
  }
  

  String humedityPush = Firebase.pushInt("humidity", humidity);
  String temperaturePush = Firebase.pushInt("temperature", temperature);

  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }

  Serial.print("pushed: /humedity/");
  Serial.println(humedityPush);
  Serial.print("pushed: /temperature/");
  Serial.println(temperaturePush);

  delay(5000);
}
