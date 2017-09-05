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
#include <QStringBuilder>

#include "readimage.h"
#include <dataobject.h>

#define IRANGE 4095                         // so far I'm assuming 12 bits for princeton and SPAD so far. Arf may be higher....

ReadImage::ReadImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
       dirStr = QDir::currentPath();

       iWidth = 32;
       iHeight = 32;
       imageMin = 0;
       imageMax = 256;
       magnification = 11;
       pixelScale = 1.0;
       flipX = false;
       flipY = false;

       startFrame = 17;
       currentFrame = 1;
       endFrame = 123;
       numberOfFrames = 125;
       outlierThreshold = 5;
       loopMode = true;
       playMode = false;
//       exportARFFlag = false;
//       exportBMPFlag = false;
       dirStr = QDir::currentPath();
       colourTableType = true;
       genLCIIColourTable(0, 65535);


       qApp->processEvents(QEventLoop::AllEvents);     // THIS KEEPS UI RESPONSIVE!!!!!!!!!!!!!!!!!!!!!!!!!!!

       QString pixStr = "";
}


//void ReadImage::setModel(QString m) {
//  m_model = m.split(" ");
//  modelChanged();
//}



void ReadImage::setModel(QStringList m) {
    m_model = m;
    modelChanged();
}

void ReadImage::setFastModel(QStringList m) {
    n_model = m;
    fastModelChanged();
}



QStringList ReadImage::getModel() {
    return m_model;
}

QStringList ReadImage::getFastModel() {
    return n_model;
}

void ReadImage::setCTType(bool cTType)
{
    colourTableType = cTType;
    recalcColourTable(ctMin, ctMax);
    redraw = 1;
    timerTimeout();
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

    emit ctMinChanged(ctMin);
    emit ctMaxChanged(ctMax);

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

    qDebug() << "LCII min" << min << "max" << max ;
    colourTableType = true;                                             // true is a LCII colour table
    redraw = 1;
    emit newColourTable(colourTable);
}



QStringList ReadImage::listIFiles(QString aFileName)
{
    qDebug() << aFileName;
    QFileInfo iFileInfo(aFileName);                             //lets get the file info
    qDebug() << iFileInfo.makeAbsolute();

    dirStr = iFileInfo.absolutePath();                          //separate the path string
    iFileName = iFileInfo.baseName();                           //now this is just the filename alone
    iExt = iFileInfo.suffix();                                  //and the extension

    QDir iDir(iFileInfo.absoluteDir());                         //create a QDir path
    iDir.setFilter(QDir::Files | QDir::NoSymLinks);
    iDir.setSorting(QDir::Name | QDir::Reversed);

    QStringList filter(QString("*.%1").arg(iExt));              //filter on arfs or bins depending on what I selected in qml
    QStringList iFList = iDir.entryList(filter, QDir::Files, QDir::Name);   //list all arf files in current directory

    qDebug() << filter << dirStr << iFileName << iExt << iFileList;
    return iFList;
}



bool ReadImage::openIFileName(QString tFileName)

{
#ifdef Q_OS_UNIX
    tFileName.remove(0,7);                                      // qml returns a URL with File:/// prefixed before c:/.... so remove it
#else
    tFileName.remove(0,8);                                      // qml returns a URL with File:/// prefixed before c:/.... so remove it
#endif
    qDebug() << tFileName;

    iFileList = listIFiles(tFileName);                          //return a list of similar files in that Dir
    iFileList = listIFiles(tFileName);                          //doing this twice gets the list properly

    file.setFileName(tFileName);                                //lets create a file

    getBinHeaderData();
    return true;
}



bool ReadImage::prevIFile()
{
    file.close();                                           //close off the open file
    iFileList = listIFiles(file.fileName());                      //what if the dir is added too

    int iNumber = iFileList.indexOf(iFileName % "." % iExt, 0);          //get the index of the last file

    if (iNumber == 0){                                      //if we're at zero go back to the end
        iNumber = iFileList.size();
    }

    if (iNumber > 0){
        iFileName = iFileList.at(iNumber - 1);
        file.setFileName(dirStr % "/" % iFileName);  //lets create a file
    }

    qDebug() << dirStr << iFileName << iNumber;
    getBinHeaderData();
    return true;
}



