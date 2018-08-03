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

#define SPEEDOFLIGHT  300                   // used to calc distance with SPAD PLL value

ReadImage::ReadImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
       dirStr = QDir::currentPath();
       iWidth = 32;
       iHeight = 32;
       imageMin = 0;
       imageMax = 256;
       iRange = 4095;
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
       exportFlag = 0;                                  // no export in progress
       dirStr = QDir::currentPath();
       colourTableType = true;
       genLCIIColourTable(0, 65535);
       genByteSwapTable();

       HasFrameHeaders = true;                          // assume we are usually working with our data which has a header per frame.
       qApp->processEvents(QEventLoop::AllEvents);      // THIS KEEPS UI RESPONSIVE!!!!!!!!!!!!!!!!!!!!!!!!!!!

       QString pixStr = "";
}



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



void ReadImage::genByteSwapTable()
{
    byteSwapTable.clear();

    //generate byte swap lookup table
    for(int i = 0; i < 65536; i++){
        byteSwapTable.append(quint16(((i & 0xff00) >> 8) + ((i & 0x00ff) << 8)));
    }
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
        max = iRange;
    }

    while (max - min < 15) {        // if too close together push the min and max apart
        if (min > 0) { min -= 1;}
        if (max < iRange) {max += 1;}
    }

    emit ctMinChanged();
    emit ctMaxChanged();

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
//    colourTableChanged();
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
//    colourTableChanged();
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

    if (iExt == "bin"){
        getBinHeaderData();                                     //bin files have a frame by frame header
    } else {
        getArfHeaderData();                                     //arf files only have a X & Y resolution and number of frames in the first 32 bytes
    }

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

    if (iExt == "bin"){
        getBinHeaderData();                                     //bin files have a frame by frame header
    } else {
        getArfHeaderData();                                     //arf files only have a X & Y resolution and number of frames in the first 32 bytes
    }
    return true;
}



bool ReadImage::nextIFile()
{
    file.close();                                               //close off the open file
    iFileList = listIFiles(file.fileName());                    //what if the dir is added too

    int iNumber = iFileList.indexOf(iFileName % "." % iExt, 0);

    if ((iNumber + 1) == iFileList.size()){                     //if we're at the end go back to zero
        iNumber = -1;
    }

    if ((iNumber + 1) < iFileList.size()){
        iFileName = iFileList.at(iNumber + 1);
        file.setFileName(dirStr + "/" + iFileName);                                                  //lets create a file
    }

    qDebug() << dirStr << iFileName << iExt << iNumber;

    if (iExt == "bin"){
        getBinHeaderData();                                     //bin files have a frame by frame header
    } else {
        getArfHeaderData();                                     //arf files only have a X & Y resolution and number of frames in the first 32 bytes
    }
    return true;
}




