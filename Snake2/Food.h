#pragma once

#include "Position.h"
#include "Macros.h"

class Food {
private:
	Position position;
public:
	Food();
	int getX();
	int getY();
	void generateNewFood();
};

