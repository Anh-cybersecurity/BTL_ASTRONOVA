#include "GameOverMenu.h"
#include <iostream>

using namespace std;

GameOverMenu::GameOverMenu(SDL_Renderer* renderer)
	: renderer(renderer), backgroundTexture(nullptr), selectedOption(0) {

	TTF_Init();
	font = TTF_OpenFont("Gameplay.ttf", 48);
	textColor = { 255, 255, 255 };

	options.push_back("Retry");
	options.push_back("Main Menu");
	options.push_back("Quit");

	backgroundTexture = loadTexture("GameOverBG.png");
}

GameOverMenu::~GameOverMenu() {
	TTF_CloseFont(font);
	TTF_Quit();
	if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
}

SDL_Texture* GameOverMenu::loadTexture(const string& path) {
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (!surface) {
		cerr << "Failed to load " << path << ": " << IMG_GetError() << endl;
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void GameOverMenu::renderText(const string& text, int x, int y, bool selected) {
	SDL_Color color = selected ? SDL_Color{ 150, 100, 255 } : textColor;
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect dst = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, nullptr, &dst);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

int GameOverMenu::show() {
	SDL_Event event;
	bool running = true;

	while (running) {
		SDL_RenderClear(renderer);

		if (backgroundTexture) {
			SDL_Rect bgRect = { 0, 0, 1000, 800 };
			SDL_RenderCopy(renderer, backgroundTexture, nullptr, &bgRect);
		}

		int centerX = 1000 / 2;
		int centerY = 1000 / 2;
		int spacing = 70;
		int totalHeight = options.size() * spacing;

		for (size_t i = 0; i < options.size(); ++i) {
			int x = centerX - 100;
			int y = centerY - totalHeight / 2 + i * spacing;
			renderText(options[i], x, y, i == selectedOption);
		}

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				return 2;

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					selectedOption = (selectedOption - 1 + options.size()) % options.size();
					break;
				case SDLK_DOWN:
					selectedOption = (selectedOption + 1) % options.size();
					break;
				case SDLK_RETURN:
					return selectedOption;

				}
			}
		}

		SDL_Delay(16);
	}

	return 2;
}