void ReadImage::getArfHeaderData()
{
    // Check that the file size is greater than 0.
    // get the image size details and the number of frames
    // the header data is 32 bytes long.

    file.open(QIODevice::ReadOnly);                             //Open the file for reading

    if (file.size() == 0){
       qDebug() << "File" << iFileName << "has zero file size.";
       return;
    }

//    QByteArray baHeader(32, 0);
//    baHeader = file.read(32);                                           // read the header to a bytearray
//    quint32 *headerPtr = reinterpret_cast<quint32 *>(baHeader.data());  // create a pointer to that bytearray and

//    if (*headerPtr++ == 0xBBBBADBA){                // a magic number to denote ARF files.
//        headerPtr++;                                // we've only ever used 2 byte data in arf
//        iHeight = *headerPtr++;                     // Y res
//        iWidth = *headerPtr++;                      // X res
//        headerPtr++;                                // image type - always 3
//        numberOfFrames = *headerPtr++;              // number of frames
//        fileStartOffset = *headerPtr++;             // get our offset for start of data
//        qDebug() << iFileName << "loaded";
//    } else {
//        qDebug() << "File" << iFileName << "has bad magic number.";
//        return;
//    }

    quint32 nMagic, nBytes, nIType, nUnusd;
    QDataStream in(&file);              // file should be open already

    in >> nMagic >> nBytes >> iHeight >> iWidth >> nIType >> numberOfFrames >> fileStartOffset >> nUnusd;
    qDebug() << nMagic << nBytes << iHeight << iWidth << nIType << numberOfFrames << fileStartOffset << nUnusd;

    if ((nMagic != 0xBBBBBAAD) || (nBytes != 0x00000002) || (nIType != 0x00000003)){
        qDebug() << "File" << iFileName << "contains bad Header Data.";
        return;
    }


    imageType = "unknown";
    magnification = 2;
    skipPixels = 0;
    iRange = 4095;

    if (iWidth == 32) {
        imageType = "SPAD";
        magnification = 16;
        outlierThreshold = 3;       // this is 3 pixels
    }

    if (iWidth == 64) {
        imageType = "SPAD Histogram";   //4,16,64 or 256 images taken in quick succession with a high speed laser
        magnification = 8;
        outlierThreshold = 3;
        iRange = 32767;
    }

    if (iWidth == 256) {
        imageType = "Amber";
        magnification = 2;
        outlierThreshold = 512;     // this 2 lines
    }

    if (iWidth == 384) {
        imageType = "Otherseeker";
        magnification = 2;
        outlierThreshold = 385;     // this is 1 line
    }
    if (iWidth == 576) {
        imageType = "LWRTS";
        magnification = 1;
        outlierThreshold = 600;
        iRange = 32767;
    }

    if (iWidth == 640) {
        imageType = "Bioseeker";
        magnification = 1;
        outlierThreshold = 690;
        iRange = 32767;
    }

    if (iWidth == 896) {
        imageType = "LWRTS";
        magnification = 1;
        outlierThreshold = 600;
        iRange = 32767;
    }

    if (iWidth == 1024) {
        imageType = "HSV";
        magnification = 1;
        outlierThreshold = 3000;    //24576 is 3%
    }

    if (iWidth == 1028) {
        imageType = "SPAD Histogram Raw";   //4,16,64 or 256 images taken in quick succession witha high speed laser
        magnification = 16;
        outlierThreshold = 3;
        iWidth = 32;
        iHeight = 32;
        skipPixels = 4;
        iRange = 32767;
    }

    qDebug() << "Outlier Threshold: " << outlierThreshold;

    totalPixels = iWidth * iHeight;         //calc it here once for this file

    QImage image(iWidth * magnification, iHeight * magnification, QImage::Format_RGB32);
    image.fill(0xFF0606f0);

    emit nFramesChanged();        //set our number of frames to the widget

    setAGCOn();

    imageMin = 0;
    imageMax = quint16(iRange * pixelScale);

    startFrame = 1;                     //setup arfpos.qml values
    emit startFrameChanged();

    endFrame = numberOfFrames;
    emit endFrameChanged();

    currentFrame = 1;
    emit currentFrameChanged();

    redraw = 1;                         // 1 redraws current frame

    playMode = true;
    emit playModeChanged(playMode);

    metaData.clear();
    metaData.append("Dir: " + dirStr);
    metaData.append("File : " + iFileName);
    metaData.append("Image Type: " + imageType);
    metaData.append(QString("Resolution: %1 x %2").arg(iWidth).arg(iHeight));
    setModel(metaData);

    return;
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
    baHeader = file.read(64);                                           // read the header to a bytearray
    quint16 *headerPtr = reinterpret_cast<quint16 *>(baHeader.data());  // create a pointer to that bytearray and
    quint16 *headerPtr2 = reinterpret_cast<quint16 *>(baHeader.data() + 2);  // create a pointer to the next 2 bytes to see if its SPC2

    if (*headerPtr == 0x55AA){                                          // Princeton - col 1, bytes 0, 1 magic number 0xAA55 Endianess is reversed
        quint32 frameNum  = quint32(*headerPtr++) + quint32((*headerPtr++) << 16);  // col 2, bytes 2, 3, 4 & 5 frame number
        quint16 frameType       = *headerPtr++;                         // col 4, bytes 6, 7 frame type
        quint16 funcStatus      = *headerPtr++;                         // col 5, bytes 8, 9 functional status
        quint16 totHits         = *headerPtr++;                         // col 6, bytes 10, 11 total hits
        quint16 earlyFExc       = *headerPtr++;                         // col 7, bytes 12, 13 early fire exclusions
        quint16 termCountExc    = *headerPtr++;                         // col 8, bytes 14, 15 terminal count exclusions
        quint16 rangeExt        = *headerPtr++;                         // col 9, bytes 16, 17 range extension
        quint16 natTOFWidth     = *headerPtr++;                         // col 10, bytes 18, 19 native TOF bin width ps
        quint16 gateWidth       = *headerPtr++;                         // col 11, bytes 20, 21 Gate width
        quint16 gateDelay       = *headerPtr++;                         // col 12, bytes 22, 23 Gate delay
        quint16 clkMode         = *headerPtr++;                         // col 13, bytes 24, 25 Clock mode
        quint16 trgMode         = *headerPtr++;                         // col 14, bytes 26, 27 Trigger mode
        quint16 apdSens         = *headerPtr++;                         // col 15, bytes 28, 29 APD sensitivity
        quint16 apdBias         = *headerPtr++;                         // col 16, bytes 30, 31 APD bia
        quint16 apdTemp         = *headerPtr++;                         // col 17, bytes 32, 33 APD temp

        metaData.clear();
        metaData.append("Dir: " + dirStr);
        metaData.append("File : " + iFileName);

        switch (frameType){
            case 0: metaData.append("Image Type: Princeton SPAD - TOF");
                break;
            case 1: metaData.append("Image Type: Princeton SPAD - Grayscale");
                break;
            case 2: metaData.append("Image Type: Princeton SPAD - Test Pattern");
                break;
        }

        iWidth = 32;
        iHeight = 32;
        metaData.append("Resolution: 32 x 32");

        switch (clkMode){
            case 0: metaData.append("Clocking: Internal");
                break;
            case 1: metaData.append("Clocking: External");
                break;
        }

        switch (trgMode){
            case 0: metaData.append("Triggering: Internal");
                break;
            case 2: metaData.append("Triggering: External");
                break;
        }

        metaData.append(QString("Gate Delay: %1 ns").arg(gateDelay, 4,10, QChar(' ')));
        metaData.append(QString("Nat TOF bin Width: %1 ps").arg(natTOFWidth, 4,10, QChar(' ')));
        metaData.append(QString("Range Ext: %1").arg(rangeExt, 4,10, QChar(' ')));

        switch (rangeExt){
            case 0: metaData.append(QString("Gate Width: %1 ns").arg(gateWidth / 4, 4,10, QChar(' ')));
                break;
            case 1: metaData.append(QString("Gate Width: %1 ns").arg(gateWidth * 2, 4,10, QChar(' ')));
                break;
            case 2: metaData.append(QString("Gate Width: %1 ns").arg(gateWidth, 4,10, QChar(' ')));
                break;
            case 3: metaData.append(QString("Gate Width: %1 ns").arg(gateWidth, 4,10, QChar(' ')));
                break;
        }

        metaData.append(QString("Early Fire Exclusions: %1").arg(earlyFExc));
        metaData.append(QString("Terminal Count Exclusions: %1").arg(termCountExc));
        metaData.append(QString("APD Sensitivity: %1").arg(apdSens, 2,10, QChar(' ')));
        metaData.append(QString("APD Bias Volatage: %1 V").arg(double(apdBias / 100.0), 5, 'f', 2, QChar(' ')));
        metaData.append(QString("APD Temp: %1 C").arg(double(apdTemp / 100.0), 5, 'f', 2, QChar(' ')));

        magnification = 20;
        HasFrameHeaders = true;
    }


    if ((*headerPtr == 0x5350) && (*headerPtr2 != 0x4332)){          // Dennis' SPADs
        headerPtr++;                                                 // characters A and D
        headerPtr++;
        quint16 fileVersion      = *headerPtr++;
        headerPtr++;                                                 // characters A and D
        headerPtr++;                                                 // characters A and D
        quint32 timeStamp        = quint32(*headerPtr++);            // col 2, bytes 2, 3 frame number
        timeStamp                += quint32((*headerPtr++) << 16);   // col 3, bytes 4, 5 frame number
        quint32 timeNsStamp      = quint32(*headerPtr++);            // col 2, bytes 2, 3 frame number
        timeNsStamp              += quint32((*headerPtr++) << 16);   // col 3, bytes 4, 5 frame number
        quint16 spadVersion      = *headerPtr++;
        headerPtr++;                                                 // characters A and D
        quint16 spadPLL          = *headerPtr++;
        quint16 delay            = *headerPtr++;
        quint16 window           = *headerPtr++;
        quint32 frameCount       = quint32(*headerPtr++);            // col 2, bytes 2, 3 frame number
        frameCount               += quint32((*headerPtr++) << 16);   // col 3, bytes 4, 5 frame number
        headerPtr++;
        headerPtr++;
        iWidth                  = *headerPtr++;
        iHeight                 = *headerPtr++;
        quint32 frameInt        = quint32(*headerPtr++);            // col 2, bytes 2, 3 frame number
        frameInt                += quint32((*headerPtr++) << 16);   // col 3, bytes 4, 5 frame number
        quint32 laserInt        = quint32(*headerPtr++);            // col 2, bytes 2, 3 frame number
        laserInt                += quint32((*headerPtr++) << 16);   // col 3, bytes 4, 5 frame number

        switch (iWidth){
            case 32:    imageType = "Dennis's Gen 1 SPADs   ";
                        magnification = 20;
                    break;

            case 64:    imageType = "Processed Gen 1 SPADs   ";
                        magnification = 10;
                    break;

            case 128:   imageType = "Dennis's Gen 2 SPADs   ";
                        magnification = 5;
                    break;
            default:    imageType = "Bad image size";
                        magnification = 1;
                        qDebug() << "Bad image size" << iWidth;
                    break;
        }

        metaData.clear();
        metaData.append("Dir: " + dirStr);
        metaData.append("File: " + iFileName);
        metaData.append("Image Type: " + imageType);
        metaData.append(QString("File Version: %1").arg(fileVersion));
        metaData.append(QString("SPAD Version: %1").arg(spadVersion));
        metaData.append(QString("SPAD PLL: %1MHz").arg(spadPLL));
        metaData.append(QString("Resolution: %1 x %2").arg(iWidth).arg(iHeight));

        HasFrameHeaders = true;
    }


    if (*headerPtr == 0x5053){                                       // Monash or BAE SPAD data
        headerPtr++;                                                 // characters A and D
        headerPtr++;                                                 // characters A and D
        fileVersion             = *headerPtr++;
        headerPtr++;                                                 // characters A and D
        iWidth                  = *headerPtr++;
        iHeight                 = *headerPtr++;

        if ((iWidth == 64) & (iHeight == 64)) {
            imageType = "Monash 64 x 64 SPAD   ";
            magnification = 10;
            fileStartOffset = 32;
         } else {
            imageType = "BAE Gen 1 SPADs   ";
            iWidth = 32;
            iHeight = 32;
            magnification = 20;
            fileStartOffset = 64;
        }

        HasFrameHeaders = false;

        metaData.clear();
        metaData.append("Dir: " + dirStr);
        metaData.append("File: " + iFileName);
        metaData.append("Image Type: " + imageType);
        metaData.append(QString("File Version: %1").arg(fileVersion));
        metaData.append(QString("Resolution: %1 x %2").arg(iWidth).arg(iHeight));
    }

    if ((*headerPtr == 0x5350) && (*headerPtr2 == 0x4332)){         // SPC2
        headerPtr++;                                                // 0
        headerPtr++;                                                // 2
        quint16 fileVersion      = *headerPtr++;                    // 4

        iWidth = 32;
        iHeight = 32;
        imageType = "SPC2   ";
        magnification = 20;

        metaData.clear();
        metaData.append("Dir: " + dirStr);
        metaData.append("File: " + iFileName);
        metaData.append("Image Type: " + imageType);
        metaData.append(QString("File Version: %1").arg(fileVersion));
        metaData.append(QString("Resolution: %1 x %2").arg(iWidth).arg(iHeight));
        metaData.append(QString("Frame Rate: 50 Hz"));

        HasFrameHeaders = true;
    }

    qDebug() << imageType << *headerPtr << iWidth << iHeight << fileVersion;

    setModel(metaData);

    fNameandImagetype = QString("%1/%2  %3").arg(dirStr).arg(iFileName).arg(imageType);
    emit fNameInfoChanged(fNameandImagetype);

    QImage image(iWidth * magnification, iHeight * magnification, QImage::Format_RGB32);
    image.fill(0xFF0606f0);

    numberOfFrames = int(file.size() / (iWidth * (iHeight + 1) * 2));
    emit nFramesChanged();        //set our number of frames to the widget

    totalPixels = iWidth * iHeight;

    setAGCOn();
    redraw = 1;

    imageMin = 0;
    imageMax = quint16(4095 * pixelScale);

    startFrame = 1;                     //setup arfpos.qml values
    emit startFrameChanged();

    endFrame = numberOfFrames;
    emit endFrameChanged();

    currentFrame = 1;
    emit currentFrameChanged();

    redraw = 1;                         // 1 redraws current frame

    playMode = true;
    emit playModeChanged(playMode);
    return;
}



