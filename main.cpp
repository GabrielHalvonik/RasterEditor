#include "Widget.hpp"

#include <QApplication>

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(Widget::PageSize, Widget::PageSize);
    // w.setWindowFlags(Qt::FramelessWindowHint);

    w.show();
    return a.exec();
}
