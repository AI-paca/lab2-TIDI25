#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <utility> // for std::pair

class Game {

protected:
    enum Color { RED, YELLOW, BLACK, WHITE }; // цвета шаров

    // Вложенный класс Ball
    class Ball {
    public:
        std::pair<int, int> position; // позиция/центр шара {x, y}
        std::pair<int, int> velocity; // вектор скорости {speedX, speedY}
        int radius;
        Color color; //цвет шара
    };

    // Вложенный класс Cue
    class Cue {
    public:
        std::pair<int, int> position; // позиция кия {x, y}
        std::pair<int, int> direction; // направление кия {dirX, dirY}
        int force; // сила удара
    };

    // Вложенный класс Table
    class Table {
    public:
        std::pair<int, int> leftTop; // левый верхний угол стола {x, y}
        std::pair<int, int> rightBottom; // правый нижний угол стола
        float frictionCoefficient; // коэффициент трения
    };

public:
    // Конструктор
    Game(int aSteps);

    // Основные методы
    void resetGame();
    void checkBoundaries();
    void checkCollisions();
    void updateBallCollisions();
    void calculateBallMovement(Ball& ball);
    void strikeCueAtBall(Cue& cue, Ball& ball);
    void transferImpulse(Cue& cue, Ball& ball);

    // Методы для доступа к объектам игры
    const std::vector<std::unique_ptr<Ball>>& getBalls() const { return balls; }
    const std::unique_ptr<Cue>& getCue() const { return cue; }
    const std::unique_ptr<Table>& getTable() const { return table; }

private:
    // Контейнеры для объектов игры
    std::vector<std::unique_ptr<Ball>> balls;
    std::unique_ptr<Cue> cue;
    std::unique_ptr<Table> table;

    // Количество шагов в игре
    int steps;
};

#endif // GAME_H
