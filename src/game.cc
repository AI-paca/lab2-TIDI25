#include "game.h"
#include "GameObj.h"
#include <cmath>

void Game::initialize(int steps, float left, float top, float right, float bottom) {
        // Инициализация игры
    time = steps;
    isMoving = false;
    isGameEnd = false;

    table = std::make_unique<GameObj::Table>();
    table->leftTop = vec(left, top);
    table->rightBottom = vec(right, bottom);
    table->frictionCoefficient = 0.015f;

    float tableWidth = table->rightBottom.x - table->leftTop.x;
    
    float calculatedRadius = tableWidth * 0.0225f;    
    if (calculatedRadius < 5.0f) calculatedRadius = 5.0f;

    table->borderThickness = calculatedRadius * 2.0f;

    cue = std::make_unique<GameObj::Cue>();
    cue->position = vec(left + tableWidth/2, top + (bottom-top)/2); // Центр стола
    cue->direction = vec(1.0f, 0.0f);
    cue->force = 0.0f;
    cue->isActive = true;

    for (int i = 0; i < BALLS_COUNT; i++) {
        balls[i].radius = calculatedRadius;
        balls[i].speed = vec(0, 0);
        balls[i].isPocketed = false;
    }

    initPockets();
    initBalls(); 
}

Game::Game(int animationSteps) {
    initialize(animationSteps, 50, 50, 750, 1000);
}

Game::Game(int animationSteps, int left, int top, int right, int bottom) {
    initialize(animationSteps, left, top, right, bottom);
}

void Game::initPockets() {
    float left = table->leftTop.x;
    float top = table->leftTop.y;
    float right = table->rightBottom.x;
    float bottom = table->rightBottom.y;
    float centerX = (left + right) / 2.0f;
    float pocketRadius = balls[0].radius * 1.6f;

    vec positions[POCKETS_COUNT] = {
        vec(left, top),        
        vec(centerX, top),     
        vec(right, top),       
        vec(left, bottom),     
        vec(centerX, bottom),  
        vec(right, bottom)     
    };

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
    const float BALL_DIAMETER = balls[0].radius * 2.0f;
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
    balls[0].color = GameObj::Color::WHITE;
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
            ballIdx++;
        }
    }
}

