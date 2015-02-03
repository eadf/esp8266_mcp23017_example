
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "stdout/stdout.h"
#include "c_types.h"
#include "mcp23017/mcp23017.h"

#define user_procTaskPeriod      1000
static volatile os_timer_t loop_timer;
static uint8_t deviceAddr = 0;
static MCP23017_Self mcpSelf;

static void loop(void);
static void setup(void);

/**
 * This is the main user program loop
 */
static void ICACHE_FLASH_ATTR
loop(void) {
  static uint8_t i = 0;

  mcp23017_pinMode(&mcpSelf, deviceAddr, 0, MCP23017_OUTPUT);
  os_printf("mcp23017_pinMode(0, MCP23017_OUTPUT);\n");
  os_delay_us(10000);

  //mcp23017_writeRegister(MCP23017_IODIRB, i);
  //os_printf("mcp23017_writeRegister(MCP23017_IODIRB, %d);\n", i);
  //os_delay_us(500);

  //uint8_t reg = mcp23017_readRegister(MCP23017_IODIRB);
  //os_printf("mcp23017_readRegister(MCP23017_IODIRB, 0x00) == %d\n", reg);
  //os_delay_us(20000);

  //mcp23017_writeRegister(MCP23017_OLATA, 0xFF);  // set all bits on
  //os_delay_us(20000);

  //mcp23017_writeRegister(MCP23017_OLATA, 0x00);  // set all bits off
  //os_delay_us(20000);

  mcp23017_digitalWrite(&mcpSelf, deviceAddr, 0, i&1);
  os_printf("mcp23017_digitalWrite(deviceAddr, 0, %d);\n", i&1);
  os_delay_us(10000);
  i += 1;
}

/**
 * Setup program. When user_init runs the debug printouts will not always
 * show on the serial console. So i run the inits in here, 2 seconds later.
 */
static void ICACHE_FLASH_ATTR
setup(void) {
  // setup stuff
  mcp23017_init(&mcpSelf, 0, 2);
  os_printf("mcp23017_init(0,2);\n");

  // Start loop timer
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) loop, NULL);
  os_timer_arm(&loop_timer, 1000, 1);
}

void user_init(void)
{
  // Make os_printf working again. Baud:115200,n,8,1
  stdoutInit();
  //turn off wifi - it's not needed in this demo
  wifi_set_opmode( NULL_MODE );

  // Run setup() 3 seconds from now
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
  os_timer_arm(&loop_timer, 3000, 0);

  os_printf("\nSystem started ...\n");
}
