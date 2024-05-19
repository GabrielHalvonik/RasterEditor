#pragma once

#include <QFormLayout>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct FormLayout : QFormLayout {

        parametrize (FormLayout) {
            parameter <const list<tuple<QString, QWidget*>>&> { affect (FormLayout::addMultipleRows) } rows;
        };

        FormLayout() : QFormLayout() { }

        virtual ~FormLayout() { }



        void addMultipleRows(const list<tuple<QString, QWidget*>>& rows) {
            for (auto& [name, widget] : rows) {
                QFormLayout::addRow(name, widget);
            }
        }

    };

}

