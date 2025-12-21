// Реализация класса игры бильярд
#include "game.h"

Game::Game(int aSteps) {
    // Инициализация игры
    steps = aSteps;
    balls.reserve(10); // резервируем место шаров

    // Создаем белый шар
    balls.push_back(std::make_unique<Ball>());
    balls[0]->position = {100, 100}; // задаем позицию белого шара
    balls[0]->velocity = {0, 0}; // начальная скорость
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

void Game::calculateBallMovement(Ball& ball) {
    // функция расчета конечной точки (т.е. скорость шара падает со временем из-за трения)
}

void Game::strikeCueAtBall(Cue& cue, Ball& ball) {
    // расчет скорости кия //дельта x и дельта y
}

void Game::transferImpulse(Cue& cue, Ball& ball) {
    // передача импульса от кия к белому шару //записываем дельта х и дельта у в скорость шара
}

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
            balls_array[i].velocity = balls[i]->velocity;
            balls_array[i].radius = balls[i]->radius;
            balls_array[i].color = balls[i]->color;
        }
    }

    return count;
}
