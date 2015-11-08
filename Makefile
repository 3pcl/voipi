CC=gcc
INCL=-lpulse-simple -lm -lpthread


all:
	$(CC) main.c $(INCL) -o main
	$(CC) client.c $(INCL) -o client 

clean:
	rm main client
