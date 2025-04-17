#include "Menu.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;

Menu::Menu(SDL_Renderer* renderer) : renderer(renderer), selectedOption(0), backgroundTexture(nullptr), titleTexture(nullptr) {
    TTF_Init();
    font = TTF_OpenFont("Space.otf", 48);
    textColor = {255, 255, 255};
    options.push_back("Play");
    options.push_back("Quit");

    backgroundTexture = loadTexture("Space Background.png");
    titleTexture = renderTitle("ASTRONOVA");
}

Menu::~Menu() {
    TTF_CloseFont(font);
    TTF_Quit();
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (titleTexture) SDL_DestroyTexture(titleTexture);
}

SDL_Texture* Menu::loadTexture(const string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        cerr << "Failed to load " << path << ": " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

SDL_Texture* Menu::renderTitle(const string& titleText) {
    SDL_Color titleColor = {255, 215, 0};
    SDL_Surface* surface = TTF_RenderText_Solid(font, titleText.c_str(), titleColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void Menu::renderText(const string& text, int x, int y, bool selected) {
    SDL_Color color = selected ? SDL_Color{0, 0, 255} : textColor;
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int Menu::show() {
    SDL_Event event;
    bool running = true;

    while (running) {
        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            SDL_Rect bgRect = {0, 0, 1000, 800};
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
        }

        if (titleTexture) {
            SDL_Rect titleRect = {300, 100, 400, 100};
            SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
        }

        for (size_t i = 0; i < options.size(); ++i) {
            renderText(options[i], 350, 300 + i * 60, i == selectedOption);
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                return 0;

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selectedOption = (selectedOption - 1 + options.size()) % options.size();
                        break;
                    case SDLK_DOWN:
                        selectedOption = (selectedOption + 1) % options.size();
                        break;
                    case SDLK_RETURN:
                        return selectedOption == 0 ? 1 : 0;
                }
            }
        }

        SDL_Delay(16);
    }

    return 0;
}
