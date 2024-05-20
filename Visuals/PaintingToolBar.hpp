#pragma once

#include <QToolBar>

class QButtonGroup;
class QAbstractButton;
class PaintingToolRegistry;

struct PaintingToolBar : QToolBar {

    PaintingToolBar(PaintingToolRegistry*, QWidget* parent = nullptr);
    ~PaintingToolBar();

    void constructToolButtons();

    void selectPaintingTool(QAbstractButton*);

    void updatePosition(const QSize&);

private:
    const int leftMargin = 8;

    PaintingToolRegistry* registry { };

    QButtonGroup* group { };
};
