#ifndef INTERFACES_H
#define INTERFACES_H

#include <vector>
#include <memory>
#include "GameObj.h"

class IGameController {
public:
    virtual ~IGameController() = default;
    virtual void start() = 0;
    virtual void update(int steps) = 0;

    virtual void aimCue(int mouseX, int mouseY) = 0;
    virtual void shootCue() = 0;

    virtual std::vector<GameObj::Ball> getBalls() const = 0;
    virtual std::vector<GameObj::Pocket> getPockets() const = 0;
    virtual GameObj::Cue getCue() const = 0;
    virtual GameObj::Table getTable() const = 0;
};

// ф-ии для создания контроллера
std::unique_ptr<IGameController> createController(int steps);
std::unique_ptr<IGameController> createController(int steps, int screenWidth, int screenHeight);
std::unique_ptr<IGameController> createController(int steps, int left, int top, int right, int bottom);

#endif // INTERFACES_H
