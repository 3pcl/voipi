#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>


#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer
#define PORT 8888   //The port on which to send data
 

void die(char *s)
{
    perror(s);
    exit(1);
}


int main(int argc, char *argv){


// init pulseaudio
	pa_simple *s;

	static const pa_sample_spec ss = { .format = PA_SAMPLE_U8, .rate = 8000, .channels = 1 };

	s= pa_simple_new(NULL,			//default server
		"Voipi_play",		//app name
		PA_STREAM_PLAYBACK,
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
	unsigned char buf[BUFLEN];


	if ( (sck=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);

	if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	if( bind(sck, (struct sockaddr*)&si_other, sizeof(si_other) ) == -1)
	{
		die("bind");
	}

// end of socket init

	int x, max;
	int recv_len;	

	while(1) {
	
		// receive
		recv_len = recvfrom(sck, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);	
	
		max=0;
		for(x=0;x<BUFLEN;x++) max += abs(buf[x] - 128);
		max /= BUFLEN;
		for(x=0;x<max;x++) printf("#");
		printf(" %d \n", max);

	}




	return 0;
}
