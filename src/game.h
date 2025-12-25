#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <utility>
#include "GameObj.h"

//class GameController; // Forward declaration; данный класс нужен питону для получения данных из структуры

class Game {

public:
    // Конструкторы
    Game(int animationSteps);
    Game(int animationSteps, int left, int top, int right, int bottom);

    // Основные методы
    void resetGame();
    void checkBoundaries();
    void checkCollisions();
    void updateBallCollisions();
    int sign(float x);
    void calculateBallMovement(GameObj::Ball& ball, int steps);
    void calculateBallMovement(GameObj::Ball& ball);
    void transferImpulse(GameObj::Cue& cue, GameObj::Ball& ball);
    void update();
    void update(int steps);
    void calculateCueVelocityFromMouse(int mouseX, int mouseY);
    void aimCue(int mouseX, int mouseY);
    void shoot();
    void initPockets();
    void checkPockets();
    void initBalls();

    int aSteps; // количество шагов анимации (для расчета скорости и т.д.)

    // // GameController - друг для доступа к protected методам
    // friend class GameController;

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
    GameObj::Ball balls[BALLS_COUNT]; // фиксированный массив шаров
    std::unique_ptr<GameObj::Cue> cue;
    std::unique_ptr<GameObj::Table> table;
    GameObj::Pocket pockets[POCKETS_COUNT]; // массив луз

    // Методы для доступа к объектам игры (только для чтения)
    const GameObj::Ball* getBalls() const { return balls; }
    int getBallsCount() const { return BALLS_COUNT; } // фиксированное количество шаров
    const std::unique_ptr<GameObj::Cue>& getCue() const { return cue; }
    const std::unique_ptr<GameObj::Table>& getTable() const { return table; }
    const GameObj::Pocket* getPockets() const { return pockets; }
    int getPocketsCount() const { return POCKETS_COUNT; }


};

#endif // GAME_H
