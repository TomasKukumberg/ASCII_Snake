#pragma once

#include "position.h"
#include "macros.h"

class Food {
private:
	Position position;
public:
	Food();
	int getX();
	int getY();
	void generateNewFood();
};

