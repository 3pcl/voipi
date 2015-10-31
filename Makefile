CC=gcc
INCL=-lpulse-simple -lm

all:
	$(CC) main.c $(INCL) -o main
clean:
	rm main
