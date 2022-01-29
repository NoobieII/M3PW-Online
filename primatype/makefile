#Lithio
#December 30, 2020

#Makefile

#necessary files to compile
OBJS = player.cpp primatype.cpp pwbutton.cpp pwengine.cpp pwobject.cpp pwtextbox.cpp space.cpp

#name of executable on WINDOWS
OBJ_NAME = primatype

#files to compile for testing the engine
TEST_ENGINE_OBJS = background.cpp ball.cpp compilation_test.cpp pwengine.cpp \
	pwobject.cpp

TEST_SPACE_OBJS = space.cpp space_test.cpp

TEST_CONTROLS_OBJS = -DPW_WATCH_CONTROLS pwengine.cpp pwobject.cpp space.cpp watch_controls.cpp 

TEST_ANIMATION_OBJS = pwengine.cpp pwobject.cpp space.cpp test_dragon.cpp test_animation.cpp

TEST_BUTTON_OBJS = test_button.cpp pwbutton.cpp pwengine.cpp pwobject.cpp space.cpp

TEST_TEXTBOX_OBJS = test_textbox.cpp pwtextbox.cpp pwengine.cpp pwobject.cpp space.cpp

#which compiler we're using
CC = g++

#compiler flags

#Windows
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
INCLUDE_PATHS = -Iinclude/windows/SDL2
LIBRARY_PATHS = -Llib/windows

#Linux
#LINKER_FLAGS = -lSDL2 -lSDL2_image
#INCLUDE_PATHS = -I/usr/local/include/SDL2/
#LIBRARY_PATHS = -L/usr/local/lib/


primatype : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME) 

test_engine: $(TEST_ENGINE_OBJS)
	$(CC) $(TEST_ENGINE_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) \
	$(LINKER_FLAGS) -o test_engine 

test_space: $(TEST_SPACE_OBJS)
	$(CC) $(TEST_SPACE_OBJS) -o test_space

test_controls: $(TEST_CONTROL_OBJS)
	$(CC) $(TEST_CONTROLS_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o test_controls

test_animation: $(TEST_ANIMATION_OBJS)
	$(CC) $(TEST_ANIMATION_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o test_animation

test_button: $(TEST_BUTTON_OBJS)
	$(CC) $(TEST_BUTTON_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o test_button

test_textbox: $(TEST_TEXTBOX_OBJS)
	$(CC) $(TEST_TEXTBOX_OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o test_textbox
