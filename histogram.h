#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QRgb>
#include <QImage>
#include <QVector>
#include <QPainter>

#include <QtQuick/QQuickPaintedItem>

class Histogram : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QVector<quint32> myHistogram WRITE setHistogram NOTIFY histogramChanged)
    Q_PROPERTY(QVector<QRgb> waitCTable WRITE setColourTable NOTIFY colourTableChanged)
//    Q_PROPERTY(quint32 ctMax READ getCTMax WRITE setCTMax NOTIFY ctMaxChanged)
//    Q_PROPERTY(bool colourTableType READ getCTMax WRITE setCTType NOTIFY ctTypeChanged)

//    Q_PROPERTY(quint32 magnification READ getMagnification WRITE setMagnification)


public:
    Histogram(QQuickItem *parent = 0);
    
signals:
    void histVals(QString histStr);         //value under the mouse pointer
    void colourTableChanged();

public slots:
    Q_INVOKABLE void setHistogram(QVector<quint32> tHistogram);
    Q_INVOKABLE void setColourTable(QVector<QRgb> tColourTable);
    Q_INVOKABLE void setImageSize(QPoint tArfSize);
    Q_INVOKABLE void setiMinMax(QPoint tMinMax);        //these are the min and max image values
    Q_INVOKABLE void setPixelScale(double);             //only affects the values displayed

    void paint(QPainter *painter);

private slots:
    int niceNumber(int number, bool Round);




private:
    bool logVertAxis;
    QImage histImage;
    quint32 histWidth, histHeight, histHOffset, histVOffset;
    quint32 imageMin, imageMax, pltWidth, pltHeight, range;
    quint32 histXMin, histXMax, HistYMax, tickXSpacing, tickYSpacing;
    double magnitude, fraction, pixelScale;
    double histXScale, histYScale;                      //each pixel width=??? This allows scaling SPAD data to metres
    QVector<QRgb> myColourTable, waitCTable;            //colour table
    QVector<quint32> myHistogram;           //where we keep the histogram data
    quint32 xPos, yPos, ordersMag, maxPixels, initTicks, finalTicks, linZoom;                 //current mouse coords wrt image pixels
};

#endif // HISTOGRAM_H
