# GeyserGuard Makefile using edam's (http://ed.am/dev/make/arduino-mk) makefile
# for command line compiling.

# Set the arduino dir
ARDUINODIR := /home/tomc/Documents/Electronics/Arduino/arduino-1.0.5
ARDUINOCONST := 105
# Set the board
BOARD := uno
#BOARD := attiny85

# For the ATtiny85, we need to also define the upload protocol and speed
ifeq "$(BOARD)" "attiny85"
# We're using the arduino ISP so the protocol and speed is
PROGRAMMER_PROTOCOL := stk500v1
PROGRAMMER_SPEED := 19200
PROGRAMMER_VERBOSITY := -v -v -v
endif

# The monitor program to use if other than 'screen ' is required
MONITOR_PROG := miniterm.py
MONITOR_SPEED := 57600

# The real Makefile
include arduino-mk

#
#----------------------------------------------------------------------------
#
environ:
	@echo "Arduino dir     :" $(ARDUINODIR)
	@echo "Arduino Const   :" $(ARDUINOCONST)
	@echo "Sketchbook      :" $(SKETCHBOOK)
	@echo "AVR tools path  :" $(AVRTOOLSPATH)
	@echo "Sources         :" $(SOURCES)
	@echo "Headers         :" $(HEADERS)
	@echo "Libraries       :" $(LIBRARIES)
	@echo "Board files     :" $(BOARDS_FILE)

