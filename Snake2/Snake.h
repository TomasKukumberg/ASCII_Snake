#pragma once

#include <deque>
#include <conio.h>

#include "Position.h"
#include "Direction.h"
#include "Food.h"
#include "Macros.h"
#include "Console.h"

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

