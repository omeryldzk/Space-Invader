#include "Position.h"
#include "Bullet.h"
#include "Constants.h"

#ifndef PLAYER_H
#define PLAYER_H
class Player {
public:
    Position pos;
    char symbol;
    int lives;
    Bullet bullet; // Player has one bullet

    Player(int startX, int startY, int initialLives)
        : pos{startX, startY}, symbol('O'), lives(initialLives) {}

    void moveLeft() {
        if (pos.x > 0) {
            pos.x--;
        }
    }

    void moveRight() {
        if (pos.x < BOARD_WIDTH - 1) {
            pos.x++;
        }
    }

    void shoot() {
        if (!bullet.isActive()) {
            bullet.activate(pos.x, pos.y - 1, '|', -1);
        }
    }

    void updateBullet() {
        bullet.update();
    }

    void takeDamage() {
        lives--;
    }

    bool isAlive() const {
        return lives > 0;
    }

    Position getPosition() const {
        return pos;
    }

    char getSymbol() const {
        return symbol;
    }

    const Bullet& getBullet() const {
        return bullet;
    }
    
    void resetBullet() { 
        bullet.deactivate();
    }
};
#endif // PLAYER_H