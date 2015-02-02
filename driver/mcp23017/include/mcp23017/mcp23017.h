/*
* mcp23017.h
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

#ifndef DRIVER_MCP23017_INCLUDE_MCP23017_MCP23017_H_
#define DRIVER_MCP23017_INCLUDE_MCP23017_MCP23017_H_

#include "c_types.h"

#define MCP23017_OLATA 0x14
#define MCP23017_OLATB 0x15

#define MCP23017_IODIRA 0x00
#define MCP23017_IODIRB 0x01

typedef enum {
  MCP23017_INPUT=0,
  MCP23017_OUTPUT=1
} MCP23017_PinMode;

/**
 * initiates the device. Sets the SCL and SDA pins
 */
bool mcp23017_init(uint8_t scl_pin, uint8_t sda_pin);

/**
 * Initializes the MCP23017 given its HW selected address, see datasheet for Address selection.
 */
void mcp23017_begin(uint8_t addr);

/**
 * Sets the pin mode to either MCP23017_INPUT or MCP23017_OUTPUT
 */
void mcp23017_pinMode(uint8_t pin, MCP23017_PinMode pinmode);

/**
 * Sets the pin mode to either MCP23017_INPUT or MCP23017_OUTPUT
 */
void mcp23017_digitalWrite(uint8_t pin, bool data);



void mcp23017_writeRegister(uint8_t regAddr, uint8_t regValue);
uint8_t mcp23017_readRegister(uint8_t regAddr);

#endif /* DRIVER_MCP23017_INCLUDE_MCP23017_MCP23017_H_ */
