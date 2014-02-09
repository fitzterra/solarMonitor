/**
 * Sensor data logger task header.
 **/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <Arduino.h>
#include <Task.h>
#include "config.h"
#include "Streaming.h"
#include "sensor.h"
#include <SPI.h>
#include "PCD8544_SPI.h"
#include <VirtualWire.h>

// Data logger class
class Logger : public TimedTask {
	private:
		PSensor **_sensor;	// Array of pointers to sensors
		uint8_t _sensors;	// Number of sensors in the list
		uint32_t _logRate;	// The rate at which to log data in millis
		sensor_data_struct _data;	// Stores data read from a sensor
		#ifdef LOG_LCD
		PCD8544_SPI _lcd;	// The Nokia 5110 type LCD accessed via the PCD8544_SPI lib.
		uint8_t _showSensor;	// Which of the current sensor values are displayed.
		#endif

	public:
		Logger(PSensor **sensor, uint8_t sensors, uint32_t logRate);
		virtual void run(uint32_t now);

		#ifdef LOG_LCD
		void updateLCD();
		#endif

		#ifdef LOG_VWIRE
		void logRF();
		#endif
};

#endif	//_LOGGER_H_

