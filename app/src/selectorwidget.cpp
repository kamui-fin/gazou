#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <iostream>
#include <QPainterPath>

#include "selectorwidget.h"

QPixmap grabScreenshot()
{
    QPixmap desktopPixmap = QPixmap(QApplication::desktop()->geometry().size());
    QPainter p(&desktopPixmap);

    for (QScreen *screen : QApplication::screens())
        p.drawPixmap(screen->geometry().topLeft(), screen->grabWindow(0));

    return desktopPixmap;
}

SelectorWidget::SelectorWidget(QWidget *parent) : QDialog(parent, Qt::FramelessWindowHint)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setGeometry(QApplication::desktop()->geometry());

    desktopPixmap = grabScreenshot();
}

void SelectorWidget::mousePressEvent(QMouseEvent *event)
{
    selectedRect.setTopLeft(event->globalPos());
}

void SelectorWidget::mouseMoveEvent(QMouseEvent *event)
{
    selectedRect.setBottomRight(event->globalPos());
    update();
}

void SelectorWidget::mouseReleaseEvent(QMouseEvent *event)
{
    selectedPixmap = desktopPixmap.copy(selectedRect.normalized());
    selectedPixmap.toImage().save("core/data/images/temp.png");
    accept();
}

void SelectorWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, desktopPixmap);

    QPainterPath path;
    path.addRect(rect());
    path.addRect(selectedRect);
    p.fillPath(path, QColor::fromRgb(255, 255, 255, 50));

    p.setPen(Qt::red);
    p.drawRect(selectedRect);
}
