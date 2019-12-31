@echo off
echo [BAT] game is building 
cd ..
cd src
mingw32-make
echo [BAT] Done!
cd ..
cd build
