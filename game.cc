// Реализация класса игры бильярд
#include "game.h"

Game::Game(int aSteps) {
    // Инициализация игры
    steps = aSteps;
    balls.reserve(10); // резервируем место шаров

    //balls.push_back(std::make_unique<Ball>(std::pair<int, int>(100, 100), std::pair<int, int>(0, 0), 10, Color::WHITE)); // белый шар
    balls.push_back(std::make_unique<Ball>()); // добавляем белый шар
    balls[1]->position = {100, 100}; // задаем позицию белого шара
    balls[1]->velocity = {0, 0}; // начальная скорость
    balls[1]->radius = 10; // радиус шара
    balls[1]->color = Color::WHITE; // цвет шара
    
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
