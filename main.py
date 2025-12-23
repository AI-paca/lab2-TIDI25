import pygame
import sys
import time
import ctypes

# ==========================================
# ПРОСТОЙ ДОСТУП К C++ ДАННЫМ
# ==========================================

# Загрузить C++ библиотеку
libgame = ctypes.CDLL('./libgame.so')

# Определить структуру Ball для Python (как в C++)
class Ball(ctypes.Structure):
    _fields_ = [
        ("position", ctypes.c_int * 2),  # pair<int, int>
        ("velocity", ctypes.c_float * 2),  # pair<float, float>
        ("radius", ctypes.c_int),
        ("color", ctypes.c_int)
    ]

# Определить структуру Cue для Python (как в C++)
class Cue(ctypes.Structure):
    _fields_ = [
        ("position", ctypes.c_int * 2),  # pair<int, int>
        ("direction", ctypes.c_int * 2),  # pair<int, int>
        ("force", ctypes.c_int)
    ]

# Определить структуру Table для Python (как в C++)
class Table(ctypes.Structure):
    _fields_ = [
        ("leftTop", ctypes.c_int * 2),  # pair<int, int>
        ("rightBottom", ctypes.c_int * 2),  # pair<int, int>
        ("frictionCoefficient", ctypes.c_float)
    ]

# Функция для получения данных из C++ через массив
def get_cpp_ball_data():
    # Создать массив для 10 шаров
    balls_array = (Ball * 10)()

    # Вызвать C++ функцию для получения данных
    count = libgame.get_balls_array(balls_array, 10)

    # Преобразовать в список кортежей для Python
    result = []
    for i in range(count):
        ball = balls_array[i]
        result.append((ball.position[0], ball.position[1], ball.radius, ball.color, ball.velocity[0], ball.velocity[1]))

    # Вывести данные в консоль для проверки
    print(f"C++ вернул {count} шаров:")
    for i, obj in enumerate(result):
        print(f"  Шар {i}: x={obj[0]}, y={obj[1]}, radius={obj[2]}, color={obj[3]}, vx={obj[4]}, vy={obj[5]}")

    return result

# Функция для получения данных стола из C++
def get_cpp_table_data():
    # Создаем массивы для хранения данных
    leftTop = (ctypes.c_int * 2)()
    rightBottom = (ctypes.c_int * 2)()
    friction = ctypes.c_float()

    # Вызываем C++ функцию для получения данных стола
    libgame.get_table(leftTop, rightBottom, ctypes.byref(friction))

    # Создаем и возвращаем объект Table
    table = Table()
    table.leftTop = leftTop
    table.rightBottom = rightBottom
    table.frictionCoefficient = friction.value
    return table

# Инициализировать C++ контроллер
libgame.create_game_controller(60)

# ==========================================
# НОВЫЙ КОД С ПРАВИЛЬНОЙ СИНХРОНИЗАЦИЕЙ
# ==========================================

# 1. Настройка Pygame
pygame.init()
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("My Physics Lab (correct catch-up)")
clock = pygame.time.Clock()

# Шрифт для показа FPS
font = pygame.font.Font(None, 36)

# Цвета (соответствуют enum Color в C++)
COLOR_MAP = [
    (255, 255, 255),  # 0: WHITE (биток)
    (255, 255, 50),   # 1: YELLOW
    (50, 50, 255),    # 2: BLUE
    (255, 50, 50),    # 3: RED
    (128, 0, 128),    # 4: PURPLE
    (255, 165, 0),    # 5: ORANGE
    (50, 255, 50),    # 6: GREEN
    (128, 0, 0),      # 7: MAROON
    (0, 0, 0),        # 8: BLACK
    (255, 255, 0)     # 9: YELLOW_STRIPED
]

# 2. Инициализация движка
class SimpleEngine:
    def __init__(self):
        self.current_step = 0
        
    def update(self, steps_count):
        self.current_step += steps_count
        
    def get_render_data(self):
        # Получаем данные из C++ через массив
        return get_cpp_ball_data()
    
    def handle_click(self, x, y):
        # Телепортируем шар
        pass  # Не используется, данные из C++

engine = SimpleEngine()

# 3. Переменные для синхронизации
running = True
last_update_time = pygame.time.get_ticks()
frame_count = 0
fps_update_time = pygame.time.get_ticks()
current_fps = 0

# 4. НАСТРОЙКА FPS
TARGET_FPS = 40
MS_PER_STEP = 1000 // TARGET_FPS

# АВТО РАСЧЕТ FPS (ЗАКОММЕНТИРОВАНО)
# TARGET_FPS = clock.get_fps()  # <- Распакуйте если нужен авто-расчет
# MS_PER_STEP = 1000 // int(TARGET_FPS) if TARGET_FPS > 0 else 16


while running:
    # === ОБРАБОТКА СОБЫТИЙ ===
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            engine.handle_click(x, y)

    # === СИНХРОНИЗАЦИЯ ===
    current_time = pygame.time.get_ticks()
    time_since_update = current_time - last_update_time
    
    if time_since_update >= MS_PER_STEP:
        steps_needed = int(time_since_update // MS_PER_STEP)
        engine.update(steps_needed)
        last_update_time += steps_needed * MS_PER_STEP

        # Обновляем состояние игры в C++
        for _ in range(steps_needed):
            libgame.update_game()
        
        frame_count += 1
        if current_time - fps_update_time >= 1000:
            current_fps = frame_count
            frame_count = 0
            fps_update_time = current_time

    # === ОТРИСОВКА ===
    screen.fill((0, 0, 0))  # BLACK

    # Получаем данные стола и отображаем его
    table_data = get_cpp_table_data()
    if table_data:
        left, top = table_data.leftTop[0], table_data.leftTop[1]
        right, bottom = table_data.rightBottom[0], table_data.rightBottom[1]
        width = right - left
        height = bottom - top

        # Draw table (green felt)
        pygame.draw.rect(screen, (0, 100, 0), (left, top, width, height))

        # Draw table borders (brown wood)
        pygame.draw.rect(screen, (139, 69, 19), (left, top, width, height), 10)

        # Display table info
        table_info = f"Table: ({left},{top}) to ({right},{bottom})"
        table_surface = font.render(table_info, True, (255, 255, 255))
        screen.blit(table_surface, (10, 210))

    # Получаем данные из C++ через массив
    objects = engine.get_render_data()

    for obj in objects:
        px, py, radius, p_type, vx, vy = obj
        color = COLOR_MAP[p_type] if 0 <= p_type < len(COLOR_MAP) else (255, 255, 255)
        pygame.draw.circle(screen, color, (int(px), int(py)), radius)

    # Показать FPS
    fps_text = font.render(f"Real FPS: {current_fps}", True, (255, 255, 255))  # WHITE
    screen.blit(fps_text, (10, 10))

    target_text = font.render(f"Target: {TARGET_FPS} FPS", True, (50, 255, 50))  # GREEN
    screen.blit(target_text, (10, 50))

    ms_text = font.render(f"Step: {MS_PER_STEP}ms", True, (255, 255, 50))  # YELLOW
    screen.blit(ms_text, (10, 90))

    status_text = f"CATCH-UP: OK - C++ data loaded"
    steps_info = f"Balls: {len(objects)}"

    status_surface = font.render(status_text, True, (50, 255, 50))  # GREEN
    screen.blit(status_surface, (10, 130))

    steps_surface = font.render(steps_info, True, (255, 255, 255))  # WHITE
    screen.blit(steps_surface, (10, 170))

    pygame.display.flip()
    clock.tick(TARGET_FPS)

pygame.quit()
sys.exit()
