#include <thread>

#include "snake.hpp"

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
