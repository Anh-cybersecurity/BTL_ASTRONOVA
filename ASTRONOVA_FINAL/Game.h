#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Boss.h"

using namespace std;

class Game {
public:
	Game();
	~Game();
	bool init();
	void run();
	void handleEvents();
	void update(float deltaTime);
	void render();
	void clean();
	void reset();
	bool checkCollision(SDL_Rect a, SDL_Rect b);
	void drawHealthBar(int x, int y, int width, int height, int health, int maxHealth);
	SDL_Renderer* getRenderer() {
		return renderer;
	}
	void renderScore();
	int getScore() const { return score; }
	int getHighScore() const { return highscore; }
	void loadHighScore();
	void saveHighScore();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* shipTexture;
	SDL_Texture* backgroundTexture;
	SDL_Texture* bulletTexture;
	SDL_Texture* EbulletTexture;
	SDL_Texture* enemyTexture;
	TTF_Font* font;
	SDL_Texture* scoreTexture;
	SDL_Texture* bossTexture;
	Mix_Chunk* shootSound;
	Mix_Chunk* explosion;
	bool isRunning;
	Uint32 startTicks;
	int backgroundY = 0;
	int backgroundSpeed = 2;
	Player* player;
	vector<Bullet> bullets;
	vector<Bullet> enemyBullets;
	int fireCooldown;
	vector<Enemy> enemies;
	int enemySpawnCooldown;
	int score;
	int highscore;
	Boss* boss;
	bool bossAppeared = false;
	int bossThreshold;
};

#endif

