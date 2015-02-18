# esp8266_mcp23017_example

I²C Port expander [driver](https://github.com/eadf/esp8266_mcp23017) example for esp8266. It connects to the mcp2307 with two GPIO pins of your own choice.

The API is very arduino:like, here is the mandatory blinky example:
```
#include "mcp23017/mcp23017.h"
....
// setup
MCP23017_Self mcpSelf;  // in lieu of C++ member data we have structs :)
uint8_t scl_pin=0;
uint8_t sda_pin=2;
uint8_t deviceAddr=0;
uint8_t aMcpPin = 2; // the pin on the mcp23017 we want to write to

mcp23017_init(&mcpSelf, scl_pin, sda_pin);
mcp23017_pinMode(&mcpSelf, deviceAddr, aMcpPin, MCP23017_OUTPUT);

....

// loop
mcp23017_digitalWrite(&mcpSelf, deviceAddr, aMcpPin, 0);
delay
mcp23017_digitalWrite(&mcpSelf, deviceAddr, aMcpPin, 1);
delay
  ```
  
You can access the bits one by one like above, by whole bytes or all the bits at once (uint16_t).

The I²C driver is based on [zarya's I²C driver](https://github.com/zarya/esp8266_i2c_driver).

While the rest of the MCP23017 driver is a heavily modified port of [Adafruit-MCP23017-Arduino-Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).

### Todo

* interrupt support. I don't need this so it won't happend unless someone requests it.


### Basic test
I've added a very basic, basic test into the demo.
To use this test you should connect each output pin of the
mcp23017 to corresponding pin on the 'other side' via a 10+K resistor.

 left side pin | via | right side pin
 --------------|-----|---------------
  (bit8)  GPBO |10KOhm | GPA7  (bit7)
  (bit9)  GPB1 |10KOhm | GPA6  (bit6)
  (bit10) GPB2 | 10KOhm | GPA5  (bit5)
  (bit11) GPB3 | 10KOhm | GPA4  (bit4)
  (bit12) GPB4 |10KOhm | GPA3  (bit3)
  (bit13) GPB5 | 10KOhm | GPA2  (bit2)
  (bit14) GPB6 | 10KOhm | GPA1  (bit1)
  (bit15) GPB7 | 10KOhm | GPA0  (bit0)
 
 Hopefully you should see the text ```all tests passed``` in the console.
 
# License 

GNU GENERAL PUBLIC LICENSE Version 3

The makefile is copied from [esp_mqtt.](https://github.com/tuanpmt/esp_mqtt)

###Building and installing:

First you need to install the sdk and the easy way of doing that is to use [esp_open_sdk.](https://github.com/pfalcon/esp-open-sdk)

You can put that anywhere you like (/opt/local/esp-open-sdk, /esptools etc etc)

Then you could create a small ```setenv.sh``` file, containing the location of your newly compiled sdk and other platform specific info;
```
export SDK_BASE=/opt/local/esp-open-sdk/sdk
export PATH=${SDK_BASE}/../xtensa-lx106-elf/bin:${PATH}
export ESPPORT=/dev/ttyO0  
```
(or setup your IDE to do the same)

To make a clean build, flash and connect to the esp console you just do this in a shell:
```
source setenv.sh # This is only needed once per session
make clean && make test
```

You won't be needing esptool, the makefile only uses esptool.py (provided by [esp_open_sdk](https://github.com/pfalcon/esp-open-sdk))

I have tested this with sdk v0.9.5 and v0.9.4 (linux & mac)
