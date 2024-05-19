#pragma once

#include <QMainWindow>

struct ApplicationWindow : QMainWindow {

    ApplicationWindow();

    ~ApplicationWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;

};
