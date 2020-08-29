#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <deque>

#define GAME_HEIGHT 31
#define GAME_WIDTH 51

typedef struct Coordinates {
    int x;
    int y;
} Coordinates;

enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

typedef struct Snake {
    std::deque<Coordinates> coordinates;
} Snake;

void setCharOnPosition(char array[][GAME_WIDTH], const Coordinates& coord, char c) {
    array[coord.x][coord.y] = c;
}

void changeFoodPos(Coordinates& food) {
    food.x = rand() % (GAME_HEIGHT - 2) + 1;
    food.y = rand() % (GAME_WIDTH - 2) + 1;
}

Coordinates& spawnFood(char array[][GAME_WIDTH]) {
    Coordinates* food = new Coordinates;
    changeFoodPos(*food);
    return *food;
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

bool headEqualsFood(const Snake& snake, const Coordinates& food) {
    return (snake.coordinates[0].x == food.x) && (snake.coordinates[0].y == food.y);
}

Coordinates calcPotentialMove(const Snake& snake, Direction& direction) {
    Coordinates potentialMove;
    switch (direction) {
    case Direction::DOWN:
        potentialMove = { snake.coordinates[0].x + 1, snake.coordinates[0].y };
        break;
    case Direction::UP:
        potentialMove = { snake.coordinates[0].x - 1, snake.coordinates[0].y };
        break;
    case Direction::LEFT:
        potentialMove = { snake.coordinates[0].x, snake.coordinates[0].y - 1 };
        break;
    case Direction::RIGHT:
        potentialMove = { snake.coordinates[0].x, snake.coordinates[0].y + 1 };
        break;
    default:
        break;
    }
    return potentialMove;
}

bool legalMove(const Snake& snake, Direction& direction) {
    Coordinates potentialMove = calcPotentialMove(snake, direction);
    for (size_t i = 0; i < snake.coordinates.size(); i++) {
        if (snake.coordinates[i].x == potentialMove.x && snake.coordinates[i].y == potentialMove.y) {
            return false;
        }
    }
    return true;
}

void printEndScreen() {
    std::cout << "GAME OVER!\n";
    system("pause");
}

void changeSnakeDirection(char array[][GAME_WIDTH], Snake& snake, Coordinates& food, Direction& direction) {
    char ch = tolower(_getch() );
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
    bool continuePlaying = legalMove(snake, direction);
    if (!continuePlaying) {
        printEndScreen();
    }
}

void shiftHead(Snake& snake, Direction& direction) {
    switch (direction) {
    case Direction::UP:
        snake.coordinates[0].x = snake.coordinates[0].x - 1;
        break;
    case Direction::DOWN:
        snake.coordinates[0].x = snake.coordinates[0].x + 1;
        break;
    case Direction::LEFT:
        snake.coordinates[0].y = snake.coordinates[0].y - 1;
        break;
    case Direction::RIGHT:
        snake.coordinates[0].y = snake.coordinates[0].y + 1;
        break;
    }
}


void shiftBody(char array[][GAME_WIDTH], Snake& snake, Coordinates& food, Direction& direction) {
    std::deque<Coordinates> coordinates2(snake.coordinates);

    for (size_t i = 1; i < snake.coordinates.size(); ++i) { //shift array to the right starting from index 1
        snake.coordinates[i] = coordinates2[i - 1];
    }
    shiftHead(snake, direction);  
}

void eatFood(Snake& snake, Coordinates& food) {
    snake.coordinates.push_front({ food.x, food.y });
}

void moveSnake(char array[][GAME_WIDTH], Snake& snake, Coordinates& food, Direction& direction) {
    clearScreen();
    switch (direction) {
    case Direction::UP:
        if ((food.x == (snake.coordinates[0].x - 1)) && (food.y == snake.coordinates[0].y)) {
            eatFood(snake, food);
            changeFoodPos(food);
        }
        else {
            shiftBody(array, snake, food, direction);
        }
        break;
    case Direction::DOWN:
        if ((food.x == (snake.coordinates[0].x + 1)) && (food.y == snake.coordinates[0].y)) {
            eatFood(snake, food);
            changeFoodPos(food);
        }
        else {
            shiftBody(array, snake, food, direction);
        }
        break;
    case Direction::LEFT:
        if ((food.x == (snake.coordinates[0].x)) && (food.y == snake.coordinates[0].y - 1)) {
            eatFood(snake, food);
            changeFoodPos(food);
        }
        else {
            shiftBody(array, snake, food, direction);
        }
        break;
    case Direction::RIGHT:
        if ((food.x == (snake.coordinates[0].x)) && (food.y == snake.coordinates[0].y + 1)) {
            eatFood(snake, food);
            changeFoodPos(food);
        }
        else {
            shiftBody(array, snake, food, direction);
        }
        break;
    }
}


void addWalls(char array[][GAME_WIDTH]) {
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
}

void fillArray(char array[][GAME_WIDTH], Snake& snake, Coordinates& food) {
    
    Coordinates head = { GAME_HEIGHT / 2, GAME_WIDTH / 2 };
    Coordinates body = { GAME_HEIGHT / 2, (GAME_WIDTH / 2) + 1 };
    
    addWalls(array);
    setCharOnPosition(array, head, '<'); //set snakes init head pos
    snake.coordinates.push_back(head); //save init coord of snakes head
    setCharOnPosition(array, body, 'o'); //set snakes init body pos
    snake.coordinates.push_back(body); //save init coord of snakes body 
    food = spawnFood(array);
}

void addSnakeInArray(char array[][GAME_WIDTH], Snake& snake, Direction& direction, int i) {
    if (i == 0) {
        if (direction == Direction::UP) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '^';
        }
        else if (direction == Direction::LEFT) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '<';
        }
        else if (direction == Direction::RIGHT) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '>';
        }
        else {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = 'v';
        }
    }
    else {
        array[snake.coordinates[i].x][snake.coordinates[i].y] = 'o';
    }
}

