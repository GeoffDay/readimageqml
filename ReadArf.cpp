//to do: handle pixelScale in palette.qml and histogram
// add scroll bars to the main image window.
//

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

#include "ReadArf.h"


ReadArf::ReadArf(QWidget *parent): QWidget(parent)
{
    iWidth = 100;
    iHeight = 100;
    imageMin = 0;
    imageMax = 4095;
    ctMin = 0;
    ctMax = 65535;
    iRange = 65535;
    Magnification = 1;
    pixelScale = 1.0;
    flipX = false;
    flipY = false;
    currentFrame = 1;
    loopMode = true;
    playMode = false;
    exportARFFlag = false;
    exportBMPFlag = false;
    arfDirStr = QDir::currentPath();
    imageChangeRate = 80;                 //percentage change rate
    outlierThreshold = 100;               //an arbitrary ratio of good pixels to outliers per image
    genByteSwapTable();
    setMouseTracking(true);
    colTableType = true;
    genLCIIColourTable(0, 65535);

    QImage image(iWidth * Magnification, iHeight * Magnification, QImage::Format_RGB32);
    qApp->processEvents(QEventLoop::AllEvents);     // THIS KEEPS UI RESPONSIVE!!!!!!!!!!!!!!!!!!!!!!!!!!!
    image.fill(0xFF0606f0);
    QString pixStr = "";
    QFont font("times, 19");
    setFont(font);

    timeInterval = 16;      // default to 62.5fps

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    activateWindow();
    show();
}



QStringList ReadArf::listArfFiles(QFileInfo aFInfo)
{
    QDir aDir(aFInfo.absoluteDir());                            //create a QDir path

    QStringList filter("*.arf");                                //string list of extensions to search for
    QStringList aFList = aDir.entryList(filter, QDir::Files, QDir::Name);   //list all arf files in current directory

//    qDebug() << aDir << aFList;
    return aFList;
}



bool ReadArf::openArf()
{
    timer.stop();
    arfFileName = QFileDialog::getOpenFileName(this, tr("Open File"), arfDirStr, tr("ARF Image files (*.arf)"));

    file.setFileName(arfFileName);                                      //lets create a file
    QFileInfo arfFileInfo(arfFileName);                                 //lets get the file info

    arfDirStr = arfFileInfo.absolutePath();                             //separate the path string
    arfFileName = arfFileInfo.baseName();                               //now this is just the filename alone

    arfFileList = listArfFiles(arfFileInfo);
    qDebug() << arfDirStr << arfFileName << arfFileList;

    file.open(QIODevice::ReadOnly);         //Open the file for reading

    getHeaderData();
    return true;
}



bool ReadArf::prevArf()
{
    file.close();                                                       //close off the open file
    timer.stop();
    QFileInfo arfFileInfo(arfDirStr + "/" + arfFileName);               //recreate complete filename to get info again

    arfDirStr = arfFileInfo.absolutePath();                             //separate the path string
    arfFileName = arfFileInfo.baseName();                               //now this is just the filename alone

    arfFileList = listArfFiles(arfFileInfo);
    qDebug() << arfDirStr << arfFileName << arfFileList;

    int arfNumber = arfFileList.indexOf(arfFileName + ".arf", 0);

    if (arfNumber == 0){
        arfNumber = arfFileList.size();;
    }

    if (arfNumber > 0){
        arfFileName = arfFileList.at(arfNumber - 1);
        file.setFileName(arfDirStr + "/" + arfFileName);                                                  //lets create a file
        qDebug() << arfDirStr << arfFileName << arfNumber;
    }

    getHeaderData();
    return true;
}



