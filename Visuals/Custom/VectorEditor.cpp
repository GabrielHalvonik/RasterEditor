#include "VectorEditor.hpp"

#include "LeftToolBar.hpp"
#include "EditorTabBar.hpp"
#include "BottomToolBar.hpp"
#include "VectorEditorView.hpp"

#include <QFrame>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QTabBar>
#include <QTabWidget>
#include <QSlider>
#include <QPushButton>
#include <QLabel>

using namespace Visuals::Basics;
using namespace Visuals::Custom;

VectorEditor::VectorEditor() : QFrame() {
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(new EditorTabBar() );
    layout->addWidget(new VectorEditorView ({
        .scene = new QGraphicsScene(0, 0, 1000, 1800),
        .sizeChanged = {
            delegate {
                if (auto self = reinterpret_cast<VectorEditorView*>(source); source != nullptr) {

                }
            }
        }
        // .scene = new QGraphicsScene(0, 0, 800, 500)
      }));

            //     auto bar = new QTabWidget(this);
            //     bar->setStyleSheet("QTabBar { background-color: red; }");
            //     bar->setTabsClosable(true);
            //     bar->setMovable(true);
            //     bar->setDocumentMode(true);
            //     bar->setUsesScrollButtons(true);
            //     bar->setElideMode(Qt::ElideRight);
            //     bar->setUsesScrollButtons(false);

            //     QToolButton *tb = new QToolButton();
            //     tb->setText("+");

            //     bar->addTab(new QWidget(), "");
            //     bar->setCornerWidget(new QSlider(), Qt::BottomRightCorner);
            //     bar->setTabEnabled(0, false);
            //     bar->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
            //     // bar->setStyleSheet("QWidget { padding: 0px; }");
            //     // bar->setGeometry(40, 40, 100, 800);
            //     // bar->setFixedHeight(120);
            //     // bar->setContentsMargins(0, 0, 0, 0);

            //     bar->addTab(new QSlider(), "AHelloWorldWhatIsGoingOn");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "Hello");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "World");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "What");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "is");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "going");
            //     bar->addTab(new QSlider(Qt::Orientation::Horizontal), "on");

                // layout->addWidget(bar);
    this->setLayout(layout);

    // auto scene = new QGraphicsScene(0, 0, 900, 600);
    // scene->addText("A");
    // scene->addText("B")->setPos(800, 500);

    // auto scene2 = new QGraphicsScene(0, 0, 900, 600);
    // scene2->addText("C");
    // scene->addText("B")->setPos(800, 500);

    // auto vec = new VectorEditorView ({
    //     .scene = scene
    // });

    // auto vec2 = new VectorEditorView ({
    //     .scene = scene2
    // });

    // auto layout = new QStackedLayout();
    // layout->setContentsMargins(0, 0, 0, 0);

    // layout->addWidget(vec);
    // layout->addWidget(vec2);

    // layout->setCurrentIndex(1);
    // // layout->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // this->setLayout(layout);

    // vec->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    // vec->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // this->setStyleSheet("QFrame { background-color: red; }");

    new LeftToolBar ({
        .parent = this,
        .geometry = bind (editorViewSizeObservable) into <QRect> {
            // return QRect(10, 10, 32, 12 * 32);
            // return QRect(10, 10, 34, value.height() - 20);
            return QRect(10, value.height() / 2 - value.height()/2 + 100, 34, value.height() - 200);
        }
    });

    new BottomToolBar ({
        .parent = this,
        .geometry = bind (editorViewSizeObservable) into <QRect> {
            if (BottomToolBar* self = reinterpret_cast<BottomToolBar*>(source); self != nullptr) {
                return QRect(value.width() / 2 - self->width() / 2, value.height() - 42, 150, 32);
            }
            return { };
// return QRect(value.width() / 2 - source, value.height() - 42, 150, 32);
            // return QRect(10, value.height() / 2 - value.height()/2 + 100, 32, value.height() - 200);
        }
    });

//     this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//     new VectorEditorView ({
//         .parent = this,
//         // .scene = new QGraphicsScene(0, 0, 200, 200),
//         .items = {
//             new LeftToolBar ({
//                 .geometry = bind (editorViewSizeObservable) into <QRect> {
//                         // return QRect(10, 10, 32, 12 * 32);
//                     return QRect(10, 10, 34, value.height() - 20);
//                     // return QRect(10, value.height() / 2 - value.height()/2 + 100, 34, value.height() - 200);
//                 }
//             }),
//             new BottomToolBar ({
//                 .geometry = bind (editorViewSizeObservable) into <QRect> {
//                     if (BottomToolBar* self = reinterpret_cast<BottomToolBar*>(source); self != nullptr) {
//                         return QRect(value.width() / 2 - self->width() / 2, value.height() - 42, 150, 32);
//                     }
//                     return { };
// // return QRect(value.width() / 2 - source, value.height() - 42, 150, 32);
//                     // return QRect(10, value.height() / 2 - value.height()/2 + 100, 32, value.height() - 200);
//                 }
//             }),
//         },
//         .sizeChanged = editorViewSizeObservable,
//         // .sizeChanged = { [this](const QSize& a) { qInfo() << a.height() << "aaa"; } },
//     })

}

void VectorEditor::resizeEvent(QResizeEvent* event) {
    QFrame::resizeEvent(event);

    editorViewSizeObservable = event->size();
}

VectorEditor::~VectorEditor() { }

