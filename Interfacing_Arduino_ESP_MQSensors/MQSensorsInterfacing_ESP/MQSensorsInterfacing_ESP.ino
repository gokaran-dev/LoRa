float AQI=0;

void setup() {
// Open serial communications and wait for port to open:
Serial.begin(115200);

while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

void loop() { // run over and over

  if (Serial.available()) {
   AQI=Serial.read();
  }

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
  delay(1000);
}