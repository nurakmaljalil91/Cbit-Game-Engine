#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

#TARGET specifies which files to compile as part of the project
TARGET = main

#CC specifies which compiler we're using
CC = g++

STD11 = -std=c++11
STD17 = -std=c++17
#Game object
CORE = Game.cpp cthead.cpp

#Math Object
MATH = math\Math.cpp

#Component
ECS = ecs\Entity.cpp ecs\Component.cpp ecs\Transform.cpp

#Object-Oriented Classic 
OOA = object\Actor.cpp

#Sandbox object
SAND = sandbox\Player.cpp

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I..\vendors\SDL2-2.0.9\i686-w64-mingw32\include -I..\vendors\jsoncpp\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L..\vendors\SDL2-2.0.9\i686-w64-mingw32\lib -L..\vendors\jsoncpp\build\src\lib_json

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lOpenGL32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -ljsoncpp

#This is the target that compiles our executable
all : $(TARGET).cpp
	$(CC) $(STD11) $(TARGET).cpp $(CORE) $(MATH) $(OOA) $(SAND) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o ../bin/$(TARGET)