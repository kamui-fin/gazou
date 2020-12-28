#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "selectorwidget.h"
#include "utils.h"

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
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    desktopPixmap = grabScreenshot();
    setGeometry(activeScreen->geometry());
    showFullScreen();
    this->setCursor(Qt::CrossCursor);
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
    selectedPixmap.toImage().save("/tmp/tempImg.png");
    selectedRect.setRect(0,0,0,0);
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
