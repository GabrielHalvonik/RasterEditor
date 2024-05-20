#include "CentralWidget.hpp"

#include <QResizeEvent>

CentralWidget::CentralWidget(QWidget* parent) : QWidget(parent) {

}

void CentralWidget::resizeEvent(QResizeEvent* event) {
    emit sizeChanged(event->size());
}
