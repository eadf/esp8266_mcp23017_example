
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "user_interface.h"
#include "mem.h"
#include "stdout/stdout.h"
#include "c_types.h"
#include "mcp23017/mcp23017.h"
#include "mcp23017_basictest.h"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

#define user_procTaskPeriod      1000
static volatile os_timer_t loop_timer;
static uint8_t deviceAddr = 0;
static MCP23017_Self mcpSelf;
os_event_t user_procTaskQueue[user_procTaskQueueLen];

static void nop_procTask(os_event_t *events);
static void loop(void *timer_arg);
static void setup(void *timer_arg);

/**
 * This is the main user program loop
 */
static void ICACHE_FLASH_ATTR
loop(void *timer_arg) {
  static uint8_t i = 0;

  mcp23017_pinModeAB(&mcpSelf, deviceAddr, MCP23017_INPUT);
  mcp23017_pinModeAB(&mcpSelf, deviceAddr, MCP23017_OUTPUT);
  os_printf("mcp23017_pinMode(0, MCP23017_OUTPUT);\n");
  os_delay_us(10000);

  mcp23017_digitalWrite(&mcpSelf, deviceAddr, 0, i&1);
  os_printf("mcp23017_digitalWrite(deviceAddr, 0, %d);\n", i&1);
  bool sample = false;
  mcp23017_digitalRead(&mcpSelf, deviceAddr, 0, &sample);
  os_printf("mcp23017_digitalRead(deviceAddr, 0)==%d;\n", sample);
  os_delay_us(10000);
  i += 1;
}

/**
 * Setup program. When user_init runs the debug printouts will not always
 * show on the serial console. So i run the inits in here, 2 seconds later.
 */
static void ICACHE_FLASH_ATTR
setup(void *timer_arg) {
  // setup stuff
  mcp23017_init(&mcpSelf, 2, 0);
  os_printf("mcp23017_init(0,2);\n");

  mcp23017_basictest(&mcpSelf);

  // Start loop timer
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) loop, NULL);
  os_timer_arm(&loop_timer, 1000, 1);
}

//Do nothing function
static void ICACHE_FLASH_ATTR
nop_procTask(os_event_t *events) {
  os_delay_us(10);
}

void user_init(void) {

  // Make uart0 work with just the TX pin. Baud:115200,n,8,1
  // The RX pin is now free for GPIO use.
  stdout_init();

  // turn off WiFi for this console only demo
  wifi_station_set_auto_connect(false);
  wifi_station_disconnect();

  // Run setup() 3 seconds from now
  os_timer_disarm(&loop_timer);
  os_timer_setfn(&loop_timer, (os_timer_func_t *) setup, NULL);
  os_timer_arm(&loop_timer, 3000, false);

  //Start no-operation os task
  system_os_task(nop_procTask, user_procTaskPrio, user_procTaskQueue, user_procTaskQueueLen);
  system_os_post(user_procTaskPrio, 0, 0);

  os_printf("\nSystem started ...\n");
}
