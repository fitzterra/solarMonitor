/**
 * Various utility functions.
 */

#include "utils.h"

// No serial out on the Tiny
#if TARGET!=TINY85
/**
 * Helper for opening the serial port only once.
 *
 * At times is it neccessary for multiple objects to try to open the serial port
 * without knowing if it has already been opened. This function can be used as a
 * proxy to open the serial port. It may be called multiple times but will ignore
 * any requests if the port has already been opened.
 * If a new request to open the ports comes it with a different speed, it will
 * write an error to the serial output and then ignore the request. Only the port
 * speed setting is supported at this time.
 *
 * @param speed The port speed. It may be defined as SERIAL_SPEED in config.h, or
 *        else it will default to 57600 if this value is not supplied.
 */
void OpenSerial(long speed) {
	static bool isOpen=false;
	static long s=0;

	// Not open yet?
	if (!isOpen) {
		// Save the speed
		s = speed;
		// Open it
		Serial.begin(speed);
		// Indicate that it is open
		isOpen = true;
		return;
	}

	#ifdef DEBUG
	// Different speed attempted?
	if(speed!=s) {
		Serial << F("Serial open attempted with new speed: ") << speed \
			   << F(". Not changed from: ") << s << endl;
	}
	#endif // DEBUG
};
#endif  //TARGET == UNO

/**
 * Function to read the true 1.1v reference voltage from the ATmega and uses
 * that to calculate the true Vcc which is used by the ADC when measuring analog
 * input. The result is in mV.
 *
 * In order to obtain a more accurate analog input value, use this Vcc value to
 * calculate the real analog input:
 *
 * Vinput = (readVcc * pinVal) / 1023  -> in mV
 *
 * Credit here: http://hacking.majenko.co.uk/node/57
 */
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}
