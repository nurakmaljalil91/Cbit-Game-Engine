#include <iostream>
#include <string>

void Clear_String_Copy(std::string copy)
{
    copy.clear();
}

void Clear_String_By_Reference(std::string &reference)
{
    reference.clear();
}

int main(int argc, char const *argv[])
{
    std::string word = "Please delete this";
    std::cout << "1. Before anything happen " << word << std::endl;
    Clear_String_Copy(word);
    std::cout << "2. Clear the copy " << word << std::endl;
    Clear_String_By_Reference(word);
    std::cout << "3. Clear by reference " << word << std::endl;
    return 0;
}
