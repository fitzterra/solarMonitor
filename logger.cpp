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
	#ifdef LOG_VWIRE
	// Set up the VirtualWire pins
	vw_set_tx_pin(VWIRE_TX);
	vw_set_rx_pin(VWIRE_RX);
	vw_set_ptt_pin(VWIRE_PTT);
    // Initialise the IO and ISR
    vw_setup(VWIRE_SPEED);
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
	#ifdef LOG_VWIRE
	logRF();
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
	if (viewCount > 3) {
		_showSensor++;
		if(_showSensor >= _sensors) {
			_showSensor = 0;
		}
		viewCount = 0;
	}
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
#endif	// LOG_LCD

#ifdef LOG_VWIRE
void Logger::logRF() {
	uint8_t res;
	String logLine = String(millis());

	// Read each sensor and add to the string
	for (int i=0; i<_sensors; i++) {
		res = _sensor[i]->lastReading(&_data);
		logLine += ':';
		logLine += _data.id;
		logLine += ':';
		logLine += _data.mV;
		logLine += ':';
		logLine += _data.mA;
	}
	// We need a trailing newline for the receiver to signal end of line
	logLine += '\n';
	#ifdef DEBUG
	Serial << "RF out: " << logLine;
	#endif
	// Send if
	vw_send((uint8_t*)&(logLine[0]), logLine.length());
	// Wait for the transmission to be completed.
	vw_wait_tx();
}
#endif // LOG_VWIRE
