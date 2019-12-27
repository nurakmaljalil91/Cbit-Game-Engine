#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

.PHONY: all clean

# Define required raylib variables
PROJECT_NAME       ?= main
CBIT_VERSION       ?= 1.0.0
SDL2_VERSION       ?= 2.5.0
OPENGL_VERSION     ?= 
MINGW_VERSION      ?= 8.2.0

# Define compiler path on Windows
COMPILER_PATH      ?= C:\MinGW\bin

# Define default options
# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM           ?= PLATFORM_DESKTOP

#TARGET specifies which files to compile as part of the project
TARGET = main

#CC specifies which compiler we're using
CC = g++

STD11 = -std=c++11
STD14 = -std=c++14
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
SAND = sandbox2D\Player.cpp

# Define all source files required
PROJECT_SOURCE_FILES ?= \
	main.cpp \
	Game.cpp \
	cthead.cpp \
	math/Math.cpp \
	utils/ResourcesDirectory.cpp \
	ui/SimpleText.cpp \
	ecs/Entity.cpp \
	ecs/Component.cpp \
	ecs/EntitiesManager.cpp \
	ecs/Image2D.cpp \
	object/Actor.cpp \
	sandbox2D/Player.cpp \
	scene/Scene.cpp \
	scene/SceneManager.cpp \
	scene/SplashScreenScene.cpp \
	sandbox2D/PlayScene.cpp 
	   							
	
# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I..\vendors\SDL2-2.0.10\i686-w64-mingw32\include -I..\vendors\jsoncpp\include

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L..\vendors\SDL2-2.0.10\i686-w64-mingw32\lib -L..\vendors\jsoncpp\build\src\lib_json

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lOpenGL32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljsoncpp

#This is the target that compiles our executable
all : $(TARGET).cpp
	$(CC) $(STD14) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o ../bin/$(TARGET)

# This is to clean .o and exe
clean:
	$(RM) *.o *~ ../bin/$(TARGET)