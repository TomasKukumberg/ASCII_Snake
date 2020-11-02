#pragma once

#include <deque>
#include <conio.h>

#include "position.hpp"
#include "direction.hpp"
#include "food.hpp"
#include "macros.hpp"
#include "console.hpp"

class Snake {
private:
    std::deque<Position> body;
    Direction direction = Direction::LEFT;
public:
    Snake();
    Direction getDirection();
    void setDirection(Direction direction);
    std::deque<Position> getBody();
    bool eatItself(Position newPosition);
    bool crashIntoWalls(Position nextMove);
    Position getNextMove();
    bool canMove();
    void move(Food& food, Console& console);
    void changeDirection();
    bool foodNearby(Food& food);
    void redraw(Food& food, Console& console);
};

