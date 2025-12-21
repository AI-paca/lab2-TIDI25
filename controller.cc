#include "controller.h"

// Реализация конструктора
GameController::GameController(int animationSteps) {
    game = new Game(animationSteps);
    // инициализация
    // определить единую систему координат для всего проекта
    // понять где тут начало, а где конец координат (вдруг эту библиотеку запускает в на чем-то где 0, 0 это центр или левый нижний угол или правый верхний, моя библиотека вообще хз кто и где её запускает)
}

// 2. start/reset
void GameController::start() {
    if (game) {
        game->resetGame();
    }
}

// 3. Is everyting OK? //проверка границ, стоконовений, выброс исключений и прочие методы
bool GameController::validateState() {
    if (game) {
        game->checkBoundaries();
        game->checkCollisions();
    }
    return true;
}

// 4. What should I do? (какие-то параметры которые передал нам питон) -> вызов чего-нибудь из основного класса // методы обработки пользовательского ввода/действий
void GameController::processInput(int x, int y) {
    // Обработка пользовательского ввода
}

// 5. I guessos all good //вернуть питону объекты класса через get которые он попросил (а судя по pygame он просит все, т.ч. этот метод будет возвращать список из всех шаров, кия и стола)
std::vector<void*> GameController::getRenderData() {
    // Возврат всех объектов для рендеринга
    return {};
}

//////////////////////////////////////////////

//на случай если у нас будет не pygame - вернуть список не всех эллементов, а только шаров, только кия или только стола (+ 3 ф-ии)
std::vector<void*> GameController::getBalls() {
    std::vector<void*> result;
    if (game) {
        const auto& balls = game->getBalls();
        if (!balls.empty()) {
            // Возвращаем указатели на все шары из контейнера
            for (const auto& ball : balls) {
                result.push_back(ball.get());
            }
        }
    }
    return result;
}

// std::vector<void*> getCue() {
//     return {};
// }

// std::vector<void*> getTable() {
//     return {};
// }

/////////////////////////////////////////////////
    
void GameController::update() {
    // Обновление состояния
    if (game) {
        game->updateBallCollisions();
    }
}
