#pragma once

#include <QFile>
#include <QString>
#include <QWidget>
#include <QPainter>
#include <QApplication>
#include <QSvgRenderer>

namespace Utilities::Resources {

    static void setStyleSheet(QWidget* widget, const QString& resource) {
        QFile file(resource);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            widget->setStyleSheet(stream.readAll());
            file.close();
        }
    }

    static void setStyleSheet(QApplication& app, const QString& resource) {
        QFile file(resource);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            app.setStyleSheet(stream.readAll());
            file.close();
        }
    }

    static QIcon loadSvgIcon(const QString& path, const QSize& size) {
        QFile file(path);

        if (file.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream stream(&file);
            auto svgData = stream.readAll();
            QSvgRenderer renderer(svgData.toUtf8());
            QPixmap pixmap(size);
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            // painter.setRenderHint(QPainter::RenderHint::Antialiasing, false);
            renderer.render(&painter);
            return QIcon(pixmap);
        }

        throw std::exception();
    }

}
