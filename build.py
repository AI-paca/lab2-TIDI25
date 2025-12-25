#!/usr/bin/env python3
import os
import sys
import subprocess
import platform
import shutil

def main():
    print("=== Universal Pool Game Builder ===")

    # Определяем систему
    system = platform.system()
    print(f"Detected system: {system}")

    # Создаем папку build
    build_dir = "build"
    if os.path.exists(build_dir):
        shutil.rmtree(build_dir)
    os.makedirs(build_dir)
    print(f"Created {build_dir} directory")

    # Переходим в папку build
    os.chdir(build_dir)

    # Генерируем проект с помощью CMake
    print("Running CMake...")
    cmake_cmd = ["cmake", ".."]
    if system == "Windows":
        # Пробуем найти генератор для Windows
        try:
            # Пробуем Visual Studio
            result = subprocess.run(["cmake", "-G", "Visual Studio 17 2022", ".."],
                                  capture_output=True, text=True)
            if result.returncode != 0:
                # Пробуем MinGW
                result = subprocess.run(["cmake", "-G", "MinGW Makefiles", ".."],
                                      capture_output=True, text=True)
        except:
            pass
    else:
        # Linux/macOS - используем стандартный генератор
        subprocess.run(cmake_cmd, check=True)

    # Собираем проект
    print("Building project...")
    if system == "Windows":
        # На Windows используем cmake --build
        subprocess.run(["cmake", "--build", ".", "--config", "Release"], check=True)
    else:
        # На Linux/macOS используем make
        subprocess.run(["make"], check=True)

    # Копируем результат в корневую папку
    print("Copying result to root directory...")
    for file in os.listdir("."):
        if file.startswith("pool_game") and file.endswith(".so"):
            shutil.copy(file, "../")
            print(f"Copied {file} to root directory")
            break

    print("=== Build completed successfully! ===")
    print("You can now run: python main.py")

if __name__ == "__main__":
    main()
