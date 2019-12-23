#include <windows.h>
#include <string>
#include <iostream>
using namespace std;;

string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}

int main() {
    cout << "my directory is " << ExePath() << "\n";
}