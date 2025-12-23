#include <iostream>
#include "controller.h"


void printBallData(const Game::Ball& ball, int index) {
    std::cout << "  Шар " << index << ": x=" << ball.position.first
              << ", y=" << ball.position.second
              << ", radius=" << ball.radius
              << ", color=" << static_cast<int>(ball.color)
              << ", vx=" << ball.speed.first
              << ", vy=" << ball.speed.second << std::endl;
}

int main() {
    GameController controller(60);

    Game::Ball balls[10]; 

    std::cout << "Начальное состояние игры:" << std::endl;
    int count = controller.getBallsAsArray(balls, 10);
    for (int i = 0; i < count; i++) {
        printBallData(balls[i], i);
    }

    controller.start();

    for (int step = 0; step < 60; step++) {
        controller.update();

        std::cout << "Шаг " << step + 1 << ":" << std::endl;
        count = controller.getBallsAsArray(balls, 10);
        for (int i = 0; i < count; i++) {
            printBallData(balls[i], i);
        }
        std::cout << std::endl;
    }

    return 0;
}
