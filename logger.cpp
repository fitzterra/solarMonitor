// Data logger task

#include "logger.h"

/**
 * Data Logger contrsuctor
 *
 * @param sensor An array of pointers to sensors from which we will log readings
 * @param sensors The number of sensors on the array
 */
Logger::Logger(PSensor **sensor, uint8_t sensors, uint32_t logRate) : TimedTask(millis()),
 _sensor(sensor),
 _sensors(sensors),
_logRate(logRate) {
	#ifdef LOG_LCD
	// Initalize the lcd
	_lcd.begin();
	// Set the first sensor as the current sensor to show values for
	_showSensor = 0;
	_lcd.print(F("Solar Monitor"));
	#endif
}

/**
 * Called when the task is ready to run.
 *
 * @param now The current millis() time.
 */
void Logger::run(uint32_t now) {
	uint8_t res;

	#ifdef DEBUG
	for (int i=0; i<_sensors; i++) {
		res = _sensor[i]->lastReading(&_data);
		Serial << "[" << _data.id << "]: " << _data.mV << "mV, ";
		Serial << _data.mA << "mA, " << _data.mW << "mW" << endl;
	}
	#endif
	#ifdef LOG_LCD
	updateLCD();
	#endif

    // Run again in the required number of milliseconds.
    incRunTime(_logRate);
}


#ifdef LOG_LCD
void Logger::updateLCD() {
	static uint8_t viewCount=0;
	uint8_t res;

	// If the view count for this sensor has been reached, we update to the next
	viewCount++;
	Serial << "viewCount: " << viewCount << endl;
	if (viewCount > 3) {
		_showSensor++;
		if(_showSensor >= _sensors) {
			_showSensor = 0;
		}
		viewCount = 0;
	}
	Serial << "_showSensor: " << _showSensor << endl;
	// Get the sensor values
	res = _sensor[_showSensor]->lastReading(&_data);
	// Update the display
	_lcd.clear();
	_lcd.print(F("Sensor: "));
	_lcd.print(_data.id);
	_lcd.gotoXY(0, 1);
	_lcd.print(_data.mV);
	_lcd.print(" mv");
	_lcd.gotoXY(0, 2);
	_lcd.print(_data.mA);
	_lcd.print(" mA");
	_lcd.gotoXY(0, 3);
	_lcd.print(_data.mW);
	_lcd.print(" mW");
}
#endif
