# esp8266_mcp23017

*WORK IN PROGRESS*

I²C Port expander driver for esp8266. It connects to the mcp2307 with two GPIO pins of your own choice.

The API is very arduino:like, here is the mandatory blinky example:
```
#include "mcp23017/mcp23017.h"
....
// setup
MCP23017_Self mcpSelf;  // in lieu of C++ member data we have structs :)
uint8_t scl_pin=0;
uint8_t sda_pin=2;
uint8_t deviceAddr=0;
uint8_t aMcpPin = 2;

mcp23017_init(&mcpSelf, scl_pin, sda_pin);
mcp23017_pinMode(&mcpSelf, deviceAddr, aMcpPin, MCP23017_OUTPUT);

....

// loop
mcp23017_digitalWrite(&mcpSelf, deviceAddr, aMcpPin, 0);
delay
mcp23017_digitalWrite(&mcpSelf, deviceAddr, aMcpPin, 1);
delay
  ```
  
The I²C driver is based on [zarya's I²C driver](https://github.com/zarya/esp8266_i2c_driver).

While the rest of the MCP23017 driver is a heavily modified port of [Adafruit-MCP23017-Arduino-Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).

###Required:

esp_iot_sdk_v0.9.4_14_12_19

I have not tested this with v0.9.5. I tested a [clean sdk 0.9.5 install](https://github.com/pfalcon/esp-open-sdk) with one of the basic examples ([blinky](https://github.com/esp8266/source-code-examples)). It compiled and uploaded fine but the esp had a infinite crash loop with some message about "MEM CHK FAIL" on the console. So i threw the whole sdk out (aint nobody got time fo dat). I will try upgrading the sdk again once [mqtt](https://github.com/tuanpmt/esp_mqtt) upgrades to 0.9.5+.
