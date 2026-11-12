/**
 * @file signal_segfault.c
 * @brief Program that dereferences a null pointer causing a segmentation fault
 */

/**
 * Modified by: Logan Amar
 * 
 * Brief summary of modifications: Added handle_segfault function and registered
 * SIGSEGV to it.
 */


#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_segfault(int sig) {
    (void)sig;
    printf("Received Segmentation fault\n");
    exit(1);
}

int main(){

    signal(SIGSEGV, handle_segfault);

    // Declare a null pointer
    int* i = NULL;

    // Dereference the null pointer
    printf("The value of i is: %d\n", *i);

    // Return to exit the program
    return 0;
}