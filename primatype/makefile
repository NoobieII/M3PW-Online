#Lithio
#December 30, 2020

#Makefile

#necessary files to compile
OBJS = player.cpp primatype.cpp pwengine.cpp pwobject.cpp space.cpp

#name of executable on WINDOWS
OBJ_NAME = primatype

#files to compile for testing the engine
TEST_ENGINE_OBJS = background.cpp ball.cpp compilation_test.cpp pwengine.cpp \
	pwobject.cpp

TEST_SPACE_OBJS = space.cpp space_test.cpp

#which compiler we're using
CC = g++

#include paths for SDL on my WINDOWS computer
INCLUDE_PATHS = -IC:/MinGW/include/SDL2

#library paths for SDL on my WINDOWS computer
LIBRARY_PATHS = -LC:/MinGW/lib/SDL2

#compiler flags

#linker flags
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

primatype : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME) 

test_engine: $(TEST_ENGINE_OBJS)
	$(CC) $(TEST_ENGINE_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) \
	$(LINKER_FLAGS) -o test_engine 

test_space: $(TEST_SPACE_OBJS)
	$(CC) $(TEST_SPACE_OBJS) -o test_space
