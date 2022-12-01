/*
  kuneRF
  A small sketch to interface with a simple 4CH relay module, using
  433MHz radio communication, using RCSwitch library.
  https://github.com/sui77/rc-switch

  Ideal for RF light switching. Or anything, really.
  The Arduino will remember the last state when it was powered off, so next time
  you "turn the lights on", the previous state will be used.
  
  Yeah, it's pretty basic, but that's really all.
*/

#include <RCSwitch.h>
#include <EEPROM.h>

RCSwitch mySwitch = RCSwitch();

const unsigned long COMMANDS_ON[] =  {
  1064981,    // relay 1
  1052693,    // relay 2
  1069077,    // relay 3
  1066005     // relay 4
};
const unsigned long COMMANDS_OFF[] = {
  1064980,    // relay 1
  1052692,    // relay 2
  1069076,    // relay 3
  1066004     // relay 4
};

const unsigned int RELAY_PINS[] = {
  3,  // relay 1
  4,  // relay 2
  5,  // relay 3
  6   // relay 4
};

const unsigned int MEMORY_ADDR[] = {
  6,    // address of relay 1 state in EEPROM
  7,    // address of relay 2 state in EEPROM
  8,    // address of relay 3 state in EEPROM
  9     // address of relay 4 state in EEPROM
};

const int RELAY_ON = LOW;
const int RELAY_OFF = HIGH;

void relaySwitch(int _id, bool _state){
  Serial.println("Turning relay id=" + String(_id) + " " + (_state?"ON":"OFF"));
  digitalWrite(RELAY_PINS[_id], (_state?RELAY_ON:RELAY_OFF));
  EEPROM[MEMORY_ADDR[_id]] = (_state?1:0);
}

bool checkEEPROM(){
  if(EEPROM[0] == 'k' && 
     EEPROM[1] == 'u' && 
     EEPROM[2] == 'n' && 
     EEPROM[3] == 'e' && 
     EEPROM[4] == 'r' && 
     EEPROM[5] == 'f'){
       return true;
  } else {
    return false;
  }
}

bool formatEEPROM(){
  Serial.println("Preparing to format EEPROM");
  // creating signature
  EEPROM[0] = 'k';
  EEPROM[1] = 'u';
  EEPROM[2] = 'n';
  EEPROM[3] = 'e';
  EEPROM[4] = 'r';
  EEPROM[5] = 'f';
  // initialize every relay disabled
  for(int i=0; i<4; i++){
    EEPROM[MEMORY_ADDR[i]] = 0;
  }
  // quick check
  return checkEEPROM();
}

void setup() {
  Serial.begin(9600);
  
  if(!checkEEPROM()){
    bool result = formatEEPROM();
    Serial.println((result?"EEPROM formatted succesfully.":"EEPROM error. Couldn't format properly."));
  } else {
    Serial.println("EEPROM OK!");
  }
  
  // visszaállítjuk a mentett állást
  for(int i=0; i<4; i++){
    relaySwitch(i, (EEPROM[MEMORY_ADDR[i]]==1));
    pinMode(RELAY_PINS[i], OUTPUT);
  }

  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2

}

void loop() {
  if (mySwitch.available()) {
    // először nézzük, h. ki kell e kapcsolni valamit
    for(int i=0; i<4; i++){
      if(mySwitch.getReceivedValue() == COMMANDS_OFF[i]){
        relaySwitch(i, false);
        break;
      }
    }

    // aztán, hogy be kell e kapcsolni valamit
    for(int i=0; i<4; i++){
      if(mySwitch.getReceivedValue() == COMMANDS_ON[i]){
        relaySwitch(i, true);
        break;
      }
    }

    mySwitch.resetAvailable();
  }
}
