
# kuneRF
A really basic RF 433MHz switch for a 4 channel relay module.

This will save and restore states from EEPROM when powercycling the Arduino. (So, like turning lights on or off.)

| PIN | function |
|--|--|
| D2 | RF receiver input |
| D3 | Relay #1 IN |
| D4 | Relay #2 IN |
| D5 | Relay #3 IN |
| D6 | Relay #4 IN |

Remote values are hard-coded into the code, you may need to change those.

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

To get codes, use the RCSwitch library's ReceiveDemo sketch on your Arduino.
https://github.com/sui77/rc-switch/tree/master/examples/ReceiveDemo_Advanced
