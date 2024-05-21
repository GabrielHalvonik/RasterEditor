#pragma once

#include <QMainWindow>

class QDialog;
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

    QDialog* creditDialog { }; // lazy-loadable
};
