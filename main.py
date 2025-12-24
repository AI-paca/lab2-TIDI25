import pygame
import sys
import time
import ctypes



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
TARGET_FPS = 10
MS_PER_STEP = 1000 // TARGET_FPS

# АВТО РАСЧЕТ FPS (ЗАКОММЕНТИРОВАНО)
# TARGET_FPS = clock.get_fps()  # <- Распакуйте если нужен авто-расчет
# MS_PER_STEP = 1000 // int(TARGET_FPS) if TARGET_FPS > 0 else 16

# ==========================================
# ПРОСТОЙ ДОСТУП К C++ ДАННЫМ
# ==========================================

# Загрузить C++ библиотеку
libgame = ctypes.CDLL('./libgame.so')

# Установить типы возвращаемых значений для функций
libgame.get_cue.restype = ctypes.c_void_p

# Определить структуру Ball для Python (как в C++)
class Ball(ctypes.Structure):
    _fields_ = [
        ("position", ctypes.c_int * 2),  # pair<int, int>
        ("velocity", ctypes.c_float * 2),  # pair<float, float>
        ("radius", ctypes.c_int),
        ("color", ctypes.c_int),
        ("isPocketed", ctypes.c_bool)
    ]

# Определить структуру Pocket для Python (как в C++)
class Pocket(ctypes.Structure):
    _fields_ = [
        ("position", ctypes.c_float * 2),  # pair<float, float>
        ("radius", ctypes.c_float)
    ]

# Определить структуру Cue для Python (как в C++)
class Cue(ctypes.Structure):
    _fields_ = [
        ("position", ctypes.c_float * 2),  # pair<float, float>
        ("direction", ctypes.c_float * 2),  # pair<float, float>
        ("force", ctypes.c_float),
        ("isActive", ctypes.c_bool)
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
        result.append((ball.position[0], ball.position[1], ball.radius, ball.color, ball.velocity[0], ball.velocity[1], ball.isPocketed))

    # Вывести данные в консоль для проверки
    print(f"C++ вернул {count} шаров:")
    for i, obj in enumerate(result):
        print(f"  Шар {i}: x={obj[0]}, y={obj[1]}, radius={obj[2]}, color={obj[3]}, vx={obj[4]}, vy={obj[5]}, pocketed={obj[6]}")

    return result

# Функция для получения данных луз из C++
def get_cpp_pocket_data():
    # Создать массив для 6 луз
    pockets_array = (Pocket * 6)()

    # Вызвать C++ функцию для получения данных
    count = libgame.get_pockets_array(pockets_array, 6)

    # Преобразовать в список кортежей для Python
    result = []
    for i in range(count):
        pocket = pockets_array[i]
        result.append((pocket.position[0], pocket.position[1], pocket.radius))

    # Вывести данные в консоль для проверки
    print(f"C++ вернул {count} луз:")
    for i, obj in enumerate(result):
        print(f"  Луза {i}: x={obj[0]}, y={obj[1]}, radius={obj[2]}")

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

# Функция для получения данных кия из C++
def get_cpp_cue_data():
    cue_ptr = libgame.get_cue()
    if cue_ptr and cue_ptr != 0:
        try:
            return ctypes.cast(cue_ptr, ctypes.POINTER(Cue)).contents
        except:
            return None
    return None

# Инициализировать C++ контроллер
libgame.create_game_controller(TARGET_FPS)

# ==========================================
# ГЛАВНЫЙ ЦИКЛ ИГРЫ
# ==========================================

while running:
    # === ОБРАБОТКА СОБЫТИЙ ===
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            engine.handle_click(x, y)
            libgame.shoot_cue()
            print(f"Клик мыши: x={x}, y={y} | Удар кием")
        elif event.type == pygame.MOUSEMOTION:
            mouse_x, mouse_y = event.pos
            libgame.aim_cue(mouse_x, mouse_y)
            print(f"Движение мыши: x={mouse_x}, y={mouse_y} | Прицеливание кия")

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

    # Получаем данные луз и отображаем их
    pockets = get_cpp_pocket_data()
    for pocket in pockets:
        px, py, radius = pocket
        pygame.draw.circle(screen, (128, 128, 128), (int(px), int(py)), int(radius), 2)  # Серые круги

    for obj in objects:
        px, py, radius, p_type, vx, vy, is_pocketed = obj
        if not is_pocketed:  # Не отображаем забитые шары
            color = COLOR_MAP[p_type] if 0 <= p_type < len(COLOR_MAP) else (255, 255, 255)
            pygame.draw.circle(screen, color, (int(px), int(py)), radius)

    # === ОТРИСОВКА КИЯ ===
    cue_data = get_cpp_cue_data()

    if cue_data and cue_data.isActive:
        # C++ вернул нам позицию НОСИКА кия (ближний к шару конец)
        tip_x = cue_data.position[0]
        tip_y = cue_data.position[1]

        # Направление удара (вектор единичной длины)
        dir_x = cue_data.direction[0]
        dir_y = cue_data.direction[1]

        # Длина кия (визуальная константа, в C++ ее больше нет)
        CUE_LENGTH = 150

        # Ручка кия находится еще дальше от шара, чем носик
        # Мы идем от носика НАЗАД по вектору направления
        handle_x = tip_x - dir_x * CUE_LENGTH
        handle_y = tip_y - dir_y * CUE_LENGTH

        # Рисуем палку (коричневая)
        pygame.draw.line(screen, (139, 69, 19), (tip_x, tip_y), (handle_x, handle_y), 8)

        # Рисуем наклейку на носу (белая точка)
        pygame.draw.circle(screen, (200, 200, 200), (int(tip_x), int(tip_y)), 4)

        # (Опционально) Линия прицеливания, куда полетит шар
        # Рисуем от шара в сторону удара
        # Для этого нужно найти белый шар в objects
        for obj in objects:
            if obj[3] == 0: # White ball
                wb_x, wb_y = obj[0], obj[1]
                # Линия полета (полупрозрачная или тонкая)
                end_aim_x = wb_x + dir_x * 100
                end_aim_y = wb_y + dir_y * 100
                pygame.draw.line(screen, (255, 255, 255), (wb_x, wb_y), (end_aim_x, end_aim_y), 1)
                break

        # Display cue info
        cue_info = f"Cue: tip=({tip_x:.1f},{tip_y:.1f}), force={cue_data.force:.1f}"
        cue_surface = font.render(cue_info, True, (255, 255, 255))
        screen.blit(cue_surface, (10, 250))

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
