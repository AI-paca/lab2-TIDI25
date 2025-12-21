#include "interfaces.cc"
#include "game.cc"

class GameController : public IGameController {
private:
    // Указатель на игровой объект
    Game* game;

public:
    GameController() {
        // инициализация
        // определить единую систему координат для всего проекта
        // понять где тут начало, а где конец координат (вдруг эту библиотеку запускает в на чем-то где 0, 0 это центр или левый нижний угол или правый верхний, моя библиотека вообще хз кто и где её запускает) 
        game = nullptr;
    }

 
    // 2. start/reset
    void start() override {
        if (game) {
            game->resetGame();
        }
    }

 
    // 3. Is everyting OK? //проверка границ, стоконовений, выброс исключений и прочие методы
    bool validateState() override {
        if (game) {
            game->checkBoundaries();
            game->checkCollisions();
        }
        return true;
    }

 
    // 4. What should I do? (какие-то параметры которые передал нам питон) -> вызов чего-нибудь из основного класса // методы обработки пользовательского ввода/действий
    void processInput(int x, int y) override {
        // Обработка пользовательского ввода
    }

 
    // 5. I guessos all good //вернуть питону объекты класса через get которые он попросил (а судя по pygame он просит все, т.ч. этот метод будет возвращать список из всех шаров, кия и стола)
    std::vector<void*> getRenderData() override {
        // Возврат всех объектов для рендеринга
        return {};
    }

    //на случай если у нас будет не pygame - вернуть список не всех эллементов, а только шаров, только кия или только стола (+ 3 ф-ии)
    // std::vector<void*> getBalls() {
    //     return {};
    // }

    // std::vector<void*> getCue() {
    //     return {};
    // }

    // std::vector<void*> getTable() {
    //     return {};
    // }

    void update() override {
        // Обновление состояния
        if (game) {
            game->updateBallCollisions();
        }
    }
};
