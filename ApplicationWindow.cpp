#include "ApplicationWindow.hpp"

ApplicationWindow::ApplicationWindow() {

}

ApplicationWindow::~ApplicationWindow() { }



void ApplicationWindow::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
}
