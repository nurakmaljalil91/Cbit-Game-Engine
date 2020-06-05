#Copyright Notice:
#The files within this zip file are copyrighted by Lazy Foo' Productions (2004-2014)
#and may not be redistributed without written permission.

.PHONY: all clean

# Define required raylib variables
PROJECT_NAME       ?= main
CBIT_VERSION       ?= 1.0.1
SDL2_VERSION       ?= 2.0.10
OPENGL_VERSION     ?= 3.3.0
MINGW_VERSION      ?= 7.3.0

# Define compiler path on Windows
COMPILER_PATH      ?= C:\MinGW\bin

# Define default options
# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM           ?= PLATFORM_DESKTOP

#TARGET specifies which files to compile as part of the project
TARGET = main

#CC specifies which compiler we're using
CC = g++

# G++ Compiler version
STD11 = -std=c++11
STD14 = -std=c++14
STD17 = -std=c++17

# Define all source files required
PROJECT_SOURCE_FILES ?= \
	src/main.cpp 

# Define all object files from source files
OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

SDL2_INC = -Ivendors\SDL2-2.0.10\i686-w64-mingw32\include
SDL2_LIB = -Lvendors\SDL2-2.0.10\i686-w64-mingw32\lib
JSONCPP_INC = -Ivendors\jsoncpp\include
JSONCPP_LIB = -Lvendors\jsoncpp\build\src\lib_json
GLEW_INC = -Ivendors\glew-2.1.0\include
GLEW_LIB = -Lvendors\glew-2.1.0\lib\Release\Win32
SOIL_INC = -Ivendors\Simple-OpenGL-Image-Library
SOIL_LIB = -Lvendors\Simple-OpenGL-Image-Library\lib
RAPIDJSON_INC = -Ivendors\rapidjson\include

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = $(SDL2_INC) $(GLEW_INC) $(JSONCPP_INC) $(SOIL_INC) $(RAPIDJSON_INC)

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = $(SDL2_LIB) $(GLEW_LIB) $(JSONCPP_LIB) $(SOIL_LIB)
#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lglu32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -ljsoncpp -lSOIL -lopengl32 -lgdi32 -lglew32 

#This is the target that compiles our executable
all : src/$(TARGET).cpp
	$(CC) $(STD14) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o bin/$(TARGET)

# This is to clean .o and exe
clean:
	$(RM) *.o *~ bin/$(TARGET)