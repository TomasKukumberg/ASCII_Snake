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
    Direction direction;
} Snake;

void setCharOnPosition(char array[][GAME_WIDTH], const Coordinates& coord, char c) {
    array[coord.x][coord.y] = c;
}

void changeFoodPos(Coordinates& food) {
    food.x = rand() % (GAME_HEIGHT - 2) + 1;
    food.y = rand() % (GAME_WIDTH - 2) + 1;
}

Coordinates& spawnFood(char array[][GAME_WIDTH], Coordinates& food) {
    changeFoodPos(food);
    return food;
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

Coordinates calcPotentialMove(const Snake& snake) {
    Coordinates potentialMove;
    switch (snake.direction) {
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

bool crashIntoWall(const Coordinates potentialMove) {
    return potentialMove.x == 0 || potentialMove.x == GAME_HEIGHT - 1 || potentialMove.y == 0
        || potentialMove.y == GAME_WIDTH - 1;
}

bool legalMove(const Snake& snake) {
    Coordinates potentialMove = calcPotentialMove(snake); 
    for (size_t i = 0; i < snake.coordinates.size(); i++) {
        bool crashIntoBody = snake.coordinates[i].x == potentialMove.x && snake.coordinates[i].y == potentialMove.y;
        if (crashIntoBody || crashIntoWall(potentialMove) ) {
            return false;
        }
    }
    return true;
}

void printEndScreen() {
    std::cout << "GAME OVER!\n";
    system("pause");
}

void changeSnakeDirection(Snake& snake, Coordinates& food) {
    char ch = tolower(_getch() );
    switch (ch) {
    case 'w': 
        if (snake.direction != Direction::DOWN) {
            snake.direction = Direction::UP;
        }
        break;
    case 'a':
        if (snake.direction != Direction::RIGHT) {
            snake.direction = Direction::LEFT;
        }
        break;
    case 's':
        if (snake.direction != Direction::UP) {
            snake.direction = Direction::DOWN;
        }
        break;
    case 'd': 
        if (snake.direction != Direction::LEFT) {
            snake.direction = Direction::RIGHT;
        }
        break;
    default:
        break;
    }
    bool continuePlaying = legalMove(snake);
    if (!continuePlaying) {
        printEndScreen();
    }
}

void shiftHead(Snake& snake) {
    switch (snake.direction) {
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


void shiftBody(char array[][GAME_WIDTH], Snake& snake, Coordinates& food) {
    std::deque<Coordinates> coordinates2(snake.coordinates);

    for (size_t i = 1; i < snake.coordinates.size(); ++i) { //shift array to the right starting from index 1
        snake.coordinates[i] = coordinates2[i - 1];
    }
    shiftHead(snake);  
}

void eatFood(Snake& snake, Coordinates& food) {
    snake.coordinates.push_front({ food.x, food.y });
}

//HERE FIX MAGIC COMPARISONS

void moveSnake(char array[][GAME_WIDTH], Snake& snake, Coordinates& food) {
    bool foodX, foodY; //bools if snakes head is facing the food and is exactly 1 distance away 
    clearScreen();
    switch (snake.direction) {
        case Direction::UP: {
            foodX = food.x == snake.coordinates[0].x - 1;
            foodY = food.y == snake.coordinates[0].y;
            break;
        }
        case Direction::DOWN: {
            foodX = food.x == snake.coordinates[0].x + 1;
            foodY = food.y == snake.coordinates[0].y;
            break;
        }
        case Direction::LEFT: {
            foodX = food.x == snake.coordinates[0].x;
            foodY = food.y == snake.coordinates[0].y - 1;
            break;
        }
        case Direction::RIGHT: {
            foodX = food.x == snake.coordinates[0].x;
            foodY = food.y == snake.coordinates[0].y + 1;
            break;
        }
    }

    if (foodX && foodY) {
        eatFood(snake, food);
        changeFoodPos(food);
    }
    else {
        shiftBody(array, snake, food);
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
    
    int xCenter = GAME_WIDTH / 2;
    int yCenter = GAME_HEIGHT / 2;
    
    Coordinates head = { yCenter, xCenter };
    Coordinates body = { yCenter, xCenter + 1 };
    
    addWalls(array);
    setCharOnPosition(array, head, '<'); //set snakes init head pos
    snake.coordinates.push_back(head); //save init coord of snakes head
    setCharOnPosition(array, body, 'o'); //set snakes init body pos
    snake.coordinates.push_back(body); //save init coord of snakes body 
    spawnFood(array, food);
}

void addSnakeInArray(char array[][GAME_WIDTH], Snake& snake, int i) {
    if (i == 0) { //adding head
        if (snake.direction == Direction::UP) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '^';
        }
        else if (snake.direction == Direction::LEFT) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '<';
        }
        else if (snake.direction == Direction::RIGHT) {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = '>';
        }
        else {
            array[snake.coordinates[i].x][snake.coordinates[i].y] = 'v';
        }
    }
    else { //adding body
        array[snake.coordinates[i].x][snake.coordinates[i].y] = 'o';
    }
}

void modifyArray(char array[][GAME_WIDTH], Snake& snake, const Coordinates& food) {

    addWalls(array);
    
    for (size_t i = 0; i < snake.coordinates.size(); i++) {
        addSnakeInArray(array, snake, i);
    }

    array[food.x][food.y] = '*'; // save food in array
}

void initArray(char array[][GAME_WIDTH], Snake& snake, Coordinates& food) {
    fillArray(array, snake, food);
    modifyArray(array, snake, food);
}

bool isEnd(const Snake& snake) {
    bool inLeftWall = snake.coordinates[0].y == 0;
    bool inRightWall = snake.coordinates[0].y == GAME_WIDTH;
    bool inUpperWall = snake.coordinates[0].x == 0;
    bool inBottomWall = snake.coordinates[0].x == GAME_HEIGHT;
    
    if (inLeftWall || inRightWall || inUpperWall || inBottomWall) {
        return true;
    }
    else {
        return false;
    }
}

bool getInputFromPlayer(char array[][GAME_WIDTH], Snake& snake, Coordinates& food){
    delay(0.85);
    
    if (_kbhit()) {
        changeSnakeDirection(snake, food);
    }
    moveSnake(array,snake, food);
    modifyArray(array, snake, food);
    drawArray(array);
    return isEnd(snake);
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

//MAYBE TODO: FIX X,Y SWAPPED COORDINATES

int main() {
    
    char array[GAME_HEIGHT][GAME_WIDTH];
    Snake snake;
    Coordinates food;
    snake.direction = Direction::LEFT;
    bool gameOver = false;

    srand((unsigned int)time(NULL));
    initConsole(false);
    initArray(array, snake, food);
    drawArray(array);
    
    while (!gameOver) {
       gameOver = getInputFromPlayer(array, snake, food);
    }
    
    
    printEndScreen();
}
