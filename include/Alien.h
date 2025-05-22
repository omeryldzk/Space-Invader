#include "Position.h"
#include "Bullet.h"
#include <cstdlib>  
#ifndef ALIEN_H
#define ALIEN_H  
class Alien {
public:
    Position pos;
    Bullet alienBullet;
    char symbol;
    bool alive;
    bool canShoot;
    static int alienCount;



    Alien(int startX, int startY, bool canShoot = false)
        : pos{startX, startY}, symbol('X'), alive(true), canShoot(canShoot) {}

    void move(int dx, int dy) {
        pos.x += dx;
        pos.y += dy;
    }

    void destroy() {
        alive = false;
    }

    bool isAlive() const {
        return alive;
    }

    Position getPosition() const {
        return pos;
    }

    char getSymbol() const {
        return symbol;
    }

    void tryToShoot() {
        if(alive && std::rand() % 100 < 5 && canShoot) { // 5% chance to shoot
            if (!alienBullet.isActive()) {
                alienBullet.activate(pos.x, pos.y + 1, 'v', 1); // Shoot down
            }
        }
    }

    void setCanShoot(bool canShoot) {
        this->canShoot = canShoot;
    }

    void updateBullet() {
        if (alienBullet.isActive()) {
            alienBullet.update();
        }
    }
    void resetBullet() {
        alienBullet.deactivate();
    }

};
#endif // ALIEN_H