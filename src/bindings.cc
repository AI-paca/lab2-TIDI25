#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "interfaces.h"
#include "GameObj.h"
#include "vec.h"

namespace py = pybind11;

PYBIND11_MODULE(pool_game, m) {
    m.doc() = "Pool Game Physics Engine";

    // 0. vec type binding
    py::class_<vec>(m, "vec")
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &vec::x)
        .def_readwrite("y", &vec::y)
        .def("__repr__", [](const vec& v) {
            return "vec(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
        })
        .def("__getitem__", [](const vec& v, size_t i) {
            if (i == 0) return v.x;
            if (i == 1) return v.y;
            throw py::index_error("vec index out of range");
        })
        .def("__len__", [](const vec& v) {
            return 2;
        });

    // 1. Enum Color
    py::enum_<GameObj::Color>(m, "Color")
        .value("WHITE", GameObj::Color::WHITE)
        .value("YELLOW", GameObj::Color::YELLOW)
        .value("BLUE", GameObj::Color::BLUE)
        .value("RED", GameObj::Color::RED)
        .value("PURPLE", GameObj::Color::PURPLE)
        .value("ORANGE", GameObj::Color::ORANGE)
        .value("GREEN", GameObj::Color::GREEN)
        .value("MAROON", GameObj::Color::MAROON)
        .value("BLACK", GameObj::Color::BLACK)
        .value("YELLOW_STRIPED", GameObj::Color::YELLOW_STRIPED)
        .export_values();

    // 2. Структуры данных
    py::class_<GameObj::Ball>(m, "Ball")
        .def_readwrite("position", &GameObj::Ball::position)
        .def_readwrite("speed", &GameObj::Ball::speed)
        .def_readwrite("radius", &GameObj::Ball::radius)
        .def_readwrite("color", &GameObj::Ball::color)
        .def_readwrite("is_pocketed", &GameObj::Ball::isPocketed);

    py::class_<GameObj::Pocket>(m, "Pocket")
        .def_readwrite("position", &GameObj::Pocket::position)
        .def_readwrite("radius", &GameObj::Pocket::radius);

    py::class_<GameObj::Cue>(m, "Cue")
        .def_readwrite("position", &GameObj::Cue::position)
        .def_readwrite("direction", &GameObj::Cue::direction)
        .def_readwrite("force", &GameObj::Cue::force)
        .def_readwrite("is_active", &GameObj::Cue::isActive);

    py::class_<GameObj::Table>(m, "Table")
        .def_readwrite("left_top", &GameObj::Table::leftTop)
        .def_readwrite("right_bottom", &GameObj::Table::rightBottom)
        .def_readwrite("friction", &GameObj::Table::frictionCoefficient)
        .def_readwrite("border_thickness", &GameObj::Table::borderThickness);

    // 3. ТОЛЬКО ИНТЕРФЕЙС (без конструкторов!)
    py::class_<IGameController, std::unique_ptr<IGameController>>(m, "IGameController")
        .def("start", &IGameController::start)
        .def("update", &IGameController::update)
        .def("aim_cue", &IGameController::aimCue)
        .def("shoot_cue", &IGameController::shootCue)
        .def("get_balls", &IGameController::getBalls)
        .def("get_pockets", &IGameController::getPockets)
        .def("get_cue", &IGameController::getCue)
        .def("get_table", &IGameController::getTable);

    // 4. ФАБРИЧНЫЕ ФУНКЦИИ (3 перегрузки)
    // Pybind11 автоматически выберет нужную по количеству аргументов
    m.def("create_controller", 
          py::overload_cast<int>(&createController),
          py::arg("steps"),
          "Create controller with default table size (800x600)");

    m.def("create_controller", 
          py::overload_cast<int, int, int>(&createController),
          py::arg("steps"), py::arg("screen_width"), py::arg("screen_height"),
          "Create controller with auto-centered table");

    m.def("create_controller", 
          py::overload_cast<int, int, int, int, int>(&createController),
          py::arg("steps"), py::arg("left"), py::arg("top"), py::arg("right"), py::arg("bottom"),
          "Create controller with explicit table coordinates");
}
