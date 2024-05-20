#pragma once

#include <QWidget>

class CentralWidget : public QWidget {
    Q_OBJECT
public:
    CentralWidget(QWidget* parent = nullptr);

signals:
    void sizeChanged(const QSize&);

protected:
    void resizeEvent(QResizeEvent*) override;
};
