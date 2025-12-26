// Реализация класса игры бильярд
#include "game.h"
#include <cmath>
#include <iostream>


Game::Game(int animationSteps) {
    // Инициализация игры
    time  = animationSteps; // количество шагов анимации
    isMoving = false;

    // Инициализация массива шаров (стандартный набор Aramith)
    for (int i = 0; i < 10; i++) {
        balls[i].radius = 10;
        balls[i].speed = vec(0, 0);
        balls[i].isPocketed = false;
    }

    cue = std::make_unique<GameObj::Cue>();
    table = std::make_unique<GameObj::Table>();

    cue->position = vec(300.0f, 300.0f);
    cue->direction = vec(1.0f, 0.0f);
    cue->force = 0.0f; 
    cue->isActive = true; //default

    table->leftTop = vec(50, 50);
    table->rightBottom = vec(750, 1000);
    table->frictionCoefficient = 0.015f;
    table->borderThickness = balls[0].radius * 2; // ширина бортика = диаметру шара

    // Инициализируем лузы и шары с правильной расстановкой
    initPockets();
    initBalls();
}

Game::Game(int animationSteps, int left, int top, int right, int bottom) {
    // Инициализация игры с заданными координатами стола
    time  = animationSteps; // количество шагов анимации
    isMoving = false;

    // Инициализация массива шаров (стандартный набор Aramith)
    for (int i = 0; i < 10; i++) {
        balls[i].radius = 10;
        balls[i].speed = {0, 0};
        balls[i].isPocketed = false;
    }

    cue = std::make_unique<GameObj::Cue>();
    table = std::make_unique<GameObj::Table>();

    cue->position = {300.0f, 300.0f};
    cue->direction = {1.0f, 0.0f};
    cue->force = 0.0f; 
    cue->isActive = true; //default

    // Устанавливаем координаты стола из параметров
    table->leftTop = vec(left, top);
    table->rightBottom = vec(right, bottom);
    table->frictionCoefficient = 0.015f;
    table->borderThickness = balls[0].radius * 2; // ширина бортика = диаметру шара

    // Инициализируем лузы и шары с правильной расстановкой
    initPockets();
    initBalls();
}

void Game::initPockets() {
    float left = table->leftTop.x;
    float top = table->leftTop.y;
    float right = table->rightBottom.x;
    float bottom = table->rightBottom.y;
    float centerX = (left + right) / 2.0f;

    // Радиус лузы = 1.2 × радиус шара
    float pocketRadius = balls[0].radius * 1.2f;

    // Массив координат 6 луз (центры ровно на рамке стола)
    vec positions[POCKETS_COUNT] = {
        vec(left, top),        // 0: верхний левый угол (ровно на рамке)
        vec(centerX, top),     // 1: верхний центр (ровно на верхней рамке)
        vec(right, top),       // 2: верхний правый угол (ровно на рамке)
        vec(left, bottom),     // 3: нижний левый угол (ровно на рамке)
        vec(centerX, bottom),  // 4: нижний центр (ровно на нижней рамке)
        vec(right, bottom)     // 5: нижний правый угол (ровно на рамке)
    };

    // Создаём лузы циклом
    for (int i = 0; i < POCKETS_COUNT; i++) {
        pockets[i].position = positions[i];
        pockets[i].radius = pocketRadius;
    }
}

