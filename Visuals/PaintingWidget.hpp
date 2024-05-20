#pragma once

#include <cstdint>
#include <stack>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QPainter;
class BrushAction;
class PaintingToolRegistry;

struct Delta {
    QRect region { };
    std::vector<uint8_t> data { };
    // uint8_t* data { };
};

struct Snapshot {
    QRect region { };
    uint8_t* data { };
};

struct PaintingWidget : public QOpenGLWidget, protected QOpenGLFunctions {

    PaintingWidget(PaintingToolRegistry*, const QSize& pageSize = { 512, 512 }, QWidget* parent = nullptr);
    ~PaintingWidget();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void enterEvent(QEnterEvent*) override;
    void leaveEvent(QEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;

    Snapshot getImageSnapshot(uint8_t*, int, const QRect&);
    Delta getImageCompressedDelta(uint8_t*, int, const QRect&);

private:
    void applyPaintingAction(const QPoint&);
    QPoint interpolateSpline(const std::vector<QPoint>&, float);
    void pushIntoUndoStack(const Delta&);

private:

    const int bytesPerPixel = 4;

    PaintingToolRegistry* paintingToolRegistry { };
    QWidget* paintingToolPreviewWidget { };

    QSize pageSize { };

    QImage image { pageSize.width(), pageSize.height(), QImage::Format::Format_ARGB32 };

    bool isErase { false };
    bool isDrawing { false };

    std::optional<QPoint> previousPoint { };

    QRect affectedRegion { };

    std::stack<Delta> undoStack { };
    std::stack<Delta> redoStack { };

    Snapshot bitmapSnapshot { };

};
