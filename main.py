import sys
import os
import pygame
import time

# путь к скомпилированной библиотеке (если она в корне или build/)
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

try:
    import pool_game
except ImportError:
    print("Ошибка: Не удалось импортировать модуль 'pool_game'. Сначала скомпилируйте проект!")
    sys.exit(1)

# ==========================================
# НАСТРОЙКИ
# ==========================================
pygame.init()
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Pool Game (DIP Architecture)")
clock = pygame.time.Clock()
font = pygame.font.Font(None, 36)

# Настройка стола
TABLE_WIDTH = int(WIDTH * 0.7)
TABLE_HEIGHT = int(HEIGHT * 0.7)
TABLE_LEFT = int(WIDTH * 0.15)
TABLE_TOP = int(HEIGHT * 0.15)
TABLE_RIGHT = TABLE_LEFT + TABLE_WIDTH
TABLE_BOTTOM = TABLE_TOP + TABLE_HEIGHT

# Маппинг цветов из C++ Enum в Pygame Colors
COLOR_MAP = {
    pool_game.Color.WHITE: (255, 255, 255),
    pool_game.Color.YELLOW: (255, 255, 50),
    pool_game.Color.BLUE: (50, 50, 255),
    pool_game.Color.RED: (255, 50, 50),
    pool_game.Color.PURPLE: (128, 0, 128),
    pool_game.Color.ORANGE: (255, 165, 0),
    pool_game.Color.GREEN: (50, 255, 50),
    pool_game.Color.MAROON: (128, 0, 0),
    pool_game.Color.BLACK: (0, 0, 0),
    pool_game.Color.YELLOW_STRIPED: (255, 255, 0)
}

# ==========================================
# ИНИЦИАЛИЗАЦИЯ
# ==========================================
TARGET_FPS = 60
MS_PER_STEP = 1000 // TARGET_FPS

# === ДЕМОНСТРАЦИЯ ВСЕХ ТРЕХ ФАБРИЧНЫХ ФУНКЦИЙ ===

# # Вариант 1: Дефолтный размер стола (800x600)
# print("\n1. Создание контроллера с дефолтными размерами стола...")
# controller1 = pool_game.create_controller(TARGET_FPS)
# print(f"   Стол: {controller1.get_table().left_top} - {controller1.get_table().right_bottom}")

# # Вариант 2: Автоцентрирование стола
# print("\n2. Создание контроллера с автоцентрированием стола...")
controller = pool_game.create_controller(TARGET_FPS, WIDTH, HEIGHT)
# table = controller2.get_table()
# print(f"   Размер окна: {WIDTH}x{HEIGHT}")
# print(f"   Стол: {table.left_top} - {table.right_bottom}")

# # Вариант 3: Явные координаты (тот, который используется в игре)
# print("\n3. Создание контроллера с явными координатами...")
# controller3 = pool_game.create_controller(TARGET_FPS, TABLE_LEFT, TABLE_TOP, TABLE_RIGHT, TABLE_BOTTOM)
# print(f"   Стол: {controller.get_table().left_top} - {controller.get_table().right_bottom}")

running = True
last_update_time = pygame.time.get_ticks()

while running:
    # 1. СОБЫТИЯ
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            controller.shoot_cue() 
            print("Удар!")
        elif event.type == pygame.MOUSEMOTION:
            x, y = event.pos
            controller.aim_cue(x, y) 

    # 2. cинхронизация
    current_time = pygame.time.get_ticks()
    time_since_update = current_time - last_update_time

    if time_since_update >= MS_PER_STEP:
        steps_needed = int(time_since_update // MS_PER_STEP)
        controller.update(steps_needed)
        last_update_time += steps_needed * MS_PER_STEP

    # 3. ОТРИСОВКА
    screen.fill((0, 0, 0))

    # --- СТОЛ ---
    table = controller.get_table() 
    l, t = table.left_top
    r, b = table.right_bottom
    w = r - l
    h = b - t

    # Сукно
    pygame.draw.rect(screen, (0, 100, 0), (l, t, w, h))
    # Бортики
    border = table.border_thickness
    pygame.draw.rect(screen, (139, 69, 19), (l - border, t - border, w + border*2, h + border*2), border)

    # --- ЛУЗЫ ---
    pockets = controller.get_pockets() # Возвращает список объектов Pocket
    for p in pockets:
        px, py = p.position # tuple unfolding
        rad = int(p.radius)
        pygame.draw.circle(screen, (0, 0, 0), (int(px), int(py)), rad)

    # --- ШАРЫ ---
    balls = controller.get_balls() # Возвращает список объектов Ball
    for b in balls:
        if b.is_pocketed: continue

        bx, by = b.position # tuple unfolding
        color = COLOR_MAP.get(b.color, (255, 255, 255))
        pygame.draw.circle(screen, color, (int(bx), int(by)), b.radius)

    # --- КИЙ ---
    cue = controller.get_cue()
    if cue.is_active:
        tip_x, tip_y = cue.position
        dir_x, dir_y = cue.direction
        CUE_LEN = 150

        handle_x = tip_x - dir_x * CUE_LEN
        handle_y = tip_y - dir_y * CUE_LEN

        pygame.draw.line(screen, (139, 69, 19), (tip_x, tip_y), (handle_x, handle_y), 8)

        # Линия прицеливания (ищем белый шар в списке)
        white_ball = next((b for b in balls if b.color == pool_game.Color.WHITE), None)
        if white_ball:
            wb_x, wb_y = white_ball.position
            pygame.draw.line(screen, (255, 255, 255), (wb_x, wb_y), (wb_x + dir_x*100, wb_y + dir_y*100), 1)

    # UI
    fps = int(clock.get_fps())
    screen.blit(font.render(f"DIP: FPS: {fps}", True, (255, 255, 255)), (10, 10))
    screen.blit(font.render("IGameController only!", True, (0, 255, 0)), (10, 50))

    pygame.display.flip()
    clock.tick(TARGET_FPS)

pygame.quit()
