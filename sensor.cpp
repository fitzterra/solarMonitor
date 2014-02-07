// Power sensor task

#include "sensor.h"

/**
 * Power sensor contrsuctor
 *
 * Circuit:
 *   The power sensor consists of two parts:
 *    - A Voltage divider over the input/output lines with the divider output
 *      connected to an anolog input. This is required since the voltages are
 *      always more than can be read by the Arduino analog inputs.
 *    - A Pololu currect sensor breakout (ACS7xx type hall effect sensor) with
 *      sensor output connected to an analog input.
 *   
 * @param pinV The analog pin connected to the sensor voltage divider.
 * @param pinI The analog pin connected to the current sensor VIOUT pin.
 * @param reaRate How often we need to take a sensor reading for both voltage
 *                and current. THis is in millis.
 * @param id The sensor id.
 */
PSensor::PSensor(uint8_t pinV, uint8_t pinI, uint8_t mV_A, uint16_t r1,
				 uint16_t r2, uint32_t readRate, char id, int16_t calI,
				 int16_t calV) : TimedTask(millis()),
 _pinV(pinV),
 _pinI(pinI),
 _mV_A(mV_A),
 _r1(r1),
 _r2(r2),
 _readRate(readRate),
 _id(id),
 _calI(calI),
 _calV(calV),
 _VpinVal(0),
 _IpinVal(0),
 _ts(0) {
	// Not much to do for now
}


/**
 * Called when the task is ready to run.
 *
 * @param now The current millis() time.
 */
void PSensor::run(uint32_t now) {
	// Set the read time stamp
	_ts = now;
	// Get the current Vcc value
	_Vcc = readVcc();
	// Read the voltage value
	_VpinVal = analogRead(_pinV);
	// Read the current value
	_IpinVal = analogRead(_pinI);
	
	#ifdef DEBUG
	Serial << "[" << _id << "] : Vcc: " << _Vcc << "V pin: " << _VpinVal << "  V cal: " << _calV;
	Serial << " - I pin: " << _IpinVal << "  I cal: " << _calI << endl;
	#endif

    // Run again in the required number of milliseconds.
    incRunTime(_readRate);
}

/**
 * Calculate the current through the sensor based on the current pin value and
 * apply any calibration value.
 **/
int16_t PSensor::current() {
	// The current sensor output sits at VCC/2 when no current is measured and
	// rises proportionally with a forward current, and lowers proportionally
	// with a reverse current. The change in voltage will be at a factor of 
	// _mv_A millivolt per amp.
	// To calclate the actual current, we first need to remove 512 for the analog
	// pin input reading because this offset is the 0 amp and is half of Vcc (512
	// ADC steps to read 2.5V)
	//   zeroComp = _IpinVal - 512
	//
	// There may be calibration value, so this needs to be added to the zeroComp
	// value. The calibration value may be negative:
	//
	//   zeroComp += _calI
	//
	// Now we need to convert this value to a voltage, which means multiplying
	// by Vcc/1024steps. The true Vcc upon which the ADC relies for it's
	// measurements is available in _Vcc, and this _Vcc is in milivolts:
	//             zeroComp * Vcc
	//   mVSense = --------------
	//                 1023
	//
	// Now to get the actual current we divide by the sensor milliVolts/Amp
	// sensitivity value (or multiply by it's resiprocal)
	//
	//                     1
	//   Si = mVSense * --------
	//                   _mV_A
	//
	// By combining the two formulas we get:
	//
	//        (_IpinVal - 512 + _calI) * Vcc
	//   Si = ------------------------------
	//               1023 * _mV_A
	//
	// But we need the current in milliAmps, so we multiply by 1000:
	//
	//         (_IpinVal - 512 + _calI) * Vcc * 1000
	//   mA =  -------------------------------------
	//                     1023 * _mV_A
	//
	
	return (((int32_t)_IpinVal - 512 + _calI) * (int32_t)_Vcc * 1000) / ((int32_t)1023 * _mV_A);
}

/**
 * Calculate voltage from the voltage divider based on pin reading and calibration.
 **/
int16_t PSensor::voltage() {
	// The voltage is calculated based on the true arduino Vcc at the instant
	// the analog input was read and the resistors used for the voltage divider:
	//
	//       (_VpinVal + _calV) x Vcc x (R1 + R2)
	// Vin = ------------------------------------
	//                  1023    x      R2
	//       
	// The Vcc value is calculated in mV and the returned voltage is also in mV, the
	// value can become quite large and has the potential of rolling over even with
	// 32bit integers. We compensate for this by dividing top and bottom by 100
	// before doing the final division.

	return (((int32_t)_VpinVal + _calV) * (int32_t)_Vcc * (((int32_t)_r1 + _r2)/100)) / ((int32_t)1023 * _r2/100);
}

/**
 * Called to return the last read sensor value.
 *
 * This method takes the last voltage and current pin values, calculates the
 * correct voltage and current based on the voltage divider max voltage and the
 * current sensor sensitivity, and then calculates the power input/output based
 * on P=V*I.
 *
 * @param data: A pointer to a sensor_data_struct to fill with the relevant values.
 *
 * @return: 0 if there was data available to return, 1 otherwise.
 **/
uint8_t PSensor::lastReading(sensor_data_struct *data) {
	// If _ts is 0, no valid reading is available.
	//if (_ts==0) return 1;

	// Set the sensor ID, time stamp and the pin readings we already have
	data->id = _id;
	data->ts = _ts;
	data->pinValI = _IpinVal;
	data->pinValV = _VpinVal;

	// Add voltage, current and power. Note that power should use the
	// calculated current and voltage since calibration has been applied to them.
	data->mV = voltage();
	data->mA = current();
	data->mW = power(data->mV, data->mA);

	// Set _ts to zero as indicator that the last value has been returned
	//_ts = 0;

	return 0;
}
