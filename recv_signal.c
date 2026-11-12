/**
 * CPE2600 Lab 10
 * File: recv_signal.c
 * Modified by: Logan Amar
 * 
 * 
 * Brief summary of program: Receives SIGUSR1 from send_signal.c
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void handle_sigusr1(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;

    printf("Received SIGUSR1 with value %d\n", info->si_value.sival_int);

    exit(1);
}

int main() {
    printf("PID: %d\n", getpid());

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));

    //Register sigaction with handle_sigusr1
    sa.sa_sigaction = handle_sigusr1;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    //Register sigaction
    sigaction(SIGUSR1, &sa, NULL);

    while(1) {
        sleep(1);
    }

    return 0;
}