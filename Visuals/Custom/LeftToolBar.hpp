#pragma once

#include <QFrame>
#include <QActionGroup>
#include <QToolBar>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QGraphicsItem>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QIconEngine>
#include <QColorDialog>

#include "../Basics/ToolButton.hpp"

#include "../../Utilities/General.hpp"

#include "ToolBarSeparator.hpp"

using namespace Visuals::Basics;

namespace Visuals::Custom {

    struct LeftToolBar : QScrollArea {

        parametrize (LeftToolBar) {
            parameter <QWidget*> { affect (QWidget::setParent) } parent;
            parameter <int> { affect (QToolBar::setFixedWidth) } width;
            parameter <int> { affect (QToolBar::setFixedHeight) } height;
            parameter <QLayout*> { affect (QWidget::setLayout) } layout;
            parameter <const QRect&> { affect (QWidget::setGeometry) } geometry;
            // parameter <const list<QAction*>&> { affect(LeftToolBar::addActions) } actions;
        };

        LeftToolBar() : QScrollArea() {
            QObject::setObjectName("LeftToolBar");
            // this->setFixedSize(32, 12*32);
            // auto area = new QScrollArea();
            this->horizontalScrollBar()->setDisabled(true);
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

            // this->setStyleSheet("QWidget { background-color: transparent; } QScrollArea { background-color: orange; }");
            // area->setFixedSize(32, 14*32);
            auto w = new QWidget();
            auto layout = new QVBoxLayout();
            layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomFitBest));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border-top-left-radius: 5px; border-top-right-radius: 5px; border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                // tb->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(tb);
                // tb->setGeometry(0, 0, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomFitBest));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border-top-left-radius: 6px; border-top-right-radius: 6px; border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                // tb->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(tb);
                // tb->setGeometry(0, 0, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomFitBest));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border-top-left-radius: 6px; border-top-right-radius: 6px; border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                // tb->setContentsMargins(0, 0, 0, 0);
                layout->addWidget(tb);
                // tb->setGeometry(0, 0, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 1*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 2*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyLeft));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 3*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::FormatJustifyRight));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 4*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::InputKeyboard));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 5*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::InputTablet));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 6*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaTape));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 9*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 10*32, 32, 32);
                // tb->setParent(this);
            }
            {
                auto tb = new ToolButton();
                tb->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoUp));
                tb->setStyleSheet("QToolButton { border: 0px; background-color: transparent; }"
                                  "QToolButton:pressed { border: 0px; background-color: rgba(100, 100, 100, 100); }");
                // tb->setStyleSheet("QToolButton {background-color: transparent;}");
                tb->setFixedSize(32, 32);
                layout->addWidget(tb);
                // tb->setGeometry(0, 10*32, 32, 32);
                // tb->setParent(this);
            }

            w->setLayout(layout);
            this->setWidget(w);
            // area->setWidget(this);
            // w->setParent(this);
            // this->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
            // this->setFixedHeight(300);
            // this->setFixedWidth(30);
            // auto btn = new QToolButton(this);
            // btn->setGeometry(0, 20, 20, 20);
            // this->addAction(new QAction (QIcon::fromTheme(QIcon::ThemeIcon::EditUndo), "Undo"));
            return;
            // layout->setGeometry(QRect(0, 0, 40, 100));
            // layout->setAlignment(new ToolButton({.width= 30}), Qt::AlignLeft);
            // w->setLayout(layout);

            // layout->setAlignment(layout, Qt::AlignLeft);
            // auto tb = new QToolBar(this);



        }

        virtual ~LeftToolBar() { }



        // void addActions(const list<QAction*>& actions) {
        //     for (QAction* action : actions) {
        //         QToolBar::addAction(action);
        //     }
        // }

    };

}


