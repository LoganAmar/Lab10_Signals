CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = signal_handler signal_alarm signal_segfault signal_sigaction recv_signal send_signal tennis_recv tennis_send

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)