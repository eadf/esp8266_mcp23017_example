/*
 * mcp23017_basictest.c
 *
 *  Created on: Feb 5, 2015
 *      Author: eadf
 *
 * To use this basic tests you should connect each output pin of the
 * mcp23017 to corresponding pin on the 'other side' via a 10+K resistor.
 *
 *  (bit8)  GPBO <- 10KOhm -> GPA7  (bit7)
 *  (bit9)  GPB1 <- 10KOhm -> GPA6  (bit6)
 *  (bit10) GPB2 <- 10KOhm -> GPA5  (bit5)
 *  (bit11) GPB3 <- 10KOhm -> GPA4  (bit4)
 *  (bit12) GPB4 <- 10KOhm -> GPA3  (bit3)
 *  (bit13) GPB5 <- 10KOhm -> GPA2  (bit2)
 *  (bit14) GPB6 <- 10KOhm -> GPA1  (bit1)
 *  (bit15) GPB7 <- 10KOhm -> GPA0  (bit0)
 */

#include "mcp23017_basictest.h"
#include "c_types.h"


uint8_t correspondingBit(uint8_t bit) {
  if(bit < 16) {
    return 8-(bit-7);
  } else {
    os_printf("bit %d does not exist\n", bit);
    return 255;
  }
}


bool ICACHE_FLASH_ATTR
mcp23017_basictest(MCP23017_Self * mcpSelf) {
  uint8_t i=0;
  uint8_t j=0;
  uint8_t d = 0;
  uint8_t rv = 0;
  bool testFail = false;

  uint8_t deviceAddr = 0;

  if (false) for (i=0; i<16; i++){
    os_printf("bit %d corresponds to %d\n", i, correspondingBit(i));
  }

  for (d=0;d<2;d++) {
    for (i=0; i<16; i++){
      j = correspondingBit(i);
      // test single bit access
      mcp23017_pinMode(mcpSelf, deviceAddr, i, MCP23017_OUTPUT);
      mcp23017_digitalWrite(mcpSelf, deviceAddr, i, d);
      mcp23017_pinMode(mcpSelf, deviceAddr, j, MCP23017_INPUT);
      mcp23017_digitalRead(mcpSelf, deviceAddr, j, &rv);
      if ( rv != d) {
        os_printf("bit %d should be %d, but it was %d\n", j, d, rv);
        testFail = true;
      }
    }
  }

  for (d=0;d<2;d++) {
    for (i=0; i<16; i++){
      j = correspondingBit(i);

      // test whole AB block access
      mcp23017_pinModeAB(mcpSelf, deviceAddr, MCP23017_OUTPUT);
      mcp23017_digitalWriteAB(mcpSelf, deviceAddr, d<<i);

      mcp23017_pinMode(mcpSelf, deviceAddr, j, MCP23017_INPUT);
      mcp23017_digitalRead(mcpSelf, deviceAddr, j, &rv);
      if ( rv != d) {
        os_printf("uint16: bit %d should be %d, but it was %d\n", j, d, rv);
        testFail = true;
      }
    }
  }

  for (d=0;d<2;d++) {
    for (i=0; i<16; i++){
      j = correspondingBit(i);
      // test whole byte block access
      int ii = i>7?i-8:i;
      int jj = j>7?j-8:j;
      if (i>7) {
        mcp23017_pinModeA(mcpSelf, deviceAddr, MCP23017_OUTPUT);
        mcp23017_digitalWriteA(mcpSelf, deviceAddr, d<<ii);

        mcp23017_pinModeB(mcpSelf, deviceAddr, MCP23017_INPUT);
        mcp23017_digitalReadB(mcpSelf, deviceAddr, &rv);
        if ( rv != d<<jj) {
          os_printf("uint8: byte B should be %d, but it was %d i=%d, ii=%d, j=%d, jj=%d \n", d<<jj, rv, i, ii, j, jj);
          testFail = true;
        }
      } else {
        mcp23017_pinModeB(mcpSelf, deviceAddr, MCP23017_OUTPUT);
        mcp23017_digitalWriteB(mcpSelf, deviceAddr, d<<jj);

        mcp23017_pinModeA(mcpSelf, deviceAddr, MCP23017_INPUT);
        mcp23017_digitalReadA(mcpSelf, deviceAddr, &rv);
        if ( rv != d<<ii) {
          os_printf("uint8: byte A should be %d, but it was %d i=%d, ii=%d, j=%d, jj=%d \n", d<<ii, rv, i, ii, j, jj);
          testFail = true;
        }
      }
    }
  }

  mcp23017_pinModeA(mcpSelf, deviceAddr, MCP23017_OUTPUT);
  mcp23017_digitalWriteA(mcpSelf, deviceAddr, 0x8d);

  mcp23017_pinModeB(mcpSelf, deviceAddr, MCP23017_INPUT);
  mcp23017_digitalReadB(mcpSelf, deviceAddr, &rv);
  if ( rv != 0xb1) {
    os_printf("uint8: byte B should be %d, but it was %d\n", 0xb1, rv);
    testFail = true;
  }

  mcp23017_pinModeB(mcpSelf, deviceAddr, MCP23017_OUTPUT);
  mcp23017_digitalWriteB(mcpSelf, deviceAddr, 0x8d);

  mcp23017_pinModeA(mcpSelf, deviceAddr, MCP23017_INPUT);
  mcp23017_digitalReadA(mcpSelf, deviceAddr, &rv);
  if ( rv != 0xb1) {
    os_printf("uint8: byte A should be %d, but it was %d\n", 0xb1, rv);
    testFail = true;
  }


  if (!testFail) {
    os_printf("All tests passed \n");
  }

  return testFail;
}
