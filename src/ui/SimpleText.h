#ifndef SIMPLE_TEXT
#define SIMPLE_TEXT

#include <string>
#include <unordered_map>
#include <vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "../math/Math.h"

class SimpleText
{
private:
    std::unordered_map<int, TTF_Font *> font_data;
    std::string text;
    SDL_Color color;
    int iw, ih; 

public:
    SimpleText(const std::string &file_path, SDL_Color _color); // default Constructor
    virtual ~SimpleText();                                      // default Deconstructor

    void Write(std::string _text);                                    // Write the text your want
    SDL_Texture *Render_Text( SDL_Renderer *renderer,int _size = 30); // Render the text
   
    void Clear();

}; // class SimpleText

#endif // SIMPLE_TEXT