bool ReadArf::nextArf()
{
    file.close();                                                       //close off the open file
    timer.stop();
    QFileInfo arfFileInfo(arfDirStr + "/" + arfFileName);               //recreate complete filename to get info again

    arfDirStr = arfFileInfo.absolutePath();                             //separate the path string
    arfFileName = arfFileInfo.baseName();                               //now this is just the filename alone

    arfFileList = listArfFiles(arfFileInfo);
    qDebug() << arfDirStr << arfFileName << arfFileList;

    int arfNumber = arfFileList.indexOf(arfFileName + ".arf", 0);

    if ((arfNumber + 1) == arfFileList.size()){
        arfNumber = -1;
    }

    if ((arfNumber + 1) < arfFileList.size()){
        arfFileName = arfFileList.at(arfNumber + 1);
        file.setFileName(arfDirStr + "/" + arfFileName);                                                  //lets create a file
        qDebug() << arfDirStr << arfFileName << arfNumber;
    }

    getHeaderData();
    return true;
}



bool ReadArf::getHeaderData()
{
    // Read and check the header. bytes 0 - 3 magic number 0xBBBBBAAD or 3,149,642,413, bytes 4 - 7 number of bytes / word 0x00000002 ?
    // bytes 8 - 11 number of Y pixels iHeight, bytes 12 - 15 number of x pixels iWidth, bytes 16 - 19 Image type 0x00000003 ?
    // bytes 20 - 23 total Arf frames. bytes 23 - 27 start of first image offset 0x00000020. bytes 28 - 31 unused 0x00000000

    file.open(QIODevice::ReadOnly);     //Open the file for reading
    quint32 nMagic, nBytes, nIType, nUnusd;
    QDataStream in(&file);              // file should be open already

    in >> nMagic >> nBytes >> iHeight >> iWidth >> nIType >> numberOfFrames >> beginPos >> nUnusd;
    qDebug() << nMagic << nBytes << iHeight << iWidth << nIType << numberOfFrames << beginPos << nUnusd;

    if ((nMagic != 0xBBBBBAAD) || (nBytes != 0x00000002) || (nIType != 0x00000003)){
        QMessageBox::warning(this, tr("Qt Arf player"),tr("File %1:\n contains bad Header Data.").arg(arfFileName));
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



    imageType += (arfDirStr  + "/" + arfFileName + " mag x" + QString::number(Magnification, 10));
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



int ReadArf::getImage(int rdrw, int frame)
{
    QByteArray baImage(iWidth * iHeight * 2, 0);
    QByteArray baImageR(iWidth * iHeight * 2, 0);   // this is a result array. we can write this to a file easily
    QImage thisFrame(iWidth, iHeight, QImage::Format_RGB32);

    quint16 colour, tColour, thisImageMin = 4095, thisImageMax = 0;
    quint32 thisImageAve = 0;

    if ((rdrw > 0) && file.isOpen()) {
        rdrw--;
        emit currFrame((int)(frame));     // shows us where we are in the file * sliderScale
        quint32 pixelPos = (xPos + (yPos * iWidth));    //calculate memory location of the pixel
        quint16 histValue;

        quint32 seekPos = (((iWidth * iHeight) + skipPixels) * 2 * (frame - 1)) + beginPos;
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
            thisImageAve = thisImageAve + tColour;

            if (pixelPos == i){
                double sColour = tColour * pixelScale;
                pixStr = QString(" %1, %2, %3 ").arg(xPos,0,10,QChar(' ')).arg(yPos,0,10,QChar(' ')).arg(sColour, 0, 'f', 1, QChar(' '));
            }
        }

        image = thisFrame.scaledToWidth(iWidth * Magnification).mirrored(flipX, flipY);

        if (exportBMPFlag) {
            QString iName = QString("%1/%2_%3.bmp").arg(arfDirStr).arg(arfFileName).arg(currentFrame, 4, 10, QChar('0'));
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
        histValue = 0;

        for (quint16 i = 0; i < iRange; i++){
            histValue += histogram[i];
            if (histValue > outlierThreshold){
                thisImageMin = i;
                break;
            }
        }

        histValue = 0;

        for (quint16 i = iRange; i > 0; i--){
            histValue += histogram[i];
            if (histValue > outlierThreshold){
                thisImageMax = i;
                break;
            }
        }

        imageMin = (quint16)(((imageMin * imageChangeRate) + ((100 - imageChangeRate) * thisImageMin)) / 100);
        imageMax = (quint16)(((imageMax * imageChangeRate) + ((100 - imageChangeRate) * thisImageMax)) / 100);

        if (AGC){recalcColourTable(imageMin, imageMax);}

        emit newHistogram(histogram);
    }
    return rdrw;
}



void ReadArf::exportARF()
{
    exportARFFlag = true;
    currentFrame = startFrame;

    exportFileName = arfFileName;
    exportFileName.replace(QString(".arf"), QString(""));

    QString iName = QString("%1/%2_%3-%4.arf").arg(arfDirStr).arg(exportFileName).arg(startFrame).arg(endFrame);
    exportFile.setFileName(iName);
    QDataStream out(&exportFile);
    out.setByteOrder(QDataStream::BigEndian);
    exportFile.open(QIODevice::WriteOnly);

    quint32 nMagic = 0xBBBBBAAD;                    // bytes 0 - 3 magic number
    quint32 nBytes = 0x00000002;                    // bytes 4 - 7 number of bytes / word ????
    quint32 nYPxls = iHeight;                     // bytes 8 - 11 number of Y pixels
    quint32 nXPxls = iWidth;                      // bytes 12 - 15 number of x pixels
    quint32 nIType = 0x00000003;                    // bytes 16 - 19 Image type ???????
    quint32 nFrams = endFrame - startFrame;         // bytes 20 - 23 total Arf frames.
    quint32 nOffst = 0x00000020;                    // bytes 23 - 27 start of first image offset
    quint32 nUnusd = 0x00000000;                    // bytes 28 - 31 unused

    if (imageType.contains("SPAD Histogram Raw", Qt::CaseInsensitive)) {
        nYPxls = 1;
        nXPxls = 1028;
    }

    out << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;
    qDebug() << iName << " is open." << "arf header data written";
    qDebug() << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;
}


void ReadArf::exportBMP()
{
    exportBMPFlag = true;
    currentFrame = startFrame;

    exportFileName = arfFileName;
    exportFileName.replace(QString(".arf"), QString(""));
}



void ReadArf::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timer.timerId()) {
        if (playMode){
            currentFrame++;
            redraw = 1;

            if ((currentFrame - startFrame) == 100){setAGCOff();}

            if (currentFrame > endFrame){
                if (loopMode){
                    currentFrame = startFrame;
                } else {
                    playMode = false;
                    redraw = 0;
                }

                setAGCOff();

                if (exportBMPFlag || exportARFFlag){
                    qDebug() << "Files Exported";
                    exportBMPFlag = false;
                    exportARFFlag = false;
                }
            }
        }

        if (currentFrame > endFrame) {currentFrame = endFrame;}         //limit to current frames. QML can go past endstops and read
        if (currentFrame < startFrame) {currentFrame = startFrame;}     //memory after end of file. Good for a laugh but can crash.

        redraw = getImage(redraw, currentFrame);
    } else {
        QWidget::timerEvent(event);
    }
}