void ReadImage::getFastBinHeaderData(QByteArray baFastHeader)
{
    // This is the stuff that changes frame by frame
    QDateTime UTC;

    fastMetaData.clear();
    quint16 *headerPtr = reinterpret_cast<quint16 *>(baFastHeader.data());  // create a pointer to that bytearray and
    quint16 *headerPtr2 = reinterpret_cast<quint16 *>(baFastHeader.data() + 2);  // create a pointer to that bytearray and

    if (*headerPtr == 0x55AA) {                                                   // col 1, bytes 0, 1 magic number 0xAA55 Endianess is reversed
        quint32 frameNum  = quint32(*(headerPtr + 1)) + quint32(*(headerPtr + 2) << 16);    // col 2, bytes 2, 3, 4 & 5 frame number
        quint16 totHits         = *(headerPtr + 5);                                 // col 6, bytes 10, 11 total hits
        quint16 framePeriod     = *(headerPtr + 20);                                 // col 21, bytes 40, 41 frame period,

        fastMetaData.append(QString("Frame Number: %1 ").arg(frameNum));
        fastMetaData.append(QString("Total Hits: %1").arg(totHits));

        fastMetaData.append(QString("Frame Period: %1ns").arg(framePeriod));
    }

    if ((*headerPtr == 0x5350) && (*headerPtr2 != 0x4332)){         // Dennis' SPADs
        quint32 timeStamp        = quint32(*(headerPtr + 5)) + quint32((*(headerPtr + 6)) << 16);   // col 3, bytes 4, 5 frame number
//        quint32 timeNsStamp      = quint32(*(headerPtr + 7)) + quint32((*(headerPtr + 1)) << 16);   // col 3, bytes 4, 5 frame number
        quint16 spadPLL          = *(headerPtr + 11);
        quint16 delay            = *(headerPtr + 12);
        quint16 window           = *(headerPtr + 13);
        quint32 frameCount       = quint32(*(headerPtr + 14)) + quint32((*(headerPtr + 15)) << 16) + 1;   // col 3, bytes 4, 5 frame number
        quint32 frameInt         = quint32(*(headerPtr + 20)) + quint32((*(headerPtr + 21)) << 16);   // col 3, bytes 4, 5 frame number

        UTC = QDateTime::fromTime_t(timeStamp);

        fastMetaData.append(QString("Time %1").arg(UTC.toString("d MMM yyyy hh:mm:ss")));
        fastMetaData.append(QString("Frame Number: %1 ").arg(frameCount));

        if (spadPLL){
            fastMetaData.append(QString("Delay: %1 clocks (%2m)").arg(delay).arg(delay * SPEEDOFLIGHT / spadPLL / 2));
            fastMetaData.append(QString("Window: %1 clocks (%2m)").arg(window).arg(window * SPEEDOFLIGHT / spadPLL / 2));
        }

        fastMetaData.append(QString("Frame Interval: %1s (%2Hz)").arg(frameInt * 3.3333333E-09).arg(1/double(frameInt * 3.3333333E-09)));
    }

    if ((*headerPtr == 0x5350) && (*headerPtr2 == 0x4332)) {                // SPC2
        quint32 timeStamp       = quint32(*(headerPtr + 5)) + quint32((*(headerPtr + 6)) << 16);   // col 3, bytes 4, 5 frame number
        quint16 Gate            = *(headerPtr + 12);                        // 24 width of gate in 50MHz clocks
        quint16 Integ           = *(headerPtr + 13);                        // 26 number of images hardware averaged
        quint32 frameCount      = quint32(*(headerPtr + 14));               // 28 col 2, bytes 2, 3 frame number
        frameCount              += quint32((*(headerPtr + 15)) << 16);      // 30 col 3, bytes 4, 5 frame number
        quint16 minCount        = *(headerPtr + 16);                        // 32 min photon count
        quint16 maxCount        = *(headerPtr + 17);                        // 34 max photon count

        UTC = QDateTime::fromTime_t(timeStamp);

        fastMetaData.append(QString("Time %1").arg(UTC.toString("d MMM yyyy hh:mm:ss")));
        fastMetaData.append(QString("Frame Number: %1 ").arg(frameCount));
        fastMetaData.append(QString("Gate width: %1 Clocks").arg(Gate));
        fastMetaData.append(QString("Integrated: %1 images").arg(Integ));
        fastMetaData.append(QString("Min Photons: %1").arg(minCount));
        fastMetaData.append(QString("Max Photons: %1").arg(maxCount));
     }
}



