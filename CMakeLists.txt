cmake_minimum_required(VERSION 3.10)
project(PoolGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Находим Python и pybind11
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

# Исходники
set(SOURCES 
    src/game.cc 
    src/controller.cc 
    src/bindings.cc 
)

# Создаем модуль Python (он сам добавит префикс и расширение .so/.pyd)
pybind11_add_module(pool_game ${SOURCES})

# Включаем директории заголовков
target_include_directories(pool_game PRIVATE src)

# Добавляем цель для очистки
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMENT "Cleaning build directory"
)
