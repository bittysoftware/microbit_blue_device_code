/*
 * Temperature Alarm
 * Uses the Bluetooth Event Service to inform registered clients whenever the temperature falls below xx or rises above yy
 */

#include "MicroBit.h"
MicroBit uBit;

uint16_t state = 0; // 0=ok, 1=alarming because it's cold, 2=alarming because it's hot
uint16_t ok = 0;
uint16_t any = 0;
uint16_t temp_alarm = 9000;
uint16_t set_lower = 9001;
uint16_t set_upper = 9002;
uint16_t cold = 1;
uint16_t hot = 2;
uint8_t upper = 24;
uint8_t lower = 19;
int reading_count = 0;
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

void onSetLower(MicroBitEvent e)
{
	lower = e.value;
	uBit.display.scroll("L=");
	uBit.display.scrollAsync(lower);
}

void onSetUpper(MicroBitEvent e)
{
	upper = e.value;
	uBit.display.scroll("U=");
	uBit.display.scrollAsync(upper);
}

void onTemperatureReading(MicroBitEvent e)
{
	int temperature = uBit.thermometer.getTemperature();
	reading_count++;
	if (reading_count == 10)
	{
		uBit.display.scrollAsync(temperature);
		reading_count = 0;
	}
	if (temperature > upper && state == ok)
	{
		MicroBitEvent evt(temp_alarm, hot);
		state = hot;
		return;
	}
	if (temperature < lower && state == ok)
	{
		MicroBitEvent evt(temp_alarm, cold);
		state = cold;
		return;
	}
	if (temperature >= lower && temperature <= upper && state != ok)
	{
		MicroBitEvent evt(temp_alarm, ok);
		state = ok;
	}
}

int main()
{
	// Initialise the micro:bit runtime.
	uBit.init();

	uBit.display.scrollAsync("TEMP ALARM-NP");
	new MicroBitTemperatureService(*uBit.ble, uBit.thermometer);
	// listen for client events which set the upper and lower temperature limits
	uBit.messageBus.listen(set_lower, any, onSetLower);
	uBit.messageBus.listen(set_upper, any, onSetUpper);
	// listen to the temperature sensor
	uBit.messageBus.listen(MICROBIT_ID_THERMOMETER, MICROBIT_THERMOMETER_EVT_UPDATE, onTemperatureReading);
	// listen for Bluetooth connect and disconnect events
	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);

	// If main exits, there may still be other fibers running or registered event handlers etc.
	// Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
	// sit in the idle task forever, in a power efficient sleep.
	release_fiber();
}
