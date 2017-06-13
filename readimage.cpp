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

#define IRANGE 4095

ReadImage::ReadImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
       dirStr = QDir::currentPath();

       iWidth = 32;
       iHeight = 32;
       imageMin = 0;
       imageMax = 4095;

       magnification = 1;
       pixelScale = 1.0;
       flipX = false;
       flipY = false;
       currentFrame = 1;
       loopMode = true;
       playMode = false;
//       exportARFFlag = false;
//       exportBMPFlag = false;
       dirStr = QDir::currentPath();
       colourTableType = true;
       genLCIIColourTable(0, 65535);

       QImage image(iWidth * magnification, iHeight * magnification, QImage::Format_RGB32);
       qApp->processEvents(QEventLoop::AllEvents);     // THIS KEEPS UI RESPONSIVE!!!!!!!!!!!!!!!!!!!!!!!!!!!
       image.fill(0xFF0606f0);
       QString pixStr = "";

//       setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
//       activateWindow();
//       show();


}




void ReadImage::setCTType(bool cTType)
{
    colourTableType = cTType;
    recalcColourTable(ctMin, ctMax);
}



void ReadImage::getMin(int Value)
{
    ctMin = Value;
    recalcColourTable(ctMin, ctMax);
}



void ReadImage::getMax(int Value)
{
    ctMax = Value;
    recalcColourTable(ctMin, ctMax);
}



void ReadImage::recalcColourTable(quint16 min, quint16 max)
{
    if (min >= max) {        //this is an error
        min = 0;
        max = IRANGE;
    }

    while (max - min < 15) {        // if too close together push the min and max apart
        if (min > 0) { min -= 1;}
        if (max < IRANGE) {max += 1;}
    }

    if (colourTableType) {
        genLCIIColourTable(min, max);
    } else {
        genGreyColourTable(min, max);
    }
   // emit newImageLimits(QPoint(min, max));
}





void ReadImage::genGreyColourTable(quint16 min, quint16 max)
{
    colourTable.clear();
    if (max < 15) {max = 15;}
    float scale = 255.0 / (qAbs(max - min));

    if (qAbs(max - min) == 0){
            scale = 255;
            qDebug() << "nnnnnnoooooooo";
    }

    //generate level greyscale colour table
    for(int i = 0; i < 65536; i++){                         //generate a greyscale. Added a few spare on top!
        int j = int((i - min) * scale);
        if (i < min){colourTable.append(qRgb(0,0,0));}    // make numbers below min blue
        if ((i >= min) && (i <= max)){colourTable.append(qRgb(j, j, j));}
        if (i > max){colourTable.append(qRgb(255,255,255));}    // make numbers above max red
    }

//    qDebug() << "grey colour table. min" << min << "max" << max << "number of elements" << colourTable.size();
    colourTableType = false;                                   // false is a grey colour table
    redraw = 1;
    emit newColourTable(colourTable);
}



void ReadImage::genLCIIColourTable(quint16 min, quint16 max)
{
    //generate a LCII colour table
    colourTable.clear();
    if (max < 15) {max = 15;}
    quint16 ranges = (int)(qAbs(max - min) / 7);    //DONT add 7 so that truncation to integer does ceiling function
    if (ranges > (65536/7)) {ranges = (65536/7);}
    float scale = 255.0/ranges;

    if (ranges == 0){               //This all needs a good cleanup !!!!!
        scale = 255;
        ranges = 1;
    }

//    qDebug() << "ranges" << ranges << "scale" << scale;

    for (int i = 0; i < qMin(min, max); i++){colourTable.append(qRgb(0,0,0));}                                      // make numbers below min black
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb((int)(i * scale), 0, (int)(i * scale)));}              //generate Black to Magenta
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb((int)((ranges - i) * scale), 0, 255));}                //generate Magenta to Blue
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb(0, (int)(i * scale), 255));}                           //generate Blue to Cyan
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb(0, 255, (int)((ranges - i) * scale)));}                //generate Cyan to Green
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb((int)(i * scale), (int)((ranges - i) * scale), 0));}   //generate Green to Red
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb(255, (int)(i * scale), 0));}                           //generate Red to Yellow
    for (int i = 0; i < ranges; i++){colourTable.append(qRgb(255, 255, (int)(i * scale)));}                         //generate Yellow to White
    for (int i = (qMin(min, max) + 7 * ranges); i < 65536; i++){colourTable.append(qRgb(255,255,255));}             // make numbers above max white

    qDebug() << "LCII colour table. min" << min << "max" << max << "number of elements" << colourTable.size();
    colourTableType = true;                                             // true is a LCII colour table
    redraw = 1;
    emit newColourTable(colourTable);
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

    getBinHeaderData();
    return true;
}



