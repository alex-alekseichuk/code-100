import math
import pygame

pygame.init()

# ============================================================
# Settings
# ============================================================

U = 50.0  # metafont unit scale

WIDTH = 900
HEIGHT = 700

BG = (245, 240, 225)
INK = (20, 20, 20)
RED = (180, 60, 60)
GREEN = (60, 180, 60)
BLUE = (60, 60, 180)

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("METAFONT -> pygame")

clock = pygame.time.Clock()

# ============================================================
# Coordinate conversion
# METAFONT uses upward Y
# pygame uses downward Y
# ============================================================

OFFSET_X = 120
BASELINE_Y = 620


def pt(x, y):
    return (
        int(OFFSET_X + x),
        int(BASELINE_Y - y),
    )


# ============================================================
# Cubic Bezier helper
# ============================================================

def bezier(p0, p1, p2, p3, steps=40):
    pts = []

    for i in range(steps + 1):
        t = i / steps

        x = (
            (1 - t) ** 3 * p0[0]
            + 3 * (1 - t) ** 2 * t * p1[0]
            + 3 * (1 - t) * t * t * p2[0]
            + t ** 3 * p3[0]
        )

        y = (
            (1 - t) ** 3 * p0[1]
            + 3 * (1 - t) ** 2 * t * p1[1]
            + 3 * (1 - t) * t * t * p2[1]
            + t ** 3 * p3[1]
        )

        pts.append((x, y))

    return pts


def control(p0, d0, p1, d1, scale=0.35):
    c1 = (
        p0[0] + d0[0] * scale,
        p0[1] + d0[1] * scale,
    )

    c2 = (
        p1[0] - d1[0] * scale,
        p1[1] - d1[1] * scale,
    )

    return c1, c2


def draw_metafont_curve(p_start, dir_start, p_end, dir_end, color=INK, scale=0.35):
    c1, c2 = control(p_start, dir_start, p_end, dir_end, scale)

    curve = bezier(p_start, c1, c2, p_end)

    pygame.draw.lines(
        screen,
        color,
        False,
        [pt(x, y) for x, y in curve],
        4,
    )


def draw_metafont_curve_with_mid_point(p_start, dir_start, mid, p_end, dir_end, color=INK, scale1=0.35, scale2=0.35):
    # approximate tangent at mid using geometry
    d_mid = (
        (p_end[0] - p_start[0]),
        (p_end[1] - p_start[1]),
    )

    # split directions (heuristic smoothing)
    dir_mid_in = d_mid
    dir_mid_out = d_mid

    # segment 1: start → mid
    c1a, c2a = control(p_start, dir_start, mid, dir_mid_in, scale1)
    curve1 = bezier(p_start, c1a, c2a, mid)

    # segment 2: mid → end
    c1b, c2b = control(mid, dir_mid_out, p_end, dir_end, scale2)
    curve2 = bezier(mid, c1b, c2b, p_end)

    curve = curve1 + curve2

    pygame.draw.lines(
        screen,
        color,
        False,
        [pt(x, y) for x, y in curve],
        4,
    )


# ============================================================
# METAFONT points
# ============================================================

x1 = 4.5 * U
y1 = 9.0 * U

x2 = 6.0 * U
y2 = 5.5 * U + math.sqrt((3.5 * U) ** 2 - (x2 - 4.5 * U) ** 2)

x3 = 6.5 * U
y3 = 8.5 * U

x4 = 6.0 * U
y4 = 7.0 * U

x5 = (6.0 + 16.0 / 17) * U
y5 = (8.0 + 13.0 / 17) * U

x6 = 4.0 * U
y6 = 9.0 * U

x7 = 3.0 * U
y7 = 7.0 * U - math.sqrt((2.0 * U) ** 2 - (x7 - 4.0 * U) ** 2)

x8 = 5.0 * U
y8 = 4.0 * U

x9 = 3.5 * U
y9 = 6.0 * U

x15 = 4.0 * U
y15 = 7.0 * U + math.sqrt(
    (x9 - 4.5 * U) ** 2 +
    (y9 - 7.125 * U) ** 2
)

x10 = 6.0 * U
y10 = 4.5 * U

x11 = 3.0 * U
y11 = 0.5 * U

