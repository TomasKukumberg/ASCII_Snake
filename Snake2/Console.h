#pragma once

#include <Windows.h>
#include <iostream>

#include "Food.h"

class Console {
private:
	const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	HWND console = GetConsoleWindow();
	RECT r;
public:
	void setCursorPosition(int x, int y);
	void center();
	void endGame();
	void showCursor(bool showFlag);
	void init(bool cursorFlag);
	void drawArray(Food& food);
};