QStringList ReadImage::listIFiles(QFileInfo aFInfo)
{
    QDir aDir(aFInfo.absoluteDir());                                        //create a QDir path

    QStringList filter(QString("*.%1").arg(aFInfo.suffix()));               //filter on arfs or bins depending on what I selected in qml
    QStringList aFList = aDir.entryList(filter, QDir::Files, QDir::Name);   //list all arf files in current directory

//    qDebug() << aDir << aFList;
    return aFList;
}


void ReadImage::getBinHeaderData()
{
    // Check that the file size is greater than 0.
    // each frame has a line of header data. Image size will be constant.
    // get the image size details and calculate the number of frames
    // the header data is 64 bytes long regardless of actual resolution due 1st SPADs 32 x 32.
    qDebug() << "hello from getBinHeaderData";

    if (file.size() == 0){
     //   QMessageBox::warning(this, tr("Qt Arf/Bin player"),tr("File %1:\n contains has zero file size.").arg(iFileName));
        return;
    }

    QByteArray baHeader(64, 0);
    baHeader = file.read(64);      // read the header to a bytearray
    iWidth = baHeader.mid(4,2).toInt();
    iHeight = baHeader.mid(6,2).toInt();
    spadVersion = baHeader.mid(8,2).toInt();
    fileVersion = baHeader.mid(10,2).toInt();

//    numberOfFrames = int(file.size() / (iWidth * (iHeight + 1) * 2));
    numberOfFrames = 10;

    if (iWidth == 32) {
        imageType = "Gen 1 SPADs   ";
        magnification = 20;
     }

    if (iWidth == 128) {
        imageType = "Gen 2 SPADs   ";
        magnification = 5;
     }

    emit nFrames(numberOfFrames);           //set our number of frames to the widget

    filePos = file.pos();                   // get our current file position

//    resize(QSize((iWidth * magnification), (iHeight * magnification)));

    setAGCOn();
    redraw = 1;

    imageMin = 0;
    imageMax = quint16(4095 * pixelScale);

    startFrame = 1;                     //setup arfpos.qml values
    endFrame = numberOfFrames;
    currentFrame = 1;
    redraw = 1;                         // 1 redraws current frame

    playMode = true;
    emit changePlay(playMode);
    return;
}



int ReadImage::getBinImage(int rdrw, int frame)
{
    qDebug() << "hello from getbinimage";

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

        image = thisFrame.scaledToWidth(iWidth * magnification).mirrored(flipX, flipY);

//        if (exportBMPFlag) {
//            QString iName = QString("%1/%2_%3.bmp").arg(dirStr).arg(iFileName).arg(currentFrame, 4, 10, QChar('0'));
//            exportFile.setFileName(iName);

//            if (!exportFile.open(QIODevice::WriteOnly)){
//                qDebug() << "export file" << iName <<  "not open";
//            } else {
//                image.save(iName, "BMP", 100); // writes image into ba in PNG format
//            }
//            exportFile.close();
//            qDebug() << "Exported" << iName;
//        }

//        if (exportARFFlag) {
//            QDataStream out(&exportFile);
//            out.writeRawData(baImageR, (totalPixels) * 2);
//        }

//        repaint();

        emit newHistogram(histogram);
    }
    return rdrw;
}



