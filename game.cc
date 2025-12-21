// Реализация класса игры бильярд
#include "game.h"

Game::Game(int animationSteps) {
    // Инициализация игры
    time  = animationSteps; // количество шагов анимации
    isMoving = true; // В начале шары движутся
    balls.reserve(10); // резервируем место шаров

    // Создаем белый шар
    balls.push_back(std::make_unique<Ball>());
    balls[0]->position = {100, 100}; // задаем позицию белого шара
    balls[0]->speed = {10, 10}; // начальная скорость
    balls[0]->radius = 10; // радиус шара
    balls[0]->color = Color::WHITE; // цвет шара
    
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

void Game::calculateBallMovement(Ball& ball) { //торможение шара
    

    if (ball.speed.first == 0 && ball.speed.second == 0) {
        return; // шар уже остановился
    }

    // F_тр = μ_k · m · g
    // v(t) = v₀ – μ_k · g · t
    float a = frictionCoefficient * gravity; // коэффициент замедления: a = μ_k · g
    a = a/time;

    //v(t) = v₀ – a · t
    ball.speed = {ball.speed.first - a * time, ball.speed.second - a * time};

    if (ball.speed.first*ball.speed.first < 0.1f) {
        ball.speed.first = 0;
    }
    if (ball.speed.second*ball.speed.second < 0.1f) {
        ball.speed.second = 0;
    }
    
    //dx = vx0 * dt - 0.5 * (vx0 / v0) * a * dt * dt
    //dy = vy0 * dt - 0.5 * (vy0 / v0) * a * dt * dt
    int dx = int(ball.speed.first * time - 0.5f * (ball.speed.first / ball.radius) * a * time * time);
    int dy = int(ball.speed.second * time - 0.5f * (ball.speed.second / ball.radius) * a * time * time);
    ball.position = {ball.position.first + dx, ball.position.second + dy};
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
        for (auto& ball : balls) {
            calculateBallMovement(*ball);
        }
        updateBallCollisions();
        time++; // Увеличиваем счетчик шагов

        // Проверяем, остановились ли все шары
        bool anyBallMoving = false;
        for (const auto& ball : balls) {
            if (ball->speed.first != 0 || ball->speed.second != 0) {
                anyBallMoving = true;
                break;
            }
        }
        isMoving = anyBallMoving;
    }
}

/////////////////////////////////////////////////

// Метод для копирования данных из Ball в массив
int Game::getBallsAsArray(Ball* balls_array, int max_count) const {
    if (!balls_array || max_count <= 0) {
        return 0;
    }

    // Получить шары из контейнера
    int count = std::min((int)balls.size(), max_count);

    // Заполнить массив данными из каждого шара
    for (int i = 0; i < count; i++) {
        if (balls[i]) {
            balls_array[i].position = balls[i]->position;
            balls_array[i].speed = balls[i]->speed;
            balls_array[i].radius = balls[i]->radius;
            balls_array[i].color = balls[i]->color;
        }
    }

    return count;
}
