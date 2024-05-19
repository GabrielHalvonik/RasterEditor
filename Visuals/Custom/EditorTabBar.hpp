#pragma once

#include <QWidget>
#include <QTabBar>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QApplication>
#include <QPushButton>
#include <QScrollBar>
#include <QPainter>
#include <QStyle>
#include <QDialog>
#include <QSlider>
#include <QIntValidator>

#include "../../Utilities/General.hpp"
#include "../General/Styles.hpp"
#include "../Basics/PushButton.hpp"
#include "../Basics/Dialog.hpp"
#include "../Basics/Widget.hpp"
#include "../Basics/LineEdit.hpp"
#include "../Basics/FormLayout.hpp"
#include "../Basics/VerticalBoxLayout.hpp"
#include "../Basics/HorizontalBoxLayout.hpp"

using namespace Visuals::Basics;

namespace Visuals::Custom {

    struct EditorTabBar : QScrollArea {

        // Property <int> prop;

        EditorTabBar(QWidget* parent = nullptr) : QScrollArea(parent) {
            // prop = 2;
            // qInfo() << prop;


            QScrollArea::setFixedHeight(barHeight);
            tabBar->setFixedHeight(barHeight);

            // this->setWidgetResizable(true);
            this->verticalScrollBar()->setDisabled(true);
            this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
            this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

            // tabBar->addTab("A");
            // tabBar->addTab("B");
            // tabBar->addTab("C");
            // tabBar->addTab("D");
            // tabBar->addTab("E");
            // tabBar->addTab("F");
            // tabBar->addTab("G");
            // tabBar->addTab("H");

            QObject::connect(tabBar, &QTabBar::tabCloseRequested, this, [this](int index) {
                tabBar->removeTab(index);
            });

            tabBar->setMovable(true);
            tabBar->setTabsClosable(true);

            auto inner = new QWidget();
            auto layout = new QHBoxLayout();
            layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->setSpacing(0);

            layout->addWidget(tabBar);
            layout->addWidget(new PushButton ({
                .name = { "TabBarAddTabButton" },
                .width = barHeight,
                .height = barHeight,
                .text = { "+" },
                .clicked = {
                    [this](bool state) {

                        QPushButton* accept { };
                        QPushButton* cancel { };
                        LineEdit* nameLineEdit { };
                        LineEdit* widthLineEdit { };
                        LineEdit* heightLineEdit { };

                        auto dialog = Dialog ({
                            .layout = new VerticalBoxLayout ({
                                .items = {
                                    new Widget ({
                                        .layout = new FormLayout ({
                                            .rows = {
                                                { { "name" }, nameLineEdit = new LineEdit ({ .text = { "default" } }) },
                                                { { "width" }, widthLineEdit = new LineEdit ({ .text = { "512" }, .validator = new QIntValidator(10, 10000) }) },
                                                { { "height" }, heightLineEdit = new LineEdit ({ .text = { "512" }, .validator = new QIntValidator(10, 10000) }) },
                                            }
                                        })
                                    }),
                                    new Widget ({
                                        .layout = new HorizontalBoxLayout ({
                                            .items = {
                                                accept = new QPushButton("Accept"),
                                                cancel = new QPushButton("Cancel"),
                                            }
                                        })
                                    }),
                                }
                            }),
                        });

                        QObject::connect(accept, &QPushButton::clicked, [&dialog, nameLineEdit, widthLineEdit, heightLineEdit] () {
                            if (nameLineEdit->hasAcceptableInput() and widthLineEdit->hasAcceptableInput() and heightLineEdit->hasAcceptableInput()) {
                                dialog.accept();
                            }
                        });

                        QObject::connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

                        if (dialog.exec() == QDialog::Accepted) {
                            auto name = nameLineEdit->text();
                            int width = widthLineEdit->text().toInt();
                            int height = heightLineEdit->text().toInt();
                            qInfo() << width << " : " << height;
                            tabBar->addTab(name);
                        }

                    }
                }
            }));

            inner->setLayout(layout);

            this->setWidget(inner);
        }

