#pragma once

#include <stack>
#include <QWidget>
#include <QPixmap>
#include <QImage>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class QPainter;

struct Delta {
    QRect region { };
    std::vector<uchar> data { };
    // uchar* data { };
};

struct Snapshot {
    QRect region { };
    uchar* data { };
};

struct Widget : public QOpenGLWidget, protected QOpenGLFunctions {

    Widget(QWidget* parent = nullptr);
    ~Widget();

    static const int PageSize = 512;

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;

    void initializeGL() override;
    void resizeGL(int, int) override;
    void paintGL() override;

    Snapshot getImageSnapshot(uchar*, int, const QRect&);
    Delta getImageCompressedDelta(uchar*, int, const QRect&);

    void getSubRectangle(uchar*, const uchar*, int, int, int, int, int);
    void getSubRectangle(uchar*, const uchar*, int, const QRect&);

private:
    void drawBrushStroke(const QPoint&);
    QPoint interpolateSpline(const std::vector<QPoint>&, float);
    void pushIntoUndoStack(const Delta&);

private:
    const int bytesPerPixel = 4;

    QImage image { PageSize, PageSize, QImage::Format::Format_ARGB32 };
    QPixmap brush { 50, 50 };
    QPixmap erase { 50, 50 };

    bool isErase { false };
    bool isDrawing { false };

    std::optional<QPoint> previousPoint { };

    QRect affectedRegion { };

    std::stack<Delta> undoStack { };
    std::stack<Delta> redoStack { };

    Snapshot bitmapSnapshot { };
};






