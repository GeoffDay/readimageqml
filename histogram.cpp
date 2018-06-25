#include <QPen>
#include <QtGui>
#include <QDebug>
#include <QColor>
#include <qmath.h>
#include <QPainter>
#include <QEventLoop>
#include <QMouseEvent>

#include "histogram.h"

Histogram::Histogram(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    histWidth = 850;
    histHeight = 500;
    imageMin = 0;
    imageMax = 65536;
    histHOffset = 50;
    histVOffset = 20;

    linZoom = 4;
    histXScale = 1.0;
    pixelScale = 1.0;
    initTicks = 10;
    finalTicks = 10;
    setMouseTracking(true);

  //  QWidget::setWindowIcon("Histogram.ico");
    setGeometry(450,30,histWidth, histHeight + 40);  //new
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowTitle("Histogram");
    activateWindow();
    show();
}




void Histogram::setHistogram(QVector<quint32> tHistogram)
{
    myHistogram = tHistogram;
    histImage.fill(0xF47f7f7f);
    QPainter painter(&histImage);

    QString histStr = "";
    int offset = imageMin - histXMin;

    pltHeight = histImage.height() - histVOffset;
    tickYSpacing = niceNumber(int(maxPixels / 6), false);
//    qDebug() << "tickYspacing" << tickYSpacing;

    int tickSpacing = pltWidth / finalTicks;

//    qDebug() << "in setHistogram" << imageMin << imageMax << "hist x scale" << histXScale << myColourTable.size();
    painter.setPen("lightgray");
    painter.drawText(xPos - 40,15,histStr);
    painter.drawLine(histHOffset, pltHeight, histHOffset + tickSpacing * finalTicks, pltHeight);

    // draw the horizontal lines
    if (logVertAxis){
        for(quint32 i = 0; i <= ordersMag; i++){
            int j = pltHeight - int(qLn(maxPixels / qPow(10, (ordersMag - i))) * histYScale);
            painter.drawLine(histHOffset, j, histHOffset + (tickSpacing * finalTicks), j);
            histStr = QString("%1").arg(int((maxPixels / qPow(10, (ordersMag - i)))),5,10,QChar(' '));
            painter.drawText(10, j + 10, histStr);
        }
    } else {
        for(quint32 i = 0; i <= 7; i++){
            int j = int(pltHeight - (pltHeight / 6) * i);
            painter.drawLine(histHOffset, j, histHOffset + (tickSpacing * finalTicks), j);
            histStr = QString("%1").arg(int(i * tickYSpacing / linZoom),5,10,QChar(' '));
            painter.drawText(10, j + 10, histStr);
        }
    }
    // draw the vertical lines
    for(quint32 i = 0; i <= finalTicks; i++) {
        int j = histHOffset + (tickSpacing * i);
        painter.drawLine(j, 0, j, pltHeight);
        histStr = QString("%1").arg((histXMin + double(tickXSpacing * i)) * pixelScale,6,'f',1,QChar(' '));
        painter.drawText(j - 10, pltHeight + 15,histStr);
    }

    myColourTable = waitCTable;

    if (!myColourTable.isEmpty()) {
        for (uint i = imageMin; i <= imageMax; i++) {
            quint32 j = int(((i - imageMin) + offset) / histXScale);   //j is the offset version so it plots from zero
            int k = myHistogram.at(i);

            if (xPos - histHOffset == j) {
                histStr = QString("%1, %2").arg(double(i * pixelScale),6,'f',1,QChar(' ')).arg(k,4,10,QChar(' '));
            }

            if (logVertAxis && (k > 1)) {
                 k = qLn(k) * histYScale;
            } else {
                 k *= histYScale;
            }

            painter.setPen(myColourTable.at(i));
            painter.drawLine(j + histHOffset, pltHeight, j + histHOffset, pltHeight - k);
        }
    }
    update();
}



void Histogram::setColourTable(QVector<QRgb> tColourTable)
{
    waitCTable = tColourTable;
}



