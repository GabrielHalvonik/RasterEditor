#pragma once

#include <QMainWindow>

class CentralWidget;
class PaintingWidget;
class PaintingToolBar;
class PaintingToolRegistry;

struct ApplicationWindow : QMainWindow {

    ApplicationWindow();

    ~ApplicationWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;

    void displayCreditDialog(bool);

private:
    PaintingToolRegistry* paintingToolRegistry { };

    CentralWidget* centralWidget { };
    PaintingWidget* paintingWidget { };
    PaintingToolBar* paintingToolBar { };
};
