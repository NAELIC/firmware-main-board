#include <mbed.h>
#include <shell.h>
#include "hardware.h"
#include "mux.h"

//pin init    
AnalogIn mux(MUX);
DigitalOut addr1(ADDR1);
DigitalOut addr2(ADDR2);
DigitalOut addr3(ADDR3);

void mux_init()
{
   addr1 = 0;
   addr2 = 0;
   addr3 = 0;
}

int mux_sample(int addr)
{

    addr1 = (((addr>>0)&1) ? 1 : 0 );
    addr2 = (((addr>>1)&1) ? 1 : 0 );
    addr3 = (((addr>>2)&1) ? 1 : 0 );

    return mux.read_u16();
}

bool get_hall(int addr)
{
  return mux_sample(addr)<HALL_THRES;
}

SHELL_COMMAND(mdb, "Mux debug")
{
    for (int k=0; k<8; k++) {
        shell_print("Mux #");
        shell_print(k);
        shell_print(": ");
        shell_println(mux_sample(k));
        // Watchdog::get_instance().kick();
    }
}
