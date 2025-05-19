#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ss 5
#define rst 14
#define dio0 2
String LoRaData;
#define LED 4
String inString = "";  // string to hold input
String data_read = "";
int val = 0;
////////////////////////////////////
#include <Arduino_JSON.h>
String inputData = "";
boolean data_complete = false;
String vala;
String valb;
String valc;
String vald;
String vale;
void setup() {
  Serial.begin(9600);
  inputData.reserve(200);
  pinMode(LED, OUTPUT);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  display.println("LoRa Receiver");
  display.display();

  while (!Serial)
    ;
  Serial.println("LoRa Receiver");

  LoRa.setPins(ss, rst, dio0);  //setup LoRa transceiver module

  while (!LoRa.begin(433E6))  //433E6 - Asia, 866E6 - Europe, 915E6 - North America
  {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xA5);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  int packetSize = LoRa.parsePacket();  // try to parse packet
  if (packetSize) {
    Serial.println("Received packet");
    while (LoRa.available())  // read packet
    {
      digitalWrite(LED, HIGH);
      int inChar = LoRa.read();
      inString += (char)inChar;
    }
    demoParse();
    //    lcd.clear();
    //data_read=inString;
    inString = "";
    LoRa.packetRssi();

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(20, 0);
    display.println("LoRa Receiver");

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 5);
    display.println(LoRaData);
    display.print("TM: ");
    display.println(vala);
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 15);
    display.println(LoRaData);
    display.print("Hm: ");
    display.println(valb);
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 25);
    display.println(LoRaData);
    display.print("Am Pr: ");
    display.println(valc);
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 35);
    display.println(LoRaData);
    display.print("Pm 1.0: ");
    display.println(vald);
    display.display();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 45);
    display.println(LoRaData);
    display.print("Pm 2.5: ");
    display.println(vale);
    display.display();
  }
  Serial.println(data_read);
  digitalWrite(LED, LOW);
}

void demoParse() {
  Serial.println("parse");
  Serial.println(vala);
  Serial.println("=====");
  JSONVar myObject = JSON.parse(inString);
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    return;
  }
  Serial.print("JSON.typeof(myObject) = ");
  Serial.println(JSON.typeof(myObject));  // prints: object
  // myObject.hasOwnProperty(key) checks if the object contains an entry for key
  if (myObject.hasOwnProperty("anloga")) {
    Serial.print("myObject[\"anloga\"] = ");
    vala = (const char*)myObject["anloga"];  //to get value in  vala
    Serial.println(vala);
    Serial.println((const char*)myObject["anloga"]);
  }
  if (myObject.hasOwnProperty("anlogb")) {
    Serial.print("myObject[\"anlogb\"] = ");
    valb = (const char*)myObject["anlogb"];
    Serial.println(valb);
    Serial.println((const char*)myObject["anlogb"]);
  }
  if (myObject.hasOwnProperty("anlogc")) {
    Serial.print("myObject[\"anlogc\"] = ");
    valc = (const char*)myObject["anlogc"];
    Serial.println(valc);
    Serial.println((const char*)myObject["anlogc"]);
  }
  if (myObject.hasOwnProperty("anlogd")) {
    Serial.print("myObject[\"anlogd\"] = ");
    vald = (const char*)myObject["anlogd"];
    Serial.println(vald);
    Serial.println((const char*)myObject["anlogd"]);
  }
  if (myObject.hasOwnProperty("anloge")) {
    Serial.print("myObject[\"anloge\"] = ");
    vale = (const char*)myObject["anloge"];
    Serial.println(vale);
    Serial.println((const char*)myObject["anloge"]);
  }
  // JSON vars can be printed using print or println
  Serial.print("myObject = ");
  Serial.println(myObject);
  Serial.println();
}
