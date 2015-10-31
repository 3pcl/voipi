CC=gcc
INCL=-lpulse-simple

all:
	$(CC) main.c $(INCL) -o main
clean:
	rm main
