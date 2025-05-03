#include "Menu.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;


Menu::Menu(SDL_Renderer* renderer, Game& game) : renderer(renderer), game(game), selectedOption(0), backgroundTexture(nullptr) {
    TTF_Init();
    font = TTF_OpenFont("Gameplay.ttf", 48);
    textColor = {255, 255, 255};
    options.push_back("Play");
    options.push_back("Quit");

    backgroundTexture = loadTexture("MenuBG.png");
}

Menu::~Menu() {
    TTF_CloseFont(font);
    TTF_Quit();
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
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

void Menu::renderText(const string& text, int x, int y, bool selected) {
    SDL_Color color = selected ? SDL_Color{150, 100, 255} : textColor;
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

    int hs = game.getHighScore();
    string hsText = "Highscore: " + to_string(hs);

    SDL_Color textColor = {250, 255, 0};
    SDL_Surface* hsSurface = TTF_RenderText_Solid(font, hsText.c_str(), textColor);
    SDL_Texture* hsTexture = SDL_CreateTextureFromSurface(renderer, hsSurface);

    int hsW, hsH;
    SDL_QueryTexture(hsTexture, NULL, NULL, &hsW, &hsH);

    while (running) {
        SDL_RenderClear(renderer);

        if (backgroundTexture) {
            SDL_Rect bgRect = {0, 0, 1000, 800};
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
        }

    SDL_Rect hsRect = {SCREEN_WIDTH / 2 - hsW / 2, SCREEN_HEIGHT - hsH - 420, hsW, hsH};
    SDL_RenderCopy(renderer, hsTexture, NULL, &hsRect);

    int centerX = 950 / 2;
    int centerY = 1000 / 2;
    int spacing = 70;
    int totalHeight = options.size() * spacing;

    for (size_t i = 0; i < options.size(); ++i) {
        int x = centerX - 50;
        int y = centerY - totalHeight / 2 + i * spacing;
        renderText(options[i], x, y, i == selectedOption);
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
                        SDL_FreeSurface(hsSurface);
                        SDL_DestroyTexture(hsTexture);

                        return selectedOption == 0 ? 1 : 0;
                }
            }
        }
        SDL_Delay(16);
    }

    return 0;
}
