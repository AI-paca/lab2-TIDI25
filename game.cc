// Реализация класса игры бильярд
#include "game.h"
#include <cmath>


Game::Game(int animationSteps) {
    // Инициализация игры
    time  = animationSteps; // количество шагов анимации
    isMoving = true; // В начале шары движутся

    // Инициализация массива шаров (стандартный набор Aramith)
    for (int i = 0; i < 10; i++) {
        balls[i].radius = 10;
        balls[i].speed = {0, 0};
        balls[i].position = {100 + i * 20, 200 + i * 10};
    }

    // Биток (белый шар) - специальная инициализация
    balls[0].position = {100, 100};
    balls[0].speed = {10, 12};
    balls[0].color = Color::WHITE;

    // Остальные шары по порядку
    balls[1].color = Color::YELLOW;     // 1: жёлтый
    balls[2].color = Color::BLUE;       // 2: синий
    balls[3].color = Color::RED;        // 3: красный
    balls[4].color = Color::PURPLE;     // 4: фиолетовый
    balls[5].color = Color::ORANGE;     // 5: оранжевый
    balls[6].color = Color::GREEN;      // 6: зелёный
    balls[7].color = Color::MAROON;     // 7: бордовый
    balls[8].color = Color::BLACK;      // 8: чёрный
    balls[9].color = Color::YELLOW_STRIPED; // 9: жёлтый с полосой

    cue = std::make_unique<Cue>();
    table = std::make_unique<Table>();
    
    table->leftTop = {200, 200};
    table->rightBottom = {600, 400};
    table->frictionCoefficient = 0.015f;
}

void Game::resetGame() {
    // Внутренний сброс игры
}

void Game::checkBoundaries() {
    // Внутренняя проверка границ
}

void Game::checkCollisions() {
    // Внутренняя проверка столкновений
}

void Game::updateBallCollisions() {    
    const int maxIterations = 8; //несколько проходов для стабильности при множественных столкновениях
    
    for (int iter = 0; iter < maxIterations; iter++) {
        bool hadCollision = false;
        
        for (int i = 0; i < BALLS_COUNT; i++) {
            for (int j = i + 1; j < BALLS_COUNT; j++) {
                Ball& ballA = balls[i];
                Ball& ballB = balls[j];

                // Вектор между центрами
                float dx = ballB.position.first - ballA.position.first;
                float dy = ballB.position.second - ballA.position.second;
                float distSq = dx * dx + dy * dy;
                
                float minDist = ballA.radius + ballB.radius;
                float minDistSq = minDist * minDist;

                // Нет столкновения
                if (distSq >= minDistSq || distSq < 0.0001f) {
                    continue;
                }

                float distance = std::sqrt(distSq);
                hadCollision = true;

                // нормаль 
                float nx = dx / distance;
                float ny = dy / distance;

                float overlap = minDist - distance;
                float separation = (overlap / 2.0f) + 0.001f; // небольшой зазор во избежание залипания
                
                ballA.position.first  -= separation * nx;
                ballA.position.second -= separation * ny;
                ballB.position.first  += separation * nx;
                ballB.position.second += separation * ny;


                // относительная скорость
                float vRelX = ballA.speed.first - ballB.speed.first;
                float vRelY = ballA.speed.second - ballB.speed.second;
                
                // проекция относительной скорости на нормаль
                float vn = vRelX * nx + vRelY * ny;

                // если шары уже расходятся — не меняем скорости
                if (vn <= 0.0f) {
                    continue;
                }

                // обмен компонентами вдоль нормали
                ballA.speed.first  -= vn * nx;
                ballA.speed.second -= vn * ny;
                ballB.speed.first  += vn * nx;
                ballB.speed.second += vn * ny;
            }
        }
        
        // Если не было столкновений — выходим раньше
        if (!hadCollision) {
            break;
        }
    }
}


void Game::calculateBallMovement(Ball& ball){
    calculateBallMovement(ball, 1);
}

void Game::calculateBallMovement(Ball& ball, int steps) { //торможение шара
    

    if (ball.speed.first == 0 && ball.speed.second == 0) {
        return; // шар уже остановился
    }

    // F_тр = μ_k · m · g
    // v(t) = v₀ – μ_k · g · t
    float a = table->frictionCoefficient * gravity; // коэффициент замедления: a = μ_k · g
    if (abs(ball.speed.first)  <= abs(a * steps )) {
        ball.speed.first = 0;
    }  
    else{ //v(t) = v₀ – a · t, но t=1, поэтому v(t) = v₀ – a; steps - количество пропущеных шагов анимации 
        ball.speed.first -= a * steps * sign(ball.speed.first); //добавляем знак, т.к. при отрицательной скорости шары ускорялись 
    }
    if (abs(ball.speed.second)  <= abs(a * steps )) {
        ball.speed.second = 0; 
    }
    else{
        ball.speed.second -= a * steps * sign(ball.speed.second);
    }

    ball.position = {ball.position.first + ball.speed.first, ball.position.second  + ball.speed.second};
}

int Game::sign(float x) {
    if (x > 0) return 1;
    else return -1;
}

void Game::strikeCueAtBall(Cue& cue, Ball& ball) {
    // расчет скорости кия //дельта x и дельта y
}

void Game::transferImpulse(Cue& cue, Ball& ball) {
    // передача импульса от кия к белому шару //записываем дельта х и дельта у в скорость шара
}

void Game::update() {
    // Обновление состояния игры только если есть движение
    if (isMoving) {
        for (int i = 0; i < BALLS_COUNT; i++) {
            calculateBallMovement(balls[i]);
        }
        updateBallCollisions();
        time++; // Увеличиваем счетчик шагов

        // Проверяем, остановились ли все шары
        bool anyBallMoving = false;
        for (int i = 0; i < BALLS_COUNT; i++) {
            if (balls[i].speed.first != 0 || balls[i].speed.second != 0) {
                anyBallMoving = true;
                break;
            }
        }
        isMoving = anyBallMoving;
    }
}
