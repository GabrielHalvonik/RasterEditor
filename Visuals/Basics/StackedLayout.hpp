#pragma once

#include <QStackedLayout>

#include "../../Utilities/General.hpp"

namespace Visuals::Basics {

struct StackedLayout : QStackedLayout {

    parametrize (StackedLayout) {
        parameter <const list<QWidget*>&> { affect (StackedLayout::addMultipleWidget) } items;
    };

    StackedLayout() : StackedLayout() { }

    virtual ~StackedLayout() { }



    void addMultipleWidget(const list<QWidget*>& widgets) {
        for (QWidget* widget : widgets) {
            this->addWidget(widget);
        }
    }

};

}

