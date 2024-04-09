#include <DHT.h> //Include the DHT library
#include "secrets.h"
#include "ThingSpeak.h"
#include <WiFiNINA.h>

#define DHTPIN 2     //Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   //DHT 11

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char* myWriteAPIKey = SECRET_WRITE_APIKEY;


String myStatus = "";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  ThingSpeak.begin(client);

  dht.begin(); //Initialize DHT sensor
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  float temperature = dht.readTemperature(); //Read temperature from DHT sensor

  ThingSpeak.setField(1, temperature);

  ThingSpeak.setStatus(myStatus);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
if (x == 200) {
  Serial.print("Channel update successful. ");
  Serial.print(temperature);
  Serial.println("°C");
}

  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(60000); //Update ThingSpeak every 60 seconds
}