        virtual ~EditorTabBar() { }

    private:

        const int barHeight = 20;

        struct ScrollableTabBar : QTabBar {

            EditorTabBar* parent { };

            ScrollableTabBar(EditorTabBar* parent = nullptr) : QTabBar(parent), parent(parent) {
                QObject::setObjectName("ScrollableTabBar");

                QTabBar::setDrawBase(false);

                QObject::connect(this, &QTabBar::tabBarDoubleClicked, this, [this](int index) {
                    qInfo() << "? " << index;
                });
            }

            virtual ~ScrollableTabBar() { }

            void tabInserted(int index) override {
                QTabBar::tabInserted(index);

                QTabBar::setTabButton(index, QTabBar::ButtonPosition::RightSide,
                    new CustomCloseButton ({
                        .parent = this,
                        .size = 12,
                        .clicked = {
                            delegate {
                                if (auto self = reinterpret_cast<PushButton*>(source); self != nullptr) {
                                    QTabBar::tabCloseRequested(QTabBar::tabAt(self->pos()));
                                }
                            }
                        },
                    })
                );
            }

            void wheelEvent(QWheelEvent* event) override {
                if (parent != nullptr) {
                    QApplication::sendEvent(parent->horizontalScrollBar(), event);
                } else {
                    QTabBar::wheelEvent(event);
                }
            }

            void tabLayoutChange() override {
                if (QTabBar::count() == 0) {
                    Styles::set(this, Styles::Fulfillment, Styles::Fulfillment.Values.Empty);
                } else {
                    Styles::set(this, Styles::Fulfillment, Styles::Fulfillment.Values.Filled);
                }
            }

            struct CustomCloseButton : public QPushButton {

                parametrize (CustomCloseButton) {
                    parameter <QWidget*> { affect (QWidget::setParent) } parent;
                    parameter <int> { affect (CustomCloseButton::setSize) } size;
                    connection <bool, QPushButton> { subscribe (QPushButton::clicked) } clicked;
                };

                CustomCloseButton() : QPushButton() {
                    QObject::setObjectName("TabBarCloseButton");
                }

                void mousePressEvent(QMouseEvent* event) override { QPushButton::mousePressEvent(event); pressed = true; repaint(); }
                void mouseReleaseEvent(QMouseEvent* event) override { QPushButton::mouseReleaseEvent(event); pressed = false; repaint(); }
                void enterEvent(QEnterEvent* event) override { QPushButton::enterEvent(event); hovered = true; }
                void leaveEvent(QEvent* event) override { QPushButton::leaveEvent(event); hovered = false; }

                void paintEvent(QPaintEvent* event) override {
                    QPainter painter(this);
                    painter.setRenderHint(QPainter::Antialiasing);

                    painter.setPen(Qt::NoPen);
                    if (pressed) {
                        painter.setBrush(QColor(200, 100, 100));
                        painter.drawRoundedRect(rect(), 2, 2);
                        painter.setPen(QColor(160, 160, 160));
                    } else if (hovered) {
                        painter.setBrush(QColor(100, 100, 100));
                        painter.drawRoundedRect(rect(), 2, 2);
                        painter.setPen(QColor(160, 160, 160));
                    } else {
                        painter.setPen(QColor(120, 120, 120));
                    }

                    int lineLength = size * 0.6;
                    int centerX = width() / 2;
                    int centerY = height() / 2;

                    QPainterPath path;
                    path.moveTo(centerX - lineLength / 2, centerY - lineLength / 2);
                    path.lineTo(centerX + lineLength / 2, centerY + lineLength / 2);
                    path.moveTo(centerX + lineLength / 2, centerY - lineLength / 2);
                    path.lineTo(centerX - lineLength / 2, centerY + lineLength / 2);
                    painter.drawPath(path);
                }

                inline void setSize(int value) { QPushButton::setFixedSize(size, size);  size = value; }

            private:
                int size = 12;

                bool pressed { };
                bool hovered { };

            };

        } * tabBar = new ScrollableTabBar { this };

    };

}