x16 = 2.5 * U
y16 = y11 - math.sqrt(U * U - (x11 - x16) ** 2)

x12 = 1.875 * U
y12 = y16 + math.sqrt(U * U - (x12 - x16) ** 2)

x13 = 4.5 * U

x17 = 4.0 * U
y17 = y8 - math.sqrt((2.0 * U) ** 2 - (x8 - x17) ** 2)

y13 = y17 - math.sqrt((2.0 * U) ** 2 - (x13 - x17) ** 2)

x18 = 4.5 * U
y18 = y13 + math.sqrt((2.0 * U) ** 2 - (x18 - x13) ** 2)

y14 = 2.0 * U
x14 = x18 - math.sqrt((2.0 * U) ** 2 - (y18 - y14) ** 2)

# ============================================================
# Main loop
# ============================================================

running = True

while running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill(BG)

    # --------------------------------------------------------
    # Curves from METAFONT
    # --------------------------------------------------------

    draw_metafont_curve(
        (x1, y1),
        (y1 - 5.5 * U, 4.5 * U - x1),
        (x2, y2),
        (y2 - 5.5 * U, 4.5 * U - x2),
        scale = 0.2
    )

    draw_metafont_curve(
        (x3, y3),
        (9 * U - y3, x3 - 6.5 * U),
        (x5, y5),
        (9 * U - y5, x5 - 6.5 * U),
    )

    draw_metafont_curve(
        (x2, y2),
        (y2 - 5.5 * U, 4.5 * U - x2),
        (x3, y3),
        (9.0 * U - y3, x3 - 6.5 * U),
        scale = 0.1
    )

    pygame.draw.line(
        screen,
        INK,
        pt(x4, y4),
        pt(x5, y5),
        4,
    )

    draw_metafont_curve(
        (x6, y6),
        (7 * U - y6, x6 - 4 * U),
        (x7, y7),
        (7 * U - y7, x7 - 4 * U),
        scale = 0.9
    )

    pygame.draw.line(
        screen,
        INK,
        pt(x1, y1),
        pt(x6, y6),
        4,
    )

    pygame.draw.line(
        screen,
        INK,
        pt(x7, y7),
        pt(x8, y8),
        4,
    )

    draw_metafont_curve_with_mid_point(
        (x4, y4),
        (7.125 * U - y4, x4 - 4.5 * U),
        (x15, y15),
        (x9, y9),
        (7.125 * U - y9, x9 - 4.5 * U),
        # color = RED,
        scale1 = 0.5,
        scale2 = 0.5,
    )

    pygame.draw.line(
        screen,
        INK,
        pt(x9, y9),
        pt(x10, y10),
        4,
    )

    draw_metafont_curve(
        (x10, y10),
        (y10 - 2.5 * U, 4.5 * U - x10),
        (x11, y11),
        (y11 - 2.5 * U, 4.5 * U - x11),
        scale = 1.45
    )

    draw_metafont_curve(
        (x11, y11),
        (y16 - y11, x11 - x16),
        (x12, y12),
        (y16 - y12, x12 - x16),
    )

    draw_metafont_curve(
        (x8, y8),
        (y8 - y17, x17 - x8),
        (x13, y13),
        (y13 - y17, x17 - x13),
        scale = 0.98,
    )

    draw_metafont_curve(
        (x13, y13),
        (y13 - y18, x18 - x13),
        (x14, y14),
        (y14 - y18, x18 - x14),
        scale = 0.5,
    )

    pygame.draw.line(
        screen,
        INK,
        pt(x14, y14),
        pt(x12, y12),
        4,
    )

    # --------------------------------------------------------
    # Draw control points
    # --------------------------------------------------------

    # points = [
    #     (x1, y1), (x2, y2), (x3, y3), (x4, y4),
    #     (x5, y5), (x6, y6), (x7, y7), (x8, y8),
    #     (x9, y9), (x10, y10), (x11, y11),
    #     (x12, y12), (x13, y13), (x14, y14),
    #     (x15, y15), (x16, y16), (x17, y17),
    #     (x18, y18),
    # ]

    # for i, (x, y) in enumerate(points, start=1):
    #     pygame.draw.circle(screen, RED, pt(x, y), 4)

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
