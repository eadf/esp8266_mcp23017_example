/*
* mcp23017.c
*
* Copyright (c) 2015, eadf (https://github.com/eadf)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
* * Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
* * Neither the name of Redis nor the names of its contributors may be used
* to endorse or promote products derived from this software without
* specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "mcp23017/mcp23017.h"
#include "i2c/i2c.h"

#define MCP23017_ADDRESS 0x20

// registers

#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12

#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13

#define MCP23017_INT_ERR 255


static uint8_t i2caddr = 0;  // global variable for now

// forward declarations
static void mcp23017_beginTransmission(uint8_t address);
static void mcp23017_endTransmission(void);
static uint8_t mcp23017_bitForPin(uint8_t pin);
static uint8_t mcp23017_regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);
static void mcp23017_updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);
static uint8_t mcp23017_bitWrite(uint8_t regValue,uint8_t bit,uint8_t pValue);

/**
 * initiates the device. Sets the SCL and SDA pins
 */
bool ICACHE_FLASH_ATTR
mcp23017_init(uint8_t scl_pin, uint8_t sda_pin) {
  i2c_init(scl_pin, sda_pin);
  return true;
}

/**
 * Initializes the MCP23017 given its HW selected address, see datasheet for Address selection.
 */
void ICACHE_FLASH_ATTR
mcp23017_begin(uint8_t addr) {
  if (addr > 7) {
    addr = 7;
  }
  i2caddr = addr;

  // set defaults!
  // all inputs on port A and B
  mcp23017_writeRegister(MCP23017_IODIRA,0xff);
  mcp23017_writeRegister(MCP23017_IODIRB,0xff);
}

void ICACHE_FLASH_ATTR
mcp23017_digitalWrite(uint8_t pin, bool data) {
  uint8_t gpio;
  uint8_t bit=mcp23017_bitForPin(pin);


  // read the current GPIO output latches
  uint8_t regAddr=mcp23017_regForPin(pin,MCP23017_OLATA,MCP23017_OLATB);
  gpio = mcp23017_readRegister(regAddr);
  // set the pin and direction
  gpio = mcp23017_bitWrite(gpio,bit,data);

  // write the new GPIO
  regAddr=mcp23017_regForPin(pin,MCP23017_GPIOA,MCP23017_GPIOB);
  mcp23017_writeRegister(regAddr,gpio);
}

/**
 * Sets the pin mode to either MCP23017_INPUT or MCP23017_OUTPUT
 */
void ICACHE_FLASH_ATTR
mcp23017_pinMode(uint8_t pin, MCP23017_PinMode pinmode) {
  mcp23017_updateRegisterBit(pin,(pinmode==MCP23017_INPUT),MCP23017_IODIRA,MCP23017_IODIRB);
}

/**
 * Reads a given register
 */
uint8_t ICACHE_FLASH_ATTR
mcp23017_readRegister(uint8_t regAddr) {
  // read the current GPINTEN
  mcp23017_beginTransmission(MCP23017_ADDRESS | i2caddr);
  i2c_writeByteCheckAck(regAddr);
  mcp23017_endTransmission();
  //Wire.requestFrom(MCP23017_ADDRESS | i2caddr, 1);
  i2c_start();
  return i2c_readByte();
}

/**
 * Writes a given register
 */
void ICACHE_FLASH_ATTR
mcp23017_writeRegister(uint8_t regAddr, uint8_t regValue) {
  // Write the register
  mcp23017_beginTransmission(MCP23017_ADDRESS | i2caddr);
  i2c_writeByteCheckAck(regAddr);
  i2c_writeByteCheckAck(regValue);
  mcp23017_endTransmission();
}

/**
 * Register address, port dependent, for a given PIN
 */
static uint8_t ICACHE_FLASH_ATTR
mcp23017_regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr) {
  return(pin<8) ?portAaddr:portBaddr;
}

/**
 * Bit number associated to a give Pin
 */
static uint8_t ICACHE_FLASH_ATTR
mcp23017_bitForPin(uint8_t pin) {
  return pin%8;
}

static void ICACHE_FLASH_ATTR
mcp23017_beginTransmission(uint8_t address) {
  os_printf("mcp23017_beginTransmission(%d)\n", address);
  i2c_start();
  i2c_writeByteCheckAck(address<<1); // why shift one??????
}

static void ICACHE_FLASH_ATTR
mcp23017_endTransmission(void) {
  i2c_stop();
}

/**
 * Helper to update a single bit of an A/B register.
 * - Reads the current register value
 * - Writes the new register value
 */
static void ICACHE_FLASH_ATTR
mcp23017_updateRegisterBit(uint8_t pin, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr) {
  uint8_t regValue;
  uint8_t regAddr=mcp23017_regForPin(pin,portAaddr,portBaddr);
  uint8_t bit=mcp23017_bitForPin(pin);
  regValue = mcp23017_readRegister(regAddr);

  // set the value for the particular bit
  regValue = mcp23017_bitWrite(regValue,bit,pValue);

  mcp23017_writeRegister(regAddr,regValue);
}

static uint8_t ICACHE_FLASH_ATTR
mcp23017_bitWrite(uint8_t regValue,uint8_t bit,uint8_t pValue) {
  pValue = pValue&1;
  if (pValue) {
    return regValue | pValue << bit;
  } else {
    return regValue & ~(pValue << bit);
  }
}

