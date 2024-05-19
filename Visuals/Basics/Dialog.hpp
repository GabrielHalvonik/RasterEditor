#pragma once

#include <QDialog>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct Dialog : QDialog {

        parametrize (Dialog) {
            parameter <QWidget*> { affect(QDialog::setParent) } parent;
            parameter <QLayout*> { affect(QDialog::setLayout) } layout;
            parameter <const QSize&> { affect(QDialog::setFixedSize) } size;
        };

        Dialog() : QDialog() {

        }

        virtual ~Dialog() { }
    };

}
