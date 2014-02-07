/**
 * Power sensor task header.
 **/

#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <Arduino.h>
#include <Task.h>
#include "config.h"
#include "Streaming.h"
#include "utils.h"

// Structure to hold Power sensor reading results
struct sensor_data_struct {
	char id;			// Sensor ID
	uint32_t ts;		// Time stamp in millis reading was taken
	uint16_t pinValI;	// The ADC value on the current pin
	uint16_t pinValV;	// The ADC value on the voltage pin
	int16_t mA;			// The current value in milliAmp: -32A to 32A
	int16_t mV;			// The voltage value in milliVolt: -32V to 32V
	int32_t mW;		// The power input/output in milliWatt: 0 to 65W 
};

// Power sensor class
class PSensor : public TimedTask {
	private:
		uint8_t _pinV;		// Analog pin the voltage divider is connected to
		uint8_t _pinI;		// Analog pin the voltage divider is connected to
		int16_t _calI;		// Calibration value to add to current ADC - my be negative
		int16_t _calV;		// Calibration value to add to voltage ADC - my be negative
		uint8_t _mV_A;		// Current sensor mv/A sensitivity - Note mV!
		uint16_t _r1;		// Voltage divider R1 (to Vin) in ohm. NOTE: max 65000
		uint16_t _r2;		// Voltage divider R2 (to GND) in ohm. NOTE: max 65000
		uint32_t _readRate;	// The rate at which to take readings in millis
		char _id;			// The sensor unique ID
		uint16_t _VpinVal;	// The ADC value for the voltage sensor pin
		uint16_t _IpinVal;	// The ADC value for the current sensor pin
		uint32_t _ts;		// Time stamp for last reading
		uint32_t _Vcc;		// The Vcc value just before reading the sensors
	
		// Returns the current value in mA based on the pin reading and
		// calibration value.
		int16_t current();
		// Returns the voltage value in mV based on the pin reading and
		// calibration value
		int16_t voltage();
		// Return the power in mW based on the voltage(mV) and current(mA)
		// values supplied
		int32_t power(int16_t v, int16_t c) { return ((int32_t)v * c)/1000; }

	public:
		PSensor(uint8_t pinV, uint8_t pinA, uint8_t mVA, uint16_t r1, uint16_t r2,
				uint32_t readRate, char id, int16_t calI=0, int16_t calV=0);
		virtual void run(uint32_t now);
		uint8_t lastReading(sensor_data_struct *data);
};


#endif	//_SENSOR_H_
