#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <utility>

class GameController; // Forward declaration; данный класс нужен питону для получения данных из структуры 
// (возможно я его перепишу на интерфейс)

class Game {

public:
    enum Color { RED, YELLOW, BLACK, WHITE }; // цвета шаров

    struct Ball {
        std::pair<int, int> position; // позиция/центр шара {x, y}
        std::pair<int, int> velocity; // вектор скорости {speedX, speedY}
        int radius;
        Color color; //цвет шара
    };

protected:
    class Cue {
    public:
        std::pair<int, int> position; // позиция кия {x, y}
        std::pair<int, int> direction; // направление кия {dirX, dirY}
        int force; // сила удара
    };

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

    // Метод для копирования данных из Ball в массив
    int getBallsAsArray(Ball* balls, int max_count) const;

    // GameController - друг для доступа к protected методам
    friend class GameController;

protected:
    // Методы для доступа к объектам игры (только для чтения)
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