void ReadArf::setImageNumber(int newFrame)
{
    currentFrame = (int)(newFrame / sliderScale);
    redraw = 1;

    if (currentFrame > endFrame){
        if (loopMode){
            currentFrame = startFrame;
        } else {
            playMode = false;
            redraw = 0;
        }
    }
}


void ReadArf::getStartFrame(int mFrames)
{
//    qDebug() << "start frame from qml" << startFrame;
    startFrame = mFrames;
}


void ReadArf::getCurrentFrame(int mFrames)
{
    currentFrame = mFrames;
    redraw = 1;
//    qDebug() << "current frame from qml" << currentFrame;
}


void ReadArf::getEndFrame(int mFrames)
{
    endFrame = mFrames;
//    qDebug() << "end frame from qml" << endFrame;
}


void ReadArf::setFrameRate(int fps)
{
    timer.stop();

    switch (fps) {
        case 0: timeInterval = 16;      //62.5fps
            break;
        case 1: timeInterval = 20;      //50fps
            break;
        case 2: timeInterval = 40;      //25fps
            break;
        case 3: timeInterval = 80;      //12.5fps
            break;
        case 4: timeInterval = 125;     //8fps
            break;
        case 5: timeInterval = 250;     //4fps
            break;
        case 6: timeInterval = 500;     //2fps
            break;
        case 7: timeInterval = 1000;    //1fps
    }

    timer.start(timeInterval, this);
}



