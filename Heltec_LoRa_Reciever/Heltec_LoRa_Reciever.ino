
// Turns the 'PRG' button into the power button, long press is off 
#define HELTEC_POWER_BUTTON   // must be before "#include <heltec_unofficial.h>"
#include <heltec_unofficial.h>


#define PAUSE               5000
#define FREQUENCY           470.0       
// #define FREQUENCY           905.2       // for US
#define BANDWIDTH           125.0
#define SPREADING_FACTOR    7
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
  radio.setCRC(1);
  radio.invertIQ(0);
  // Start receiving
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
}

void loop() {
  heltec_loop();

  if (rxFlag) {
    rxFlag = false;
    radio.readData(rxdata);
    if (_radiolib_status == RADIOLIB_ERR_NONE) {
      both.printf("RX [%s]\n",  rxdata.c_str());
      both.printf("  RSSI: %.2f dBm\n", radio.getRSSI());
      both.printf("  SNR: %.2f dB\n", radio.getSNR());
    }
    RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
  }
 
}


