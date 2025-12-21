#include <vector>

class IUpdate { //попросить класс обновить все на один шаг
public:
    virtual ~IUpdate() = default; // деструктор для интерфейса
    virtual void update() = 0; // "=0" делает метод полностью виртуальным 
};

class IRenderable { //крассивое название для I guessos all good, т.е. get_список_всех_эллементов
public:
    virtual ~IRenderable() = default;
    virtual std::vector<void*> getAllElements() = 0;
};

class IGameController { // I_все_методы_контроллера; для того что бы питон знал об их существовании и мог вызвать     
public:
    virtual ~IGameController() = default;
    virtual void start() = 0;
    virtual bool validateState() = 0;
    virtual void processInput(int x, int y) = 0;
    virtual std::vector<void*> getRenderData() = 0;
    virtual void update() = 0;
};
