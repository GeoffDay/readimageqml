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
    void changePlay(bool);
    void agcState(bool);
//    void nFrames(quint32);


private:
    QFile file, exportFile;
    QString dirStr, pixStr, imageType;
    QString iFileName, iExt, exportFileName;
    QStringList iFileList;
    quint32 spadVersion, fileVersion, filePos, beginPos, endPos, redraw;
    quint32 outlierThreshold, ctMin, ctMax;               //colour table min and max

    QImage image;
    double pixelScale;
    quint32 iWidth, iHeight, imageMin, imageMax,totalPixels, xPos, yPos;
    quint32 numberOfFrames, currentFrame, startFrame, endFrame, magnification;
    bool flipX, flipY, loopMode, playMode, colourTableType, AGC;
    QString m_name;

    QVector<quint16> byteSwapTable;     //used for byteswapping the file data
    QVector<quint32> histogram;         //used to create a histogram of values needs to be 32 as 1024 * 768 = 786432
    QVector<QRgb> colourTable;          //colour table
};

#endif // READIMAGE_H
