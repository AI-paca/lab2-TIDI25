import pygame
import sys

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

    # Метод update(), риссует новый кадр
    def update(self):
        self.ball_x += self.velocity_x
        
        # Простая проверка границ (типа физика отскока)
        if self.ball_x > 800 or self.ball_x < 0:
            self.velocity_x *= -1

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

# 1. Настройка Pygame
pygame.init()
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("My Physics Lab")
clock = pygame.time.Clock() # Чтобы не было 10000 FPS

# Цвета (Питон знает про цвета, C++ - нет)
BLACK = (0, 0, 0)
RED = (255, 50, 50)
BLUE = (50, 50, 255)

# 2. Инициализация движка
# Когда напишешь C++, поменяешь эту строку на engine = cpp_engine.Engine()
engine = FakeCppEngine()

running = True

# 3. бесконечный цикл
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        
        elif event.type == pygame.MOUSEBUTTONDOWN:
            # обращаемся к движку
            x, y = event.pos
            engine.handle_click(x, y)

    # просим C++ посчитать новый кадр
    engine.update()

    # RENDER
    # 1. Очистить экран
    screen.fill(BLACK)

    # 2. Забрать данные у C++
    objects = engine.get_render_data()

    # 3. Нарисовать каждый объект
    for obj in objects:
        # Разбираем данные (x, y, radius, type)
        px, py, radius, p_type = obj
        
        color = RED if p_type == 1 else BLUE
        
        # Рисуем кружок. Обрати внимание: int() важен, пиксели не могут быть дробными
        pygame.draw.circle(screen, color, (int(px), int(py)), radius)

    # 4. Показать кадр
    pygame.display.flip()

    # Держим 60 кадров в секунду
    clock.tick(60)

pygame.quit()
sys.exit()