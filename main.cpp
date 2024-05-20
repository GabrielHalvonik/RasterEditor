#include <QApplication>
#include <QStyleFactory>

#include "Utilities/Resources.hpp"
#include "Visuals/ApplicationWindow.hpp"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    Utilities::Resources::setStyleSheet(app, ":/resources/styles/General.qss");

    ApplicationWindow window { };
    window.resize(1920 * 2/3, 1080 * 2/3);
    // window.setWindowFlags(Qt::FramelessWindowHint);
    window.show();
    return app.exec();
}
