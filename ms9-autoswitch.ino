#include <FreqCount.h>

#define FREQUENCY_15KHZ 60*262    // 15720Hz
#define FREQUENCY_24KHZ 60*416    // 24960Hz
#define FREQUENCY_31KHZ 60*525    // 31500Hz

#define THRESHOLD_1 (FREQUENCY_15KHZ+FREQUENCY_24KHZ)/2    // 20340Hz
#define THRESHOLD_2 (FREQUENCY_24KHZ+FREQUENCY_31KHZ)/2    // 28230Hz

#define PIN_CSYNC_INPUT    12     // Reserved by FreqCount lib
#define PIN_SWITCH_RELAYS  15
#define PIN_RESISTOR_RELAY 11

unsigned int currentFreq = FREQUENCY_15KHZ;
bool switchRelays = HIGH;
bool resistorRelay = HIGH;
bool updateOutputs = false;

void setup() {
  pinMode(PIN_SWITCH_RELAYS, OUTPUT);
  digitalWrite(PIN_SWITCH_RELAYS,switchRelays);
  pinMode(PIN_RESISTOR_RELAY, OUTPUT);
  digitalWrite(PIN_RESISTOR_RELAY,resistorRelay);
  Serial.begin(57600);
  FreqCount.begin(1000);
}

void loop() {
  if (FreqCount.available()) {
    unsigned int count = FreqCount.read();

    if(count<THRESHOLD_1){
      if(currentFreq!=FREQUENCY_15KHZ){
        currentFreq=FREQUENCY_15KHZ;
        switchRelays=HIGH;
        resistorRelay=HIGH;
        updateOutputs=true;
      }
    }else if(count<THRESHOLD_2){
      if(currentFreq!=FREQUENCY_24KHZ){
        currentFreq=FREQUENCY_24KHZ;
        switchRelays=LOW;
        resistorRelay=HIGH;
        updateOutputs=true;
      }
    }else{
      if(currentFreq!=FREQUENCY_31KHZ){
        currentFreq=FREQUENCY_31KHZ;
        switchRelays=LOW;
        resistorRelay=LOW;
        updateOutputs=true;
      }
    }

    if(updateOutputs){
      digitalWrite(PIN_SWITCH_RELAYS,switchRelays);
      digitalWrite(PIN_RESISTOR_RELAY,resistorRelay);
      updateOutputs = false;
    }
    
    Serial.print("Input=");
    Serial.print(count);
    Serial.print("Hz -> Setting=");
    Serial.print(currentFreq);
    Serial.print("Hz");
    Serial.println("");
  }
}
