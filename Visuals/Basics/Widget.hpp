#pragma once

#include <QWidget>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct Widget : QWidget {

        parametrize (Widget) {
            parameter <QLayout*> { affect (QWidget::setLayout) } layout;
            parameter <const list<QWidget*>&> { affect(Widget::addMultipleWidget) } items;
        };

        Widget() { }

        virtual ~Widget() { }



        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                widget->setParent(this);
            }
        }

    };

}
