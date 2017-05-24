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

    iFileList = listIFiles(iFileInfo);
    qDebug() << dirStr << iFileName << iFileList;

    file.open(QIODevice::ReadOnly);         //Open the file for reading

//    getHeaderData();
    return true;
}



QStringList ReadImage::listIFiles(QFileInfo aFInfo)
{
    QDir aDir(aFInfo.absoluteDir());                            //create a QDir path

    QStringList filter(QString("*.%1").arg(aFInfo.suffix()));   //filter on arfs or bins depending on what I selected in qml
    QStringList aFList = aDir.entryList(filter, QDir::Files, QDir::Name);   //list all arf files in current directory

    qDebug() << aDir << aFList;
    return aFList;
}


void ReadImage::getBinHeaderData()
{



    emit arfSize(QPoint(iWidth, iHeight));



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
        emit currFrame((int)(frame));     // shows us where we are in the file
        quint32 pixelPos = (xPos + (yPos * iWidth));    //calculate memory location of the pixel
        quint16 histValue;

        quint32 seekPos = (((iWidth * iHeight)) * 2 * (frame - 1)) + beginPos;
        file.seek(seekPos);
        baImage = file.read(iWidth * iHeight * 2);      // read 1 frame to a bytearray
        baImageR = baImage;
        quint16 *imagePtr = reinterpret_cast<quint16 *>(baImage.data());   // create a pointer to that bytearray and
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
