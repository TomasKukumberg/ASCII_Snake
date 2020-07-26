#include <iostream>
#include <vector>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <fstream>

#define GAME_WIDTH 51
#define GAME_HEIGHT 31

typedef struct Coordinates {
    int x;
    int y;
} Coordinates;

enum Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

typedef struct SnakeBody {
    std::deque<Coordinates> coordinates;
} SnakeBody;

void setCharOnPosition(char array[][GAME_WIDTH], int x, int y, SnakeBody& snakeBody, char c) {
    array[x][y] = c;
}

Coordinates& spawnFood(char array[][GAME_WIDTH]) {
    srand((unsigned int)time(NULL));
    int x = rand() % GAME_HEIGHT;
    int y = rand() % GAME_WIDTH;
    Coordinates* food = new Coordinates;
    food->x = x;
    food->y = y;
    return *food;
}

void generateFood(Coordinates& food) {
    food.x = rand() % GAME_HEIGHT;
    food.y = rand() % GAME_WIDTH;
}

void drawArray(char array[][GAME_WIDTH]) {
    for (int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            std::cout << array[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void delay(double seconds) {
    DWORD milliseconds = (DWORD) seconds * 1000;
    Sleep(milliseconds);
}

void clearScreen() {
    system("CLS");
}

bool headEqualsFood(const SnakeBody& snakeBody, const Coordinates& food) {
    return (snakeBody.coordinates[0].x == food.x) && (snakeBody.coordinates[0].y == food.y);
}

void changeSnakeDirection(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction) {
    char ch = tolower(_getch() );
    
    switch (ch) {
    case 'w': 
        direction = Direction::UP;
        break;
    case 'a':
        direction = Direction::LEFT;
        break;
    case 's':
        direction = Direction::DOWN;
        break;
    case 'd': 
        direction = Direction::RIGHT;
        break;
    default:
        break;
    }
}

void shiftHead(SnakeBody& snakeBody, Direction& direction) {
    switch (direction) {
    case Direction::UP:
        snakeBody.coordinates[0].x = snakeBody.coordinates[0].x - 1;
        break;
    case Direction::DOWN:
        snakeBody.coordinates[0].x = snakeBody.coordinates[0].x + 1;
        break;
    case Direction::LEFT:
        snakeBody.coordinates[0].y = snakeBody.coordinates[0].y - 1;
        break;
    case Direction::RIGHT:
        snakeBody.coordinates[0].y = snakeBody.coordinates[0].y + 1;
        break;
    }
}


void shiftBody(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction) {
    std::deque<Coordinates> coordinates2(snakeBody.coordinates);

    for (size_t i = 1; i < snakeBody.coordinates.size(); ++i) { //shift array to the right starting from index 1
        snakeBody.coordinates[i] = coordinates2[i - 1];
    }

    shiftHead(snakeBody, direction);
    
}

void eatFood(SnakeBody& snakeBody, Coordinates& food) {
    snakeBody.coordinates.push_front({ food.x, food.y });
}

void moveSnakeBody(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction) {
    switch (direction) {
    case Direction::UP:
        if ((food.x == (snakeBody.coordinates[0].x - 1)) && (food.y == snakeBody.coordinates[0].y)) {
            eatFood(snakeBody, food);
            generateFood(food);
        }
        else {
            shiftBody(array, snakeBody, food, direction);
        }
        break;
    case Direction::DOWN:
        if ((food.x == (snakeBody.coordinates[0].x + 1)) && (food.y == snakeBody.coordinates[0].y)) {
            eatFood(snakeBody, food);
            generateFood(food);
        }
        else {
            shiftBody(array, snakeBody, food, direction);
        }
        break;
    case Direction::LEFT:
        if ((food.x == (snakeBody.coordinates[0].x)) && (food.y == snakeBody.coordinates[0].y - 1)) {
            eatFood(snakeBody, food);
            generateFood(food);
        }
        else {
            shiftBody(array, snakeBody, food, direction);
        }
        break;
    case Direction::RIGHT:
        if ((food.x == (snakeBody.coordinates[0].x)) && (food.y == snakeBody.coordinates[0].y + 1)) {
            eatFood(snakeBody, food);
            generateFood(food);
        }
        else {
            shiftBody(array, snakeBody, food, direction);
        }
        break;
    }
}

void printTest(SnakeBody& snakeBody) {
    for (int i = 0; i < snakeBody.coordinates.size(); i++) {
        std::cout << "x: " << snakeBody.coordinates[i].x << "y: " << snakeBody.coordinates[i].y << "\n";
    }
}


void moveSnake(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction) {
    //TODO CHECK IF MOVE WILL EAT THE FOOD, IF YES, MAKE SNAKE LONGER
    clearScreen();
    moveSnakeBody(array, snakeBody, food, direction);
   
}

void fillArray(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, char c) {
    for (int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            if (j == 0 || j == GAME_WIDTH - 1 || i == 0 || i == GAME_HEIGHT - 1) {
                array[i][j] = 'X';
            }
            else {
                array[i][j] = ' ';
            }
        }
    }

    setCharOnPosition(array, GAME_HEIGHT / 2, GAME_WIDTH / 2, snakeBody, c); //set player start head
    snakeBody.coordinates.push_back({ GAME_HEIGHT / 2, GAME_WIDTH / 2 });
    //TEST
    setCharOnPosition(array, GAME_HEIGHT / 2, (GAME_WIDTH / 2) + 1, snakeBody, 'o'); //set player start body
    snakeBody.coordinates.push_back({ GAME_HEIGHT / 2, (GAME_WIDTH / 2) + 1 });
    //
    food = spawnFood(array);
    
}

void modifyArray(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction) {
    //refactor name of function to something more meaningful
    //PROBABLY WRONG - FIX SO THE WHOLE ARRAY GETS REDRAWN INSTEAD OF DRAWING SNAKE BODY ONLY

    for (int i = 0; i < GAME_HEIGHT; i++) {
        for (int j = 0; j < GAME_WIDTH; j++) {
            if (j == 0 || j == GAME_WIDTH - 1 || i == 0 || i == GAME_HEIGHT - 1) {
                array[i][j] = 'X';
            }
            else {
                array[i][j] = ' ';
            }
        }
    }
    
    for (size_t i = 0; i < snakeBody.coordinates.size(); i++) {
        if (i == 0) {
            if (direction == Direction::UP) {
                array[snakeBody.coordinates[i].x][snakeBody.coordinates[i].y] = '^';
            }
            else if (direction == Direction::LEFT) {
                array[snakeBody.coordinates[i].x][snakeBody.coordinates[i].y] = '<';
            }
            else if (direction == Direction::RIGHT) {
                array[snakeBody.coordinates[i].x][snakeBody.coordinates[i].y] = '>';
            }
            else {
                array[snakeBody.coordinates[i].x][snakeBody.coordinates[i].y] = 'v';
            }
        }
        else {
            array[snakeBody.coordinates[i].x][snakeBody.coordinates[i].y] = 'o';
        }
    }

    array[food.x][food.y] = '*'; //drawing food
}

void initArray(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, char c, Direction& direction) {
    fillArray(array, snakeBody, food, 'c');
    modifyArray(array, snakeBody, food, direction);
}

void getInputFromPlayer(char array[][GAME_WIDTH], SnakeBody& snakeBody, Coordinates& food, Direction& direction){
    delay(0.85);
    
    if (_kbhit()) {
        changeSnakeDirection(array, snakeBody, food, direction);
    }
    moveSnake(array,snakeBody, food, direction); //if no direction change, keep moving
    modifyArray(array, snakeBody, food, direction);
    drawArray(array);
}

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
    centerConsole();
    ShowConsoleCursor(cursorState);
}

//TODO SPAWNING NEW FOOD
//TODO FIX MOVEMENT SO UP AND DOWN MOVEMENT IN A ROW IS NOT POSSIBLE
//TODO MAKING SNAKE LONGER WHEN FOOD IS EATEN
//TODO FIX SPAWNING FOOD IN WALLS, MAKE BUMPING WALLS GAME OVER
//TODO RELEASE MEMORY IF FOOD IS EATEN
//MAYBE TODO: REWORK WHOLE PROGRAM FOR OOP, MAIN CLASS SNAKE
//TODO MAYBE: FIX X,Y SWAPPED COORDINATES
//TODO ADD GAMEOVER TO PREVENT CRASH
int main() {
    
    char array[GAME_HEIGHT][GAME_WIDTH];
    SnakeBody snakeBody;
    Coordinates food;
    Direction direction = Direction::LEFT;
    
    initConsole(false);
    initArray(array, snakeBody, food, '<', direction);
    drawArray(array);
    
    while (true) {
        getInputFromPlayer(array, snakeBody, food, direction);
    }
}
