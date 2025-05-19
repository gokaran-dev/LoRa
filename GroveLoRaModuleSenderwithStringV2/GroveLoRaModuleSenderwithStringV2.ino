#include <RH_RF95.h>

#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(6, 7); // RX, TX
    #define COMSerial SSerial
    #define ShowSerial Serial

    RH_RF95<SoftwareSerial> rf95(COMSerial);
#endif
uint8_t a=0;
void setup() {
    ShowSerial.begin(115200);
    ShowSerial.println("RF95 client test.");

    if (!rf95.init()) {
        ShowSerial.println("init failed");
        while (1);
    }

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    //rf95.setTxPower(13, false);

    rf95.setFrequency(470.0);
    rf95.setTxPower(2);
    uint8_t a=1;
}

void loop() {
    ShowSerial.println("Sending to rf95_server");
    // Send a message to rf95_server
    char someString[15] = "battery: %i";
    char test[30];
    snprintf(test, 30, someString, a);
    ShowSerial.println(test);
    
    delay(10);
    a++;
    rf95.send((uint8_t *)test, 30);
    delay(5000);
}


