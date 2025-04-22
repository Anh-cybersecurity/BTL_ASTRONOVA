#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>
#include <vector>

using namespace std;

class Menu {
public:
    Menu(SDL_Renderer* renderer);
    ~Menu();

    int show();
private:
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Color textColor;
    vector<string> options;
    int selectedOption;

    SDL_Texture* backgroundTexture;

    void renderText(const string& text, int x, int y, bool selected);

    SDL_Texture* loadTexture(const string& path);
};

#endif

