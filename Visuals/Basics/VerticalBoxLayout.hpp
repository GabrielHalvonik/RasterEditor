#pragma once

#include <QVBoxLayout>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct VerticalBoxLayout : QVBoxLayout {

        parametrize (VerticalBoxLayout) {
            parameter <const list<QWidget*>&> { affect (VerticalBoxLayout::addMultipleWidget) } items;
        };

        VerticalBoxLayout() { }

        virtual ~VerticalBoxLayout() { }

        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                this->addWidget(widget);
            }
        }

    };

}
