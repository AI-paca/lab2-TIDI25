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
        std::pair<float, float> speed; // скорость шара {vx, vy}
        int radius;
        Color color; //цвет шара
        bool isPocketed = false; // флаг, что шар забит в лузу

    };
    struct Pocket {
        std::pair<float, float> position;
        float radius;
    };
    struct Cue {
        std::pair<float, float> position;  // Координаты "носика" кия {x, y}
        std::pair<float, float> direction; // направление кия {dirX, dirY}
        float force;                       // Сила удара
        bool isActive;                     
    };

    struct Table {
        std::pair<int, int> leftTop; // левый верхний угол стола {x, y}
        std::pair<int, int> rightBottom; // правый нижний угол стола
        float frictionCoefficient; // коэффициент трения
        int borderThickness; // ширина бортика = диаметру шара
    };
    
    //структура лузы{
    // наследуем структуру шаров
    // добавляем окресность поглощения шара
    //  }


    // Конструкторы
    Game(int animationSteps);
    Game(int animationSteps, int left, int top, int right, int bottom);

    // Основные методы
    void resetGame();
    void checkBoundaries();
    void checkCollisions();
    void updateBallCollisions();
    int sign(float x);
    void calculateBallMovement(Ball& ball, int steps);
    void calculateBallMovement(Ball& ball);
    void transferImpulse(Cue& cue, Ball& ball);
    void update();
    void calculateCueVelocityFromMouse(int mouseX, int mouseY);
    void aimCue(int mouseX, int mouseY);
    void shoot();
    void initPockets();
    void checkPockets();
    void initBalls();

    int aSteps; // количество шагов анимации (для расчета скорости и т.д.)

    // GameController - друг для доступа к protected методам
    friend class GameController;

private:
    static constexpr int BALLS_COUNT = 10; // фиксированное количество шаров
    static constexpr int POCKETS_COUNT = 6; // фиксированное количество луз

    float gravity = 9.81f; // гравитация
    // Количество шагов в игре
    float time;

    // Флаг движения: true если хотя бы один шар движется
    bool isMoving;

    // Начальная позиция белого шара
    std::pair<int, int> whiteBallInitialPosition;
    
protected:
    // Контейнеры для объектов игры
    Ball balls[BALLS_COUNT]; // фиксированный массив шаров
    std::unique_ptr<Cue> cue;
    std::unique_ptr<Table> table;
    Pocket pockets[POCKETS_COUNT]; // массив луз

    // Методы для доступа к объектам игры (только для чтения)
    const Ball* getBalls() const { return balls; }
    int getBallsCount() const { return BALLS_COUNT; } // фиксированное количество шаров
    const std::unique_ptr<Cue>& getCue() const { return cue; }
    const std::unique_ptr<Table>& getTable() const { return table; }
    const Pocket* getPockets() const { return pockets; }
    int getPocketsCount() const { return POCKETS_COUNT; }


};

#endif // GAME_H
