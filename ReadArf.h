#ifndef READARF_H
#define READARF_H

#include <QRgb>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QByteArray>
#include <QBasicTimer>
#include <QWheelEvent>

class ReadArf : public QWidget
{
    Q_OBJECT

public:
    ReadArf(QWidget *parent = 0);

public slots:
    QStringList listArfFiles(QFileInfo);            // for next and prev file buttons

    void getStartFrame(int);                        // from arfPos.qml code
    void getEndFrame(int);
    void getCurrentFrame(int);

    void setCTType(bool);
    void getMin(int);                               // from paletteRanges.qml code
    void getMax(int);
    void setImageNumber(int);
    void genGreyColourTable(quint16, quint16);
    void genLCIIColourTable(quint16, quint16);
    void setFrameRate(int);
    void setImageSize(int);
    void pixScl(QString);

    void setAGCOn();
    void setAGCOff();

    void setFlipH(bool);
    void setFlipV(bool);

    bool prevArf();
    bool openArf();
    bool nextArf();
    void zoomIn();
    void zoomOut();
    bool getHeaderData();
    int getImage(int, int);
    void exportARF();
    void exportBMP();
    bool begin();
    bool back();
    bool play();
    bool loop(bool);
    bool forward();
    bool end();

private slots:
    void resizeImage(QImage *image, const QSize &newSize);
    void genByteSwapTable();
    void recalcColourTable(quint16, quint16);

signals:
    void pixVals(QString);
    void changePlay(bool);
    void nFrames(int);
    void currFrame(int);
    void arfSize(QPoint);
    void newImValLimits(QPoint);
    void range(quint16);
    void newHistogram(QVector<quint32>);
    void newColourTable(QVector<QRgb>);
    void wTitle(QString);
    void imageMag(int);
    void agcState(bool);

protected:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QImage image;
    QString imageType;
    QString pixStr;

    QFile file, exportFile;
    quint32 filePos, beginPos, endPos;
    QString arfDirStr;
    QString arfFileName, exportFileName;
    QStringList arfFileList;

    bool playMode, loopMode, exportARFFlag, exportBMPFlag;
    float sliderScale;
    quint32 imageChangeRate;                //invokes AGC changes now a percentage as it avoids rounding error
    double pixelScale;                  //adjusts integer values to read other scales

    quint32 redraw;                         //redraw current image
    quint32 numberOfFrames;
    quint32 startFrame, currentFrame, endFrame;

    QBasicTimer timer;
    int timeInterval;                   //used to set fps

    QVector<quint16> byteSwapTable;     //used for byteswapping the file data
    QVector<quint32> histogram;         //used to create a histogram of values needs to be 32 as 1024 * 768 = 786432

    bool AGC;
    bool colTableType, flipX, flipY;    // true indicates LCII colour table or flipped images
    QVector<QRgb> colourTable;          //colour table
    quint16 iRange, skipPixels;         //skip pixels allows me to jump over data pixels
    quint32 iWidth, iHeight, totalPixels, outlierThreshold;
    quint32 xPos, yPos, Magnification;  //current mouse coords wrt image pixels
    quint32 imageMin, imageMax, imageAve, threshold;
    quint32 ctMin, ctMax;               //need to decouple
};

#endif // READARF_H
