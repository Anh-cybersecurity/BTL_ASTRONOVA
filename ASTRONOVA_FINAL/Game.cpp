#include "Game.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;
const int FIRE_RATE = 10;
const int ENEMY_COOLDOWN = 200;
const int FPS = 60;
const int FRAMERATE = 1000 / FPS;

Game::Game()
	: window(nullptr),
	renderer(nullptr),
	shipTexture(nullptr),
	backgroundTexture(nullptr),
	bulletTexture(nullptr),
	enemyTexture(nullptr),
	isRunning(false),
	player(nullptr),
	fireCooldown(0),
	enemySpawnCooldown(0),
	score(0),
	highscore(0),
	boss(nullptr),
	bossAppeared(false),
	bossThreshold(1000) {
}



Game::~Game() {
	clean();
}

bool Game::init() {
	srand(time(0));
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << SDL_GetError();
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		cerr << IMG_GetError();
		SDL_Quit();
		return false;
	}

	window = SDL_CreateWindow("Astro Nova", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!window) {
		cerr << SDL_GetError();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		cerr << SDL_GetError();
		return false;
	}

	SDL_Surface* loadedSurface = IMG_Load("tiny_ship10.png");
	if (!loadedSurface) {
		cerr << IMG_GetError();
	}
	else {
		shipTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface);
	}
	if (!shipTexture) {
		cerr << SDL_GetError();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	SDL_Surface* bg = IMG_Load("Space Background.png");
	if (!bg) {
		cerr << "Failed to load background image! Error: " << IMG_GetError() << endl;
	}
	else {
		backgroundTexture = SDL_CreateTextureFromSurface(renderer, bg);
		SDL_FreeSurface(bg);
	}

	if (!backgroundTexture) {
		cerr << "Failed to create texture from background image! Error: " << SDL_GetError() << endl;
	}

	SDL_Surface* bullet = IMG_Load("bullet.png");
	if (!bullet) {
		cerr << "Failed to load bullet image! Error: " << IMG_GetError() << endl;
	}
	else {
		bulletTexture = SDL_CreateTextureFromSurface(renderer, bullet);
		SDL_FreeSurface(bullet);
	}
	if (!bulletTexture) {
		cerr << "Failed to create texture from bullet image! Error: " << SDL_GetError() << endl;
	}

	SDL_Surface* Ebullet = IMG_Load("Enemy bullet.png");
	if (!Ebullet) {
		cerr << "Failed to load bullet image! Error: " << IMG_GetError() << endl;
	}
	else {
		EbulletTexture = SDL_CreateTextureFromSurface(renderer, Ebullet);
		SDL_FreeSurface(Ebullet);
	}
	if (!EbulletTexture) {
		cerr << "Failed to create texture from bullet image! Error: " << SDL_GetError() << endl;
	}

	SDL_Surface* enemySurface = IMG_Load("enemy.png");
	if (!enemySurface) {
		cerr << "Failed to load enemy image! Error: " << IMG_GetError() << endl;
	}
	else {
		enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
		SDL_FreeSurface(enemySurface);
	}
	enemySpawnCooldown = ENEMY_COOLDOWN;

	SDL_Surface* bossSurface = IMG_Load("Agis.png");
	if (!bossSurface) {
		cerr << "Failed to load boss image! Error: " << IMG_GetError() << endl;
	}
	else {
		bossTexture = SDL_CreateTextureFromSurface(renderer, bossSurface);
		SDL_FreeSurface(bossSurface);
	}

	if (TTF_Init() == -1) {
		cerr << "SDL_ttf could not initialize! Error: " << TTF_GetError() << endl;
		return false;
	}

	font = TTF_OpenFont("Gameplay.ttf", 32);
	if (!font) {
		cerr << "Failed to load font! Error: " << TTF_GetError() << endl;
		return false;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		cerr << "SDL_mixer could not initialize! Error: " << Mix_GetError() << endl;
		return false;
	}

	shootSound = Mix_LoadWAV("laser-shot-fx.wav");
	if (!shootSound) {
		cerr << "Failed to load shoot sound! Error: " << Mix_GetError() << endl;
	}

	explosion = Mix_LoadWAV("explosion.mp3");
	if (!explosion) {
		cerr << "Failed to load explosion sound! Error: " << Mix_GetError() << endl;
	}

	player = new Player(1000 / 2 - Player::WIDTH / 2, 800 - Player::HEIGHT - 20);
	isRunning = true;

	loadHighScore();

	return true;

}

