#include "DHT.h"
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#define DHTPIN 2


#define DHTTYPE DHT11 

SoftwareSerial espSerial(5, 6);
DHT dht(DHTPIN, DHTTYPE);
String str;

void setup(){
Serial.begin(115200);
espSerial.begin(115200);
dht.begin();
delay(2000);
}


void loop()
{
float h = dht.readHumidity();
// Read temperature as Celsius
float t = dht.readTemperature();

espSerial.println(h);
delay(1000);
espSerial.println(t);
delay(1000);
}