int ReadImage::getArfImage(int rdrw, int frame)
{
    QByteArray baImage(iWidth * iHeight * 2, 0);
    QImage thisFrame(iWidth, iHeight, QImage::Format_RGB32);
    quint16 colour, tColour, thisImageMin = 0, thisImageMax = 0;

    if ((rdrw > 0) && file.isOpen()) {
        rdrw--;
        emit currentFrameChanged();                                             // shows us where we are in the file
        quint32 pixelPos = (xPos + (yPos * iWidth));                            //calculate memory location of the pixel
        quint16 histValue;

        quint32 seekPos = fileStartOffset + ((totalPixels + skipPixels) * 2 * (frame - 1)) + skipPixels * 2;
        file.seek(seekPos);
        baImage = file.read(totalPixels * 2);                                   // read 1 frame to a bytearray
        quint16 *imagePtr = reinterpret_cast<quint16 *>(baImage.data());        // create a pointer to that bytearray and
        quint32 *thsFrmePtr = reinterpret_cast<quint32 *>(thisFrame.bits());
        histogram.fill(0,65536);


        for (quint32 i = 0; i < totalPixels; i++, imagePtr++, thsFrmePtr++){    //iterate image pointer through memory
            colour = byteSwapTable.at(*imagePtr);
            *thsFrmePtr = colourTable.at(colour);
//            tColour = *imagePtr;
            tColour = colour;
            histValue = histogram[tColour];
            histogram[tColour] = histValue + 1;                                 //since we're trawling through the image do the histogram


            if (pixelPos == i){
                double sColour = tColour * pixelScale;
                pixStr = QString(" %1, %2, %3 ").arg(xPos,0,10,QChar(' ')).arg(yPos,0,10,QChar(' ')).arg(sColour, 0, 'f', 1, QChar(' '));
                fastMetaData.clear();
                fastMetaData.append(pixStr);
                setFastModel(fastMetaData);
            }
        }

        image = thisFrame.scaledToWidth(iWidth * magnification).mirrored(flipX, flipY);

        if (exportFlag == 1){                                           // bin
            QDataStream out(&exportFile);

            out.writeRawData(baImage, totalPixels * 2);
            qDebug() << "export bin file" << currentFrame << exportFlag;
        }

        if (exportFlag == 2){                                           // arf
            QDataStream out(&exportFile);
            QByteArray baARFImage(iWidth * iHeight * 2, 0);

            for(int i = 0; i < totalPixels; i++) {
                baARFImage[i * 2] = baImage[i * 2 + 1];
                baARFImage[i * 2 + 1] = baImage[i * 2];
            }

            out.writeRawData(baARFImage, (totalPixels) * 2);
            qDebug() << "export arf file" << currentFrame << exportFlag;
        }

        if (exportFlag == 3){                                           // bmp
            QString iName = QString("%1/%2_%3.bmp").arg(dirStr).arg(iFileName).arg(currentFrame, 4, 10, QChar('0'));
            exportFile.setFileName(iName);

            if (!exportFile.open(QIODevice::WriteOnly)){
                qDebug() << "export file" << iName <<  "not open";
            } else {
                image.save(iName, "BMP", 100); // writes image into ba in PNG format
            }
            exportFile.close();
            qDebug() << "Exported" << iName;
        }

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


        if (AGC && (abs(quint16(imageMin - ctMin)) > 20)){
            setCTMin(imageMin);
            emit ctMinChanged();
        }

        if (AGC && (abs(quint16(imageMax - ctMax)) > 20)){
            setCTMax(imageMax);
            emit ctMaxChanged();}
        }

        emit newHistogram(histogram);
    return rdrw;
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
        emit currentFrameChanged();     // shows us where we are in the file
        quint32 pixelPos = (xPos + (yPos * iWidth));    //calculate memory location of the pixel
        quint16 histValue;
        quint32 seekPos = 0;

        if (HasFrameHeaders){
            seekPos = (((iWidth * iHeight) + iWidth) * 2 * (frame - 1));
            file.seek(seekPos);
            baHeader = file.read(iWidth * 2);
        } else {
            seekPos = ((iWidth * iHeight) * 2 * (frame - 1)) + fileStartOffset;
            file.seek(seekPos);
        }
        getFastBinHeaderData(baHeader);

        baImage = file.read(iWidth * iHeight * 2);      // read 1 frame to a bytearray
        baImageR = baImage;
        quint16 *imagePtr = reinterpret_cast<quint16 *>(baImage.data());   // create a pointer to that bytearray and
        quint32 *thsFrmePtr = reinterpret_cast<quint32 *>(thisFrame.bits());
        histogram.fill(0,65536);


        for (quint32 i = 0; i < totalPixels; i++, imagePtr++, thsFrmePtr++){  //iterate image pointer through memory
            colour = *imagePtr;
            *thsFrmePtr = colourTable.at(colour);
            tColour = colour;
            histValue = histogram[tColour];
            histogram[tColour] = histValue + 1; //since we're trawling through the image do the histogram


            if (pixelPos == i){
                double sColour = tColour * pixelScale;
                pixStr = QString(" %1, %2, %3 ").arg(xPos,0,10,QChar(' ')).arg(yPos,0,10,QChar(' ')).arg(sColour, 0, 'f', 1, QChar(' '));
                fastMetaData.append(pixStr);
                setFastModel(fastMetaData);
            }
        }

          image = thisFrame.scaledToWidth(iWidth * magnification).mirrored(flipX, flipY);
//        image = thisFrame.scaled(iWidth * magnification, iWidth * magnification, Qt::IgnoreAspectRatio).mirrored(flipX, flipY);

        if (exportFlag == 1){                                           // bin
            QDataStream out(&exportFile);

            out.writeRawData(baHeader, iWidth * 2);
            out.writeRawData(baImageR, totalPixels * 2);
            qDebug() << "export bin file" << currentFrame << exportFlag;
        }

        if (exportFlag == 2){                                           // arf
            QDataStream out(&exportFile);
            QByteArray baARFImage(iWidth * iHeight * 2, 0);

            for(int i = 0; i < totalPixels; i++) {
                baARFImage[i * 2] = baImageR[i * 2 + 1];
                baARFImage[i * 2 + 1] = baImageR[i * 2];
            }

            out.writeRawData(baARFImage, (totalPixels) * 2);
            qDebug() << "export arf file" << currentFrame << exportFlag;
        }

        if (exportFlag == 3){                                           // bmp
            QString iName = QString("%1/%2_%3.bmp").arg(dirStr).arg(iFileName).arg(currentFrame, 4, 10, QChar('0'));
            exportFile.setFileName(iName);

            if (!exportFile.open(QIODevice::WriteOnly)){
                qDebug() << "export file" << iName <<  "not open";
            } else {
                image.save(iName, "BMP", 100); // writes image into ba in PNG format
            }
            exportFile.close();
            qDebug() << "Exported" << iName;
        }

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


        if (AGC && (abs(quint16(imageMin - ctMin)) > 20)){
            setCTMin(imageMin);
            emit ctMinChanged();
//            recalcColourTable(ctMin, ctMax);
        }

        if (AGC && (abs(quint16(imageMax - ctMax)) > 20)){
            setCTMax(imageMax);
            emit ctMaxChanged();}
//            recalcColourTable(ctMin, ctMax);
        }

        emit newHistogram(histogram);
    return rdrw;
}



