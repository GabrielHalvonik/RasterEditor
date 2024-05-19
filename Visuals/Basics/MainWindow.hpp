#pragma once

#include <QMainWindow>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct MainWindow : QMainWindow {

        parametrize (MainWindow) {
            parameter <QWidget*> { affect (QMainWindow::setCentralWidget) } centralWidget;
        };

        MainWindow() { }

        virtual ~MainWindow() { }

    };

}
