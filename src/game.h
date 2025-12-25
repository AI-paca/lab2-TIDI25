#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>
#include <utility>
#include "GameObj.h"

class Game {

public:
    static constexpr int BALLS_COUNT = 10;
    static constexpr int POCKETS_COUNT = 6;

    // Конструкторы
    Game(int animationSteps);
    Game(int animationSteps, int left, int top, int right, int bottom);

    // Основные методы
    void resetGame();     
    void update();
    void update(int steps);

    void aimCue(int mouseX, int mouseY);
    void shoot();

    
    const GameObj::Ball* getBalls() const { return balls; }
    const GameObj::Pocket* getPockets() const { return pockets; }
    const GameObj::Cue* getCue() const { return cue.get(); }
    const GameObj::Table* getTable() const { return table.get(); }

    int getBallsCount() const { return BALLS_COUNT; }
    int getPocketsCount() const { return POCKETS_COUNT; }

private:
    static constexpr float gravity = 9.81f; 

    GameObj::Ball balls[BALLS_COUNT];
    GameObj::Pocket pockets[POCKETS_COUNT];
    
    std::unique_ptr<GameObj::Cue> cue;
    std::unique_ptr<GameObj::Table> table;

    float time;
    bool isMoving;
    std::pair<int, int> whiteBallInitialPosition;

    void initBalls();
    void initPockets();
    
    void checkBoundaries();
    void checkCollisions();    
    void checkPockets();       
    void updateBallCollisions();
    
    void calculateBallMovement(GameObj::Ball& ball, int steps);
    void calculateBallMovement(GameObj::Ball& ball);
    void transferImpulse(GameObj::Cue& cue, GameObj::Ball& ball);
    void calculateCueVelocityFromMouse(int mouseX, int mouseY);
    
    int sign(float x);


    int aSteps; // количество шагов анимации (для расчета скорости и т.д.)
};

#endif // GAME_H
