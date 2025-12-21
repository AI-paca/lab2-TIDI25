import pygame
import sys
import time

# ==========================================
# ЗАГЛУШКА
# import my_cpp_engine as cpp_engine
# ==========================================

class FakeCppEngine:
    def __init__(self):
        # Типа данные внутри C++
        self.ball_x = 400.0
        self.ball_y = 300.0
        self.velocity_x = 2.0
        self.ball_radius = 20
        self.color_type = 1 # 1 - красный, 2 - синий
        self.current_step = 0

    # Метод update(), делает указанное количество шагов за раз (БЕЗ FOR!)
    def update(self, steps_count):
        # Фиксированный шаг = предсказуемая физика
        fixed_step = 1.0 / 60.0  # Шаг = 1/60 секунды игры
        
        # НИКАКОГО FOR! Просто умножаем на количество шагов
        self.ball_x += self.velocity_x * fixed_step * steps_count * 100  # ← * steps_count вместо loop!
        
        # Простая проверка границ (типа физика отскока)
        if self.ball_x > 800 or self.ball_x < 0:
            self.velocity_x *= -1
        
        self.current_step += steps_count

    # Метод, чтобы отдать координаты Питону
    def get_render_data(self):
        # Возвращаем список кортежей: (x, y, radius, type)
        return [(self.ball_x, self.ball_y, self.ball_radius, self.color_type)]

    # Обработка клика (симуляция метода C++)
    def handle_click(self, x, y):
        # Допустим, клик меняет цвет и телепортирует мяч
        self.ball_x = x
        self.ball_y = y
        self.color_type = 2 if self.color_type == 1 else 1

# ==========================================
# КОНЕЦ ЗАГЛУШКИ
# ==========================================

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

# Цвета
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 50, 50)
BLUE = (50, 50, 255)
GREEN = (50, 255, 50)
YELLOW = (255, 255, 50)

# 2. Инициализация движка
engine = FakeCppEngine()

# 3. Переменные для синхронизации
running = True
last_update_time = pygame.time.get_ticks()
frame_count = 0
fps_update_time = pygame.time.get_ticks()
current_fps = 0

# 4. НАСТРОЙКА FPS (можно менять здесь)
TARGET_FPS = 40  # ← ИЗМЕНИТЕ ЭТО ЗНАЧЕНИЕ ДЛЯ ДРУГОГО FPS
MS_PER_STEP = 1000 // TARGET_FPS  # 16ms для 60 FPS, 33ms для 30 FPS и т.д.

# АВТО РАСЧЕТ FPS (ЗАКОММЕНТИРОВАНО)
# TARGET_FPS = clock.get_fps()  # <- Распакуйте если нужен авто-расчет
# MS_PER_STEP = 1000 // int(TARGET_FPS) if TARGET_FPS > 0 else 16

# 4. Главный игровой цикл с ПРАВИЛЬНОЙ catch-up синхронизацией
while running:
    # === ОБРАБОТКА СОБЫТИЙ ===
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            engine.handle_click(x, y)

    # === ПРАВИЛЬНАЯ CATCH-UP СИНХРОНИЗАЦИЯ ===
    current_time = pygame.time.get_ticks()
    time_since_update = current_time - last_update_time
    
    if time_since_update >= MS_PER_STEP:
        # СКОЛЬКО ШАГОВ ПРОПУСТИЛИ?
        steps_needed = int(time_since_update // MS_PER_STEP)
        
        # ПРАВИЛЬНО: ОДИН вызов с параметром!
        engine.update(steps_needed)  # ← БЕЗ FOR!
        
        last_update_time += steps_needed * MS_PER_STEP
        
        # Считаем FPS для информации
        frame_count += 1
        if current_time - fps_update_time >= 1000:  # Каждую секунду
            current_fps = frame_count
            frame_count = 0
            fps_update_time = current_time
            print(f"Real FPS: {current_fps} | Steps: {engine.current_step}")

    # === ОТРИСОВКА ===
    # 1. Очистить экран
    screen.fill(BLACK)

    # 2. Забрать данные у C++
    objects = engine.get_render_data()

    # 3. Нарисовать каждый объект
    for obj in objects:
        px, py, radius, p_type = obj
        color = RED if p_type == 1 else BLUE
        pygame.draw.circle(screen, color, (int(px), int(py)), radius)

    # 4. Показать настройки FPS
    fps_text = font.render(f"Real FPS: {current_fps}", True, WHITE)
    screen.blit(fps_text, (10, 10))
    
    target_text = font.render(f"Target: {TARGET_FPS} FPS", True, GREEN)
    screen.blit(target_text, (10, 50))
    
    ms_text = font.render(f"Step: {MS_PER_STEP}ms", True, YELLOW)
    screen.blit(ms_text, (10, 90))

    # 5. Показать статус синхронизации
    if time_since_update < MS_PER_STEP:
        status_color = GREEN
        status_text = f"CATCH-UP: OK"
        steps_info = f"Total steps: {engine.current_step}"
    else:
        status_color = RED
        status_text = f"CATCH-UP: LAG ({time_since_update}ms)"
        steps_info = f"Steps needed: {int(time_since_update // MS_PER_STEP)}"
    
    status_surface = font.render(status_text, True, status_color)
    screen.blit(status_surface, (10, 130))
    
    steps_surface = font.render(steps_info, True, WHITE)
    screen.blit(steps_surface, (10, 170))

    # 6. Показать кадр
    pygame.display.flip()

    # === ОГРАНИЧЕНИЕ FPS ===
    clock.tick(TARGET_FPS)

pygame.quit()
sys.exit()

# ==========================================
# ПОЯСНЕНИЕ ПРАВИЛЬНОЙ CATCH-UP ЛОГИКИ:
# 
# 1. TARGET_FPS = 60  # ← Меняйте это значение!
# 2. MS_PER_STEP = 1000 // 60 = 16ms
# 3. Если прошло 50ms:
#    - steps_needed = 50 // 16 = 3 шага
#    - ОДИН вызов: engine.update(3)
#    - C++ делает: position += velocity * delta * 3 (БЕЗ FOR!)
# 4. Если прошло 10ms:
#    - steps_needed = 10 // 16 = 0 шагов
#    - Ждем дальше
# 5. Результат: НЕТ overhead от множественных вызовов, физика стабильная
# ==========================================