void Game::run() {
	Uint32 lastFrameTime = SDL_GetTicks();
	float deltaTime;

	while (isRunning) {
		Uint32 currentTime = SDL_GetTicks();
		deltaTime = (currentTime - lastFrameTime) / 1000.0f;
		lastFrameTime = currentTime;

		handleEvents();
		update(deltaTime);
		render();

		Uint32 frameTime = SDL_GetTicks() - currentTime;
		if (frameTime < FRAMERATE) {
			SDL_Delay(FRAMERATE - frameTime);
		}
	}

	saveHighScore();
}

void Game::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			isRunning = false;
	}
	player->handleInput();
}

bool Game::checkCollision(SDL_Rect a, SDL_Rect b) {
	return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

void Game::update(float deltaTime) {
	backgroundY += backgroundSpeed;
	if (backgroundY >= SCREEN_HEIGHT) {
		backgroundY = 0;
	}

	player->update();

	if (fireCooldown > 0) {
		fireCooldown--;
	}

	if (player->isFiring() && fireCooldown == 0) {
		int bulletX = player->getX() + Player::WIDTH / 2 - Bullet::BULLET_WIDTH / 2;
		int bulletY = player->getY();
		bullets.push_back(Bullet(bulletX, bulletY, -1));
		fireCooldown = FIRE_RATE;

		Mix_PlayChannel(-1, shootSound, 0);
	}
	for (auto& bullet : bullets) {
		bullet.update();
	}
	bullets.erase(remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.isOutOfBounds(); }), bullets.end());

	if (enemySpawnCooldown <= 0) {
		int spawnExtra = (SDL_GetTicks() - startTicks) / 20000;
		int baseEnemies = 1;
		int numEnemies = rand() % (baseEnemies + spawnExtra) + 1;
		for (int i = 0; i < numEnemies; i++) {
			int spawnX = rand() % (SCREEN_WIDTH - Enemy::ENEMY_WIDTH);
			SDL_Rect newRect = { spawnX, -Enemy::ENEMY_HEIGHT, Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };

			bool overlap = false;

			for (const auto& enemy : enemies) {
				SDL_Rect existingRect = { enemy.getX(), enemy.getY(), Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };
				if (checkCollision(newRect, existingRect)) {
					overlap = true;
					break;
				}
			}

			if (!overlap) {
				enemies.push_back(Enemy(spawnX, -Enemy::ENEMY_HEIGHT));
			}
		}
		enemySpawnCooldown = ENEMY_COOLDOWN;
	}
	else {
		enemySpawnCooldown--;
	}

	for (auto& enemy : enemies) {
		enemy.update();
		Uint32 currentTime = SDL_GetTicks();
		if (currentTime - enemy.lastShootTime > 1500) {
			int bulletX = enemy.getX() + Enemy::ENEMY_WIDTH / 2 - Bullet::BULLET_WIDTH / 2;
			int bulletY = enemy.getY() + Enemy::ENEMY_HEIGHT;
			enemy.lastShootTime = currentTime;
			enemyBullets.push_back(Bullet(bulletX, bulletY, 1));
		}
	}

	for (auto& bullet : enemyBullets) {
		bullet.update();
	}

	enemyBullets.erase(remove_if(enemyBullets.begin(), enemyBullets.end(),
		[](Bullet& b) { return b.isOutOfBounds(); }), enemyBullets.end());

	for (auto it = enemies.begin(); it != enemies.end();) {
		if (it->isOutOfBounds()) {
			isRunning = false;
			cout << "Enemy escaped! Game Over!" << endl;
			break;
		}
		else {
			++it;
		}
	}

	for (auto bullet = bullets.begin(); bullet != bullets.end();) {
		bool bulletHit = false;

		for (auto enemy = enemies.begin(); enemy != enemies.end();) {
			SDL_Rect bulletRect = { bullet->getX(), bullet->getY(), Bullet::BULLET_WIDTH, Bullet::BULLET_HEIGHT };
			SDL_Rect enemyRect = { enemy->getX(), enemy->getY(), Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };

			if (checkCollision(bulletRect, enemyRect)) {
				enemy = enemies.erase(enemy);
				score += 100;
				bulletHit = true;
				if (score > highscore) {
					highscore = score;
				}

				Mix_PlayChannel(-1, explosion, 0);

				break;
			}
			else {
				++enemy;
			}
		}

		if (bulletHit) {
			bullet = bullets.erase(bullet);
		}
		else {
			++bullet;
		}
	}

	SDL_Rect playerRect = { player->getX(), player->getY(), Player::WIDTH, Player::HEIGHT };

	for (auto enemy = enemies.begin(); enemy != enemies.end();) {
		SDL_Rect enemyRect = { enemy->getX(), enemy->getY(), Enemy::ENEMY_WIDTH, Enemy::ENEMY_HEIGHT };

		if (checkCollision(playerRect, enemyRect)) {
			player->health -= 10;
			enemy = enemies.erase(enemy);

			Mix_PlayChannel(-1, explosion, 0);

			break;
		}

		if (player->health < 1) {
			cout << "You have crashed! Game Over!" << endl;
			isRunning = false;
			return;
		}
		else {
			enemy++;
		}
	}

	for (auto bullet = enemyBullets.begin(); bullet != enemyBullets.end();) {
		bullet->update();

		SDL_Rect bulletRect = { bullet->getX(), bullet->getY(), Bullet::BULLET_WIDTH, Bullet::BULLET_HEIGHT };
		SDL_Rect playerRect = { player->getX(), player->getY(), Player::WIDTH, Player::HEIGHT };

		if (checkCollision(bulletRect, playerRect)) {
			player->health -= 10;
			bullet = enemyBullets.erase(bullet);

			Mix_PlayChannel(-1, explosion, 0);

			if (player->health <= 0) {
				cout << "You were hit by enemy fire! Game Over!" << endl;
				isRunning = false;
				return;
			}
		}
		else if (bullet->isOutOfBounds()) {
			bullet = enemyBullets.erase(bullet);
		}
		else {
			++bullet;
		}
	}
	if (!bossAppeared && score >= bossThreshold) {
		boss = new Boss(SCREEN_WIDTH / 2 - 64, -150);
		bossAppeared = true;
	}

	if (bossAppeared && boss) {
		boss->update();
		boss->fireLaser(enemyBullets);
	}

	if (boss && bossAppeared) {
		SDL_Rect bossRect = { boss->getX(), boss->getY(), 250, 250 };
		for (auto bullet = bullets.begin(); bullet != bullets.end();) {
			SDL_Rect bulletRect = { bullet->getX(), bullet->getY(), Bullet::BULLET_WIDTH, Bullet::BULLET_HEIGHT };
			if (checkCollision(bulletRect, bossRect)) {
				boss->health -= 5;
				Mix_PlayChannel(-1, explosion, 0);
				bullet = bullets.erase(bullet);
				if (boss->health <= 0) {
					delete boss;
					boss = nullptr;
					bossAppeared = false;
					score += 1000;
					bossThreshold += 2000;
				}
				break;
			}
			else {
				++bullet;
			}
		}

		SDL_Rect playerRect = { player->getX(), player->getY(), Player::WIDTH, Player::HEIGHT };

		if (checkCollision(bossRect, playerRect)) {
			player->health -= 20;

			Mix_PlayChannel(-1, explosion, 0);

			if (player->health <= 0) {
				cout << "You collided with the boss! Game Over!" << endl;
				isRunning = false;
				return;
			}

		}
	}
}