void ReadImage::exportSegment(int xportType)
{
    exportFlag = xportType;

    exportFileName = iFileName;
    exportFileName.replace(QString(".arf"), QString(""));

    if (exportFlag == 1){
        exportFile.setFileName(QString("%1/%2_%3-%4.bin").arg(dirStr).arg(exportFileName).arg(startFrame).arg(endFrame));
        exportFile.open(QIODevice::WriteOnly);
    }

    if (exportFlag == 2){
        exportFile.setFileName(QString("%1/%2_%3-%4.arf").arg(dirStr).arg(exportFileName).arg(startFrame).arg(endFrame));
        exportARF();
    }

    currentFrame = startFrame - 1;      // end frame
    redraw = 1;                         // paused
    playMode = true;                    // play
    emit playModeChanged(playMode);
    qDebug() << "playmode" << playMode;
    timerTimeout();
}



void ReadImage::exportARF()
{
    QDataStream out(&exportFile);
    exportFile.open(QIODevice::WriteOnly);

    quint32 nMagic = 0xBBBBBAAD;                    // bytes 0 - 3 magic number
    quint32 nBytes = 0x00000002;                    // bytes 4 - 7 number of bytes / word ????
    quint32 nYPxls = iHeight;                       // bytes 8 - 11 number of Y pixels
    quint32 nXPxls = iWidth;                        // bytes 12 - 15 number of x pixels
    quint32 nIType = 0x00000003;                    // bytes 16 - 19 Image type ???????
    quint32 nFrams = 1 + endFrame - startFrame;     // bytes 20 - 23 total Arf frames.
    quint32 nOffst = 0x00000020;                    // bytes 23 - 27 start of first image offset
    quint32 nUnusd = 0x00000000;                    // bytes 28 - 31 unused

    out << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;
    qDebug() << exportFile.fileName() << " is open." << "arf header data written";
    qDebug() << nMagic << nBytes << nYPxls << nXPxls << nIType << nFrams << nOffst << nUnusd;
}




