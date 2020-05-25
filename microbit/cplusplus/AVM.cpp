#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;

int connected = 0;

void onConnected(MicroBitEvent e)
{

    uBit.display.scroll("C");

    connected = 1;

    // my client will send ASCII strings terminated with the colon character
    ManagedString eom(":");

    while (1)
    {
        ManagedString msg = uart->readUntil(eom);
        uBit.display.scroll(msg);
    }
}

void onDisconnected(MicroBitEvent e)
{
    uBit.display.scroll("D");
    connected = 0;
}

void onButtonA(MicroBitEvent e)
{
    if (connected == 0)
    {
        return;
    }
    uBit.display.scroll("TX");
    uart->send(ManagedString("YES"));
}

void onButtonB(MicroBitEvent e)
{
    if (connected == 0)
    {
        return;
    }
    uBit.display.scroll("TX");
    uart->send(ManagedString("NO"));
}

void onButtonAB(MicroBitEvent e)
{
    if (connected == 0)
    {
        return;
    }
    uBit.display.scroll("TX");
    uart->send(ManagedString("GOT IT!!"));
}

/*
Recommend disabling the DFU and Event services in MicroBitConfig.h since they are not needed here:

#ifndef MICROBIT_BLE_DFU_SERVICE
#define MICROBIT_BLE_DFU_SERVICE                0
#endif

#ifndef MICROBIT_BLE_EVENT_SERVICE
#define MICROBIT_BLE_EVENT_SERVICE              0
#endif

*/

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);

    // Note GATT table size increased from default in MicroBitConfig.h
    // #define MICROBIT_SD_GATT_TABLE_SIZE             0x500
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    uBit.display.scroll("AVM-JW");

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}
