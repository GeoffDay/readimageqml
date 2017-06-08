#ifndef READIMAGE_H
#define READIMAGE_H

#include <QRgb>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QVector>
//#include <QPainter>
#include <QByteArray>
#include <QBasicTimer>
#include <QWheelEvent>


#include <QtQuick/QQuickPaintedItem>


class ReadImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(quint32 magnification READ Magnification WRITE setMagnification)
    Q_PROPERTY(QString iFileName READ IFileName WRITE openIFileName)

public:
    ReadImage(QQuickItem *parent = 0);

    QStringList listIFiles(QFileInfo);            // for next and prev file buttons
    Q_INVOKABLE bool openIFileName(QString);
    Q_INVOKABLE void setMagnification(int);
    Q_INVOKABLE int getBinImage(int, int);
    Q_INVOKABLE void timerTimeout();

    void getBinHeaderData();
    QString name() const;
    QString IFileName() const;
    void setName(const QString &name);
    void paint(QPainter *painter);
    quint32 Magnification() const;

    void setCTType(bool);
    void getMin(int);                               // from paletteRanges.qml code
    void getMax(int);
    void setAGCOn();
    void setAGCOff();
    void pixScl(QString tString);

    Q_INVOKABLE bool begin();
    Q_INVOKABLE bool back();
    Q_INVOKABLE bool play();
    Q_INVOKABLE bool loop(bool);
    Q_INVOKABLE bool forward();
    Q_INVOKABLE bool end();

private slots:
//    void genByteSwapTable();
    void genGreyColourTable(quint16, quint16);
    void genLCIIColourTable(quint16, quint16);
    void recalcColourTable(quint16, quint16);


signals:
    void currFrame(int);
    void newHistogram(QVector<quint32>);
    void newColourTable(QVector<QRgb>);
//    void newImageLimits(QPoint(int, int));
    void changePlay(bool);
    void agcState(bool);


private:
    QFile file, exportFile;
    QString dirStr, pixStr;
    QString iFileName, iExt, exportFileName;
    QStringList iFileList;
    quint32 filePos, beginPos, endPos, redraw;
    quint32 ctMin, ctMax;               //colour table min and max

    QImage image;
    double pixelScale;
    quint32 iWidth, iHeight, imageMin, imageMax,totalPixels, xPos, yPos;
    quint32  currentFrame, startFrame, endFrame, magnification;
    bool flipX, flipY, loopMode, playMode, colourTableType, AGC;
    QString m_name;

    QVector<quint16> byteSwapTable;     //used for byteswapping the file data
    QVector<quint32> histogram;         //used to create a histogram of values needs to be 32 as 1024 * 768 = 786432
    QVector<QRgb> colourTable;          //colour table
};

#endif // READIMAGE_H
