/* 
 * Increment a counter by pressing button A
 *
 * Upload the accrued counter value as a Bluetooth event when Button B is pressed
 */

#include "MicroBit.h"
MicroBit uBit;

uint16_t counter = 0;
uint16_t counter_event = 9003;
int connected = 0;

void onConnected(MicroBitEvent e)
{
    uBit.display.scroll("C");
    connected = 1;
}

void onDisconnected(MicroBitEvent e)
{
    uBit.display.scroll("D");
    connected = 0;
}

void onButtonAClick(MicroBitEvent e)
{
    counter++;
    uBit.display.scrollAsync(counter);
}

void onButtonBClick(MicroBitEvent e)
{
    if (connected == 1)
    {
        MicroBitEvent evt(counter_event, counter);
        counter = 0;
    }
    else
    {
        uBit.display.scrollAsync(counter);
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.serial.printf("counter\r\n");
    uBit.display.scrollAsync("COUNT-NP");
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonAClick);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBClick);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

    uBit.serial.printf("ready\r\n");

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
