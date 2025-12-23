#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <utility>

class GameController; // Forward declaration; данный класс нужен питону для получения данных из структуры 
// (возможно я его перепишу на интерфейс)

class Game {

public:
    enum Color { WHITE, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, MAROON, BLACK, YELLOW_STRIPED }; // цвета шаров

    struct Ball {
        std::pair<int, int> position; // позиция/центр шара {x, y}
        std::pair<float, float> speed; // вектор скорости {speedX, speedY}
        int radius;
        Color color; //цвет шара
    };
    struct Cue {
        std::pair<int, int> position; // позиция кия {x, y}
        std::pair<int, int> direction; // направление кия {dirX, dirY}
        int force; // сила удара
    };

    struct Table {
        std::pair<int, int> leftTop; // левый верхний угол стола {x, y}
        std::pair<int, int> rightBottom; // правый нижний угол стола
        float frictionCoefficient; // коэффициент трения
        //ширина бортика привязанная к радиусу лузы и размеру стола 
    };
    
    // Конструктор
    Game(int animationSteps);

    // Основные методы
    void resetGame();
    void checkBoundaries();
    void checkCollisions();
    void updateBallCollisions();
    int sign(float x);
    void calculateBallMovement(Ball& ball, int steps);
    void calculateBallMovement(Ball& ball);
    void strikeCueAtBall(Cue& cue, Ball& ball);
    void transferImpulse(Cue& cue, Ball& ball);
    void update();

    int aSteps; // количество шагов анимации (для расчета скорости и т.д.)

    // GameController - друг для доступа к protected методам
    friend class GameController;

private:
    static constexpr int BALLS_COUNT = 10; // фиксированное количество шаров

    float gravity = 9.81f; // гравитация
    // Количество шагов в игре
    float time;

    // Флаг движения: true если хотя бы один шар движется
    bool isMoving;
    
protected:
    // Контейнеры для объектов игры
    Ball balls[BALLS_COUNT]; // фиксированный массив шаров
    std::unique_ptr<Cue> cue;
    std::unique_ptr<Table> table;

    // Методы для доступа к объектам игры (только для чтения)
    const Ball* getBalls() const { return balls; }
    int getBallsCount() const { return BALLS_COUNT; } // фиксированное количество шаров
    const std::unique_ptr<Cue>& getCue() const { return cue; }
    const std::unique_ptr<Table>& getTable() const { return table; }


};

#endif // GAME_H