void Game::initBalls() {
    float tableWidth = table->rightBottom.x - table->leftTop.x;
    float tableHeight = table->rightBottom.y - table->leftTop.y;

    bool horizontal = tableWidth >= tableHeight;
    float majorAxis = horizontal ? tableWidth : tableHeight;

    // === ГЕОМЕТРИЧЕСКИЕ КОНСТАНТЫ ===
    const float BALL_RADIUS = 10.0f;
    const float BALL_DIAMETER = BALL_RADIUS * 2.0f;
    const float GAP = 1.0f; // Маленький зазор между шарами (чтобы не слипались)

    // Расстояние между центрами шаров в одном ряду
    const float SPACING_IN_ROW = BALL_DIAMETER + GAP;

    // Расстояние между рядами (высота равностороннего треугольника)
    // h = a * √3 / 2, где a = расстояние между центрами
    const float SPACING_BETWEEN_ROWS = SPACING_IN_ROW * std::sqrt(3.0f) / 2.0f;

    // === БИТОК (3/4 большой стороны) ===
    if (horizontal) {
        balls[0].position = vec(
            table->leftTop.x + majorAxis * 0.75f,
            table->leftTop.y + tableHeight / 2.0f
        );
    } else {
        balls[0].position = vec(
            table->leftTop.x + tableWidth / 2.0f,
            table->leftTop.y + majorAxis * 0.75f
        );
    }
    balls[0].speed = vec(0, 0);
    balls[0].color = GameObj::Color::WHITE;
    balls[0].isPocketed = false;
    balls[0].radius = BALL_RADIUS;
    whiteBallInitialPosition = balls[0].position;

    // === РОМБ (1/4 большой стороны) ===
    float baseX, baseY;
    if (horizontal) {
        baseX = table->leftTop.x + majorAxis * 0.25f;
        baseY = table->leftTop.y + tableHeight / 2.0f;
    } else {
        baseX = table->leftTop.x + tableWidth / 2.0f;
        baseY = table->leftTop.y + majorAxis * 0.25f;
    }

    int rows[] = {1, 2, 3, 2, 1}; // Ромб из 9 шаров
    int ballIdx = 1;

    for (int row = 0; row < 5 && ballIdx < BALLS_COUNT; row++) {
        for (int col = 0; col < rows[row] && ballIdx < BALLS_COUNT; col++) {
            // Смещение вдоль направления игры (от битка к ромбу)
            float offsetPrimary = (row - 2) * SPACING_BETWEEN_ROWS;

            // Смещение поперек направления игры (вверх-вниз или влево-вправо)
            float offsetSecondary = (col - (rows[row] - 1) / 2.0f) * SPACING_IN_ROW;

            if (horizontal) {
                balls[ballIdx].position = vec(baseX + offsetPrimary, baseY + offsetSecondary);
            } else {
                balls[ballIdx].position = vec(baseX + offsetSecondary, baseY + offsetPrimary);
            }

            balls[ballIdx].speed = vec(0, 0);
            balls[ballIdx].color = static_cast<GameObj::Color>(ballIdx);
            balls[ballIdx].isPocketed = false;
            balls[ballIdx].radius = BALL_RADIUS;

            ballIdx++;
        }
    }
}

void Game::checkPockets() {
    for (int i = 0; i < BALLS_COUNT; i++) {
        if (balls[i].isPocketed) continue; // Пропускаем уже забитые

        for (int p = 0; p < POCKETS_COUNT; p++) {
            float dx = balls[i].position.x - pockets[p].position.x;
            float dy = balls[i].position.y - pockets[p].position.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            // Центр шара попал в круг лузы?
            // Для угловых луз (0, 2, 3, 5) используем больший радиус detection
            float effectiveRadius = pockets[p].radius;

            if (distance <= effectiveRadius*1.25f) {
                if (i == 0) {
                    // Белый шар: телепортируем на начальную позицию
                    balls[i].speed = vec(0, 0);
                    balls[i].position = whiteBallInitialPosition;
                } else {
                    balls[i].isPocketed = true;
                    balls[i].speed = vec(0, 0);
                    balls[i].position = vec(-100, -100); // Убираем за экран
                }
                break;
            }
        }
    }
}

void Game::resetGame() {
    // Сбрасываем все шары
    for (int i = 0; i < BALLS_COUNT; i++) {
        balls[i].speed = {0, 0};
        balls[i].isPocketed = false;
    }
    
    // Переставляем шары в начальные позиции
    initBalls();
    
    // Сбрасываем состояние игры
    isMoving = false;
    time = 0;
    
    // Сбрасываем кий
    cue->force = 0.0f;
    cue->isActive = true;
}

