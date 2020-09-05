#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <deque>
#include <chrono>
#include <thread>

#define GAME_HEIGHT 31
#define GAME_WIDTH 51
#define LEFT_WALL 0
#define RIGHT_WALL 51
#define BOTTOM_WALL 31
#define UPPER_WALL 0


enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

class Food{
private:
    COORD coord;
public:
    COORD getCoord(void) {
        return this->coord;
    }
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
    int getY() {
        return this->y;
    }
    bool operator == (const Position& other) const {
        return this->x == other.x && this->y == other.y;
    }
};

class Snake {
private:
    std::deque<Position> body;
    Direction direction = Direction::LEFT;
public:
    Snake() {
        body.push_front({ GAME_WIDTH / 2, GAME_HEIGHT / 2 });
        body.push_back({ GAME_WIDTH / 2 + 1, GAME_HEIGHT / 2 });
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
        return (nextMove.getX() == LEFT_WALL) || (nextMove.getX() == RIGHT_WALL) || (nextMove.getY() == BOTTOM_WALL) ||
            (nextMove.getY() == UPPER_WALL);
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
            nextMove = { body[0].getX() - 1, body[0].getY() };
            break;
        case Direction::RIGHT:
            nextMove = { body[0].getX() + 1, body[0].getY() };
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
    void move() {
        setCursorPosition(body.back().getX(), body.back().getY());  
        std::cout << "  ";  //delete tail in console and then data
        body.pop_back();
        switch (direction) {
        case Direction::LEFT:
            body.push_front({ body[0].getX() - 1, body[0].getY() });
            break;
        case Direction::RIGHT:
            body.push_front({body[0].getX() + 1, body[0].getY()});
            break;
        case Direction::UP:
            body.push_front({ body[0].getX(), body[0].getY() - 1 });
            break;
        case Direction::DOWN:
            body.push_front({ body[0].getX(), body[0].getY() + 1 });
            break;
        }
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
    void redraw() {
        for (int i = 0; i < body.size(); i++) {
            setCursorPosition(body[i].getX(), body[i].getY());
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
    }
};

/*
bool getInputFromPlayer(char array[][GAME_WIDTH], Snake& snake, Coordinates& food){
    delay(0.85);
    
    if (_kbhit()) {
        changeSnakeDirection(snake, food);
    }
    moveSnake(array,snake, food);
    //modifyArray(array, snake, food);
    drawArray(array, snake, food);
    return isEnd(snake);
}*/

void centerConsole(void) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, GetSystemMetrics(SM_CXSCREEN) / 2 - 800 / 2,
        GetSystemMetrics(SM_CYSCREEN) / 2 - 550 / 2, 800, 550, TRUE); // 800 width, 550 height
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

void initConsole(bool cursorState) {
    system("cls");
    centerConsole();
    ShowConsoleCursor(cursorState);
}

void drawArray() {
    for (int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            if (i == 0 || i == GAME_HEIGHT - 1 || j == 0 || j == GAME_WIDTH - 1) {
                std::cout << "X";
            } else {
                std::cout << " ";
            }
        }
        std::cout << "\n";
    }
}

void delay(double seconds) {
    DWORD milliseconds = (DWORD)seconds * 1000;
    Sleep(milliseconds);
}

int main() {
    //auto t = std::chrono::system_clock::now();

    initConsole(false);
    Snake snake;
    drawArray();
    while (true) {
        delay(1);
        if (_kbhit()) {
            snake.changeDirection();
        }
        if (snake.canMove()) {
            snake.move();
            snake.redraw();
        } else {
            std::cout << "GAME OVER!\n";
            std::exit(EXIT_FAILURE);
        }
        //t += std::chrono::milliseconds(33);
        //std::this_thread::sleep_until(t);
    }
}
