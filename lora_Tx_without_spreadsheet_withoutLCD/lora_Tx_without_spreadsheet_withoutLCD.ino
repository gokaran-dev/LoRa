#include <Wire.h>
#include <SFE_BMP180.h>

#include <SPI.h>
#include <DHT.h> 
float t,h;  
#define DHTPIN A3
 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#include <LoRa.h> 
#define ss 10
#define rst 9
#define dio0 2

SFE_BMP180 pressure;
double T,P,p0,a;
double baseline; // baseline pressure
#define ALTITUDE 1655.0

#include<string.h>
#define PM1PIN A0//DSM501A input D6 on ESP8266
#define PM25PIN A1
byte buff[2];
unsigned long durationPM1;
unsigned long durationPM25;
unsigned long starttime;
unsigned long endtime;
unsigned long sampletime_ms = 1000;
unsigned long lowpulseoccupancyPM1 = 0;
unsigned long lowpulseoccupancyPM25 = 0;
int i=0;
float conPM1=0;
float conPM25=0;
/////////////////////////
void setup() 
{
  
  Serial.begin(9600);
  pinMode(PM1PIN,INPUT);
  pinMode(PM25PIN,INPUT);
  starttime = millis();
  dht.begin(); 
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) 
  {
   Serial.println("Starting LoRa failed!");
   delay(100);
   while (1);
  }
  delay(1000);
  Serial.println("AT");
  if (pressure.begin())
  Serial.println("BMP180 init success");
  else
  {
    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // Pause forever.
  }
  // delay(1000);
  
}

float calculateConcentration(long lowpulseInMicroSeconds, long durationinSeconds){
  
  float ratio = (lowpulseInMicroSeconds/1000000.0)/30.0*100.0; //Calculate the ratio
  float concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;
//  Serial.print("lowpulseoccupancy:");
//  Serial.print(lowpulseInMicroSeconds);
//  Serial.print("    ratio:");
//  Serial.print(ratio);
//  Serial.print("    Concentration:");
//  Serial.println(concentration);
  return concentration;
} 

void loop() 
{
 h = dht.readHumidity();
 t = dht.readTemperature();
 if (isnan(h) || isnan(t)) 
 {
  Serial.println("Fail to read from DHT sensor!");
  return;
 }

  durationPM1 = pulseIn(PM1PIN, LOW);
  durationPM25 = pulseIn(PM25PIN, LOW);
  
  lowpulseoccupancyPM1 = lowpulseoccupancyPM1 + durationPM1;
  lowpulseoccupancyPM25 = lowpulseoccupancyPM25 +  durationPM25;
  
  endtime = millis();
  if ((endtime-starttime) > sampletime_ms) //Only after 30s has passed we calcualte the ratio
  {
    conPM1 = calculateConcentration(lowpulseoccupancyPM1,30);
    conPM25 = calculateConcentration(lowpulseoccupancyPM25,30);
    Serial.print("PM1 ");
    Serial.print(conPM1);
    Serial.print("  PM25 ");
    Serial.println(conPM25);
    lowpulseoccupancyPM1 = 0;
    lowpulseoccupancyPM25 = 0;
    starttime = millis();
  } 
 
  baseline = getPressure();
 
 
 String data = "";
 data+= "{";
 data+= "\"anloga\":";
 data+= "\""+String(t)+"\",";
 data+= "\"anlogb\":";
 data+= "\""+String(h)+"\",";
 data+= "\"anlogc\":";
 data+= "\""+String(baseline)+"\",";
 data+= "\"anlogd\":";
 data+= "\""+String(conPM1)+"\",";
 data+= "\"anloge\":";
 data+= "\""+String(conPM25)+"\"";
 data+= "}";
 Serial.print('\r');
 Serial.print(data);
 delay(10);
 Serial.print('\n');
 LoRa.beginPacket();
 LoRa.print(data);
 LoRa.endPacket();
}

double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);
    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else{} //Serial.println("error retrieving pressure measurement\n");
      }
      else{} //Serial.println("error starting pressure measurement\n");
    }
    else{} //Serial.println("error retrieving temperature measurement\n");
  }
  else{}// Serial.println("error starting temperature measurement\n");
}
