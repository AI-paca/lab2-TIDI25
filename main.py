import sys
import os
import pygame

# Подключаем путь к библиотеке
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

try:
    import pool_game
except ImportError as e:
    print(f"Не удалось импортировать 'pool_game'.\n{e}\n")
    sys.exit(1)

# ==========================================
# КОНСТАНТЫ И НАСТРОЙКИ
# ==========================================
pygame.init()
WIDTH, HEIGHT = 1000, 800
TITLE = "Pool Game"
TARGET_FPS = 60
MS_PER_STEP = 1000 // TARGET_FPS

# Цвета
COLOR_MAP = {
    pool_game.Color.WHITE: (255, 255, 255),
    pool_game.Color.YELLOW: (255, 255, 50),
    pool_game.Color.BLUE: (50, 50, 255),
    pool_game.Color.RED: (255, 50, 50),
    pool_game.Color.PURPLE: (128, 0, 128),
    pool_game.Color.ORANGE: (255, 165, 0),
    pool_game.Color.GREEN: (50, 255, 50),
    pool_game.Color.MAROON: (128, 0, 0),
    pool_game.Color.BLACK: (20, 20, 20),
    pool_game.Color.YELLOW_STRIPED: (255, 255, 0)
}

# ==========================================
# ИНИЦИАЛИЗАЦИЯ
# ==========================================
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption(TITLE)
clock = pygame.time.Clock()
font = pygame.font.Font(None, 24)

# Дефолтные настройки внутри C++
# controller = pool_game.create_controller(TARGET_FPS)
# print("Controller: Default Init")

#Авто-центрирование по размеру окна (Ширина, Высота)
controller = pool_game.create_controller(TARGET_FPS, WIDTH, HEIGHT)
print(f"Controller: Auto-Center ({WIDTH}x{HEIGHT})")

# Ручное задание координат стола (Left, Top, Right, Bottom)
# TABLE_WIDTH = int(WIDTH * 0.5)
# TABLE_HEIGHT = int(HEIGHT * 0.7)
# TABLE_LEFT = int(WIDTH * 0.15)
# TABLE_TOP = int(HEIGHT * 0.15)
# TABLE_RIGHT = TABLE_LEFT + TABLE_WIDTH
# TABLE_BOTTOM = TABLE_TOP + TABLE_HEIGHT

# controller = pool_game.create_controller(TARGET_FPS, TABLE_LEFT, TABLE_TOP, TABLE_RIGHT, TABLE_BOTTOM)
# print(f"Controller: Manual Coordinates ({TABLE_LEFT}, {TABLE_TOP}) -> ({TABLE_RIGHT}, {TABLE_BOTTOM})")

# ==========================================
# ГЛАВНЫЙ ЦИКЛ
# ==========================================
running = True
last_update_time = pygame.time.get_ticks()

while running:
    # --- 1. ВВОД ---
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                controller.shoot_cue()
        elif event.type == pygame.MOUSEMOTION:
            x, y = event.pos
            controller.aim_cue(x, y)
        elif event.type == pygame.KEYDOWN:
             if event.key == pygame.K_ESCAPE:
                 print("GameEnd triggered")
                 controller.reset_game()

    # --- 2. ФИЗИКА (Fixed Timestep) ---
    current_time = pygame.time.get_ticks()
    time_since_update = current_time - last_update_time

    while time_since_update >= MS_PER_STEP:
        controller.update(1) 
        time_since_update -= MS_PER_STEP
        last_update_time += MS_PER_STEP

    # --- 3. РЕНДЕР ---
    screen.fill((20, 20, 20)) 

    # Данные из C++
    table = controller.get_table()
    pockets = controller.get_pockets()
    balls = controller.get_balls()
    cue = controller.get_cue()

    # Стол
    l, t = table.left_top
    r, b = table.right_bottom
    w, h = r - l, b - t
    
    pygame.draw.rect(screen, (0, 100, 0), (l, t, w, h))
    border = table.border_thickness
    pygame.draw.rect(screen, (101, 67, 33), (l - border, t - border, w + border*2, h + border*2), int(border))

    # Лузы
    for p in pockets:
        px, py = p.position
        pygame.draw.circle(screen, (0, 0, 0), (int(px), int(py)), int(p.radius))

    # Шары
    white_ball_pos = None
    for b in balls:
        if b.is_pocketed: continue
        if b.color == pool_game.Color.WHITE:
            white_ball_pos = b.position

        bx, by = b.position
        color = COLOR_MAP.get(b.color, (255, 0, 255))
        pygame.draw.circle(screen, color, (int(bx), int(by)), int(b.radius))
        # Блик
        pygame.draw.circle(screen, (255, 255, 255), (int(bx - b.radius*0.3), int(by - b.radius*0.3)), int(b.radius*0.2))

    # === ТВОЙ ЛЮБИМЫЙ СТАРЫЙ КИЙ ===
    if cue.is_active:
        tip_x, tip_y = cue.position
        dir_x, dir_y = cue.direction
        CUE_LENGTH = 200

        # Ручка кия
        handle_x = tip_x - dir_x * CUE_LENGTH
        handle_y = tip_y - dir_y * CUE_LENGTH

        # Палка
        pygame.draw.line(screen, (139, 69, 19), (tip_x, tip_y), (handle_x, handle_y), 8)
        # Наклейка
        pygame.draw.circle(screen, (200, 200, 200), (int(tip_x), int(tip_y)), 4)

# === ТВОЙ ЛЮБИМЫЙ СТАРЫЙ КИЙ (FIXED EDITION) ===
    if cue.is_active:
        tip_x, tip_y = cue.position
        dir_x, dir_y = cue.direction
        CUE_LENGTH = 200

        # 1. Ручка кия (Рисуем от носика назад)
        handle_x = tip_x - dir_x * CUE_LENGTH
        handle_y = tip_y - dir_y * CUE_LENGTH

        # Палка
        pygame.draw.line(screen, (139, 69, 19), (tip_x, tip_y), (handle_x, handle_y), 8)
        # Наклейка
        pygame.draw.circle(screen, (200, 200, 200), (int(tip_x), int(tip_y)), 4)

        # 2. Линия прицеливания (от белого шара)
        if white_ball_pos:
            wb_x, wb_y = white_ball_pos
            aim_length = cue.force * 10.0 
            if aim_length < 30: 
                aim_length = 30
            
            end_aim_x = wb_x + dir_x * aim_length
            end_aim_y = wb_y + dir_y * aim_length
            
            # Рисуем линию (белая, толщина 2 для видимости)
            pygame.draw.line(screen, (255, 255, 255), (wb_x, wb_y), (end_aim_x, end_aim_y), 2)
        # Debug info
        cue_info = f"Power: {cue.force:.1f}"
        cue_surface = font.render(cue_info, True, (255, 255, 255))
        screen.blit(cue_surface, (10, HEIGHT - 30))

    # UI
    fps = clock.get_fps()
    screen.blit(font.render(f"FPS: {int(fps)}", True, (0, 255, 0)), (10, 10))
    screen.blit(font.render(f"Balls Left: {len([b for b in balls if not b.is_pocketed])-1}", True, (200, 200, 200)), (10, 30))

    pygame.display.flip()
    clock.tick(TARGET_FPS) # <-- Это нужно, чтобы не сжечь видеокарту

pygame.quit()
