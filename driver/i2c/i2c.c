/*
 I2C driver for the ESP8266
 Copyright (C) 2014 Rudy Hardeman (zarya)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "i2c/i2c.h"
#include "easygpio/easygpio.h"

static uint8_t I2C_SDA_PIN = 2;
static uint8_t I2C_SCK_PIN = 14;

#define I2C_SLEEP_TIME 5
//#define I2C_SLEEP_TIME_DATA_SET 1
//#define I2C_SLEEP_TIME_DATA_WAIT 5 //4

#define i2c_read() GPIO_INPUT_GET(GPIO_ID_PIN(I2C_SDA_PIN));
#define i2c_disableOutput() GPIO_DIS_OUTPUT(GPIO_ID_PIN(I2C_SDA_PIN))

/**
 * Set SDA to state
 */
static void ICACHE_FLASH_ATTR
i2c_sda(uint8_t state) {
  state &= 0x01;
  //Set SDA line to state
  if (state)
    gpio_output_set(1 << I2C_SDA_PIN, 0, 1 << I2C_SDA_PIN, 0);
  else
    gpio_output_set(0, 1 << I2C_SDA_PIN, 1 << I2C_SDA_PIN, 0);
}

/**
 * Set SCK to state
 */
static void ICACHE_FLASH_ATTR
i2c_sck(uint8_t state) {
  //Set SCK line to state
  if (state)
    gpio_output_set(1 << I2C_SCK_PIN, 0, 1 << I2C_SCK_PIN, 0);
  else
    gpio_output_set(0, 1 << I2C_SCK_PIN, 1 << I2C_SCK_PIN, 0);
}

/**
 * I2C Start signal.
 * Data Transfer is initiated with a START bit (S) signaled by SDA being pulled low while SCL stays high.
 */
void ICACHE_FLASH_ATTR
i2c_start(void) {
  i2c_sda(1);
  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sda(0);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
}

/**
 * I2C Stop signal.
 * A STOP bit (P) is signaled when SDA is pulled high while SCL is high.
 */
void ICACHE_FLASH_ATTR
i2c_stop(void) {
  i2c_sda(0); // According to http://en.wikipedia.org/wiki/I%C2%B2C this must be set low
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sda(1);
  os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Send I2C ACK to the bus
 * uint8_t state 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
void ICACHE_FLASH_ATTR
i2c_send_ack(uint8_t state) {
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  //Set SDA
  //  HIGH for NACK
  //  LOW  for ACK
  i2c_sda((state ? 0 : 1));

  //Pulse the SCK
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);

  i2c_sda(1);
  os_delay_us(I2C_SLEEP_TIME);
}

/**
 * Receive I2C ACK from the bus
 * returns 1 or 0
 *  1 for ACK
 *  0 for NACK
 */
uint8_t ICACHE_FLASH_ATTR
i2c_check_ack(void) {
  uint8_t ack;
  i2c_sda(1);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);

  //Get SDA pin status
  ack = i2c_read();

  os_delay_us(I2C_SLEEP_TIME);
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  i2c_sda(0);
  os_delay_us(I2C_SLEEP_TIME);

  return (ack ? 0 : 1);
}

/**
 * Receive byte from the I2C bus 
 * returns the byte 
 *
uint8_t ICACHE_FLASH_ATTR
i2c_readByte(void) {
  uint8_t data = 0;
  uint8_t data_bit;
  uint8_t i;

  for (i = 0; i < 8; i++) {
    os_delay_us(I2C_SLEEP_TIME);
    if (i==0) i2c_disableOutput();
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);

    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);

    data_bit = i2c_read();
    os_delay_us(I2C_SLEEP_TIME);

    data_bit <<= (7 - i);
    data |= data_bit;
  }
  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);

  return data;
}
*/

/**
 * Receive byte from the I2C bus
 * returns the byte
 * returns the value of ACK
 */
bool ICACHE_FLASH_ATTR
i2c_readByteCheckAck(uint8_t *data) {
  *data = 0;
  uint8_t data_bit;
  uint8_t i;

  for (i = 0; i < 8; i++) {
    os_delay_us(I2C_SLEEP_TIME);
    if (i==0) i2c_disableOutput();
    i2c_sck(0);
    os_delay_us(I2C_SLEEP_TIME);

    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);

    data_bit = i2c_read();
    os_delay_us(I2C_SLEEP_TIME);

    data_bit <<= (7 - i);
    *data |= data_bit;
  }
  // allow slave to ACK or NACK
  i2c_disableOutput();
  os_delay_us(I2C_SLEEP_TIME);

  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);
  bool rv = i2c_read();

  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  return rv;
}

/**
 * Write byte to I2C bus
 * uint8_t data: to byte to be written
 * /
void ICACHE_FLASH_ATTR
i2c_writeByte(uint8_t data) {
  uint8_t data_bit;
  int8_t i;

  //os_delay_us(I2C_SLEEP_TIME_DATA_SET);

  for (i = 7; i >= 0; i--) {
    data_bit = data >> i;

    i2c_sda(data_bit);
    os_delay_us(I2C_SLEEP_TIME);

    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);

    //os_delay_us(I2C_SLEEP_TIME_DATA_SET);
  }
  os_delay_us(I2C_SLEEP_TIME);
  i2c_disableOutput();
}*/

/**
 * Write byte to I2C bus
 * uint8_t data: to byte to be written
 * returns the value of ACK
 */
bool ICACHE_FLASH_ATTR
i2c_writeByteCheckAck(uint8_t data) {
  uint8_t data_bit;
  int8_t i;
  bool rv = 0;

  for (i = 7; i >= 0; i--) {
    data_bit = data >> i;

    i2c_sda(data_bit);
    os_delay_us(I2C_SLEEP_TIME);

    i2c_sck(1);
    os_delay_us(I2C_SLEEP_TIME);
    i2c_sck(0);

  }
  // allow slave to ACK or NACK
  i2c_disableOutput();
  os_delay_us(I2C_SLEEP_TIME);

  i2c_sck(1);
  os_delay_us(I2C_SLEEP_TIME);
  rv = i2c_read();

  i2c_sck(0);
  os_delay_us(I2C_SLEEP_TIME);
  return rv;
}

/**
 * I2C init function
 * This sets up the GPIO pins
 * Returns false if something fails
 */
bool ICACHE_FLASH_ATTR
i2c_init(uint8_t scl_pin, uint8_t sda_pin) {
  I2C_SDA_PIN = sda_pin;
  I2C_SCK_PIN = scl_pin;

  if (I2C_SDA_PIN == I2C_SCK_PIN) {
    os_printf("i2c_init: Error: You must specify two unique pins for this to work\n");
    return false;
  }

  //Disable interrupts
  ETS_GPIO_INTR_DISABLE();

  if (!(easygpio_pinMode(I2C_SDA_PIN, EASYGPIO_NOPULL, EASYGPIO_OUTPUT) &&
        easygpio_pinMode(I2C_SCK_PIN, EASYGPIO_NOPULL, EASYGPIO_OUTPUT) )) {
    return false;
  }

  //Turn interrupt back on
  ETS_GPIO_INTR_ENABLE();

  i2c_sda(1);
  i2c_sck(1);
  return true;
}