void Game::checkPockets() {
    for (int i = 0; i < BALLS_COUNT; i++) {
        if (balls[i].isPocketed) continue; 

        for (int p = 0; p < POCKETS_COUNT; p++) {
            vec delta = balls[i].position - pockets[p].position;            
            float distSq = delta.lengthSq(); // Оптимизация: используем квадрат длины, чтобы не считать корень 8 раз за кадр

            float effectiveRadius = pockets[p].radius;
            float captureDistSq = effectiveRadius * effectiveRadius;

            if (distSq <= captureDistSq) {
                if (i == 0) {
                    // Белый шар
                    balls[i].speed = vec(0, 0);
                    balls[i].position = whiteBallInitialPosition;
                } else {
                    balls[i].isPocketed = true;
                    balls[i].speed = vec(0, 0);
                    balls[i].position = vec(-1000, -1000); 
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

    // Устанавливаем флаг окончания игры
    isGameEnd = true;
}

void Game::checkBoundaries() {
    float restitution = 0.3f; // Упругость бортов
    for (int i = 0; i < BALLS_COUNT; i++) {
        GameObj::Ball& ball = balls[i];
        if (ball.isPocketed) continue;

        float r = ball.radius;

        // Левая стенка
        if (ball.position.x < table->leftTop.x + r) {
            ball.position.x = table->leftTop.x + r; // Выталкиваем
            if (ball.speed.x < 0) ball.speed.x *= -restitution; // Отражаем
        }
        // Правая стенка
        else if (ball.position.x > table->rightBottom.x - r) {
            ball.position.x = table->rightBottom.x - r;
            if (ball.speed.x > 0) ball.speed.x *= -restitution;
        }

        // Верхняя стенка
        if (ball.position.y < table->leftTop.y + r) {
            ball.position.y = table->leftTop.y + r;
            if (ball.speed.y < 0) ball.speed.y *= -restitution;
        }
        // Нижняя стенка
        else if (ball.position.y > table->rightBottom.y - r) {
            ball.position.y = table->rightBottom.y - r;
            if (ball.speed.y > 0) ball.speed.y *= -restitution;
        }
    }
}



void Game::updateBallCollisions() {
    for (int i = 0; i < BALLS_COUNT; i++) {
        if (balls[i].isPocketed) continue;

        for (int j = i + 1; j < BALLS_COUNT; j++) {
            if (balls[j].isPocketed) continue;

            GameObj::Ball& A = balls[i];
            GameObj::Ball& B = balls[j];

            vec delta = B.position - A.position;
            float distSq = delta.lengthSq();
            float radiusSum = A.radius + B.radius;
            float radiusSumSq = radiusSum * radiusSum;
            if (distSq < radiusSumSq && distSq > 0.0001f) {
                float dist = std::sqrt(distSq);
                vec normal = delta / dist;

                // выталкивание
                float overlap = radiusSum - dist;
                vec separation = normal * (overlap * 0.5f);
                A.position -= separation;
                B.position += separation;

                // отскок
                // относительная скорость
                vec vRel = B.speed - A.speed;
                float vn = vRel.dot(normal);

                if (vn < 0) {
                    float restitution = 0.9f;
                    float impulseMagnitude = -(1.0f + restitution) * vn / 2.0f;

                    vec impulse = normal * impulseMagnitude;
                    A.speed -= impulse;
                    B.speed += impulse;
                }
            }
        }
    }
}

int Game::sign(float x) {
    if (x > 0) return 1;
    else return -1;
}

void Game::calculateBallMovement(GameObj::Ball& ball) {
    if (ball.isPocketed) return;

    float speed = ball.speed.length();
    if (speed < 0.05f) { // Порог остановки чуть выше 0
        ball.speed = {0, 0};
        return;
    }
    float friction = table->frictionCoefficient * gravity;

    float newSpeed = speed - friction;
    if (newSpeed < 0) newSpeed = 0;
    ball.speed = ball.speed.normalized() * newSpeed;
}

void Game::aimCue(int mouseX, int mouseY) {
    if (isMoving) {
        cue->isActive = false; // Скрываем кий
        return;
    }

    cue->isActive = true;
    GameObj::Ball& whiteBall = balls[0];

    // куда ударит кий
    vec mousePos(mouseX, mouseY);
    vec delta = whiteBall.position - mousePos;

    // сила натяжения (через расстояние от мыши до шара)
    float distance = delta.length();

    const float MAX_PULL_DISTANCE = 200.0f;
    const float MAX_SHOT_SPEED = 50.0f;

    float clampedDistance = distance;
    if (clampedDistance > MAX_PULL_DISTANCE) {
        clampedDistance = MAX_PULL_DISTANCE;
    }

    // куда полетит шар
    vec direction;
    if (distance > 0.001f) {
        direction = delta.normalized();
    } else {
        // Если мышь ровно в центре шара, направление по умолчанию (вправо)
        direction = vec(1.0f, 0.0f);
    }

    cue->direction = direction;
    cue->force = (clampedDistance / MAX_PULL_DISTANCE) * MAX_SHOT_SPEED;

    // визуальная позиции кия
    // Кий должен быть нарисован с "обратной" стороны от направления удара.
    // position = шар - (направление удара * (визуальное смещение + расстояние до мыши))
    float visualOffsetFromBall = whiteBall.radius + 5.0f; // Отступ от края шара

    // Кий визуально "отъезжает" назад при натяжении
    cue->position = whiteBall.position - direction * (visualOffsetFromBall + clampedDistance);
}

// удара (вызывается контроллер по клику мыши)
void Game::shoot() {
    if (isMoving || !cue->isActive) return;

    // передать импульс от кия к шару
    balls[0].speed = cue->direction * cue->force;

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
    if (!isMoving) return;
    
    const int SUB_STEPS = 8;
    float subDt = 1.0f / SUB_STEPS; 

    for (int s = 0; s < SUB_STEPS; s++) {
        for (int i = 0; i < BALLS_COUNT; i++) {
            if (!balls[i].isPocketed) {
                balls[i].position += balls[i].speed * subDt;
            }
        }
        
        checkPockets();
        updateBallCollisions();
        checkBoundaries();
    }

    for (int i = 0; i < BALLS_COUNT; i++) {
        calculateBallMovement(balls[i]);
    }

    isMoving = false;
    for (int i = 0; i < BALLS_COUNT; i++) {
        if (!balls[i].isPocketed && balls[i].speed.lengthSq() > 0.001f) {
            isMoving = true;
            break;
        }
    }

    // Check if game should end (all balls except white are pocketed)
    isGameEnd = true;
    for (int i = 1; i < BALLS_COUNT; i++) {
        if (!balls[i].isPocketed) {
            isGameEnd = false;
            break;
        }
    }

    // If game ended, reset
    if (isGameEnd) {
        resetGame();
        isGameEnd = false; // Reset the flag after game reset
    }
}
