void setup() {
// Open serial communications and wait for port to open:
Serial.begin(115200);
while (!Serial) {
; // wait for serial port to connect. Needed for native USB port only
}
}

void loop() { // run over and over
float h;
float t;

  if (Serial.available()) {
  h=Serial.read();
  Serial.print("Humidity is: ");
  Serial.println(h);
  delay(1000);
  t=Serial.read();
  Serial.print("Temperature is: ");
  Serial.println(t);
  delay(1000);
  
  }
}