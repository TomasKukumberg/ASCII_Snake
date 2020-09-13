#include <iostream>
#include <windows.h>
#include <conio.h>
#include <deque>
#include <chrono>
#include <thread>

#define GAME_WIDTH 89
#define GAME_HEIGHT 31
#define LEFT_WALL 0
#define RIGHT_WALL (GAME_WIDTH - 1)
#define UPPER_WALL 0
#define BOTTOM_WALL (GAME_HEIGHT - 1)

enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Position {
private:
	int x;
	int y;
public:
	Position() {
		this->x = 0;
		this->y = 0;
	}
	Position(int x, int y) {
		this->x = x;
		this->y = y;
	}
	int getX() {
		return this->x;
	}
	void setX(int x) {
		this->x = x;
	}
	int getY() {
		return this->y;
	}
	void setY(int y) {
		this->y = y;
	}
	bool operator == (const Position& other) const {
		return this->x == other.x && this->y == other.y;
	}
};

class Food {
private:
	Position position;
public:
	Food() {
		generateNewFood();
	}
	int getX() {
		return this->position.getX();
	}
	int getY() {
		return this->position.getY();
	}
	void generateNewFood() {
		position.setX(rand() % (GAME_WIDTH / 2) * 2);
		position.setY(rand() % (GAME_HEIGHT - 2) + 1);
	}
};

class Console {
private:
	const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	HWND console = GetConsoleWindow();
	RECT r;
public:
	void setCursorPosition(int x, int y)
	{
		std::cout.flush();
		COORD coord = { (SHORT)x, (SHORT)y };
		SetConsoleCursorPosition(hOut, coord);
	}
	void center(void) {
		GetWindowRect(console, &r); //stores the console's current dimensions
		MoveWindow(console, GetSystemMetrics(SM_CXSCREEN) / 2 - 800 / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - 550 / 2, 800, 550, TRUE); // 800 width, 550 height
	}
	void endGame() {
		setCursorPosition(0, GAME_HEIGHT);
		showCursor(true);
		std::cout << "GAME OVER!\n";
		std::exit(EXIT_FAILURE);
	}
	void showCursor(bool showFlag) {
		GetConsoleCursorInfo(hOut, &cursorInfo);
		cursorInfo.bVisible = showFlag;
		SetConsoleCursorInfo(hOut, &cursorInfo);
	}
	void init(bool cursorFlag) {
		system("cls");
		center();
		showCursor(cursorFlag);
	}
	void drawArray(Food& food) {
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
};

class Snake {
private:
    std::deque<Position> body;
    Direction direction = Direction::LEFT;
public:
    Snake() {
        body.push_front({ GAME_WIDTH / 2, GAME_HEIGHT / 2 });
        body.push_back({ GAME_WIDTH / 2 + 2, GAME_HEIGHT / 2 });
    }
    Direction getDirection() {
        return this->direction;
    }
    void setDirection(Direction direction) {
        this->direction = direction;
    }
    std::deque<Position> getBody() {
        return this->body;
    }
    bool eatItself(Position newPosition) {
        auto result = std::find(std::begin(body), std::end(body), newPosition);
        if (result == std::end(body)) {
            return false;
        } else {
            return true;
        }
    }
    bool crashIntoWalls(Position nextMove) {
        return (nextMove.getX() == LEFT_WALL) || (nextMove.getX() == RIGHT_WALL) ||
               (nextMove.getY() == BOTTOM_WALL) || (nextMove.getY() == UPPER_WALL);
    }
    Position getNextMove() {
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
    bool canMove() {
        Position nextMove = getNextMove();
        if(crashIntoWalls(nextMove)) return false;
        if (eatItself(nextMove)) return false;
        return true;
    }
    void move(Food& food, Console& console) {
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
    void changeDirection() {
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
    bool foodNearby(Food& food) {
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
    void redraw(Food& food, Console& console) {
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
};

void drawArray(Food& food, Console& console) {
    for (int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            if ( (i == 0 || i == GAME_HEIGHT - 1 || j == 0 || j == GAME_WIDTH - 1 ) && (j % 2 == 0) ) {
                std::cout << "X";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
    console.setCursorPosition(food.getX(), food.getY());
    std::cout << "*";
}

int main() {
    
    using clock = std::chrono::steady_clock; 
    auto next_frame = clock::now();
    bool gameOver = false;
	Console console;

    console.init(false);
    Snake snake;
    Food food;
    console.drawArray(food);
    console.setCursorPosition(0, 0);
    
    while (!gameOver) {
        next_frame += std::chrono::milliseconds(1000 / 5); // 5Hz 
        if (_kbhit()) {
            snake.changeDirection();
        }
        if (snake.canMove() == false) {
            gameOver = true;
        }
        snake.move(food, console);
        snake.redraw(food, console);
        std::this_thread::sleep_until(next_frame);
    }
    console.endGame();
}