void Histogram::setImageSize(QPoint tArfSize)
{
    // this is where we handle the size of the histogram window versus the image size.
    // y display will be logarithmic
    maxPixels = tArfSize.x() * tArfSize.y();

    if (maxPixels < 4100) {
        logVertAxis = false;
        histYScale = (histImage.height() - histVOffset) / (maxPixels / linZoom);
    } else {
        logVertAxis = true;
        ordersMag = qLn(maxPixels) / qLn(10);
        histYScale = (histImage.height() - histVOffset) / qLn(maxPixels);
    }
}



void Histogram::setiMinMax(QPoint tPoint)
{
    imageMin = tPoint.x();
    imageMax = tPoint.y();

    if (imageMin != imageMax){
        range = (imageMax - imageMin);
        tickXSpacing = niceNumber(int(range / initTicks), true);
        histXMin = imageMin / tickXSpacing * tickXSpacing;               //work out the minimum first. It'll round down!
        histXMax = (imageMax / tickXSpacing + 1) * tickXSpacing;         //calculated number of ticks

        finalTicks = (histXMax - histXMin) / tickXSpacing; // calculate number of ticks
        pltWidth = (histImage.width() - histHOffset - 10) / finalTicks * finalTicks;
        histXScale = double(double(histXMax - histXMin) / pltWidth);
    }

//    qDebug() << "Hist imageMin" << imageMin << "imageMax" << imageMax << range;
//    qDebug() << histXScale << "initTicks" << initTicks << "final" << finalTicks;
//    qDebug() << "tickSpacing" << histXMin << histXMax << tickXSpacing << pltWidth;
}


void Histogram::setPixelScale(double tDouble)
{
    pixelScale = tDouble;
    qDebug() << "Histogram pixel scale" << pixelScale;
}


int Histogram::niceNumber(int number, bool Round)
{
    // calculates a decent spacing for the horizontal axis
    int exponent, niceFraction, resultNum;
    double magnitude, fraction;
    if (number != 0) {
        exponent = log10(float(number));  //need to cast range to a float
        magnitude = pow(10.0, int(exponent));
        fraction = number / magnitude;

        if (Round){
            if (fraction < 0.19){
                niceFraction = 0.1;
            } else if (fraction < 0.39){
                niceFraction = 0.2;
            } else if (fraction < 0.99){
                niceFraction = 0.4;
            } else if (fraction < 1.99){
                niceFraction = 1;
            } else if (fraction < 3.99){
                niceFraction = 2;
            } else if (fraction < 4.99){
                niceFraction = 4;
            } else if (fraction < 9.99){
                niceFraction = 5;
            } else {
                niceFraction = 10;
            }
        } else {
            if (fraction < 0.1){
                niceFraction = 0.1;
            } else if (fraction < 0.2){
                niceFraction = 0.2;
            } else if (fraction < 0.4){
                niceFraction = 0.4;
            } else if (fraction < 1){
                niceFraction = 1;
            } else if (fraction < 2){
                niceFraction = 2;
            } else if (fraction < 4){
                niceFraction = 4;
            } else if (fraction < 5){
                niceFraction = 5;
            } else {
                niceFraction = 10;
            }
        }
        resultNum = niceFraction * pow(10.0, int(exponent));
//        qDebug() << "nice Number" << number << exponent << magnitude << niceFraction << resultNum;
    } else {
        resultNum = 0;
//        qDebug() << "nice Number" << number;
    }

    return (resultNum);
}


void Histogram::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if (event->buttons() == Qt::RightButton) {      //hold down right mouse button to change size
        if((numSteps > 0) && (linZoom < 32)) {linZoom++;}
        if((numSteps < 0) && (linZoom > 1)) {linZoom--;}
    }

    if(!logVertAxis){histYScale = (histImage.height() - histVOffset) / (maxPixels / linZoom);}
}


void Histogram::mouseMoveEvent(QMouseEvent *event)
{
    xPos = event->pos().x();
    yPos = event->pos().y();
}



void Histogram::paint(QPainter painter)
{
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, histImage, dirtyRect);
}