void Game::checkBoundaries() {
    for (int i = 0; i < BALLS_COUNT; i++) {
        GameObj::Ball& ball = balls[i];
        float r = ball.radius;

        // Левая стенка
        if (ball.position.x - r < table->leftTop.x) {
            ball.position.x = table->leftTop.x + r;
            ball.speed.x = -ball.speed.x * 0.3f; // потеря энергии при ударе
        }
        // Правая стенка
        if (ball.position.x + r > table->rightBottom.x) {
            ball.position.x = table->rightBottom.x - r;
            ball.speed.x = -ball.speed.x * 0.3f;
        }
        // Верхняя стенка
        if (ball.position.y - r <  table->leftTop.y) {
            ball.position.y = table->leftTop.y + r;
            ball.speed.y = -ball.speed.y * 0.3f;
        }
        // Нижняя стенка
        if (ball.position.y + r > table->rightBottom.y) {
            ball.position.y = table->rightBottom.y - r;
            ball.speed.y = -ball.speed.y * 0.3f;
        }
    }
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
                GameObj::Ball& ballA = balls[i];
                GameObj::Ball& ballB = balls[j];

                // Вектор между центрами
                float dx = ballB.position.x - ballA.position.x;
                float dy = ballB.position.y - ballA.position.y;
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

                ballA.position.x  -= separation * nx;
                ballA.position.y -= separation * ny;
                ballB.position.x  += separation * nx;
                ballB.position.y += separation * ny;

                // относительная скорость
                float vRelX = ballA.speed.x - ballB.speed.x;
                float vRelY = ballA.speed.y - ballB.speed.y;

                // проекция относительной скорости на нормаль
                float vn = vRelX * nx + vRelY * ny;

                // если шары уже расходятся — не меняем скорости
                if (vn <= 0.0f) {
                    continue;
                }

                // обмен компонентами вдоль нормали
                ballA.speed.x  -= vn * nx;
                ballA.speed.y -= vn * ny;
                ballB.speed.x  += vn * nx;
                ballB.speed.y += vn * ny;
            }
        }

        // Если не было столкновений — выходим раньше
        if (!hadCollision) {
            break;
        }
    }
}


void Game::calculateBallMovement(GameObj::Ball& ball){
    calculateBallMovement(ball, 1);
}

void Game::calculateBallMovement(GameObj::Ball& ball, int steps) { //торможение шара
    

    if (ball.speed.x == 0 && ball.speed.y == 0) {
        return; // шар уже остановился
    }

    // F_тр = μ_k · m · g
    // v(t) = v₀ – μ_k · g · t
    float a = table->frictionCoefficient * gravity; // коэффициент замедления: a = μ_k · g
    if (abs(ball.speed.x)  <= abs(a * steps )) {
        ball.speed.x = 0;
    }
    else{ //v(t) = v₀ – a · t, но t=1, поэтому v(t) = v₀ – a; steps - количество пропущеных шагов анимации
        ball.speed.x -= a * steps * sign(ball.speed.x); //добавляем знак, т.к. при отрицательной скорости шары ускорялись
    }
    if (abs(ball.speed.y)  <= abs(a * steps )) {
        ball.speed.y = 0;
    }
    else{
        ball.speed.y -= a * steps * sign(ball.speed.y);
    }

    ball.position = vec(ball.position.x + ball.speed.x, ball.position.y + ball.speed.y);
}

// void Game::calculateBallMovement(Ball& ball, int steps) { //gemini physics (выглядит спорно, но оставлю тут на случай если я захочу исправить проблемы с физикой, сейчас он их не исправит)
//     if (ball.speed.first == 0 && ball.speed.second == 0) return;

//     // 1. Считаем полную скорость (длину вектора)
//     float speed = std::sqrt(ball.speed.first * ball.speed.first + ball.speed.second * ball.speed.second);
    