void Game::drawHealthBar(int x, int y, int width, int height, int health, int maxHealth) {
	SDL_Rect bgRect = { x, y, width, height };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &bgRect);

	float healthPercentage = (float)health / maxHealth;
	SDL_Rect healthRect = { x, y, (int)(width * healthPercentage), height };
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &healthRect);
}

void Game::renderScore() {
	if (font) {
		SDL_Color textColor = { 255, 255, 255, 255 };
		string scoreText = "Score: " + to_string(score);

		SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
		if (textSurface) {
			SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
			if (textTexture) {
				SDL_Rect textRect = { 20, 20, textSurface->w, textSurface->h };
				SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
				SDL_DestroyTexture(textTexture);
			}
			SDL_FreeSurface(textSurface);
		}
	}
}

void Game::loadHighScore() {
	ifstream inFile("highscore.txt");
	if (inFile.is_open()) {
		inFile >> highscore;
		inFile.close();
	}
	else {
		highscore = 0;
	}
}

void Game::saveHighScore() {
	if (score > highscore) {
		highscore = score;
		ofstream outFile("highscore.txt");
		if (outFile.is_open()) {
			outFile << highscore;
			outFile.close();
		}
	}
}

void Game::render() {
	SDL_Rect bg1 = { 0, backgroundY, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect bg2 = { 0, backgroundY - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };

	SDL_RenderCopy(renderer, backgroundTexture, NULL, &bg1);
	SDL_RenderCopy(renderer, backgroundTexture, NULL, &bg2);

	player->render(renderer, shipTexture);

	drawHealthBar(player->getX(), player->getY() - 10, Player::WIDTH, 5, player->health, 100);


	for (auto& bullet : bullets) {
		bullet.render(renderer, bulletTexture);
	}
	for (auto& bullet : enemyBullets) {
		bullet.render(renderer, EbulletTexture);
	}

	for (auto& enemy : enemies) {
		enemy.render(renderer, enemyTexture);
	}

	renderScore();

	if (bossAppeared && boss) {
		boss->render(renderer, bossTexture);
		drawHealthBar(boss->getX(), boss->getY() - 10, 250, 10, boss->health, 100);
	}

	SDL_RenderPresent(renderer);

}

void Game::reset() {
	isRunning = true;
	startTicks = SDL_GetTicks();
	score = 0;
	player->reset();
	enemies.clear();
	bullets.clear();
	enemyBullets.clear();
	if (boss) {
		delete boss;
		boss = nullptr;
	}
	bossAppeared = false;
}

void Game::clean() {
	if (player) {
		delete player;
		player = nullptr;
	}

	if (bossTexture) {
		SDL_DestroyTexture(bossTexture);
		bossTexture = nullptr;
	}

	if (shipTexture) {
		SDL_DestroyTexture(shipTexture);
		shipTexture = nullptr;
	}

	if (bulletTexture) {
		SDL_DestroyTexture(bulletTexture);
		bulletTexture = nullptr;
	}

	if (enemyTexture) {
		SDL_DestroyTexture(enemyTexture);
		enemyTexture = nullptr;
	}

	if (backgroundTexture) {
		SDL_DestroyTexture(backgroundTexture);
		backgroundTexture = nullptr;
	}

	if (EbulletTexture) {
		SDL_DestroyTexture(EbulletTexture);
		EbulletTexture = nullptr;
	}

	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	if (font) {
		TTF_CloseFont(font);
		font = nullptr;
	}

	if (shootSound) {
		Mix_FreeChunk(shootSound);
		shootSound = nullptr;
	}

	if (explosion) {
		Mix_FreeChunk(explosion);
		explosion = nullptr;
	}

	Mix_CloseAudio();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

