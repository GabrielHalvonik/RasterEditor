#pragma once

#include <QRect>
#include <QSize>
#include <QPoint>
#include <algorithm>

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

    void getSubRectangle(uint8_t* destination, const uint8_t* source, int sourceWidth, int x, int y, int width, int height, int bytesPerPixel = 4) {
        for (int row = 0; row < height; ++row) {
            const uint8_t* srcRowStart = source + ((y + row) * sourceWidth + x) * bytesPerPixel;
            uint8_t* destRowStart = destination + row * width * bytesPerPixel;
            std::memcpy(destRowStart, srcRowStart, width * bytesPerPixel);
        }
    }

    void getSubRectangle(uint8_t* destination, const uint8_t* origin, int originalWidth, const QRect& rect, int bytesPerPixel = 4) {
        getSubRectangle(destination, origin, originalWidth, rect.x(), rect.y(), rect.width(), rect.height(), bytesPerPixel);
    }

    QRect clipRegionToWidgetBounds(QRect rect, const QSize& size) {
        if (rect.x() < 0) {
            rect.setX(0);
            rect.setWidth(rect.width() + rect.x());
        } else if ((rect.x() + rect.width()) > size.width()) {
            rect.setWidth(size.width() - rect.x());
        }
        if (rect.y() < 0) {
            rect.setY(0);
            rect.setHeight(rect.height() + rect.y());
        } else if ((rect.y() + rect.height()) > size.height()) {
            rect.setHeight(size.height() - rect.y());
        }

        return rect;
    }

}
