# esp8266_mcp23017

WORK IN PROGRESS

I²C Port expander driver for esp8266.

So far the driver can only write to the MCP23017 (using two pins of your own choice)

```
#include "mcp23017/mcp23017.h"
....
// setup
mcp23017_begin(0);
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

While the rest of the MCP23017 driver is influenced from [Adafruit-MCP23017-Arduino-Library](https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library).