bool ReadImage::nextIFile()
{
    file.close();                                           //close off the open file
    iFileList = listIFiles(file.fileName());                      //what if the dir is added too

    int iNumber = iFileList.indexOf(iFileName % "." % iExt, 0);

    if ((iNumber + 1) == iFileList.size()){                 //if we're at the end go back to zero
        iNumber = -1;
    }

    if ((iNumber + 1) < iFileList.size()){
        iFileName = iFileList.at(iNumber + 1);
        file.setFileName(dirStr + "/" + iFileName);                                                  //lets create a file
    }

    qDebug() << dirStr << iFileName << iNumber;
    getBinHeaderData();
    return true;
}




void ReadImage::getBinHeaderData()
{
    // Check that the file size is greater than 0.
    // each frame has a line of header data. Image size will be constant.
    // get the image size details and calculate the number of frames
    // the header data is 64 bytes long regardless of actual resolution due 1st SPADs 32 x 32.

    file.open(QIODevice::ReadOnly);                             //Open the file for reading

    if (file.size() == 0){
       qDebug() << "File %1: has zero file size.";
        return;
    }

    QByteArray baHeader(64, 0);
    baHeader = file.read(64);      // read the header to a bytearray
    quint16 *headerPtr = reinterpret_cast<quint16 *>(baHeader.data());   // create a pointer to that bytearray and


    if (*headerPtr == 0x55AA) { // col 1, bytes 0, 1 magic number 0xAA55 Endianess is reversed
        imageType = "Princeton SPAD";
        iWidth = 32;
        iHeight = 32;
        magnification = 20;
    } else {
        fileVersion = quint32(*headerPtr + 2);
        spadVersion = quint32(*headerPtr + 4);
        iWidth = quint32(*(headerPtr + 18));
        iHeight = quint32(*(headerPtr + 19));

        if (iWidth == 32) {
            imageType = "Gen 1 SPADs   ";
            magnification = 20;
         }

        if (iWidth == 64) {
            imageType = "Processed Gen 1 SPADs   ";
            magnification = 10;
        }

        if (iWidth == 128) {
            imageType = "Gen 2 SPADs   ";
            magnification = 5;
         }
    }

    metaData.clear();
    metaData.append("Dir: " + dirStr);
    metaData.append("File : " + iFileName);
    metaData.append("Image Type: " + imageType);
    metaData.append("File Version: " + QString::number(fileVersion, 10));
    metaData.append("SPAD Version: " + QString::number(spadVersion, 10));
    metaData.append("Resolution: " + QString::number(iWidth, 10) + " x " + QString::number(iHeight, 10));

    setModel(metaData);

    fNameandImagetype = QString("%1/%2  %3").arg(dirStr).arg(iFileName).arg(imageType);
    emit fNameInfoChanged(fNameandImagetype);

    QImage image(iWidth * magnification, iHeight * magnification, QImage::Format_RGB32);
    image.fill(0xFF0606f0);

    numberOfFrames = int(file.size() / (iWidth * (iHeight + 1) * 2));
    emit nFramesChanged(numberOfFrames);        //set our number of frames to the widget

    qDebug() << "imageType " << imageType << spadVersion << iWidth << iHeight << numberOfFrames;

    totalPixels = iWidth * iHeight;

    filePos = file.pos();                       // get our current file position
//    resize(QSize((iWidth * magnification), (iHeight * magnification)));

    setAGCOn();
    redraw = 1;

    imageMin = 0;
    imageMax = quint16(4095 * pixelScale);

    startFrame = 1;                     //setup arfpos.qml values
    emit startFrameChanged(startFrame);

    endFrame = numberOfFrames;
    emit endFrameChanged(endFrame);

    currentFrame = 1;
    emit currentFrameChanged(currentFrame);

    redraw = 1;                         // 1 redraws current frame

    playMode = true;
    emit playModeChanged(playMode);
    return;
}



