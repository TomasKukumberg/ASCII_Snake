#include "console.hpp"

void Console::setCursorPosition(int x, int y) {
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void Console::center() {
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, GetSystemMetrics(SM_CXSCREEN) / 2 - 800 / 2,
		GetSystemMetrics(SM_CYSCREEN) / 2 - 550 / 2, 800, 550, TRUE); // 800 width, 550 height
}

void Console::endGame() {
	setCursorPosition(0, GAME_HEIGHT);
	showCursor(true);
	std::cout << "GAME OVER!\n";
	std::exit(EXIT_FAILURE);
}

void Console::showCursor(bool showFlag) {
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(hOut, &cursorInfo);
}

void Console::init(bool cursorFlag) {
	system("cls");
	center();
	showCursor(cursorFlag);
}

void Console::drawArray(Food& food) {
	for (int i = 0; i < GAME_HEIGHT; i++) {
		for (int j = 0; j < GAME_WIDTH; j++) {
			if ((i == 0 || i == GAME_HEIGHT - 1 || j == 0 || j == GAME_WIDTH - 1) && (j % 2 == 0)) {
				std::cout << "X";
			} else {
				std::cout << " ";
			}
		}
		std::cout << "\n";
	}
	setCursorPosition(food.getX(), food.getY());
	std::cout << "*";
}