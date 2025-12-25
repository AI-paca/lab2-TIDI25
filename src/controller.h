#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "interfaces.h"

class GameController : public IGameController {
public:
    // Конструкторы
    GameController(int animationSteps);
    GameController(int animationSteps, int left, int top, int right, int bottom);

    // Основные методы интерфейса
    void start() override;
    bool validateState() override;
    std::vector<void*> getRenderData() override;
    void update() override;

    int getBallsAsArray(void* balls, int max_count) override;
    int getPocketsAsArray(void* pockets, int max_count) override;
    const void* getCue() override;
    void getTable(int* leftTop, int* rightBottom, float* friction, int* borderThickness) override;

    // Новые методы для управления кием
    void aimCue(int mouseX, int mouseY) override;
    void shootCue() override;

private:
    // Указатель на игровой объект
    Game* game;
};

#endif // CONTROLLER_H
