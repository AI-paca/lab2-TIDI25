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

// Метод для получения массива шаров для Python
// Копирует данные в предварительно выделенный буфер
int GameController::getBallsAsArray(void* balls, int max_count) {
    if (!balls || max_count <= 0 || !game) {
        return 0;
    }

    Game::Ball* balls_array = static_cast<Game::Ball*>(balls);
    int count = std::min(Game::BALLS_COUNT, max_count);
    const Game::Ball* source_balls = game->getBalls();

    // Эффективное копирование данных
    for (int i = 0; i < count; i++) {
        balls_array[i] = source_balls[i];
    }

    return count;
}

// Метод для получения кия
const void* GameController::getCue() {
    if (!game) {
        return nullptr;
    }
    return game->getCue().get();
}

// Метод для получения стола
const void* GameController::getTable() {
    if (!game) {
        return nullptr;
    }
    return game->getTable().get();
}

/////////////////////////////////////////////////

void GameController::update() {
    // Обновление состояния
    if (game) {
        game->update();
    }
}

// ==========================================
// EXTERN C: ЭКСПОРТ ДЛЯ PYTHON
// ==========================================
// Реализация функций для Python (объявления в interfaces.h)

// Глобальный контроллер
static GameController* g_controller = nullptr;

extern "C" void create_game_controller(int steps) {
    if (g_controller) {
        delete g_controller;
    }
    g_controller = new GameController(steps);
    g_controller->start();
}

extern "C" int get_balls_array(void* balls, int max_count) {
    if (!g_controller || !balls || max_count <= 0) {
        return 0;
    }

    // Использовать метод из GameController
    return g_controller->getBallsAsArray(balls, max_count);
}

extern "C" const void* get_cue() {
    if (!g_controller) {
        return nullptr;
    }
    return g_controller->getCue();
}

extern "C" const void* get_table() {
    if (!g_controller) {
        return nullptr;
    }
    return g_controller->getTable();
}

extern "C" void update_game() {
    if (g_controller) {
        g_controller->update();
    }
}
