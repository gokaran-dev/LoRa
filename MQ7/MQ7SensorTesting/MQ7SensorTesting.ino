  #include <MQUnifiedsensor.h>

  #define placa "Arduino UNO"
  #define Voltage_Resolution 5
  #define pin1 A1
  #define type1 "MQ-7"
  #define ADC_Bit_Resolution 10
  #define RatioMQ7CleanAir 27.5


  MQUnifiedsensor MQ7(placa, Voltage_Resolution, ADC_Bit_Resolution, pin1, type1);
  unsigned long oldTime=0;
  float MQ7Data;

void setup() {
  Serial.begin(9600);
  pinMode(PWMPin1, OUTPUT);
  MQ7.setRegressionMethod(1);
  MQ7.setA(99.042); MQ7.setB(-1.518);
  MQ7.init();

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ7.calibrate(RatioMQ7CleanAir);
    Serial.print(".");
  }
  MQ7.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ7.serialDebug(true);

  Serial.println("CO sensor requires a warm up period, pls wait");
  for(int i=10; i>=0; i--)
  {
    Serial.print("Warming up the sensor ");
    Serial.print(i);
    Serial.println(" seconds.");
    delay(1000);
  }

}

void loop() {
  
  while(millis() - oldTime <= (60*1000))
  {
    // VH 5 Volts
    analogWrite(5, 255); // 255 is DC 5V output
    MQ7.update(); // Update data, the arduino will read the voltage from the analog pin
    MQ7Data=MQ7.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
    Serial.println(MQ7Data);
    delay(500); //Sampling frequency
  }

  

}