int ReadImage::getBinImage(int rdrw, int frame)
{
    // image min and max are stored in the 64 byte frame header
    QByteArray baImage(iWidth * iHeight * 2, 0);
    QByteArray baImageR(iWidth * iHeight * 2, 0);   // this is a result array. we can write this to a file easily
    QByteArray baHeader(iWidth * 2, 0);

    QImage thisFrame(iWidth, iHeight, QImage::Format_RGB32);

    quint16 colour, tColour, thisImageMin = 0, thisImageMax = 0;

    if ((rdrw > 0) && file.isOpen()) {
        rdrw--;
        emit currentFrameChanged((quint32)(frame));     // shows us where we are in the file
        quint32 pixelPos = (xPos + (yPos * iWidth));    //calculate memory location of the pixel
        quint16 histValue;

        quint32 seekPos = (((iWidth * iHeight) + iWidth) * 2 * (frame - 1));
        file.seek(seekPos);
        baHeader = file.read(iWidth * 2);
        baImage = file.read(iWidth * iHeight * 2);      // read 1 frame to a bytearray
        baImageR = baImage;
        quint16 *imagePtr = reinterpret_cast<quint16 *>(baImage.data());   // create a pointer to that bytearray and
        quint32 *thsFrmePtr = reinterpret_cast<quint32 *>(thisFrame.bits());
        histogram.fill(0,65536);


        for (quint32 i = 0; i < totalPixels; i++, imagePtr++, thsFrmePtr++){  //iterate image pointer through memory
        //    colour = byteSwapTable.at(*imagePtr);
            colour = *imagePtr;
            *thsFrmePtr = colourTable.at(colour);
            tColour = colour;
//            *thsFrmePtr = *imagePtr;
            histValue = histogram[tColour];
            histogram[tColour] = histValue + 1; //since we're trawling through the image do the histogram

            if (pixelPos == i){
                double sColour = tColour * pixelScale;
                pixStr = QString(" %1, %2, %3 ").arg(xPos,0,10,QChar(' ')).arg(yPos,0,10,QChar(' ')).arg(sColour, 0, 'f', 1, QChar(' '));
                fastMetaData.append(pixStr);
                setFastModel(fastMetaData);
                fastMetaData.clear();
            }
        }

//        qDebug() << "hello from getbinimage" << magnification << iWidth;

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


        update();
        histValue = 0;

        for (quint16 i = 0; i < 4096; i++){
            histValue += histogram[i];
            if (histValue > outlierThreshold){
                thisImageMin = i;
                break;
            }
        }

        histValue = 0;

        for (quint16 i = 4096; i > 0; i--){
            histValue += histogram[i];
            if (histValue > outlierThreshold){
                thisImageMax = i;
                break;
            }
        }

        imageMin = (quint16)(((imageMin * 97) + (3 * thisImageMin)) / 100);
        imageMax = (quint16)(((imageMax * 97) + (3 * thisImageMax)) / 100);


        if (AGC && (abs(quint16(imageMin - ctMin)) > 2)){
            setCTMin(imageMin);
            emit ctMinChanged(imageMin);
//            recalcColourTable(ctMin, ctMax);
        }

        if (AGC && (abs(quint16(imageMax - ctMax)) > 2)){
            setCTMax(imageMax);
            emit ctMaxChanged(imageMax);}
//            recalcColourTable(ctMin, ctMax);
        }

        emit newHistogram(histogram);
    return rdrw;
}



