#ifndef INTERFACES_H
#define INTERFACES_H

#include <vector>

class IUpdate {
public:
    virtual ~IUpdate() = default;
    virtual void update() = 0;
};

class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual std::vector<void*> getAllElements() = 0;
};

class IGameController {
public:
    virtual ~IGameController() = default;
    virtual void start() = 0;
    virtual bool validateState() = 0;
    virtual void processInput(int x, int y) = 0;
    virtual std::vector<void*> getRenderData() = 0;
    virtual void update() = 0;
};

#endif // INTERFACES_H
