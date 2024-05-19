#pragma once

#include <QBoxLayout>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

    struct BoxLayout : QBoxLayout {

        parametrize (BoxLayout) {
            parameter <const list<QWidget*>&> { affect(BoxLayout::addMultipleWidget) } items;
        };

        BoxLayout() : QBoxLayout(Direction::TopToBottom) { }

        virtual ~BoxLayout() { }



        void addMultipleWidget(const list<QWidget*>& widgets) {
            for (QWidget* widget : widgets) {
                this->addWidget(widget);
            }
        }

    };

}