void ReadImage::timerTimeout()
{
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


void ReadImage::setMousePos(int mX, int mY)
{
    xPos = mX / magnification;
    yPos = mY / magnification;
    qDebug() << "xy" << xPos << yPos;
    redraw = 1;
    timerTimeout();
}



QString ReadImage::IFileName() const
{
    return iFileName;
}


void ReadImage::paint(QPainter *painter)
{
    QRect dirtyRect = QRect(0,0,iWidth * magnification,(iHeight + 0) * magnification);
    painter->drawImage(dirtyRect, image);

//    qDebug() << "in paintevent" << iWidth << iHeight << magnification;
}


quint32 ReadImage::getMagnification() const
{
    return magnification;
}


void ReadImage::setMagnification(int iMag)
{
    magnification = iMag;
    redraw = 1;
    timerTimeout();
}



void ReadImage::pixScl(QString tString)
{
    bool ok;
    pixelScale = tString.toFloat(&ok);

    if (!ok) {
        pixelScale = 1.0;
    }
}


QString ReadImage::fName()
{
    return fNameandImagetype;
}

quint32 ReadImage::nFrames()
{
    return numberOfFrames;
}

quint32 ReadImage::sFrame()
{
    return startFrame;
}

quint32 ReadImage::eFrame()
{
    return endFrame;
}

quint32 ReadImage::cFrame()
{
    return currentFrame;
}

void ReadImage::setCurrentFrame(int tCFrame)
{
    currentFrame = tCFrame;
    if (!playMode) {
        redraw = 1;                 // only do this if paused
        timerTimeout();
    }
}

void ReadImage::setStartFrame(int tSFrame)
{
//    qDebug() << "hello from set start frame" << startFrame;
    startFrame = tSFrame;
}

void ReadImage::setEndFrame(int tEFrame)
{
//    qDebug() << "hello from set end frame" << endFrame;
    endFrame = tEFrame;
}


void ReadImage::setPlayMode(bool tPMode)
{
    qDebug() << "hello from setPlayMode" << playMode;
    playMode = tPMode;
}


bool ReadImage::getPlayMode()
{
    return playMode;
}

void ReadImage::setCTMin(quint32 tCTMin)
{
    ctMin = tCTMin;
    recalcColourTable(ctMin, ctMax);
    redraw = 1;                 // only do this if paused
    timerTimeout();
}

void ReadImage::setCTMax(quint32 tCTMax)
{
    ctMax = tCTMax;
    recalcColourTable(ctMin, ctMax);
    redraw = 1;                 // only do this if paused
    timerTimeout();
}

quint32 ReadImage::getCTMin()
{
    return ctMin;
}

quint32 ReadImage::getCTMax()
{
    return ctMax;
}

QStringList ReadImage::getMetaData(){
    return metaData;
}


//void ReadImage::setMetaData(QByteArray tHeader)
//{

//}


void ReadImage::setAGCOn()
{
    AGC = true;
    recalcColourTable(imageMin, imageMax);
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



void ReadImage::begin()
{
    playMode = false;           // paused
    emit playModeChanged(playMode);
    currentFrame = startFrame;  // first frame
    redraw = 1;                 // paused
    timerTimeout();
}



void ReadImage::back()
{
    if(currentFrame > startFrame) {
        currentFrame -= 1;
    } else {
        currentFrame = startFrame;
    }

    playMode = false;
    emit playModeChanged(playMode);
    redraw = 1;
    timerTimeout();
}



void ReadImage::play()
{
    playMode = true;

    redraw = 1;
    emit playModeChanged(playMode);
    timerTimeout();
}


void ReadImage::pause()
{
    playMode = false;

    redraw = 1;
    emit playModeChanged(playMode);
    timerTimeout();
}


void ReadImage::loop(bool pLoop)
{
    loopMode = pLoop;
    qDebug() << "loop" << loopMode;
    timerTimeout();
}



void ReadImage::forward()
{
    if(currentFrame < endFrame) {
        ++currentFrame;
    } else {
        currentFrame = endFrame;
    }

    playMode = false;
    emit playModeChanged(playMode);
    redraw = 1;
    timerTimeout();
}



void ReadImage::end()
{
    currentFrame = endFrame;   // end frame
    playMode = false;          // paused
    emit playModeChanged(playMode);
    redraw = 1;                // paused
    timerTimeout();
}

