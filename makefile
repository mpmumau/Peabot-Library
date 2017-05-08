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

LIBS=-lwiringPi -lwiringPiPca9685 -lrt -lpthread

# Project DEPS
_DEPS = main.h config.h console.h prompt.h log.h string_utils.h utils.h robot.h
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))

# Server Objects
_OBJ = main.o config.o log.o console.o prompt.o string_utils.o utils.o robot.o 
OBJ = $(patsubst %,$(OBJ_DIR)/%,$(_OBJ))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

peabot_server: $(OBJ)
	$(CC) -o $(BIN_DIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/peabot_server

.PHONY: install
install:
	if [ ! -d /var/log/peabot ]; \
	then \
		mkdir /var/log/peabot; \
	fi; \
	chown -R peabot:peabot /var/log/peabot; \
	chmod -R 770 /var/log/peabot; \

	mv bin/peabot_server /bin/peabot_server; \
	chown root:root /bin/peabot_server; \
	
	if [ ! -e etc/peabot.conf ]; \
	then \
	    cp etc/peabot.conf.orig etc/peabot.conf; \
	fi; \
	
.PHONY: full_uninstall
full_uninstall:
	if [ -e etc/peabot.conf ]; \
	then \
		rm etc/peabot.conf; \
	fi; \

	if [ -e /bin/peabot_server ]; \
	then \
		rm /bin/peabot_server; \
	fi; \