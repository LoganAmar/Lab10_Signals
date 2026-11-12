/**
 * File: tennis_send.c
 * Modified by: Logan Amar
 * 
 * Brief summary of program: Serving player for signal tennis. Serves the
 * ball (SIGUSR1 + rally count through sigqueue) to tennis_recv, then returns
 * the ball after a random 1-2 second delay. Each return has a chance to
 * miss, in which case SIGUSR2 is sent to tell the other player they won
 * the point. This program serves again after every point and keeps score
 * with traditional tennis rules for a single game.
 * 
 * To play, run tennis_recv first and note the PID it prints, then:
 * ./tennis_send <pid>
 */

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MISS_PERCENT 30     // Chance a return is missed

#define EVENT_NONE 0
#define EVENT_BALL 1        // SIGUSR1 - ball was hit to us
#define EVENT_POINT 2       // SIGUSR2 - other player missed, we won the point

volatile sig_atomic_t event = EVENT_NONE;
volatile sig_atomic_t ball_count = 0;
volatile pid_t sender_pid = 0;

/**
 * @brief Handler for SIGUSR1 and SIGUSR2, records who sent the signal, the
 * value that came with it, and which signal it was
 */
void handle_signal(int sig, siginfo_t *info, void *context) {
    (void)context;

    sender_pid = info->si_pid;
    ball_count = info->si_value.sival_int;

    if (sig == SIGUSR1) {
        event = EVENT_BALL;
    } else {
        event = EVENT_POINT;
    }
}

/**
 * @brief Sends a signal with an integer value to a process using sigqueue
 */
void send_signal(pid_t pid, int sig, int num) {
    union sigval value;
    value.sival_int = num;

    if (sigqueue(pid, sig, value) == -1) {
        perror("sigqueue");
        exit(1);
    }
}

/**
 * @brief Sleeps for a random amount of time between 1 and 2 seconds
 */
void random_wait() {
    sleep(1);
    usleep(rand() % 1000000);
}

/**
 * @brief Returns 1 if player a has won the game against player b
 * (4 points and a lead of at least 2)
 */
int game_won(int a, int b) {
    return (a >= 4) && ((a - b) >= 2);
}

/**
 * @brief Prints the score and checks if the game is over
 * 
 * @return 1 if the game is over, 0 otherwise
 */
int check_score(int my, int opp) {
    const char* names[] = {"0", "15", "30", "40"};

    if (game_won(my, opp)) {
        printf("Game over - Server wins!\n");
        return 1;
    }
    if (game_won(opp, my)) {
        printf("Game over - Receiver wins!\n");
        return 1;
    }

    if (my >= 3 && opp >= 3) {
        if (my == opp) {
            printf("Score: Deuce\n");
        } else if (my > opp) {
            printf("Score: Advantage Server\n");
        } else {
            printf("Score: Advantage Receiver\n");
        }
    } else {
        printf("Score: Server %s - Receiver %s\n", names[my], names[opp]);
    }
    return 0;
}

/**
 * @brief Serves the ball to the other player
 */
void serve(pid_t pid) {
    printf("Getting ready to serve...\n");
    random_wait();
    printf("\007Serving the ball (#1)\n");
    send_signal(pid, SIGUSR1, 1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t opponent = atoi(argv[1]);

    srand((unsigned int)time(NULL) ^ (unsigned int)getpid());

    // Register handler for the ball (SIGUSR1) and the missed ball (SIGUSR2)
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handle_signal;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // Block both signals so they are only delivered while waiting in sigsuspend
    sigset_t block_mask;
    sigset_t wait_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGUSR1);
    sigaddset(&block_mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &block_mask, &wait_mask);

    int my_points = 0;
    int opp_points = 0;

    // Serve to start the game
    serve(opponent);

    while(1) {
        // Wait until a signal arrives
        while(event == EVENT_NONE) {
            sigsuspend(&wait_mask);
        }

        int current = event;
        int count = ball_count;
        pid_t other = sender_pid;
        event = EVENT_NONE;

        if (current == EVENT_BALL) {
            printf("Received ball #%d from PID %d\n", count, other);
            printf("Getting ready to hit the ball...\n");
            random_wait();

            if ((rand() % 100) < MISS_PERCENT) {
                // Missed the ball, other player wins the point
                printf("Missed the ball! Point to Receiver\n");
                send_signal(other, SIGUSR2, 0);
                opp_points++;
            } else {
                // Hit the ball back
                printf("\007Hit the ball back (#%d)\n", count + 1);
                send_signal(other, SIGUSR1, count + 1);
                continue;
            }
        } else {
            // Other player missed
            printf("Receiver missed the ball! Point to Server\n");
            my_points++;
        }

        // A point was played, update the score
        if (check_score(my_points, opp_points)) {
            break;
        }

        // Serve again for the next point
        serve(opponent);
    }

    return 0;
}