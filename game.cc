// Реализация класса игры бильярд
#include "game.h"

Game::Game(int animationSteps) {
    // Инициализация игры
    time  = animationSteps; // количество шагов анимации
    isMoving = true; // В начале шары движутся

    // Инициализация массива шаров (стандартный набор Aramith)
    for (int i = 0; i < 10; i++) {
        balls[i].radius = 10;
        balls[i].speed = {0, 0};
        balls[i].position = {100 + i * 20, 200};
    }

    // Биток (белый шар) - специальная инициализация
    balls[0].position = {100, 100};
    balls[0].speed = {10, 10};
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
    // обновление скорости шаров при столкновение друг с другом
    //что-то с углами и импульсами
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
    float a = frictionCoefficient * gravity; // коэффициент замедления: a = μ_k · g

    //v(t) = v₀ – a · t, но t=1, поэтому v(t) = v₀ – a; steps - количество пропущеных шагов анимации 
    ball.speed = {ball.speed.first - a * steps, ball.speed.second - a * steps};

    if (ball.speed.first*ball.speed.first < 0.1f) {
        ball.speed.first = 0;
    }
    if (ball.speed.second*ball.speed.second < 0.1f) {
        ball.speed.second = 0;
    }
    
    ball.position = {ball.position.first + ball.speed.first, ball.position.second  + ball.speed.second};
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
