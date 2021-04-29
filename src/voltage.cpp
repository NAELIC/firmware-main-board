#include <mbed.h>
#include "shell.h"
#include "hardware.h"
#include "voltage.h"
#include "mux.h"

static bool bat_init = false;

SHELL_PARAMETER_FLOAT(bat, "Battery 1", 0.0);
// Timer millis;

void voltage_init()
{
    bat = 0;
    bat_init = false;
    Kernel::Clock::now().time_since_epoch().count();
}

void voltage_tick()
{
    static int lastSample = Kernel::Clock::now().time_since_epoch().count();

    if ((Kernel::Clock::now().time_since_epoch().count() - lastSample) > 5) {
        lastSample = Kernel::Clock::now().time_since_epoch().count();

        float v1 = mux_sample(BAT_ADDR)*3.3/4096;
        v1 = v1*(BAT_R1 + BAT_R2)/BAT_R2;

        if (bat_init) {
            bat_init = false;
            bat = v1;
        } else {
            bat = bat*0.99 + v1*0.01;
        }
    }
}

float voltage_value()
{
    return bat;
}