void ReadImage::timerTimeout()
{
    if (playMode){
        currentFrame++;
        redraw = 1;

        if ((currentFrame - startFrame) == 100) setAGCOff();

        if (currentFrame > endFrame){
            if (loopMode){
                currentFrame = startFrame;
            } else {
                playMode = false;
                redraw = 0;
            }

            setAGCOff(); 

            if (exportFlag > 0){
                exportFlag = 0;
                qDebug() << "Files Exported" << exportFlag << playMode;
                exportFile.close();
            }
        }
    }

    if (currentFrame > endFrame) currentFrame = endFrame;          //limit to current frames. QML can go past endstops and read
    if (currentFrame < startFrame) currentFrame = startFrame;      //memory after end of file. Good for a laugh but can crash.

    if (iExt == "bin"){
        redraw = getBinImage(redraw, currentFrame);
    } else {
        redraw = getArfImage(redraw, currentFrame);
    }
}


void ReadImage::setMousePos(int mX, int mY)
{
    xPos = mX / magnification;
    yPos = mY / magnification;
    qDebug() << "xy" << xPos << yPos;

    if (!playMode) {
        redraw = 1;                 // only do this if paused
        timerTimeout();
    }
}



QString ReadImage::IFileName() const
{
    return iFileName;
}


void ReadImage::paint(QPainter *painter)
{
    QRect dirtyRect = QRect(0,0,iWidth * magnification,(iHeight + 0) * magnification);
    painter->drawImage(dirtyRect, image);
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
    startFrame = tSFrame;
}

void ReadImage::setEndFrame(int tEFrame)
{
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


quint32 ReadImage::getRange()
{
    return iRange;
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
//    qDebug() << "AGC Off";
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
