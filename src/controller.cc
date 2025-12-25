#include "controller.h"

// === КОНСТРУКТОРЫ ===

GameController::GameController(int animationSteps) 
    : GameController(animationSteps, 800, 600) 
{
}

GameController::GameController(int animationSteps, int screenWidth, int screenHeight) {
    int tableWidth = static_cast<int>(screenWidth * 0.7f);
    int tableHeight = static_cast<int>(screenHeight * 0.7f);
    int left = (screenWidth - tableWidth) / 2;
    int top = (screenHeight - tableHeight) / 2;
    
    game = new Game(animationSteps, left, top, left + tableWidth, top + tableHeight);
}

GameController::GameController(int animationSteps, int left, int top, int right, int bottom) {
    game = new Game(animationSteps, left, top, right, bottom);
}

GameController::~GameController() {
    if (game) delete game;
}

void GameController::start() {
    if (game) game->resetGame();
}

void GameController::update(int steps) {
    if (game) game->update(steps);
}

void GameController::aimCue(int mouseX, int mouseY) {
    if (game) game->aimCue(mouseX, mouseY);
}

void GameController::shootCue() {
    if (game) game->shoot();
}

std::vector<GameObj::Ball> GameController::getBalls() const {
    if (!game) return {};
    const auto* ballsPtr = game->getBalls();
    std::vector<GameObj::Ball> result;
    result.reserve(Game::BALLS_COUNT);
    for(int i=0; i<Game::BALLS_COUNT; ++i) {
        result.push_back(ballsPtr[i]);
    }
    return result;
}

std::vector<GameObj::Pocket> GameController::getPockets() const {
    if (!game) return {};
    const auto* pocketsPtr = game->getPockets();
    std::vector<GameObj::Pocket> result;
    for(int i=0; i<6; ++i) {
        result.push_back(pocketsPtr[i]);
    }
    return result;
}

GameObj::Cue GameController::getCue() const {
    if (!game) return {};
    return *(game->getCue());
}

GameObj::Table GameController::getTable() const {
    if (!game) return {};
    return *(game->getTable());
}

// ф-ии для создания контроллера

std::unique_ptr<IGameController> createController(int steps) {
    return std::make_unique<GameController>(steps);
}

std::unique_ptr<IGameController> createController(int steps, int screenWidth, int screenHeight) {
    return std::make_unique<GameController>(steps, screenWidth, screenHeight);
}

std::unique_ptr<IGameController> createController(int steps, int left, int top, int right, int bottom) {
    return std::make_unique<GameController>(steps, left, top, right, bottom);
}
