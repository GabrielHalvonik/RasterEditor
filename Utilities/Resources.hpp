#pragma once

#include <QFile>
#include <QString>
#include <QApplication>
#include <QWidget>

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

}
