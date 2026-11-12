/**
 * CPE2600 Lab 10
 * File: signal_alarm.c
 * Modified by: Logan Amar
 * 
 * Brief summary of program: Sets alarm to go off after 5 secs
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief Signal handler for SIGALRM - prints a message
 */
void handle_alarm() {
    printf("Alarm triggered, SIGALARM received\n");
}

int main() {

    //Register SIGALRM to handle_alarm
    signal(SIGALRM, handle_alarm);

    //Schedule alarm for 5 seconds
    alarm(5);

    printf("Waiting...\n");

    //Wait until alarm is triggered
    pause();

    return 0;
}