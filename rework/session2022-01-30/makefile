#Lithio
#February 25, 2022

OBJS = camera.cpp main.cpp map.cpp mob.cpp player.cpp pwengine.cpp pwobject.cpp pwvec2.cpp space.cpp

OBJ_NAME = prototype

INCLUDE_PATHS_WINDOWS = -I../../primatype/include/windows/SDL2
INCLUDE_PATHS_LINUX = -I/usr/local/include/SDL2/

LINKER_FLAGS_WINDOWS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
LINKER_FLAGS_LINUX = -lSDL2 -lSDL2_image

LIBRARY_PATHS_WINDOWS = -L../../primatype/lib/windows
LIBRARY_PATHS_LINUX = -L/usr/local/lib/

CC = g++

build_linux: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS_LINUX) $(LINKER_FLAGS_LINUX) $(LIBRARY_PATHS_LINUX) -o $(OBJ_NAME)

build_windows: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS_WINDOWS) $(LINKER_FLAGS_WINDOWS) $(LIBRARY_PATHS_WINDOWS) -o $(OBJ_NAME)
