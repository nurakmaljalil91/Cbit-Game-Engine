#ifndef SIMPLE_TEXT
#define SIMPLE_TEXT

#include <string>
#include <unordered_map>
#include "SDL2/SDL_ttf.h"
#include "../math/Math.h"

class SimpleText
{
private:
    std::unordered_map<int, TTF_Font *> fonts_data; // map of point sizes to font data
    std::string text;

public:
    SimpleText();          // default Constructor
    virtual ~SimpleText(); // default Deconstructor

    void Write(std::string _text);
    void Render_Text();

}; // class SimpleText

#endif // SIMPLE_TEXT