void modifyArray(char array[][GAME_WIDTH], Snake& snake, const Coordinates& food, Direction& direction) {

    addWalls(array);
    
    for (size_t i = 0; i < snake.coordinates.size(); i++) {
        addSnakeInArray(array, snake, direction, i);
    }

    array[food.x][food.y] = '*'; // save food in array
}

void initArray(char array[][GAME_WIDTH], Snake& snake, Coordinates& food, Direction& direction) {
    fillArray(array, snake, food);
    modifyArray(array, snake, food, direction);
}

bool checkIfEnd(Snake& snake) {
    if (snake.coordinates[0].x == 0 || snake.coordinates[0].x == GAME_HEIGHT ||
        snake.coordinates[0].y == 0 || snake.coordinates[0].y == GAME_WIDTH) {
        return true;
    }
    else {
        return false;
    }
}

bool getInputFromPlayer(char array[][GAME_WIDTH], Snake& snake, Coordinates& food, Direction& direction){
    delay(0.85);
    
    if (_kbhit()) {
        changeSnakeDirection(array, snake, food, direction);
    }
    moveSnake(array,snake, food, direction);
    modifyArray(array, snake, food, direction);
    drawArray(array);
    return checkIfEnd(snake);
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

//TODO FIX BUG WITH RIGHT WALL
//REFACTOR MOVESNAKEBODY FUNC 
//TODO ADD DIRECTION TO SNAKE STRUCT AND FOOD TOO
//TODO: REMOVE CHAR ARRAY AND JUST PRINT STUFF ACCORDING TO POSITIONS
//MAYBE TODO: REWORK WHOLE PROGRAM FOR OOP, MAIN CLASS SNAKE
//MAYBE TODO: FIX X,Y SWAPPED COORDINATES

int main() {
    
    char array[GAME_HEIGHT][GAME_WIDTH];
    Snake snake;
    Coordinates food;
    Direction direction = Direction::LEFT;
    bool gameOver = false;

    srand((unsigned int)time(NULL));
    initConsole(false);
    initArray(array, snake, food, direction);
    drawArray(array);
    
    while (!gameOver) {
        gameOver = getInputFromPlayer(array, snake, food, direction);
    }
    
    
    printEndScreen();
}
