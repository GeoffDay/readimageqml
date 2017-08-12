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
    Q_PROPERTY(quint32 nFrames READ nFrames NOTIFY nFramesChanged)
    Q_PROPERTY(quint32 startFrame READ sFrame WRITE setStartFrame NOTIFY startFrameChanged)
    Q_PROPERTY(quint32 endFrame READ eFrame WRITE setEndFrame NOTIFY endFrameChanged)
    Q_PROPERTY(quint32 currentFrame READ cFrame WRITE setCurrentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(bool playMode READ getPlayMode WRITE setPlayMode NOTIFY playModeChanged)
    Q_PROPERTY(quint32 ctMin READ getCTMin WRITE setCTMin NOTIFY ctMinChanged)
    Q_PROPERTY(quint32 ctMax READ getCTMax WRITE setCTMax NOTIFY ctMaxChanged)
    Q_PROPERTY(bool colourTableType READ getCTMax WRITE setCTType NOTIFY ctTypeChanged)

    Q_PROPERTY(quint32 magnification READ getMagnification WRITE setMagnification)
    Q_PROPERTY(QString iFileName READ IFileName WRITE openIFileName)

public:
    ReadImage(QQuickItem *parent = 0);

    QStringList listIFiles(QString aFileName);            // for next and prev file buttons
    Q_INVOKABLE bool openIFileName(QString);
    Q_INVOKABLE bool prevIFile();
    Q_INVOKABLE bool nextIFile();
    Q_INVOKABLE void setMagnification(int);
    Q_INVOKABLE quint32 getMagnification() const;
    Q_INVOKABLE int getBinImage(int, int);
    Q_INVOKABLE void timerTimeout();
    Q_INVOKABLE bool getPlayMode();
    Q_INVOKABLE void setPlayMode(bool);
    Q_INVOKABLE void setCTMin(quint32);
    Q_INVOKABLE void setCTMax(quint32 tCTMax);
    Q_INVOKABLE quint32 getCTMin();
    Q_INVOKABLE quint32 getCTMax();
    Q_INVOKABLE void setCTType(bool);
    Q_INVOKABLE void setAGCOn();
    Q_INVOKABLE void setAGCOff();

    void getBinHeaderData();
    QString IFileName() const;
    void paint(QPainter *painter);

//    void getMin(quint16);                               // from palett#ifdef Q_OS_UNIXeRanges.qml code
//    void getMax(quint16);
    void pixScl(QString tString);

    Q_INVOKABLE void setCurrentFrame(int tCFrame);
    Q_INVOKABLE void setStartFrame(int tSFrame);
    Q_INVOKABLE void setEndFrame(int tEFrame);
    Q_INVOKABLE quint32 nFrames();
    Q_INVOKABLE quint32 sFrame();
    Q_INVOKABLE quint32 eFrame();
    Q_INVOKABLE quint32 cFrame();

    Q_INVOKABLE void begin();
    Q_INVOKABLE void back();
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void loop(bool);
    Q_INVOKABLE void forward();
    Q_INVOKABLE void end();

private slots:
//    void genByteSwapTable();
    void genGreyColourTable(quint16, quint16);
    void genLCIIColourTable(quint16, quint16);
    void recalcColourTable(quint16, quint16);

signals:
    void currentFrameChanged(quint32 currFrame);
    void startFrameChanged(quint32 startFrame);
    void endFrameChanged(quint32 endFrame);
    void nFramesChanged(quint32 numberOfFrames);
    void newHistogram(QVector<quint32>);
    void newColourTable(QVector<QRgb>);
//    void newImageLimits(QPoint(int, int));
    void playModeChanged(bool);
    void ctTypeChanged(bool);
    void ctMinChanged(quint32 ctMin);
    void ctMaxChanged(quint32 ctMax);

    void agcState(bool);
//    void nFrames(quint32);


private:
    QImage image;
    QFile file, exportFile;
    QStringList iFileList;
    QString pixStr, imageType;
    QString dirStr, iFileName, iExt, exportFileName;
    quint32 iWidth, iHeight,totalPixels, xPos, yPos;
    quint32 numberOfFrames, currentFrame, startFrame, endFrame, magnification;
    quint32 spadVersion, fileVersion, filePos, beginPos, endPos, redraw;
    quint32 outlierThreshold, ctMin, ctMax, imageMin, imageMax;               //colour table min and max

    double pixelScale;
    bool flipX, flipY, loopMode, playMode, colourTableType, AGC;

    QVector<quint16> byteSwapTable;     //used for byteswapping the file data
    QVector<quint32> histogram;         //used to create a histogram of values needs to be 32 as 1024 * 768 = 786432
    QVector<QRgb> colourTable;          //colour table
};

#endif // READIMAGE_H
