#include "PaintingToolBar.hpp"

#include <QButtonGroup>
#include <QToolButton>

#include "Utilities/PaintingToolIconFactory.hpp"
#include "../Core/Painting/PaintingToolRegisty.hpp"

struct PaintingToolButton : QToolButton {

    PaintingToolButton(PaintingToolType type, QWidget* parent = nullptr) : QToolButton(parent), type(type) {
        QToolButton::setFixedSize(buttonSize, buttonSize);
        QToolButton::setCheckable(true);
        QIcon icon = PaintingToolIconFactory::construct(type);
        QToolButton::setIcon(icon);
    }

    inline PaintingToolType getPaintingToolType() const { return type; }

private:
    PaintingToolType type { };
    const int buttonSize = 32;
};

PaintingToolBar::PaintingToolBar(PaintingToolRegistry* registry, QWidget* parent) :
    QToolBar(parent),
    registry(registry),
    group(new QButtonGroup(this))
{
    QObject::setObjectName("PaintingToolBar");

    QToolBar::setContentsMargins(0, 0, 0, 0);
    QToolBar::setMovable(false);
    QToolBar::setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QToolBar::setOrientation(Qt::Orientation::Vertical);

    group->setExclusive(true);
    QObject::connect(group, &QButtonGroup::buttonClicked, this, &PaintingToolBar::selectPaintingTool);

    PaintingToolBar::constructToolButtons();

    QToolBar::adjustSize();
}

PaintingToolBar::~PaintingToolBar() {

}

void PaintingToolBar::constructToolButtons() {

    auto types = registry->getRegisteredPaintingToolTypes();
    for (int i = 0; i < types.size(); ++i) {
        auto button = new PaintingToolButton(types[i]);
        group->addButton(button);

        if (i == 0) {
            button->setProperty("position", "first");
        } else if (i == (types.size() - 1)) {
            button->setProperty("position", "last");
        }

        QToolBar::addWidget(button);
    }


    if (group->buttons().size() > 0) {
        QAbstractButton* first = group->buttons().at(0);
        first->setChecked(true);
        PaintingToolBar::selectPaintingTool(first);
    }
}

void PaintingToolBar::selectPaintingTool(QAbstractButton* button) {
    if (auto casted = dynamic_cast<PaintingToolButton*>(button); casted != nullptr) {
        registry->setCurrentToolType(casted->getPaintingToolType());
    }
}

void PaintingToolBar::updatePosition(const QSize& parentSize) {
    QToolBar::move(leftMargin, parentSize.height() / 2 - QToolBar::height() / 2);
    QToolBar::adjustSize();
}

