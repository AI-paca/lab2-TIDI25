#ifndef INTERFACES_H
#define INTERFACES_H

#include <vector>

// Forward declaration для GameController
class GameController;

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

    // Метод для передачи массива шаров в Python
    virtual int getBallsAsArray(void* balls, int max_count) = 0;


    virtual const void* getCue() = 0;
    virtual void getTable(int* leftTop, int* rightBottom, float* friction) = 0;
};

// ==========================================
// EXTERN C: ЭКСПОРТ ДЛЯ PYTHON
// ==========================================
// Объявления функций для Python (реализация в controller.cc)
extern "C" void create_game_controller(int steps);
extern "C" int get_balls_array(void* balls, int max_count);
extern "C" const void* get_cue();
extern "C" void get_table(int* leftTop, int* rightBottom, float* friction);
extern "C" void update_game();

#endif // INTERFACES_H
