/**
 * Send and receive LoRa-modulation packets with a sequence number, showing RSSI
 * and SNR for received packets on the little display.
 *
 * Note that while this send and received using LoRa modulation, it does not do
 * LoRaWAN. For that, see the LoRaWAN_TTN example.
 *
 * This works on the stick, but the output on the screen gets cut off.
*/



// Turns the 'PRG' button into the power button, long press is off 
#define HELTEC_POWER_BUTTON   // must be before "#include <heltec_unofficial.h>"
#include <heltec_unofficial.h>
// Pause between transmited packets in seconds.
// Set to zero to only transmit a packet when pressing the user button
// Will not exceed 1% duty cycle, even if you set a lower value.
#define PAUSE               5000

// Frequency in MHz. Keep the decimal point to designate float.
// Check your own rules and regulations to see what is legal where you are.
#define FREQUENCY           433.0       // for Europe
// #define FREQUENCY           905.2       // for US

// LoRa bandwidth. Keep the decimal point to designate float.
// Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz.
#define BANDWIDTH           125.0

// Number from 5 to 12. Higher means slower but higher "processor gain",
// meaning (in nutshell) longer range and more robust against interference. 
#define SPREADING_FACTOR    7

// Transmit power in dBm. 0 dBm = 1 mW, enough for tabletop-testing. This value can be
// set anywhere between -9 dBm (0.125 mW) to 22 dBm (158 mW). Note that the maximum ERP
// (which is what your antenna maximally radiates) on the EU ISM band is 25 mW, and that
// transmissting without an antenna can damage your hardware.
#define TRANSMIT_POWER      2

#define LORA_PREAMBLE_LENGTH  8
#define CODING_RATE 5
#define SYNC_WORD 0x12

String rxdata;
volatile bool rxFlag = false;
long counter = 0;
uint64_t last_tx = 0;
uint64_t tx_time;
uint64_t minimum_pause;



// Can't do Serial or display things here, takes too much time for the interrupt
void rx() {
  rxFlag = true;
}



struct Data {
  int seq;
  float y;
} data;


void setup() {
  heltec_setup();
  both.println("Radio init");
  RADIOLIB_OR_HALT(radio.begin());
  // Set the callback function for received packets
  radio.setDio1Action(rx);
  // Set radio parameters
  both.printf("Frequency: %.2f MHz\n", FREQUENCY);
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  both.printf("Bandwidth: %.1f kHz\n", BANDWIDTH);
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  both.printf("Spreading Factor: %i\n", SPREADING_FACTOR);
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  both.printf("TX power: %i dBm\n", TRANSMIT_POWER);
  RADIOLIB_OR_HALT(radio.setOutputPower(TRANSMIT_POWER));
  both.printf("Preamble Length: %i \n", LORA_PREAMBLE_LENGTH);
  RADIOLIB_OR_HALT(radio.setPreambleLength(LORA_PREAMBLE_LENGTH));
  both.printf("Coding Rate: %i \n", CODING_RATE);
  RADIOLIB_OR_HALT(radio.setCodingRate(CODING_RATE));
  both.printf("Sync Word: %i \n", SYNC_WORD);
  RADIOLIB_OR_HALT(radio.setSyncWord(SYNC_WORD));
  // Start receiving
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
}

void loop() {
  heltec_loop();

  static int counter = 0, errors = 0, s = 0;
  int packetSize=radio.getPacketLength();
  if(packetSize)
  {
    //recieved packet
    Serial.print("\nRecieved packet size ");
    Serial.println(packetSize);
    Serial.println("data");
    radio.readData((byte *)&data, packetSize);

    for (int i = 0; i < packetSize; i++) {
      /*((byte *) &data)[i] = radio.readData();*/
      Serial.print(' ');
      Serial.print(((byte *)&data)[i]);
  }
  }

  // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(radio.getRSSI());
    Serial.print("' with SNR ");
    Serial.println(radio.getSNR());
    Serial.print("seq = ");
    Serial.print(data.seq);
    Serial.print(" y = ");
    Serial.print(data.y);
    Serial.print(" received ");
    /*Serial.print(++counter);
    Serial.print(" errors ");
    Serial.println(errors);
    if (seq != data.seq) {
      Serial.print("sequence number error expected ");
      Serial.println(seq);
      seq = data.seq;
      errors++;*/
    
    
 

  
}

  

