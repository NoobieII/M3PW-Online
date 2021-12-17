//Lithio (the developer's pseudonym)
//November 28, 2021

//Test program for the engine's current features (no socket testing)

#include "pwengine.hpp"

int main(int argc, char *argv[]){
	PWEngine *engine = new PWEngine;
	int start_time;
	int deltatime;
	
	//test without socket capabilities
	//"Hello" argument is for window caption. It is not yet implemented.
	if(engine->initialize(80, 30, 3, "Hello") == -1){
		printf("Could not initialize engine.\n");
		return 0;
	}
	
	start_time = engine->ticks();
	deltatime = engine->deltatime();
	
	printf(" Time (ticks) is %d\n", start_time);
	printf(" deltatime is %d milliseconds\n", deltatime);
	
	//test wait
	printf(" Testing wait for 1 second (3 ticks/second).\n");
	engine->sleep_until_next_frame();
	printf(" Time (ticks) is %d\n", engine->ticks());
	printf(" deltatime is %d milliseconds\n", engine->deltatime());
	engine->sleep_until_next_frame();
	printf(" Time (ticks) is %d\n", engine->ticks());
	printf(" deltatime is %d milliseconds\n", engine->deltatime());
	engine->sleep_until_next_frame();
	printf(" Time (ticks) is %d\n", engine->ticks());
	printf(" deltatime is %d milliseconds\n\n", engine->deltatime());
	
	//test ticks
	printf(" Time (ticks) is %d\n", engine->ticks());
	printf(" ticks expected: 3\n");
	if(engine->ticks() != 3){
		printf("Ticks are incorrect!\n");
	}
	
	//test update
	printf(" calling update_all()...\n");
	engine->update_all();
	printf(" deltatime is %d milliseconds\n\n", engine->deltatime());
	
	printf(" calling update_all()...\n");
	engine->update_all();
	printf(" deltatime is %d milliseconds\n\n", engine->deltatime());
	
	printf(" calling update_all()...\n\n");
	engine->update_all();
	printf(" deltatime is %d milliseconds\n\n", engine->deltatime());
	
	//test deltatime
	if(engine->deltatime() != 1000/3){
		printf("deltatime is incorrect!\n");
	}
	
	//end of program
	return 0;
}

	
	