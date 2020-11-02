#include "position.hpp"

Position::Position() {
	this->x = 0;
	this->y = 0;
}

Position::Position(int x, int y) {
    this->x = x;
    this->y = y;
}

int Position::getX() {
    return this->x;
}

void Position::setX(int x) {
    this->x = x;
}

int Position::getY() {
    return this->y;
}

void Position::setY(int y) {
    this->y = y;
}

bool Position::operator == (const Position& other) const {
    return this->x == other.x && this->y == other.y;
}