#include <QApplication>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <iostream>
#include <QPainterPath>
#include <QWindow>
#include <QDebug>

#include "selectorwidget.h"

QScreen *getActiveScreen()
{
    QPoint globalCursorPos = QCursor::pos();
    QScreen *activeScreen = QGuiApplication::screenAt(globalCursorPos);
    return activeScreen;
}

QPixmap grabScreenshot()
{
    QScreen *activeScreen = getActiveScreen();
    QPixmap desktopPixmap = QPixmap(activeScreen->geometry().size());
    QPainter p(&desktopPixmap);

    p.drawPixmap(*(new QPoint(0, 0)), activeScreen->grabWindow(0));

    return desktopPixmap;
}

SelectorWidget::SelectorWidget(QWidget *parent) : QDialog(parent, Qt::FramelessWindowHint)
{
    QScreen *activeScreen = getActiveScreen();
    setAttribute(Qt::WA_TranslucentBackground);

    setGeometry(activeScreen->geometry());
    desktopPixmap = grabScreenshot();
}

void SelectorWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint *pnt = new QPoint(event->localPos().x(), event->localPos().y());
    selectedRect.setTopLeft(*pnt);
}

void SelectorWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint *pnt = new QPoint(event->localPos().x(), event->localPos().y());
    selectedRect.setBottomRight(*pnt);
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
