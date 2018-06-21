#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QRgb>
#include <QImage>
#include <QWidget>
#include <QVector>
#include <QPainter>


class Histogram : public QWidget
{
    Q_OBJECT
public:
    Histogram(QWidget *parent = 0);
    
signals:
    void histVals(QString histStr);         //value under the mouse pointer

public slots:
    void setHistogram(QVector<quint32> tHistogram);
    void setColourTable(QVector<QRgb> tColourTable);
    void setImageSize(QPoint tArfSize);
    void setiMinMax(QPoint tMinMax);        //these are the min and max image values
    void setPixelScale(double);             //only affects the values displayed

private slots:
    int niceNumber(int number, bool Round);

protected:
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    void resizeImage(QImage *image, const QSize &newSize);
    bool logVertAxis;
    QImage histImage;
    quint32 histWidth, histHeight, histHOffset, histVOffset;
    quint32 imageMin, imageMax, pltWidth, pltHeight, range;
    quint32 histXMin, histXMax, HistYMax, tickXSpacing, tickYSpacing;
    double magnitude, fraction, pixelScale;
    double histXScale, histYScale;                      //each pixel width=??? This allows scaling SPAD data to metres
    QVector<QRgb> myColourTable, waitCTable;            //colour table
    QVector<quint32> myHistogram;         //where we keep the histogram data
    quint32 xPos, yPos, ordersMag, maxPixels, initTicks, finalTicks, linZoom;                 //current mouse coords wrt image pixels
};

#endif // HISTOGRAM_H
