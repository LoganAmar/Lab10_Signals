# CPE-2600 Lab 10: Signals

**Name:** Logan Amar
**Course:** CPE-2600 Systems Programming, Fall 2026

## Programs

| Program | Description |
|---|---|
| `signal_handler` | Registers a SIGINT handler that prints a message. The handler does not exit, so use `kill -9 <pid>` to stop it. |
| `signal_alarm` | Calls `alarm(5)` and prints a message when SIGALRM arrives. |
| `signal_segfault` | Installs a SIGSEGV handler, then dereferences NULL. The handler returns, the faulting instruction re-runs, and the message repeats until Ctrl+C. |
| `signal_sigaction` | Uses `sigaction` with `SA_SIGINFO` to handle SIGUSR1 and print the sender's PID. |
| `recv_signal` | Prints its PID, then waits for SIGUSR1 and prints the integer sent with it. |
| `send_signal` | Sends SIGUSR1 plus a random integer to a PID using `sigqueue`. |

## Usage

**Sigaction**
./signal_sigaction
kill -USR1 <pid> # from a second terminal

