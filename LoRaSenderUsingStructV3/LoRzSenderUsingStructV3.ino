#include <RH_RF95.h>

#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(6, 7); // RX, TX
    #define COMSerial SSerial
    #define ShowSerial Serial

    RH_RF95<SoftwareSerial> rf95(COMSerial);
#endif

struct Data {
  int seq;
  float y;
};

Data data={0 , 4.5};

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

    rf95.setFrequency(433.0);
    rf95.setTxPower(12);
    uint8_t a=1;
}

void loop() {
    ShowSerial.print("\nSending packet: ");
    ShowSerial.print("seq = ");
    ShowSerial.print(data.seq);
    ShowSerial.print(" y = ");
    ShowSerial.println(data.y);
  
    //sending data
    rf95.send((byte *)&data, sizeof(data));
     
     for (unsigned int i = 0; i < sizeof(Data);i++) {
    ShowSerial.print(' ');
    //LoRa.write(((byte *) &data)[i]);
    ShowSerial.print(((byte *) &data)[i]);
  }
    
  //data.seq += 1;
  //data.y += 10;

   
    
    delay(5000);
}


