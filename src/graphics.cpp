#include "graphics.h"

#include <cmath>
#include <QColor>

void draw_line(int x0, int x1, int y0, int y1, QImage &image) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = 0; x <= x1; x++) {
        if (steep) {
            image.setPixel(y, x, qRgb(255, 255, 255));
        } else {
            image.setPixel(x, y, qRgb(255, 255, 255));
        }

        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}