void ReadArf::setImageSize(int iMag)
{
//    qDebug() << "in setImageSize now" << iMag;
    switch (iMag) {
        case 0: Magnification = 1;      //x1
            break;
        case 1: Magnification = 2;      //x2
            break;
        case 2: Magnification = 3;      //x4
            break;
        case 3: Magnification = 4;      //x8
            break;
        case 4: Magnification = 6;     //x16
            break;
        case 5: Magnification = 8;     //x32
            break;
        case 6: Magnification = 10;      //x1
            break;
        case 7: Magnification = 12;      //x2
            break;
        case 8: Magnification = 16;      //x4
            break;
        case 9: Magnification = 20;      //x8
            break;
        case 10: Magnification = 24;     //x16
            break;
        case 11: Magnification = 32;     //x32
    }
    imageType += (arfDirStr  + "/" + arfFileName + " x" + QString::number(Magnification, 10));
    resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
    redraw = 1;
    update();
}



void ReadArf::genByteSwapTable()
{
    byteSwapTable.clear();

    //generate byte swap lookup table
    for(int i = 0; i < 65536; i++){
        byteSwapTable.append(quint16(((i & 0xff00) >> 8) + ((i & 0x00ff) << 8)));
    }

//    qDebug() << "generated byte swap lookup table";
}



void ReadArf::setCTType(bool cTType)
{
    colTableType = cTType;
    recalcColourTable(ctMin, ctMax);
}



void ReadArf::getMin(int Value)
{
    ctMin = Value;
    recalcColourTable(ctMin, ctMax);
}



void ReadArf::getMax(int Value)
{
    ctMax = Value;
    recalcColourTable(ctMin, ctMax);
}



void ReadArf::recalcColourTable(quint16 min, quint16 max)
{
    if (min >= max) {        //this is an error
        min = 0;
        max = iRange;
    }

    while (max - min < 15) {
        if (min > 0) { min -= 1;}
        if (max < iRange) {max += 1;}
    }

    if (colTableType) {
        genLCIIColourTable(min, max);
    } else {
        genGreyColourTable(min, max);
    }
    emit newImValLimits(QPoint(min, max));
}



void ReadArf::genGreyColourTable(quint16 min, quint16 max)
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
    colTableType = false;                                   // false is a grey colour table
    redraw = 1;
    emit newColourTable(colourTable);
}



void ReadArf::genLCIIColourTable(quint16 min, quint16 max)
{
    //generate a LCII colour table
    colourTable.clear();
    if (max < 15) {max = 15;}
    quint16 ranges = (int)(qAbs(max - min) / 7);    //DONT add 7 so that truncation to integer does ceiling function
    if (ranges > (65536/7)) {ranges = (65536/7);}
    float scale = 255.0/ranges;

    if (ranges == 0){
        scale = 255;
        ranges = 1;
    }

//    qDebug() << "ranges" << ranges << "scale" << scale;

    for(int i = 0; i < qMin(min, max); i++){colourTable.append(qRgb(0,0,0));}  // make numbers below min black

    for(int i = 0; i < ranges; i++){                                //generate Black to Magenta
        colourTable.append(qRgb((int)(i * scale), 0, (int)(i * scale)));
    }

    for (int i = 0; i < ranges; i++){                               //generate Magenta to Blue
        colourTable.append(qRgb((int)((ranges - i) * scale), 0, 255));
    }

    for (int i = 0; i < ranges; i++){                               //generate Blue to Cyan
        colourTable.append(qRgb(0, (int)(i * scale), 255));
    }

    for (int i = 0; i < ranges; i++){                               //generate Cyan to Green
        colourTable.append(qRgb(0, 255, (int)((ranges - i) * scale)));
    }

    for (int i = 0; i < ranges; i++){                               //generate Green to Red
        colourTable.append(qRgb((int)(i * scale), (int)((ranges - i) * scale), 0));
    }

    for (int i = 0; i < ranges; i++){                               //generate Red to Yellow
        colourTable.append(qRgb(255, (int)(i * scale), 0));
    }

    for (int i = 0; i < ranges; i++){                               //generate Yellow to White
        colourTable.append(qRgb(255, 255, (int)(i * scale)));
    }

    for(int i = (qMin(min, max) + 7 * ranges); i < 65536; i++){
        colourTable.append(qRgb(255,255,255));  // make numbers above max white
    }

//    qDebug() << "LCII colour table. min" << min << "max" << max << "number of elements" << colourTable.size();
    colTableType = true;                                             // true is a LCII colour table
    redraw = 1;
    emit newColourTable(colourTable);
}



