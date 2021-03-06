/**
 * Application compile time config settings.
 */

// Define this to enable debugging
#define DEBUG

// Build target. This is mostly to modify the build when building for the less
// feature rich ATTiny85, but since this project is mostly targeted at the UNO,
// this value can be left as is.
#define TINY85 1
#define UNO 2
// The actual target
#define TARGET UNO
//#define TARGET TINY85

// ########## General Config Values #############
#define READ_RATE 2000	// The time between taking readings in millisecondes

// ########## Solar Panel Sensor #############
// The current sensor used here is the ±5A from Pololu:
//		http://www.pololu.com/product/1185 
#define PV_ID    'P'	// Sensor ID
#define PV_PIN_I A1		// Current sensor pin
#define PV_mVA  185		// Current sensor sensitivity - milliVolt/Amp
#define PV_PIN_V A0		// Voltage sensor pin
#define PV_R1 18000		// Voltage divider R1 (connected to Vin) value in ohm
#define PV_R2  4700		// Voltage divider R2 (connected to GND) value in ohm

// ############ Battery Sensor ###############
// The current sensor used here is the ±12.5A from Pololu:
//	http://www.pololu.com/product/2197	
#define BAT_ID    'B'	// Sensor ID
#define BAT_PIN_I A3	// Current sensor pin
#define BAT_mVA  167	// Current sensor sensitivity - milliVolt/Amp
#define BAT_PIN_V A2	// Voltage sensor pin
#define BAT_R1 12000	// Voltage divider R1 (connected to Vin) value in ohm
#define BAT_R2  4700	// Voltage divider R2 (connected to GND) value in ohm

// ############# Load Sensor ################
// The current sensor used here is the ±30A from Pololu:
//	http://www.pololu.com/product/1187
#define LD_ID    'L'	// Sensor ID
#define LD_PIN_I A5		// Current sensor pin
#define LD_mVA   66		// Current sensor sensitivity - milliVolt/Amp
#define LD_PIN_V A4		// Voltage sensor pin
#define LD_R1 12000		// Voltage divider R1 (connected to Vin) value in ohm
#define LD_R2  4700		// Voltage divider R2 (connected to GND) value in ohm

// ############# Logger settings ###########
#define LOG_LCD			// Define this if a PCD8544 based LCD is connected for logging
#define LOG_VWIRE		// Define this if a VirtualWire RF module is connected logging
#define VWIRE_TX  2		// VirtualWire TX pin to use. Must be defined and free even if no TX.
#define VWIRE_RX  3		// VirtualWire RX pin to use. Must be defined and free even if no RX.
#define VWIRE_PTT 4		// VirtualWire PTT pin to use. Must be defined and free even if no PTT.
#define VWIRE_SPEED 2000	// VirtualWire RF speed in bits/sec - Should not go higher than 2000

// Some things to change for ATTiny85 target
#if TARGET==TINY85
  #undef DEBUG
#endif
