#include "SimpleText.h"

SimpleText::SimpleText(const std::string &file_path, SDL_Color _color)
{
    // support these font sizes
    std::vector<int> font_sizes = {
        8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
        30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56,
        60, 64, 68, 72};

    // Call TFF_OpenFont once per every font size
    for (auto &size : font_sizes)
    {
        TTF_Font *font = TTF_OpenFont(file_path.c_str(), size);
        if (font == nullptr)
        {
            SDL_Log("[ERROR] Failed to load font %s in size %d", file_path.c_str(), size);
        }
        font_data.emplace(size, font);
    }

    text = "NULL";

    color = _color;
}

SimpleText::~SimpleText() {}

void SimpleText::Write(std::string _text)
{
    text = _text;
}

SDL_Texture *SimpleText::Render_Text(SDL_Renderer *renderer, int _size)
{
    SDL_Texture *texture = nullptr;

    // Find the font data for this size
    auto iter = font_data.find(_size);

    if (iter != font_data.end())
    {
        TTF_Font *font = iter->second;
        // Draw this to a surface (blended for alpha)
        SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);

        if (surface != nullptr)
        {
            // Convert from surface to texture
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
    else
    {
        SDL_Log("[ERROR] Size %d is unsupported", _size);
    }

    return texture;
}

void SimpleText::Clear()
{
    for (auto &font : font_data)
    {
        TTF_CloseFont(font.second);
    }
}