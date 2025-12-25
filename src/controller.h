#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "interfaces.h"
#include <vector>

class GameController : public IGameController {
private:
    Game* game;

public:
    // Конструкторы
    GameController(int animationSteps);
    GameController(int animationSteps, int screenWidth, int screenHeight);
    GameController(int animationSteps, int left, int top, int right, int bottom);
    ~GameController();

    void start() override;
    void update(int steps) override;

    void aimCue(int mouseX, int mouseY) override;
    void shootCue() override;

    std::vector<GameObj::Ball> getBalls() const override;
    std::vector<GameObj::Pocket> getPockets() const override;
    GameObj::Cue getCue() const override;
    GameObj::Table getTable() const override;
};

#endif // CONTROLLER_H
