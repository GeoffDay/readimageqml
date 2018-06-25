#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QRgb>
#include <QImage>
#include <QVector>
#include <QByteArray>
#include <QBasicTimer>
#include <QWheelEvent>

#include <QtQuick/QQuickPaintedItem>

class Histogram : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QStringList model MEMBER m_model READ getModel NOTIFY modelChanged)
    Q_PROPERTY(QStringList fastModel MEMBER n_model READ getFastModel NOTIFY fastModelChanged)

    Q_PROPERTY(QString fNameInfo READ fName NOTIFY fNameInfoChanged)
    Q_PROPERTY(quint32 nFrames READ nFrames NOTIFY nFramesChanged)

    Q_PROPERTY(quint32 currentFrame READ cFrame WRITE setCurrentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(quint32 startFrame READ sFrame WRITE setStartFrame NOTIFY startFrameChanged)
    Q_PROPERTY(quint32 endFrame READ eFrame WRITE setEndFrame NOTIFY endFrameChanged)
    Q_PROPERTY(bool playMode READ getPlayMode WRITE setPlayMode NOTIFY playModeChanged)
    Q_PROPERTY(quint32 iRange READ getRange)
    Q_PROPERTY(quint32 ctMin READ getCTMin WRITE setCTMin NOTIFY ctMinChanged)
    Q_PROPERTY(quint32 ctMax READ getCTMax WRITE setCTMax NOTIFY ctMaxChanged)
    Q_PROPERTY(bool colourTableType READ getCTMax WRITE setCTType NOTIFY ctTypeChanged)

    Q_PROPERTY(quint32 magnification READ getMagnification WRITE setMagnification)
    Q_PROPERTY(QString iFileName READ IFileName WRITE openIFileName)

public:
    Histogram(QQuickItem *parent = 0);
    
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
    void paint(QPainter painter);


private:
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
