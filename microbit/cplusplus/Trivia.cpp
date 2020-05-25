#include "MicroBit.h"
MicroBit uBit;

#define MES_TRIVIA                 1300

#define MES_TRIVIA_BLUE            1
#define MES_TRIVIA_PINK            2
#define MES_TRIVIA_YELLOW          3

#define MES_TRIVIA_BROWN           4
#define MES_TRIVIA_GREEN           5
#define MES_TRIVIA_ORANGE          6

int connected = 0;

void onConnected(MicroBitEvent e)
{
    uBit.display.print("C");
    connected = 1;
}

void onDisconnected(MicroBitEvent e)
{
    uBit.display.print("D");
    connected = 0;
}

int getGesture() {
    int x = uBit.accelerometer.getX();
    int y = uBit.accelerometer.getY();
    return uBit.accelerometer.getGesture();
}

void onButtonA(MicroBitEvent e)
{
    if (connected == 0) {
        uBit.display.scroll("NC");
        return;
    }
    
    if (getGesture() == MICROBIT_ACCELEROMETER_EVT_FACE_UP || getGesture() == MICROBIT_ACCELEROMETER_EVT_NONE) {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_BLUE);
    } else {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_BROWN);
    }
    
}

void onButtonB(MicroBitEvent e)
{
    if (connected == 0) {
        uBit.display.scroll("NC");
        return;
    }
    if (getGesture() == MICROBIT_ACCELEROMETER_EVT_FACE_UP || getGesture() == MICROBIT_ACCELEROMETER_EVT_NONE) {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_PINK);
    } else {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_GREEN);
    }
}

void onButtonAB(MicroBitEvent e)
{
    if (connected == 0) {
        uBit.display.scroll("NC");
        return;
    }
    if (getGesture() == MICROBIT_ACCELEROMETER_EVT_FACE_UP || getGesture() == MICROBIT_ACCELEROMETER_EVT_NONE) {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_YELLOW);
    } else {
        MicroBitEvent evt(MES_TRIVIA, MES_TRIVIA_ORANGE);
    }
}

void onGesture(MicroBitEvent e) {
    uBit.display.scroll("E");
    if (connected) {
      uBit.display.print("C");
    } else {
      uBit.display.print("D");
    }
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    uBit.display.scroll("TRIVIA-NP");
    
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_AB, MICROBIT_BUTTON_EVT_CLICK, onButtonAB);

    uBit.messageBus.listen(MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, MICROBIT_ACCELEROMETER_EVT_FACE_UP, onGesture);
    uBit.messageBus.listen(MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE, MICROBIT_ACCELEROMETER_EVT_FACE_DOWN, onGesture);
    


    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}