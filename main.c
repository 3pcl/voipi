#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>

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

int pa_simple_write( pa_simple* s,
		    const void * data,
		    size_t bytes,
		    int * error
		  );



	printf("Hello world bitch\n");

	
return 0;
}
