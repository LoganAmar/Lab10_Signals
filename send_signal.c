/**
 * CPE2600 Lab 10
 * File: send_signal.c
 * Modified by: Logan Amar
 * 
 * Brief summary of program: Sends SIGUSR1 plus a random integer to a process using sigqueue
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    //Print usage info
    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t pid = atoi(argv[1]);

    //Generate random number 1-100
    srand((unsigned int)time(NULL));
    int num = (rand() % 100) + 1;

    printf("Random number: %d\n", num);

    union sigval value;
    value.sival_int = num;

    //Send random number
    sigqueue(pid, SIGUSR1, value);

    return 0;
}