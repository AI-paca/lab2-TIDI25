#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "interfaces.h"

class GameController : public IGameController {
public:
    // Конструктор
    GameController(int animationSteps);

    // Основные методы интерфейса
    void start() override;
    bool validateState() override;
    void processInput(int x, int y) override;
    std::vector<void*> getRenderData() override;
    void update() override;

    int getBallsAsArray(void* balls, int max_count) override;
    const void* getCue() override;
    const void* getTable() override;

private:
    // Указатель на игровой объект
    Game* game;
};

#endif // CONTROLLER_H