void ReadArf::pixScl(QString tString)
{
    bool ok;
    pixelScale = tString.toFloat(&ok);

    if (!ok) {
        pixelScale = 1.0;
    }
}



void ReadArf::setAGCOn()
{
    AGC = true;
 //   recalcColourTable(imageMin, imageMax);
    ctMin = imageMin;
    ctMax = imageMax;
    //emit newColourTable(colourTable);
    emit agcState(true);
}



void ReadArf::setAGCOff()
{
    AGC = false;
    qDebug() << "AGC Off";
    emit agcState(false);
}



void ReadArf::setFlipH(bool chkBox)
{
    if (chkBox){
        flipX = true;
    } else {
        flipX = false;
    }

    qDebug() << "flipX" << flipX;
    redraw = 1;
}



void ReadArf::setFlipV(bool chkBox)
{
    if (chkBox){
        flipY = true;
    } else {
        flipY = false;
    }

    qDebug() << "flipY" << flipY;
    redraw = 1;
}



bool ReadArf::begin()
{
    playMode = false;           // paused
    currentFrame = startFrame;  // first frame
    redraw = 1;                 // paused
    return 1;
}



bool ReadArf::back()
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



bool ReadArf::play()
{
    if (playMode){
        playMode = false;
    } else {
        playMode = true;
    }

    redraw = 1;
    emit changePlay(playMode);
    return 1;
}



bool ReadArf::loop(bool pLoop)
{
    loopMode = pLoop;
    return 1;
}



bool ReadArf::forward()
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



bool ReadArf::end()
{
    currentFrame = endFrame;   // end frame
    playMode = false;          // paused
    redraw = 1;                // paused
    return 1;
}



void ReadArf::zoomIn()
{
    if (Magnification < 32) {Magnification++;}

    imageType = (arfDirStr  + "/" + arfFileName + " x" + QString::number(Magnification, 10));
    emit wTitle(imageType);
    resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
    redraw = 1;
    update();
}



void ReadArf::zoomOut()
{
   if (Magnification > 1) {Magnification--;}

   imageType = (arfDirStr  + "/" + arfFileName + " x" + QString::number(Magnification, 10));
   emit wTitle(imageType);
   resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
   redraw = 1;
   update();
}


void ReadArf::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;

    if (event->buttons() == Qt::RightButton) {      //hold down right mouse button to change size

        if((numSteps > 0) && (Magnification < 32)) {Magnification++;}
        if((numSteps < 0) && (Magnification > 1)) {Magnification--;}

        imageType = (arfDirStr  + "/" + arfFileName + " x" + QString::number(Magnification, 10));
        emit wTitle(imageType);
        resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
    } else {                                        //otherwise stop playing and allow mousewheel to control frames
        playMode = false;
        emit changePlay(playMode);

        if (numSteps > 0) {++currentFrame;}
        if (numSteps < 0) {--currentFrame;}
    }

    redraw = 1;
    update();
}



void ReadArf::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        play();
    }
}



void ReadArf::mouseMoveEvent(QMouseEvent *event)
{
    xPos = (event->pos().x() / Magnification);
    yPos = (event->pos().y() / Magnification);
    redraw = 1;
}



