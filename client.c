#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>
#include <string.h> 
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

unsigned char *bigbuff;
bool *writebuf;

struct listenerArgs {
	char *server;
	int port;
	int buflen;
	int lead;
};

void *recvData(void *arg) {

	int bufi = 0, recv_len;

	// parse args 
	struct listenerArgs *input = (struct listenerArgs*)arg;

	char server[100];
	strncpy(server, input->server, 100);
	int port = input->port;
	int buflen = input->buflen;
	int lead = input->lead;

	// Init socket
	struct sockaddr_in si_other;
	int sck, i, slen=sizeof(si_other);

	if ( (sck=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) { fprintf(stderr, "socket err\n"); exit(1); }

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);

	if (inet_aton(server, &si_other.sin_addr) == 0) { fprintf(stderr, "inet_aton() failed\n"); exit(1); }


	if( bind(sck, (struct sockaddr*)&si_other, sizeof(si_other) ) == -1) { fprintf(stderr, "bind err\n"); exit(1); } 

	while(1) {

		recv_len = recvfrom(sck, bigbuff+bufi, buflen, 0, (struct sockaddr *) &si_other, &slen);
//		printf(".");
		writebuf[bufi/buflen] = true;
		bufi += recv_len;
		if(bufi >= buflen*lead) bufi = 0;
		
	}

}

int main(int argc, char **argv){


	if(argc<5) { printf("Usage: %s IP PORT BUFSIZE BUFFER_LEAD\n", argv[0]); exit(1); }


// init pulseaudio
	pa_simple *s;

	static const pa_sample_spec ss = { .format = PA_SAMPLE_U8, .rate = 8000, .channels = 1 };

	s= pa_simple_new(NULL,			//default server
		"Voipi_play",		//app name
		PA_STREAM_PLAYBACK,		// we'd like to play the audio
		NULL,			//default device
		"network sound playback", //stream description
		&ss,			//sample format
		NULL,			//default channel map
		NULL, 			//default buffering atributes
		NULL			//Ignore error code
	);

	struct listenerArgs la;
	la.server = argv[1];
	la.port = atoi(argv[2]);
	
	int buflen = atoi(argv[3]);
	la.buflen = buflen;

	int lead = atoi(argv[4]);
	la.lead = lead;

	int x = 0; // for fors

	bigbuff = malloc(sizeof(unsigned char) * buflen * lead);
	for(x=0; x<buflen*lead; x++) bigbuff[x]=127;

	writebuf = malloc(sizeof(bool) * lead);
	for(x=0; x<lead; x++) writebuf[x]=false;


	pthread_t fillerThread;
	pthread_create(&fillerThread, NULL, recvData, &la);

	usleep(500*1000);	// just wait 0.5s

	printf("Filling in the buffer... \n");

	bool isBufferfull = false;
	while(!isBufferfull) {

		isBufferfull = true;
		for(x=0; x<lead; x++) {
			if(writebuf[x] == false) isBufferfull = false;
		}

	}

	printf("OK!\n");

	int idx, count=0;

	while(1) {

		
		pa_simple_write(s, bigbuff+idx, buflen, NULL);

		// displaying current status of buffer
		count=0;
		for(x=0; x<lead; x++) { 
			if(writebuf[x] == true) count++;
			if(x==idx/buflen) { printf("@"); }
			else {printf("%c", writebuf[x]==true?'#':'-'); }
		}

		printf("   %.2f%%", 100*(float)count/(lead));
		
		// if the sample we played has been used before, print underrun message
		if(writebuf[idx/buflen] == false) { printf(" underrun!"); }	
		printf("\n");

		// mark what's already been played
		writebuf[idx/buflen] = false;

		idx += buflen;
		if(idx >= buflen*lead) idx = 0;

	}


	return 0;
}
