#pragma once

#include <QWidget>

class QPixmap;

struct PaintingToolPreviewWidget : QWidget {

    PaintingToolPreviewWidget(const QSize& size, QWidget* parent = nullptr) : QWidget(parent) {
        QWidget::setFixedSize(size);
        QWidget::setMouseTracking(true);
    }

};



struct BrushPreviewWidget : PaintingToolPreviewWidget {

    BrushPreviewWidget(QPixmap* brush, QWidget* parent = nullptr) : PaintingToolPreviewWidget(brush->size(), parent), brushPixmap(brush) {

    }

protected:

    void paintEvent(QPaintEvent* event) override {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.setOpacity(0.4);
        painter.drawPixmap(0, 0, brushPixmap->width(), brushPixmap->height(), *brushPixmap);
    }

private:
    QPixmap* brushPixmap { };
};



struct EraserPreviewWidget : PaintingToolPreviewWidget {

    EraserPreviewWidget(const QSize& size, QWidget* parent = nullptr) : PaintingToolPreviewWidget(size, parent) {

    }

protected:

    void paintEvent(QPaintEvent* event) override {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.setOpacity(0.4);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::GlobalColor::darkGray);
        painter.setBrush(Qt::GlobalColor::white);
        painter.drawEllipse(1, 1, width() - 2, height() - 2);
    }

};
