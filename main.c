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


int main(int argc, char **argv){

	if(argc < 4) { printf("Usage: %s IP PORT BUFFER_SIZE\n", argv[0]); exit(1); }


	char server[100];
	strncpy(server, argv[1], 100);

	int port = atoi(argv[2]);
	int buflen = atoi(argv[3]);


// init pulseaudio
	pa_simple *s;

	static const pa_sample_spec ss = { .format = PA_SAMPLE_U8, .rate = 8000, .channels = 1 };

	s= pa_simple_new(NULL,			//default server
		"Voipi_play",		//app name
		PA_STREAM_RECORD,
		NULL,			//default device
		"network sound playback", //stream description
		&ss,			//sample format
		NULL,			//default channel map
		NULL, 			//default buffering atributes
		NULL			//Ignore error code
	);



// init UDP socket 

	struct sockaddr_in si_other;
	int sck, i, slen=sizeof(si_other);

	printf("initialising buffer: %d\n ", buflen);

	unsigned char *buf = malloc(sizeof(unsigned char) * (buflen));



	if ( (sck=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);

	if (inet_aton(server , &si_other.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

// end of socket init

	while(1) {

		pa_simple_read(s, buf, buflen, NULL);
		sendto(sck, buf, buflen, 0 , (struct sockaddr *) &si_other, slen);

	}

	printf("Hello world\n");

	return 0;
}
