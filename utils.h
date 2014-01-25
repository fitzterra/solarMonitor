/**
 * Various utility functions.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>
#include "config.h"
#include "Streaming.h"

// Calculate the size of a fixed (non-dynamic) array
#if !defined(ARRAY_SIZE)
    #define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

// If SERIAL_SPEED is not defined in config.h, default it to 57600 here.
#ifndef SERIAL_SPEED
#define SERIAL_SPEED 57600
#endif // SERIAL_SPEED

void OpenSerial(long speed = SERIAL_SPEED);

long readVcc();

#endif // _UTILS_H_
