// Position.h
#ifndef POSITION_H
#define POSITION_H

struct Position {
    int x, y;

    Position(int x_val = 0, int y_val = 0) : x(x_val), y(y_val) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

#endif // POSITION_H