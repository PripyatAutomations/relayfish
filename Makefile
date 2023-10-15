all: world

# If using a mega
BOARD_TAG         = mega
BOARD_SUB = atmega2560
FLASH_PROTO=atmega328p

# If using a nano
#BOARD_TAG = nano
#BOARD_SUB = atmega328
#FLASH_PROTO=atmega328p

# Is it an avr type board?
TARGET_AVR = y

# Serial monitor port
MONITOR_PORT      = /dev/ttyUSB0

# Serial monitor baudrate
MONITOR_BAUDRATE  = 115200

FLASH_BAUDRATE  = 57600

TARGET=relayfish
RELAYFISH_VERSION=0x0002
ARDUINO_LIBS = EEPROM Wire

# Linux tends to put it at /usr/share/arduino, check there
ifneq ("$(wildcard /usr/share/arduino/revisions.txt)","")
ARDUINO_DIR = /usr/share/arduino
endif

# If user has installed arduino into /opt, it's probably
# because the host OS has a broken version, prefer it.
ifneq ("$(wildcard /opt/arduino/arduino)","")
ARDUINO_DIR = /opt/arduino
endif

ifneq ("$(wildcard /usr/bin/avrdude)", "")
AVRDUDE          = /usr/bin/avrdude
AVR_TOOLS_DIR    = /usr
endif

# If the host avrdude is broken, it can be put into /opt/arduino
ifneq ("$(wildcard /opt/arduino/bin/avrdude)", "")
AVRDUDE          = /opt/arduino/bin/avrdude
AVR_TOOLS_DIR    = /opt/arduino
endif

PROJECT_DIR = .
ARDMK_DIR = ./mk
USER_LIB_PATH    :=  ${PROJECT_DIR}/lib
BUILDFLAGS = -DRELAYFISH=1 -DRELAYFISH_VERSION=${RELAYFISH_VERSION}
COMM_CFLAGS = ${BUILDFLAGS} -I./include -Wall -pedantic -Wno-error
CFLAGS_STD       = ${COMM_CFLAGS} -std=gnu11 -Os
CXXFLAGS_STD     = ${COMM_CFLAGS} -std=gnu++11 -Os -fno-rtti 
CURRENT_DIR      = $(shell basename ${CURDIR})
OBJDIR           = ${PROJECT_DIR}/bin/${BOARD_TAG}/${CURRENT_DIR}

CXXFLAGS_STD += -I./

# Bring in board configuration
include mk/Arduino.mk
include mk/indent.mk
include mk/clean.mk
include mk/install.mk

# Build firmwares, if needed before our source
world: arduino-all
TARBALL_DATE=$(date +%Y-%m-%d)

# Roll a distribution tarball
dist tarball: distclean
	tar -C .. -jcvf ../ampduino-${RELAYFISH_VERSION}-${TARBALL_DATE}.tar.bz2 ampduino
