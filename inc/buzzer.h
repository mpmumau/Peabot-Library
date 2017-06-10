#ifndef BUZZER_H_DEF
#define BUZZER_H_DEF

/*
 File:          buzzer.h
 Description:   Declarations for piezo buzzer (sound) functionality.
 Created:       June 10, 2017
 Author:        Matt Mumau
 */

void buzzer_init();

void buzzer_halt();

void *buzzer_main(void *arg);

#endif