void ReadArf::paintEvent(QPaintEvent *event)
{
    //qDebug() << "in paintevent";
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);

    QFont font("Arial", 12);\
    painter.setFont(font);
    QFontMetrics fm(font);
    quint32 pixelsWide = fm.width(pixStr);

    quint32 pixelsHigh = fm.height();
    quint32 xOff, yOff;

    if (xPos * Magnification > (iWidth * Magnification) - pixelsWide - 20) {
        xOff = xPos * Magnification - pixelsWide - 20;
    } else {
        xOff = (xPos * Magnification) + 20;
    }

    if (yPos * Magnification < pixelsHigh) {
        yOff = yPos * Magnification + pixelsHigh ;
    } else {
        yOff = yPos * Magnification - 20;
    }

   // qDebug() << "fm" << pixelsWide << "iWidth" << iWidth << "xPos * mag" << xPos * Magnification;

    painter.fillRect(xOff,yOff,pixelsWide,pixelsHigh, "white");
    painter.setPen("black");
    painter.scale(1.0,1.0);
    painter.drawText(xOff, yOff + pixelsHigh/1.2, pixStr);
}



void ReadArf::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    setGeometry(10,30,newSize.width(), newSize.height());

    QImage newImage(newSize, QImage::Format_RGB32);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
    qDebug() << "resize Image" << newSize.width() << " x " << newSize.height();
}



void ReadArf::resizeEvent(QResizeEvent *event)
{
    resizeImage(&image, QSize(iWidth * Magnification, iHeight * Magnification));
    update();

    QWidget::resizeEvent(event);
}





//// display & record every alternate images to cut bandwidth requirements and stop missing frames
//if (recordingFlag){
//    if (lImageCountVal % 2 == 0){
//        QDataStream out(&file);
//        out.writeRawData(reinterpret_cast<const char *>(lBuffer->GetDataPointer()), IMGMEM);
//        framesRecorded++;
//    } else {
//        emit imageBuffer(reinterpret_cast<quint16 *>(lBuffer->GetDataPointer()));   //IR image pointer
//        emit spadBuffer(reinterpret_c

//bool PleoraIface::startRecording()
//{
//    QString iName = QString("%1/%2%3.arf").arg(arfDir).arg(arfFileName).arg(arfFileNum, 4, 10, QChar('0'));
//    qDebug() << iName << "at start recording.";
//    file.setFileName(iName);
//    QDataStream out(&file);
//    out.setByteOrder(QDataStream::BigEndian);
//    file.open(QIODevice::WriteOnly);

//    quint32 nMagic = 0xBBBBBAAD;                    // bytes 0 - 3 magic number
//    quint32 nBytes = 0x00000002;                    // bytes 4 - 7 number of bytes / word ????
//    quint32 nYPxls = IMGHEIGHT;                     // bytes 8 - 11 number of Y pixels
//    quint32 nXPxls = IMGWIDTH;                      // bytes 12 - 15 number of x pixels
//    quint32 nIType = 0x00000003;                    // bytes 16 - 19 Image type ???????
//    quint32 nFrams = 0x00000001;                    // bytes 20 - 23 total Arf frames. updated when stopped
//    quint32 nOffst = 0x00000020;                    // bytes 23 - 27 start of first image offset
//    quint32 nUnusd = 0x00000000;                    // bytes 28 - 31 unused

//    out << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;
//    qDebug() << iName << " is open." << "arf header data written" << out.byteOrder();
//    qDebug() << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;

//    framesRecorded = 0;
//    recordingFlag = true;
//    qDebug() << "Recording";
//    emit RecordingArf(arfFileNum);
//    return recordingFlag;
//}



//bool PleoraIface::stopRecording()
//{
//    QString iName = QString("%1/%2%3.arf").arg(arfDir).arg(arfFileName).arg(arfFileNum, 4, 10, QChar('0'));

//    if (file.isOpen())
//    {
//        file.seek(20);
//        QByteArray ba(2,0);
//        QDataStream stream(&ba, QIODevice::WriteOnly); //default big endian
//        stream << framesRecorded;
//        file.write(ba);
//        qDebug() << "wrote" << framesRecorded << "frames to" << iName << "and closed it.";
//        file.close();
//    } else {
//        qDebug() << "File " << iName << "was already closed. Thats a cockup!\n";
//    }

//    recordingFlag = false;
//    qDebug() << "Stopped Recording";

//    emit RecordingArf(arfFileNum * -1);
//    return recordingFlag;
//}
