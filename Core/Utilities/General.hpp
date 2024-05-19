#pragma once

#include <memory>

namespace Utilities::General {

    float distance(const QPoint& p1, const QPoint& p2) {
        float dx = p2.x() - p1.x();
        float dy = p2.y() - p1.y();
        return std::sqrt(dx * dx + dy * dy);
    }

    QPoint interpolate(QPoint source, QPoint destination, float percentage) {
        float x = (1 - percentage) * source.x() + percentage * destination.x();
        float y = (1 - percentage) * source.y() + percentage * destination.y();
        return QPoint(static_cast<int>(x), static_cast<int>(y));
    }

    void getSubRectangle(u_int8_t* destination, const u_int8_t* source, int sourceWidth, int x, int y, int width, int height, int bytesPerPixel = 4) {
        for (int row = 0; row < height; ++row) {
            const u_int8_t* srcRowStart = source + ((y + row) * sourceWidth + x) * bytesPerPixel;
            u_int8_t* destRowStart = destination + row * width * bytesPerPixel;
            std::memcpy(destRowStart, srcRowStart, width * bytesPerPixel);
        }
    }

    void getSubRectangle(u_int8_t* destination, const u_int8_t* origin, int originalWidth, const QRect& rect, int bytesPerPixel = 4) {
        getSubRectangle(destination, origin, originalWidth, rect.x(), rect.y(), rect.width(), rect.height(), bytesPerPixel);
    }

}
