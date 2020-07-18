#==============================================================================
# Quad Bot: Makefile
#------------------------------------------------------------------------------
# Makefile for compiling the application for controlling the quadruped robot.
#
# Created:  April 2, 2017
# Author:   Matt Mumau
#==============================================================================

# The c compiler
CC=gcc

# Project directories
INC_DIR=inc
SRC_DIR=src
OBJ_DIR=obj
LIB_DIR=lib
BIN_DIR=bin

# Compiler flags
CFLAGS=-Wall -I$(INC_DIR) -std=c11

LIBS=-lwiringPi -lwiringPiPca9685 -lrt -lpthread -lm

# Project DEPS
_DEPS = config.h \
	config_defaults.h \
	config_file.h \
	config_stdin.h \
	configset_callbacks.h \
	console.h \
	list.h \
	log.h \
	main.h \
	math_defs.h \
	prompt.h \
	prompt_commands.h \
	robot.h \
	string_utils.h \
	usd_sensor.h \
	utils.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

# Server Objects
_OBJ = config.o \
	config_file.o \
	config_stdin.o \
	configset_callbacks.o \
	console.o \
	list.o \
	log.o \
	main.o \
	prompt.o \
	prompt_commands.o \
	robot.o \
	string_utils.o \
	usd_sensor.o \
	utils.o
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

peabot: $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS) $(LIBS)

debug: $(OBJ)
	$(CC) -g -o $(BIN_DIR)/peabot_debug $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/peabot $(BIN_DIR)/peabot_debug;

.PHONY: install
install:
	if [ ! -d /var/log/peabot ]; \
	then \
		mkdir /var/log/peabot; \
	fi; \
	chown -R peabot:peabot /var/log/peabot; \
	chmod -R 770 /var/log/peabot; \

	mv bin/peabot /bin/peabot; \
	chown root:root /bin/peabot; \
	
	if [ ! -e etc/peabot.conf ]; \
	then \
	    cp etc/peabot.conf.orig etc/peabot.conf; \
	fi; \
	
.PHONY: full_uninstall
full_uninstall:
	if [ -e /bin/peabot_server ]; \
	then \
		rm /bin/peabot_server; \
	fi; \