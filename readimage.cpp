// lets make this file type agnostic so that we can read the old Arf files and the newer Bin files
// so I'll change the arf names to inames

#include <QtGui>
#include <QFile>
#include <qmath.h>
#include <QString>
#include <QVector>
#include <QtDebug>
#include <QPainter>
#include <QtEndian>
#include <QByteArray>
#include <QEventLoop>
#include <QMouseEvent>
#include <QDataStream>
#include <QImageWriter>


#include "readimage.h"

ReadImage::ReadImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
       dirStr = QDir::currentPath();
}



bool ReadImage::openIFileName(QString tFileName)
{
    tFileName.remove(0,8);                                      // qml returns a URL with File:/// prefixed before c:/.... so remove it
    qDebug() << tFileName;
    file.setFileName(tFileName);                                //lets create a file
    QFileInfo iFileInfo(tFileName);                             //lets get the file info

    dirStr = iFileInfo.absolutePath();                          //separate the path string
    iFileName = iFileInfo.baseName();                           //now this is just the filename alone

    arfFileList = listArfFiles(iFileInfo);
    qDebug() << dirStr << iFileName << arfFileList;

    file.open(QIODevice::ReadOnly);         //Open the file for reading

//    getHeaderData();
    return true;
}



QStringList ReadImage::listArfFiles(QFileInfo aFInfo)
{
    QDir aDir(aFInfo.absoluteDir());                            //create a QDir path

    QStringList filter(QString("*.%1").arg(aFInfo.suffix()));   //filter on arfs or bins depending on what I selected in qml
    QStringList aFList = aDir.entryList(filter, QDir::Files, QDir::Name);   //list all arf files in current directory

    qDebug() << aDir << aFList;
    return aFList;
}

bool ReadImage::getBinHeaderData()
{
    // This needs to reflect the Bin header valuesRead and check the header. bytes 0 - 3 "SPAD", bytes 4 - 7
    // bytes 8 - 11 number of Y pixels iHeight, bytes 12 - 15 number of x pixels iWidth, bytes 16 - 19 Image type 0x00000003 ?
    // bytes 20 - 23 total Arf frames. bytes 23 - 27 start of first image offset 0x00000020. bytes 28 - 31 unused 0x00000000

    file.open(QIODevice::ReadOnly);     //Open the file for reading
    quint32 nMagic, nBytes, nIType, nUnusd;
    QDataStream in(&file);              // file should be open already

    in >> nMagic >> nBytes >> iHeight >> iWidth >> nIType >> numberOfFrames >> beginPos >> nUnusd;
    qDebug() << nMagic << nBytes << iHeight << iWidth << nIType << numberOfFrames << beginPos << nUnusd;

    if ((nMagic != 0xBBBBBAAD) || (nBytes != 0x00000002) || (nIType != 0x00000003)){
        QMessageBox::warning(this, tr("Qt Arf player"),tr("File %1:\n contains bad Header Data.").arg(iFileName));
        return false;
    }


    imageType = "unknown   ";
    Magnification = 2;
    iRange = 4095;
    skipPixels = 0;

    if (iWidth < 32){
        Magnification = 16;
        outlierThreshold = 5; // this is 5 pixels
    }

    if (iWidth == 32) {
        imageType = "SPAD   ";
        Magnification = 16;
        outlierThreshold = 3; // this is 3 pixels
    }

    if (iWidth == 256) {
        imageType = "Amber   ";
        Magnification = 2;
        outlierThreshold = 512; // this 2 lines
    }

    if (iWidth == 384) {
        imageType = "Otherseeker   ";
        Magnification = 2;
        outlierThreshold = 385; // this is 1 line
    }
    if (iWidth == 576) {
        imageType = "LWRTS   ";
        Magnification = 1;
        outlierThreshold = 600;
        iRange = 32767;
    }

    if (iWidth == 896) {
        imageType = "LWRTS   ";
        Magnification = 1;
        outlierThreshold = 600;
        iRange = 32767;
    }

    if (iWidth == 640) {
        imageType = "Bioseeker   ";
        Magnification = 1;
        outlierThreshold = 690;
        iRange = 32767;
    }

    if (iWidth == 1024) {
        imageType = "HSV   ";
        Magnification = 1;
        outlierThreshold = 3000;        //24576 is 3%
    }

    if (iWidth == 1028) {
        imageType = "SPAD Histogram Raw ";   //4,16,64 or 256 images taken in quick succession witha high speed laser
        Magnification = 16;
        outlierThreshold = 3;
        iWidth = 32;
        iHeight = 32;
        skipPixels = 4;
        iRange = 32767;
    }

    if (iWidth == 64) {
        imageType = "SPAD Histogram  ";   //4,16,64 or 256 images taken in quick succession with a high speed laser
        Magnification = 8;
        outlierThreshold = 3;
        iWidth = 64;
        iHeight = 64;
        skipPixels = 0;
        iRange = 32767;
    }

    emit arfSize(QPoint(iWidth, iHeight));


    qDebug() << "Outlier Threshold: " << outlierThreshold;

    totalPixels = iWidth * iHeight;         //calc it here once for this file



    imageType += (dirStr  + "/" + iFileName + " mag x" + QString::number(Magnification, 10));
    emit wTitle(imageType);
    emit imageMag(Magnification);           //so the widgets indicate the right magnification
    emit range(iRange);

    // sliderScale = 800 / numberOfFrames;   // slider has a range 0 to 500
    emit nFrames(numberOfFrames);           //set our number of frames to the widget

    filePos = file.pos();                   // get our current file position
    qDebug() << "filepos" << filePos;

    endPos = beginPos + (nBytes * iWidth * iHeight * (numberOfFrames - 1)); // this should be the start of the last frame
    qDebug() << filePos << beginPos << endPos << "Play";

    resize(QSize((iWidth * Magnification), (iHeight * Magnification)));

    setAGCOn();
    redraw = 1;

    timer.start(timeInterval, this);
    imageMin = 0;
    imageMax = quint16(4095 * pixelScale);

    startFrame = 1;                     //setup arfpos.qml values
    endFrame = numberOfFrames;
    currentFrame = 1;
    redraw = 1;                         // 1 redraws current frame
    show();
    update();

    playMode = true;
    emit changePlay(playMode);
    return true;
}

