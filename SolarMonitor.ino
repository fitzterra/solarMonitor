// Solar Monitor sketch

#include "config.h"
#include "utils.h"
#include <TaskScheduler.h>
#include "sensor.h"
#include "logger.h"

void setup()
{
#if TARGET!=TINY85
	OpenSerial();
	Serial << "Starting....\n";

	uint16_t Vcc = readVcc();
	uint16_t pinVal = 1023;
	uint32_t r1 = 12000;
	uint32_t r2 = 4700;
	float ratio = (r1 + r2)/(float)r2;
	float v = ((uint32_t)pinVal * Vcc * ratio)/1023;
	float v1 = (pinVal * Vcc)/1023 * ratio;
	uint32_t v2 = ((uint32_t)pinVal * Vcc * ratio)/1023;
	uint32_t v3 = ((uint32_t)pinVal * Vcc * (uint32_t)((r1+r2)/100))/(uint32_t)(1023*r2/100);
	uint32_t v4 = ((uint32_t)pinVal * Vcc)/1023;
	uint32_t v5 = ((uint32_t)pinVal * Vcc * ((uint32_t)r1+r2))/((uint32_t)1023*r2);
	Serial << "Ratio: " << ratio << "\nVcc: " << Vcc << endl;
	Serial << "v: " << v << endl;
	Serial << "v1: " << v1 << endl;
	Serial << "v2: " << v2 << endl;
	Serial << "v3: " << v3 << endl;
	Serial << "v4: " << v4 << endl;
	Serial << "v5: " << v5 << endl;
#endif
}

// Main program.
void loop()
{
    // Create the tasks.
    PSensor PVSens(PV_PIN_V, PV_PIN_I, PV_mVA, PV_R1, PV_R2, READ_RATE, PV_ID);
    PSensor BATSens(BAT_PIN_V, BAT_PIN_I, BAT_mVA, BAT_R1, BAT_R2, READ_RATE, BAT_ID);
    PSensor LDSens(LD_PIN_V, LD_PIN_I, LD_mVA, LD_R1, LD_R2, READ_RATE, LD_ID);

	PSensor *sensors[] = { &PVSens, &BATSens, &LDSens }; 
    Logger logger(sensors, ARRAY_SIZE(sensors), READ_RATE);
    
    // Initialise the task list and scheduler.
    Task *tasks[] = { &PVSens, &BATSens, &LDSens, &logger};
    TaskScheduler sched(tasks, NUM_TASKS(tasks));
    
    // Run the scheduler - never returns.
    sched.run();
}
