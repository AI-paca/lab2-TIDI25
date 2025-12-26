#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "vec.h"

namespace GameObj {
    enum Color { WHITE, YELLOW, BLUE, RED, PURPLE, ORANGE, GREEN, MAROON, BLACK, YELLOW_STRIPED }; // цвета шаров

    struct Ball {
        vec position; // позиция/центр шара {x, y}
        vec speed; // скорость шара {vx, vy}
        float radius;
        Color color; //цвет шара
        bool isPocketed = false; // флаг, что шар забит в лузу
    };

    struct Pocket {
        vec position;
        float radius;
    };

    struct Cue {
        vec position;  // Координаты "носика" кия {x, y}
        vec direction; // направление кия {dirX, dirY}
        float force;                       // Сила удара
        bool isActive;
    };

    struct Table {
        vec leftTop; // левый верхний угол стола {x, y}
        vec rightBottom; // правый нижний угол стола
        float frictionCoefficient; // коэффициент трения
        int borderThickness; // ширина бортика = диаметру шара
    };
}

#endif // GAMEOBJ_H
