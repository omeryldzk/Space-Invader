#include "Position.h"
#include "Constants.h"
#ifndef BULLET_H
#define BULLET_H

class Bullet {
public:
    Position pos;
    char symbol;
    bool active;
    int directionY; // -1 for up (player), 1 for down (alien)

    Bullet(int startX = 0, int startY = 0, char sym = ' ', int dirY = 0)
        : pos{startX, startY}, symbol(sym), active(false), directionY(dirY) {}

    void activate(int startX, int startY, char sym, int dirY) {
        pos = {startX, startY};
        symbol = sym;
        directionY = dirY;
        active = true;
    }

    void update() {
        if (active) {
            pos.y += directionY;
            if (pos.y < 0 || pos.y >= BOARD_HEIGHT) {
                active = false;
            }
        }
    }

    bool isActive() const {
        return active;
    }

    Position getPosition() const {
        return pos;
    }

    char getSymbol() const {
        return symbol;
    }

    void deactivate() {
        active = false;
    }
};
#endif // BULLET_H