#pragma once

#include "position.hpp"
#include "macros.hpp"

class Food {
private:
	Position position;
public:
	Food();
	int getX();
	int getY();
	void generateNewFood();
};