int ReadImage::getBinImage(int rdrw, int frame)
{
    // image min and max are stored in the 64 byte frame header
    QByteArray baImage(iWidth * iHeight * 2, 0);
    QByteArray baImageR(iWidth * iHeight * 2, 0);   // this is a result array. we can write this to a file easily
    QImage thisFrame(iWidth, iHeight, QImage::Format_RGB32);

    quint16 colour, tColour;

    if ((rdrw > 0) && file.isOpen()) {
        rdrw--;
        emit currFrame((int)(frame));     // shows us where we are in the file * sliderScale
        quint32 pixelPos = (xPos + (yPos * iWidth));    //calculate memory location of the pixel
        quint16 histValue;

        quint32 seekPos = (((iWidth * iHeight)) * 2 * (frame - 1)) + beginPos;
        file.seek(seekPos);
        baImage = file.read(iWidth * iHeight * 2);      // read 1 frame to a bytearray
        baImageR = baImage;
        quint16 *imagePtr = reinterpret_cast<quint16 *>(baImage.data()) + skipPixels;   // create a pointer to that bytearray and
        quint32 *thsFrmePtr = reinterpret_cast<quint32 *>(thisFrame.bits());
        histogram.fill(0,65536);


        for (quint32 i = 0; i < totalPixels; i++, imagePtr++, thsFrmePtr++){  //iterate image pointer through memory
            colour = byteSwapTable.at(*imagePtr);
//            colour = *imagePtr;
            *thsFrmePtr = colourTable.at(colour);
            tColour = colour;

            histValue = histogram[tColour];
            histogram[tColour] = histValue + 1; //since we're trawling through the image do the histogram

            if (pixelPos == i){
                double sColour = tColour * pixelScale;
                pixStr = QString(" %1, %2, %3 ").arg(xPos,0,10,QChar(' ')).arg(yPos,0,10,QChar(' ')).arg(sColour, 0, 'f', 1, QChar(' '));
            }
        }

        image = thisFrame.scaledToWidth(iWidth * Magnification).mirrored(flipX, flipY);

        if (exportBMPFlag) {
            QString iName = QString("%1/%2_%3.bmp").arg(DirStr).arg(iFileName).arg(currentFrame, 4, 10, QChar('0'));
            exportFile.setFileName(iName);

            if (!exportFile.open(QIODevice::WriteOnly)){
                qDebug() << "export file" << iName <<  "not open";
            } else {
                image.save(iName, "BMP", 100); // writes image into ba in PNG format
            }
            exportFile.close();
            qDebug() << "Exported" << iName;
        }

        if (exportARFFlag) {
            QDataStream out(&exportFile);
            out.writeRawData(baImageR, (totalPixels + skipPixels) * 2);
        }

        repaint();

        emit newHistogram(histogram);
    }
    return rdrw;
}





QString ReadImage::IFileName() const
{
    return iFileName;
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

quint32 ReadImage::Magnification() const
{
    return magnification;
}


void ReadImage::setMagnification(int iMag)
{
    qDebug() << "in setMagnification now" << iMag;

    switch (iMag) {
        case 0: magnification = 1;      //x1
            break;
        case 1: magnification = 2;      //x2
            break;
        case 2: magnification = 3;      //x4
            break;
        case 3: magnification = 4;      //x8
            break;
        case 4: magnification = 6;     //x16
            break;
        case 5: magnification = 8;     //x32
            break;
        case 6: magnification = 10;      //x1
            break;
        case 7: magnification = 12;      //x2
            break;
        case 8: magnification = 16;      //x4
            break;
        case 9: magnification = 20;      //x8
            break;
        case 10: magnification = 24;     //x16
            break;
        case 11: magnification = 32;     //x32
    }
//    imageType += (dirStr  + "/" + iFileName + " x" + QString::number(Magnification, 10));
//    resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
//    redraw = 1;
//    update();
}