//     // 2. Считаем, сколько скорости отнимет трение
//     float friction = table->frictionCoefficient * gravity * steps;

//     // 3. Вычитаем трение
//     float newSpeed = speed - friction;
    
//     // Если трение "съело" всю скорость — останавливаем
//     if (newSpeed <= 0) {
//         ball.speed = {0, 0};
//         return;
//     }

//     // 4. Масштабируем вектор скорости (сохраняем направление!)
//     float scale = newSpeed / speed;
//     ball.speed.first *= scale;
//     ball.speed.second *= scale;

//     // 5. Двигаем шар
//     ball.position.first += ball.speed.first;
//     ball.position.second += ball.speed.second;
// }

int Game::sign(float x) {
    if (x > 0) return 1;
    else return -1;
}


void Game::aimCue(int mouseX, int mouseY) {
    if (isMoving) {
        cue->isActive = false; // Скрываем кий
        return;
    }

    cue->isActive = true;
    GameObj::Ball& whiteBall = balls[0];

    // куда ударит кий
    float dx = whiteBall.position.x - mouseX;
    float dy = whiteBall.position.y - mouseY;

    // сила натяжения (через расстояние от мыши до шара)
    float distance = std::sqrt(dx * dx + dy * dy);

    const float MAX_PULL_DISTANCE = 200.0f; 
    const float MAX_SHOT_SPEED = 50.0f;

    float clampedDistance = distance;
    if (clampedDistance > MAX_PULL_DISTANCE) {
        clampedDistance = MAX_PULL_DISTANCE;
    }

    // куда полетит шар 
    float dirX = 0.0f;
    float dirY = 0.0f;
    
    if (distance > 0.001f) {
        dirX = dx / distance;
        dirY = dy / distance;
    } else {
        // Если мышь ровно в центре шара, направление по умолчанию (вправо)
        dirX = 1.0f; 
        dirY = 0.0f;
    }

    cue->direction = vec(dirX, dirY);
    cue->force = (clampedDistance / MAX_PULL_DISTANCE) * MAX_SHOT_SPEED;

    // визуальная позиции кия
    // Кий должен быть нарисован с "обратной" стороны от направления удара.
    // position = шар - (направление удара * (визуальное смещение + расстояние до мыши))
    float visualOffsetFromBall = whiteBall.radius + 5.0f; // Отступ от края шара

    // Кий визуально "отъезжает" назад при натяжении
    cue->position.x = whiteBall.position.x - dirX * (visualOffsetFromBall + clampedDistance);
    cue->position.y = whiteBall.position.y - dirY * (visualOffsetFromBall + clampedDistance);
}

// удара (вызывается контроллер по клику мыши)
void Game::shoot() {
    if (isMoving || !cue->isActive) return;

    // передать импульс от кия к шару
    balls[0].speed.x = cue->direction.x * cue->force;
    balls[0].speed.y = cue->direction.y * cue->force;

    if (std::abs(cue->force) > 0.1f) {
        isMoving = true;
        cue->isActive = false; 
        cue->force = 0;  
    }
}

void Game::update() {
    update(1);
}

void Game::update(int steps) {
    // Обновление состояния игры только если есть движение
    if (isMoving) {
        for (int i = 0; i < BALLS_COUNT; i++) {
            calculateBallMovement(balls[i]);
        }
        updateBallCollisions();
        checkBoundaries() ; // Проверка границ стола
        checkPockets();     // Проверка попадания в лузы
        time++; // Увеличиваем счетчик шагов

        // Проверяем, остановились ли все шары
        bool anyBallMoving = false;
        for (int i = 0; i < BALLS_COUNT; i++) {
            if (balls[i].speed.x != 0 || balls[i].speed.y != 0) {
                anyBallMoving = true;
                break;
            }
        }
        isMoving = anyBallMoving;
    }
}
