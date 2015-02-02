# esp8266_mcp23017

WORK IN PROGRESS

I²C Port expander driver for esp8266.

~~So far the driver can only write to the MCP23017~~ fixed that. (using two pins of your own choice)

```
#include "mcp23017/mcp23017.h"
....
// setup
uint8_t scl_pin=0;
uint8_t sda_pin=2;
mcp23017_init(scl_pin,sda_pin);
uint8_t adr=0;
mcp23017_begin(adr);
mcp23017_writeRegister(MCP23017_IODIRA, 0x00); // set all as output
mcp23017_writeRegister(MCP23017_IODIRB, 0x00); // set all as output
....

// loop

mcp23017_writeRegister(MCP23017_OLATA, 0xFF);  // set all bits on
mcp23017_writeRegister(MCP23017_OLATB, 0xFF);  // set all bits on
delay
mcp23017_writeRegister(MCP23017_OLATA, 0x00);  // set all bits off
mcp23017_writeRegister(MCP23017_OLATB, 0x00);  // set all bits off
  ```
  
The I²C driver is based on [zarya's I²C driver](https://github.com/zarya/esp8266_i2c_driver).

While the rest of the MCP23017 driver is influenced by [Adafruit-MCP23017-Arduino-Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).

###Required:

esp_iot_sdk_v0.9.4_14_12_19 ( v0.9.5 breaks everything ) 
