#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>


int main(int argc, char *argv){
	pa_simple *s;

	static const pa_sample_spec ss = {
		.format = PA_SAMPLE_U8,
		.rate = 8000,
		.channels = 1
	};

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

	uint8_t buf[100];
	int x;
	int error;
	srand(time(0));

	double sinpos=0;
	char beacon[] = { 1,0,1,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,1,0,1,0,1,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	while(1) {
		for(x=0; x<100; x++) { 
			buf[x] = (uint8_t)(rand()%135);
			if(beacon[(int)sinpos/130]) {
				buf[x] += 50*sin(sinpos); 
			}
			sinpos += 0.25;
			if(sinpos/130 >= sizeof(beacon)/sizeof(uint8_t)) sinpos =0; 
		}     // make some noise !!
	
			
		pa_simple_write(s, buf, 100, &error);
	}


	printf("Hello world\n");


	return 0;
}
