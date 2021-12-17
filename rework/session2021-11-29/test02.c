#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void readEventLine(FILE * in, char * data) { //read input key stream
int i;
for(i = 0; i <= 15; i++) { //each key press will trigger 16 characters of data, describing the event
data[i] = (char) fgetc(in);
}
}

int main(){
	FILE * input;
	char data[16];
	int i;
	char filename[100];

	printf("Waiting for 5 seconds - try pressing something.\n");
	usleep(5000000);
	
	//try opening event0 to event9 ... to see if that works
	for(i = 0; i < 100; ++i){
		sprintf(filename, "/dev/input/event%d", i);
		input = fopen(filename, "rb");
		if(input == NULL){
			printf("Could not open %s\n", filename);
		}
		else{
			printf("Successfully opened %s\n", filename);
			fclose(input);
		}
	}
	
	if(input == NULL){
		return 0;
	}

	//readEventLine(input, data);
	return 0;
}

