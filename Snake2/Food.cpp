#include "food.h"
#include <cstdlib>

Food::Food() {
	generateNewFood();
}

int Food::getX() {
	return this->position.getX();
}

int Food::getY() {
	return this->position.getY();
}

void Food::generateNewFood() {
	position.setX(rand() % (GAME_WIDTH / 2) * 2);
	position.setY(rand() % (GAME_HEIGHT - 2) + 1);
}