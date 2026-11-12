/**
 * CPE2600 Lab 10
 * File: signal_sigaction.c
 * Modified by: Logan Amar
 * 
 * Brief summary of program: Receives SIGUSR1 
 * 
 * To exit, enter in another termial:
 * pgrep signal_sigaction
 * kill -USR1 <pid>
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void handle_sigaction(int sig, siginfo_t *info, void *context) {
    (void) sig;
    (void) context;

    printf("Received SIGUSR1 from PID %d\n", info->si_pid);
}

int main(){

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handle_sigaction;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);

    // Wait for signals
    while(1) {
        sleep(1);
    }

    return 0;
}