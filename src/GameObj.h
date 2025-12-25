#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include <vector>
#include <utility> // для std::pair

namespace GameObj {
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
}

#endif // GAMEOBJ_H
