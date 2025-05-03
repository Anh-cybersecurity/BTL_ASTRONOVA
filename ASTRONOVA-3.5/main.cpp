#include "Game.h"
#include "Menu.h"
#include "GameOverMenu.h"

using namespace std;

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init())
        return -1;

    Menu menu(game.getRenderer(), game);
    GameOverMenu gameOverMenu(game.getRenderer());

    bool running = true;

    while (running) {
        int option = menu.show();
        if (option == 1) {
            bool inGame = true;
            while (inGame) {
                game.reset();
                game.run();

                int goOption = gameOverMenu.show();
                if (goOption == 0) {
                    continue;
                } else if (goOption == 1) {
                    inGame = false;
                } else {
                    inGame = false;
                    running = false;
                }
            }
        } else {
            running = false;
        }
    }

    game.clean();
    return 0;
}
