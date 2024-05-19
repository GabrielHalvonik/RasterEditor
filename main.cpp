#include "Visuals/Widget.hpp"

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char* argv[]) {

    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    Widget w;
    w.setFixedSize(Widget::PageSize, Widget::PageSize);
    // w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    return a.exec();
}
