#include <MQUnifiedsensor.h>
#include <SoftwareSerial.h>

#define placa "Arduino UNO"
#define Voltage_Resolution 5
#define pin1 A1
#define pin2 A0
#define type1 "MQ-7"
#define type2 "MQ-4"
#define ADC_Bit_Resolution 10
#define RatioMQ7CleanAir 27.5
#define RatioMQ4CleanAir 4.4

SoftwareSerial espSerial(5,6);
MQUnifiedsensor MQ4(placa, Voltage_Resolution, ADC_Bit_Resolution, pin2, type2);
MQUnifiedsensor MQ7(placa, Voltage_Resolution, ADC_Bit_Resolution, pin1, type1);
unsigned long oldTime=0;
float MQ7Data;
float MQ4Data;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);
  //initializing MQ7 Sensor for Carbon Monoxide
  MQ7.setRegressionMethod(1);
  MQ7.setA(99.042); MQ7.setB(-1.518);
  MQ7.init();

  //Calibrating MQ7
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    Serial.print(".");
  }
  MQ7.setR0(calcR0/10);
  Serial.println(" MQ7 done!.");

  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ7 CAlibration ********************************************/ 
  MQ7.serialDebug(true);


  //initializing MQ4 Sensor for Methane
   MQ4.setRegressionMethod(1);
   MQ4.setA(1012.7); MQ4.setB(-2.786);
   MQ4.init();

  Serial.print("Calibrating please wait.");
  float calcR1 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ4.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR1 += MQ4.calibrate(RatioMQ4CleanAir);
    Serial.print(".");
  }
  MQ4.setR0(calcR1/10);
  Serial.println("  done!.");
  
  if(isinf(calcR1)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR1 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ4 CAlibration ********************************************/ 
  MQ4.serialDebug(true);



  Serial.println("CO and Methane sensor require a warm up period, pls wait");
  for(int i=10; i>=0; i--)
  {
    Serial.print("Warming up the sensor ");
    Serial.print(i);
    Serial.println(" seconds.");
    delay(1000);
  }

}

void loop() {
   //readings from MQ7 sensor
    MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
    MQ7Data=MQ7.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
    Serial.print("Concentration of CO: ");
    Serial.println(MQ7Data);
    delay(3000); //Sampling frequency
  
  //readings from MQ4 sensor
  MQ4.update();
  MQ4Data=MQ4.readSensor();
  Serial.print("Concentration of Methane:");
  Serial.println(MQ4Data);
  delay(3000);
  float AQI;
  float AQItest= ((MQ4Data+MQ7Data)/2);
  espSerial.println(AQItest);
  /*if(AQItest>=0 && AQItest<=1.0)
  Serial.println("AQI is between 0 and 50");

  if(AQItest>=1.1 && AQItest<=2.0)
  Serial.println("AQI is between 51 and 100");

  if(AQItest>=2.1 && AQItest<=10)
  Serial.println("AQI is between 101 and 200");

  if(AQItest>=10.1 && AQItest<=17)
  Serial.println("AQI is between 201 and 300");

  if(AQItest>=17.1 && AQItest<=34)
  Serial.println("AQI is between 301 and 400");

  if(AQItest>34)
  Serial.println("AQI is between 401 and 500");*/

  delay(1500);
}
