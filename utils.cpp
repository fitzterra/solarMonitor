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
 * Function to read the 1.1v reference voltage from the ATmega and use that to
 * calculate the true Vcc which is used by the ADC when measuring analog input.
 * The result is in mV.
 *
 * In order to obtain a more accurate analog input value, use this Vcc value to
 * calculate the real analog input:
 *
 * Vinput = (readVcc * pinVal) / 1023  -> in mV
 * 
 * Credit here: https://code.google.com/p/tinkerit/wiki/SecretVoltmeter
 * with changes to make it work on other ATmega MCUs by various people in the
 * comments.
 *
 * Copying from the link above:
 * 
 * The ATmega MCUs has an internal switch that selects which pin the analogue
 * to digital converter reads. That switch has a few leftover connections, so
 * the chip designers wired them up to useful signals. One of those signals is
 * the 1.1V precision reference that may be used as Vref for ADC measurements.
 * 
 * So if you measure how large the known 1.1V reference is in comparison to Vcc,
 * you can back-calculate what Vcc is with a little algebra. That is how this
 * works.
 * 
 * From the equation in section 24.7 of the ATmega data sheet giving the formula
 * used to get the ADC input value:
 * 
 * ADC = (Vin * 1024) / Vref
 * 
 * This function internally connects Vref to Vcc and the input to the ADC comes
 * from the internal 1.1V reference. Thus, rearranging this equation and expressing
 * voltage in millivolts gives:
 * 
 * Vcc = (1100 * 1024) / ADC
 * 
 * or
 * 
 * Vcc = 1126400L / ADC
 */
int readVcc() {
	// Read 1.1V reference against AVcc
	// set the reference to Vcc and the measurement to the internal 1.1V reference
	#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
	ADMUX = _BV(MUX5) | _BV(MUX0);
	#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	ADMUX = _BV(MUX3) | _BV(MUX2);
	#else
	ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	#endif 

	delay(2); // Wait for Vref to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA,ADSC)); // measuring

	uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH 
	uint8_t high = ADCH; // unlocks both

	long result = (high<<8) | low;

	//result = 1126400L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1024*1000
	// Others use 1023 instead of 1024 as the ADC resolution calculator. Since the
	// max value for an analog input can be 1023, it makes more sense to use 1023
	result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	return (int)result; // Vcc in millivolts
}
