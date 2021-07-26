#include <mbed.h>
#include <USBSerial.h>
#include <swo.h>
#include <shell.h>

#include <common/buzzer/buzzer.h>
#include <common/utils/version.h>

#include "engine/drivers.h"
#include "ir/ir.h"
// #include "infos.h"
#include "com/com.h"
#include "voltage/voltage.h"


// uint32_t TIMEOUT_WATCHDOG_MS = 2000;
DigitalOut led(LED1);
static naelic::SWO swo;

int main()
{
  Thread buzzer_th;
  buzzer_th.start(buzzer::launch);

  Thread voltage_th;
  voltage_th.start(voltage::launch);

  Thread com_th;
  // com_th.set_priority(osPriorityHigh1);
  com_th.start(com::launch);

  Thread ir_th;
  ir_th.start(ir::launch);

  Thread engine_th;
  engine_th.start(drivers::launch);

  shell_init_usb();
  // Watchdog &watchdog = Watchdog::get_instance();
  // watchdog.start(TIMEOUT_WATCHDOG_MS);
  //   infos_init();
  led = 1;
  while (true)
  {
    ThisThread::sleep_for(100ms);
    // test_radio();
  }
}