#include <USBSerial.h>
#include <common/buzzer/buzzer.h>
#include <common/define/hardware.h>
#include <common/mux.h>
#include <mbed.h>
#include <shell.h>
#include <swo.h>
// #include <common/utils/version.h>

#include "kicker/kicker.h"

#include "engine/drivers.h"
#include "engine/dribbler.h"
#include "ir/ir.h"
#include <nrf.h>

#include "com/com.h"
#include "infos.h"
#include "voltage/voltage.h"

// uint32_t TIMEOUT_WATCHDOG_MS = 2000;
DigitalOut led(LED1);
static naelic::SWO swo;
// Override mbed printf implementation
FileHandle *mbed::mbed_override_console(int) { return &swo; }

EventQueue event_queue;

int main() {
    kicker_init();
    
    dribbler::init();

    // infos_init();

    // Thread buzzer_th;
    // buzzer_th.start(buzzer::launch);

    // bool h1=get_hall(HALL1_ADDR);
    //   // delay_us(1000);
    // bool h2=get_hall(HALL2_ADDR);
    //  // delay_us(1000);
    // bool h3=get_hall(HALL3_ADDR);
    //   // delay_us(1000);
    // bool h4=get_hall(HALL4_ADDR);

    // int id=infos_get_id();
    // swo.println(id);

    drivers::init();

    event_queue.call_every(1s, voltage::compute);
    event_queue.call_every(100ms, ir::compute);
    event_queue.call_every(100ms, kicker_tick);

    Thread com_th;
    com_th.start(com::launch);
    com_th.set_priority(osPriorityHigh1);

    // Thread dribbler_th;
    // dribbler_th.start(dribbler::launch);

    shell_init_usb();
    // Watchdog &watchdog = Watchdog::get_instance();
    // watchdog.start(TIMEOUT_WATCHDOG_MS);
    led = 1;

    event_queue.dispatch_forever();
}