// Game.h
#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Alien.h" 
#include <vector>


class Game {
private:
    char gameBoard[BOARD_HEIGHT][BOARD_WIDTH];
    Player player;
    std::vector<Alien> aliens;
    int gameSpeed;
    int score;
    bool gameOverState;
    int alienDirection;
    bool aliensNeedToShiftDown;

    void moveAliens();
    void checkCollisions();
    void drawGameBoard();
    void handleInput();
    void playerAliensShoot();
    void checkGameOver();
    void findAlienIndex(int x);

public:
    Game();
    void run();
};

#endif // GAME_H