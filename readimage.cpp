#include "readimage.h"
#include <QPainter>

ReadImage::ReadImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}


QString ReadImage::name() const
{
    return m_name;
}


void ReadImage::setName(const QString &name)
{
    m_name = name;
}


void ReadImage::paint(QPainter *painter)
{
    QPen pen(Qt::green, 2);
    painter->setPen(pen);
    painter->setRenderHints(QPainter::Antialiasing, true);
    painter->drawPie(boundingRect().adjusted(1, 1, -1, -1), 90 * 16, 290 * 16);
}

void ReadImage::clearChart()
{
    update();

    emit chartCleared();
}