void ReadImage::timerTimeout()
{
    qDebug() << "hello from timer";
    if (playMode){
        currentFrame++;
        redraw = 1;

//        if ((currentFrame - startFrame) == 100){setAGCOff();}

        if (currentFrame > endFrame){
            if (loopMode){
                currentFrame = startFrame;
            } else {
                playMode = false;
                redraw = 0;
            }

//            setAGCOff();

//            if (exportBMPFlag || exportARFFlag){
//                qDebug() << "Files Exported";

//                exportBMPFlag = false;
//                exportARFFlag = false;
//            }
        }
    }

    if (currentFrame > endFrame) {currentFrame = endFrame;}         //limit to current frames. QML can go past endstops and read
    if (currentFrame < startFrame) {currentFrame = startFrame;}     //memory after end of file. Good for a laugh but can crash.

    redraw = getBinImage(redraw, currentFrame);
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


    //qDebug() << "in paintevent";

//    QRect dirtyRect = event->rect();
//    painter.drawImage(dirtyRect, image, dirtyRect);

//    QFont font("Arial", 12);\
//    painter.setFont(font);
//    QFontMetrics fm(font);
//    quint32 pixelsWide = fm.width(pixStr);

//    quint32 pixelsHigh = fm.height();
//    quint32 xOff, yOff;

//    if (xPos * Magnification > (iWidth * Magnification) - pixelsWide - 20) {
//        xOff = xPos * Magnification - pixelsWide - 20;
//    } else {
//        xOff = (xPos * Magnification) + 20;
//    }

//    if (yPos * Magnification < pixelsHigh) {
//        yOff = yPos * Magnification + pixelsHigh ;
//    } else {
//        yOff = yPos * Magnification - 20;
//    }

//    // qDebug() << "fm" << pixelsWide << "iWidth" << iWidth << "xPos * mag" << xPos * Magnification;

//    painter.fillRect(xOff,yOff,pixelsWide,pixelsHigh, "white");
//    painter.setPen("black");
//    painter.scale(1.0,1.0);
//    painter.drawText(xOff, yOff + pixelsHigh/1.2, pixStr);
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
        case 2: magnification = 3;      //x3
            break;
        case 3: magnification = 4;      //x4
            break;
        case 4: magnification = 6;      //x6
            break;
        case 5: magnification = 8;      //x8
            break;
        case 6: magnification = 10;     //x10
            break;
        case 7: magnification = 12;     //x12
            break;
        case 8: magnification = 16;     //x16
            break;
        case 9: magnification = 20;     //x20
            break;
        case 10: magnification = 24;    //x24
            break;
        case 11: magnification = 32;    //x32
    }
}



void ReadImage::pixScl(QString tString)
{
    bool ok;
    pixelScale = tString.toFloat(&ok);

    if (!ok) {
        pixelScale = 1.0;
    }
}



void ReadImage::setAGCOn()
{
    AGC = true;
 //   recalcColourTable(imageMin, imageMax);
    ctMin = imageMin;
    ctMax = imageMax;
    //emit newColourTable(colourTable);
    emit agcState(true);
}



void ReadImage::setAGCOff()
{
    AGC = false;
    qDebug() << "AGC Off";
    emit agcState(false);
}



//void ReadImage::setFlipH(bool chkBox)
//{
//    if (chkBox){
//        flipX = true;
//    } else {
//        flipX = false;
//    }

//    qDebug() << "flipX" << flipX;
//    redraw = 1;
//}



//void ReadImage::setFlipV(bool chkBox)
//{
//    if (chkBox){
//        flipY = true;
//    } else {
//        flipY = false;
//    }

//    qDebug() << "flipY" << flipY;
//    redraw = 1;
//}



bool ReadImage::begin()
{
    playMode = false;           // paused
    currentFrame = startFrame;  // first frame
    redraw = 1;                 // paused
    return 1;
}



bool ReadImage::back()
{
    if(currentFrame > startFrame) {
        currentFrame -= 1;
    } else {
        currentFrame = startFrame;
    }

    playMode = false;
    redraw = 1;
    return 1;
}



bool ReadImage::play()
{
    playMode = true;

    redraw = 1;
    emit changePlay(playMode);
    return 1;
}


bool ReadImage::pause()
{
    playMode = false;

    redraw = 1;
    emit changePlay(playMode);
    return 1;
}


bool ReadImage::loop(bool pLoop)
{
    loopMode = pLoop;
    qDebug() << "loop" << loopMode;
    return 1;
}



bool ReadImage::forward()
{
    if(currentFrame < endFrame) {
        ++currentFrame;
    } else {
        currentFrame = endFrame;
    }

    playMode = false;
    redraw = 1;
    return 1;
}



bool ReadImage::end()
{
    currentFrame = endFrame;   // end frame
    playMode = false;          // paused
    redraw = 1;                // paused
    return 1;
}
