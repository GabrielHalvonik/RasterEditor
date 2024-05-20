#include "PaintingActionBase.hpp"

#include <QWidget>

PaintingActionBase::~PaintingActionBase() {
    if (previewWidget != nullptr) {
        previewWidget->setParent(nullptr);
        delete previewWidget;
        previewWidget = nullptr;
    }
}
