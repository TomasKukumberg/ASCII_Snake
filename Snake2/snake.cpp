#include "snake.hpp"

Snake::Snake() {
    body.push_front({ GAME_WIDTH / 2, GAME_HEIGHT / 2 });
    body.push_back({ GAME_WIDTH / 2 + 2, GAME_HEIGHT / 2 });
}

Direction Snake::getDirection() {
    return this->direction;
}

void Snake::setDirection(Direction direction) {
    this->direction = direction;
}

std::deque<Position> Snake::getBody() {
    return this->body;
}

bool Snake::eatItself(Position newPosition) {
    auto result = std::find(std::begin(body), std::end(body), newPosition);
    if (result == std::end(body)) {
        return false;
    } else {
        return true;
    }
}

bool Snake::crashIntoWalls(Position nextMove) {
    return (nextMove.getX() == LEFT_WALL) || (nextMove.getX() == RIGHT_WALL) ||
        (nextMove.getY() == BOTTOM_WALL) || (nextMove.getY() == UPPER_WALL);
}

Position Snake::getNextMove() {
    Position nextMove;
    switch (direction) {
    case Direction::DOWN:
        nextMove = { body[0].getX(), body[0].getY() + 1 };
        break;
    case Direction::UP:
        nextMove = { body[0].getX(), body[0].getY() - 1 };
        break;
    case Direction::LEFT:
        nextMove = { body[0].getX() - 2, body[0].getY() };
        break;
    case Direction::RIGHT:
        nextMove = { body[0].getX() + 2, body[0].getY() };
        break;
    }
    return nextMove;
}

bool Snake::canMove() {
    Position nextMove = getNextMove();
    if (crashIntoWalls(nextMove)) return false;
    if (eatItself(nextMove)) return false;
    return true;
}

void Snake::move(Food& food, Console& console) {
    if (!foodNearby(food)) {
        console.setCursorPosition(body.back().getX(), body.back().getY());
        std::cout << "  ";  //delete tail in console
        body.pop_back();    //delete tail data
    } else {
        console.setCursorPosition(food.getX(), food.getY());
        switch (direction) {
        case Direction::LEFT:
            std::cout << "<";
            break;
        case Direction::RIGHT:
            std::cout << ">";
            break;
        case Direction::UP:
            std::cout << "^";
            break;
        case Direction::DOWN:
            std::cout << "v";
            break;
        }
        food.generateNewFood();
    }
    body.push_front(getNextMove()); //add head data
}

void Snake::changeDirection() {
    char ch = tolower(_getch());
    switch (ch) {
    case 'w':
        if (direction != Direction::DOWN) {
            direction = Direction::UP;
        }
        break;
    case 'a':
        if (direction != Direction::RIGHT) {
            direction = Direction::LEFT;
        }
        break;
    case 's':
        if (direction != Direction::UP) {
            direction = Direction::DOWN;
        }
        break;
    case 'd':
        if (direction != Direction::LEFT) {
            direction = Direction::RIGHT;
        }
        break;
    default:
        break;
    }
}

bool Snake::foodNearby(Food& food) {
    switch (direction) {
    case Direction::LEFT:
        if (body[0].getX() - 2 == food.getX() && body[0].getY() == food.getY()) {
            return true;
        }
        return false;
    case Direction::RIGHT:
        if (body[0].getX() + 2 == food.getX() && body[0].getY() == food.getY()) {
            return true;
        }
        return false;
    case Direction::UP:
        if (body[0].getY() - 1 == food.getY() && body[0].getX() == food.getX()) {
            return true;
        }
        return false;
    case Direction::DOWN:
        if (body[0].getY() + 1 == food.getY() && body[0].getX() == food.getX()) {
            return true;
        }
        return false;
    }
}

void Snake::redraw(Food& food, Console& console) {
    for (int i = 0; i < body.size(); i++) {
        console.setCursorPosition(body[i].getX(), body[i].getY());
        if (i == 0) {
            switch (direction) {
            case Direction::LEFT:
                std::cout << "<";
                break;
            case Direction::RIGHT:
                std::cout << ">";
                break;
            case Direction::UP:
                std::cout << "^";
                break;
            case Direction::DOWN:
                std::cout << "v";
                break;
            }
        } else {
            std::cout << "o";
        }
    }
    console.setCursorPosition(food.getX(), food.getY());
    std::cout << "*";
}