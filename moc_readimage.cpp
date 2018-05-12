/****************************************************************************
** Meta object code from reading C++ file 'readimage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "readimage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'readimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ReadImage_t {
    QByteArrayData data[80];
    char stringdata0[867];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ReadImage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ReadImage_t qt_meta_stringdata_ReadImage = {
    {
QT_MOC_LITERAL(0, 0, 9), // "ReadImage"
QT_MOC_LITERAL(1, 10, 12), // "modelChanged"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 16), // "fastModelChanged"
QT_MOC_LITERAL(4, 41, 19), // "currentFrameChanged"
QT_MOC_LITERAL(5, 61, 17), // "startFrameChanged"
QT_MOC_LITERAL(6, 79, 15), // "endFrameChanged"
QT_MOC_LITERAL(7, 95, 14), // "nFramesChanged"
QT_MOC_LITERAL(8, 110, 12), // "newHistogram"
QT_MOC_LITERAL(9, 123, 16), // "QVector<quint32>"
QT_MOC_LITERAL(10, 140, 14), // "newColourTable"
QT_MOC_LITERAL(11, 155, 13), // "QVector<QRgb>"
QT_MOC_LITERAL(12, 169, 16), // "fNameInfoChanged"
QT_MOC_LITERAL(13, 186, 15), // "playModeChanged"
QT_MOC_LITERAL(14, 202, 13), // "ctTypeChanged"
QT_MOC_LITERAL(15, 216, 12), // "ctMinChanged"
QT_MOC_LITERAL(16, 229, 12), // "ctMaxChanged"
QT_MOC_LITERAL(17, 242, 8), // "agcState"
QT_MOC_LITERAL(18, 251, 16), // "genByteSwapTable"
QT_MOC_LITERAL(19, 268, 18), // "genGreyColourTable"
QT_MOC_LITERAL(20, 287, 18), // "genLCIIColourTable"
QT_MOC_LITERAL(21, 306, 17), // "recalcColourTable"
QT_MOC_LITERAL(22, 324, 12), // "setFastModel"
QT_MOC_LITERAL(23, 337, 1), // "m"
QT_MOC_LITERAL(24, 339, 8), // "setModel"
QT_MOC_LITERAL(25, 348, 8), // "getModel"
QT_MOC_LITERAL(26, 357, 12), // "getFastModel"
QT_MOC_LITERAL(27, 370, 11), // "setMousePos"
QT_MOC_LITERAL(28, 382, 2), // "mX"
QT_MOC_LITERAL(29, 385, 2), // "mY"
QT_MOC_LITERAL(30, 388, 13), // "openIFileName"
QT_MOC_LITERAL(31, 402, 9), // "prevIFile"
QT_MOC_LITERAL(32, 412, 9), // "nextIFile"
QT_MOC_LITERAL(33, 422, 16), // "setMagnification"
QT_MOC_LITERAL(34, 439, 13), // "exportSegment"
QT_MOC_LITERAL(35, 453, 16), // "getMagnification"
QT_MOC_LITERAL(36, 470, 12), // "timerTimeout"
QT_MOC_LITERAL(37, 483, 11), // "getPlayMode"
QT_MOC_LITERAL(38, 495, 11), // "setPlayMode"
QT_MOC_LITERAL(39, 507, 8), // "getRange"
QT_MOC_LITERAL(40, 516, 8), // "setCTMin"
QT_MOC_LITERAL(41, 525, 8), // "setCTMax"
QT_MOC_LITERAL(42, 534, 6), // "tCTMax"
QT_MOC_LITERAL(43, 541, 8), // "getCTMin"
QT_MOC_LITERAL(44, 550, 8), // "getCTMax"
QT_MOC_LITERAL(45, 559, 9), // "setCTType"
QT_MOC_LITERAL(46, 569, 8), // "setAGCOn"
QT_MOC_LITERAL(47, 578, 9), // "setAGCOff"
QT_MOC_LITERAL(48, 588, 11), // "getMetaData"
QT_MOC_LITERAL(49, 600, 5), // "fName"
QT_MOC_LITERAL(50, 606, 15), // "setCurrentFrame"
QT_MOC_LITERAL(51, 622, 7), // "tCFrame"
QT_MOC_LITERAL(52, 630, 13), // "setStartFrame"
QT_MOC_LITERAL(53, 644, 7), // "tSFrame"
QT_MOC_LITERAL(54, 652, 11), // "setEndFrame"
QT_MOC_LITERAL(55, 664, 7), // "tEFrame"
QT_MOC_LITERAL(56, 672, 7), // "nFrames"
QT_MOC_LITERAL(57, 680, 6), // "sFrame"
QT_MOC_LITERAL(58, 687, 6), // "eFrame"
QT_MOC_LITERAL(59, 694, 6), // "cFrame"
QT_MOC_LITERAL(60, 701, 5), // "begin"
QT_MOC_LITERAL(61, 707, 4), // "back"
QT_MOC_LITERAL(62, 712, 4), // "play"
QT_MOC_LITERAL(63, 717, 5), // "pause"
QT_MOC_LITERAL(64, 723, 4), // "loop"
QT_MOC_LITERAL(65, 728, 7), // "forward"
QT_MOC_LITERAL(66, 736, 3), // "end"
QT_MOC_LITERAL(67, 740, 5), // "model"
QT_MOC_LITERAL(68, 746, 9), // "fastModel"
QT_MOC_LITERAL(69, 756, 9), // "fNameInfo"
QT_MOC_LITERAL(70, 766, 12), // "currentFrame"
QT_MOC_LITERAL(71, 779, 10), // "startFrame"
QT_MOC_LITERAL(72, 790, 8), // "endFrame"
QT_MOC_LITERAL(73, 799, 8), // "playMode"
QT_MOC_LITERAL(74, 808, 6), // "iRange"
QT_MOC_LITERAL(75, 815, 5), // "ctMin"
QT_MOC_LITERAL(76, 821, 5), // "ctMax"
QT_MOC_LITERAL(77, 827, 15), // "colourTableType"
QT_MOC_LITERAL(78, 843, 13), // "magnification"
QT_MOC_LITERAL(79, 857, 9) // "iFileName"

    },
    "ReadImage\0modelChanged\0\0fastModelChanged\0"
    "currentFrameChanged\0startFrameChanged\0"
    "endFrameChanged\0nFramesChanged\0"
    "newHistogram\0QVector<quint32>\0"
    "newColourTable\0QVector<QRgb>\0"
    "fNameInfoChanged\0playModeChanged\0"
    "ctTypeChanged\0ctMinChanged\0ctMaxChanged\0"
    "agcState\0genByteSwapTable\0genGreyColourTable\0"
    "genLCIIColourTable\0recalcColourTable\0"
    "setFastModel\0m\0setModel\0getModel\0"
    "getFastModel\0setMousePos\0mX\0mY\0"
    "openIFileName\0prevIFile\0nextIFile\0"
    "setMagnification\0exportSegment\0"
    "getMagnification\0timerTimeout\0getPlayMode\0"
    "setPlayMode\0getRange\0setCTMin\0setCTMax\0"
    "tCTMax\0getCTMin\0getCTMax\0setCTType\0"
    "setAGCOn\0setAGCOff\0getMetaData\0fName\0"
    "setCurrentFrame\0tCFrame\0setStartFrame\0"
    "tSFrame\0setEndFrame\0tEFrame\0nFrames\0"
    "sFrame\0eFrame\0cFrame\0begin\0back\0play\0"
    "pause\0loop\0forward\0end\0model\0fastModel\0"
    "fNameInfo\0currentFrame\0startFrame\0"
    "endFrame\0playMode\0iRange\0ctMin\0ctMax\0"
    "colourTableType\0magnification\0iFileName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ReadImage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      56,   14, // methods
      14,  404, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  294,    2, 0x06 /* Public */,
       3,    0,  295,    2, 0x06 /* Public */,
       4,    0,  296,    2, 0x06 /* Public */,
       5,    0,  297,    2, 0x06 /* Public */,
       6,    0,  298,    2, 0x06 /* Public */,
       7,    0,  299,    2, 0x06 /* Public */,
       8,    1,  300,    2, 0x06 /* Public */,
      10,    1,  303,    2, 0x06 /* Public */,
      12,    1,  306,    2, 0x06 /* Public */,
      13,    1,  309,    2, 0x06 /* Public */,
      14,    1,  312,    2, 0x06 /* Public */,
      15,    0,  315,    2, 0x06 /* Public */,
      16,    0,  316,    2, 0x06 /* Public */,
      17,    1,  317,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  320,    2, 0x08 /* Private */,
      19,    2,  321,    2, 0x08 /* Private */,
      20,    2,  326,    2, 0x08 /* Private */,
      21,    2,  331,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      22,    1,  336,    2, 0x02 /* Public */,
      24,    1,  339,    2, 0x02 /* Public */,
      25,    0,  342,    2, 0x02 /* Public */,
      26,    0,  343,    2, 0x02 /* Public */,
      27,    2,  344,    2, 0x02 /* Public */,
      30,    1,  349,    2, 0x02 /* Public */,
      31,    0,  352,    2, 0x02 /* Public */,
      32,    0,  353,    2, 0x02 /* Public */,
      33,    1,  354,    2, 0x02 /* Public */,
      34,    1,  357,    2, 0x02 /* Public */,
      35,    0,  360,    2, 0x02 /* Public */,
      36,    0,  361,    2, 0x02 /* Public */,
      37,    0,  362,    2, 0x02 /* Public */,
      38,    1,  363,    2, 0x02 /* Public */,
      39,    0,  366,    2, 0x02 /* Public */,
      40,    1,  367,    2, 0x02 /* Public */,
      41,    1,  370,    2, 0x02 /* Public */,
      43,    0,  373,    2, 0x02 /* Public */,
      44,    0,  374,    2, 0x02 /* Public */,
      45,    1,  375,    2, 0x02 /* Public */,
      46,    0,  378,    2, 0x02 /* Public */,
      47,    0,  379,    2, 0x02 /* Public */,
      48,    0,  380,    2, 0x02 /* Public */,
      49,    0,  381,    2, 0x02 /* Public */,
      50,    1,  382,    2, 0x02 /* Public */,
      52,    1,  385,    2, 0x02 /* Public */,
      54,    1,  388,    2, 0x02 /* Public */,
      56,    0,  391,    2, 0x02 /* Public */,
      57,    0,  392,    2, 0x02 /* Public */,
      58,    0,  393,    2, 0x02 /* Public */,
      59,    0,  394,    2, 0x02 /* Public */,
      60,    0,  395,    2, 0x02 /* Public */,
      61,    0,  396,    2, 0x02 /* Public */,
      62,    0,  397,    2, 0x02 /* Public */,
      63,    0,  398,    2, 0x02 /* Public */,
      64,    1,  399,    2, 0x02 /* Public */,
      65,    0,  402,    2, 0x02 /* Public */,
      66,    0,  403,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UShort, QMetaType::UShort,    2,    2,
    QMetaType::Void, QMetaType::UShort, QMetaType::UShort,    2,    2,
    QMetaType::Void, QMetaType::UShort, QMetaType::UShort,    2,    2,

 // methods: parameters
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::Void, QMetaType::QStringList,   23,
    QMetaType::QStringList,
    QMetaType::QStringList,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   28,   29,
    QMetaType::Bool, QMetaType::QString,    2,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::UInt,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::UInt,
    QMetaType::Void, QMetaType::UInt,    2,
    QMetaType::Void, QMetaType::UInt,   42,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QStringList,
    QMetaType::QString,
    QMetaType::Void, QMetaType::Int,   51,
    QMetaType::Void, QMetaType::Int,   53,
    QMetaType::Void, QMetaType::Int,   55,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::UInt,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      67, QMetaType::QStringList, 0x00495003,
      68, QMetaType::QStringList, 0x00495003,
      69, QMetaType::QString, 0x00495001,
      56, QMetaType::UInt, 0x00495001,
      70, QMetaType::UInt, 0x00495103,
      71, QMetaType::UInt, 0x00495103,
      72, QMetaType::UInt, 0x00495103,
      73, QMetaType::Bool, 0x00495103,
      74, QMetaType::UInt, 0x00095001,
      75, QMetaType::UInt, 0x00495003,
      76, QMetaType::UInt, 0x00495003,
      77, QMetaType::Bool, 0x00495003,
      78, QMetaType::UInt, 0x00095103,
      79, QMetaType::QString, 0x00095003,

 // properties: notify_signal_id
       0,
       1,
       8,
       5,
       2,
       3,
       4,
       9,
       0,
      11,
      12,
      10,
       0,
       0,

       0        // eod
};

void ReadImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ReadImage *_t = static_cast<ReadImage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->modelChanged(); break;
        case 1: _t->fastModelChanged(); break;
        case 2: _t->currentFrameChanged(); break;
        case 3: _t->startFrameChanged(); break;
        case 4: _t->endFrameChanged(); break;
        case 5: _t->nFramesChanged(); break;
        case 6: _t->newHistogram((*reinterpret_cast< QVector<quint32>(*)>(_a[1]))); break;
        case 7: _t->newColourTable((*reinterpret_cast< QVector<QRgb>(*)>(_a[1]))); break;
        case 8: _t->fNameInfoChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->playModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->ctTypeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->ctMinChanged(); break;
        case 12: _t->ctMaxChanged(); break;
        case 13: _t->agcState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->genByteSwapTable(); break;
        case 15: _t->genGreyColourTable((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 16: _t->genLCIIColourTable((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 17: _t->recalcColourTable((*reinterpret_cast< quint16(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 18: _t->setFastModel((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 19: _t->setModel((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 20: { QStringList _r = _t->getModel();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 21: { QStringList _r = _t->getFastModel();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 22: _t->setMousePos((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: { bool _r = _t->openIFileName((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 24: { bool _r = _t->prevIFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 25: { bool _r = _t->nextIFile();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 26: _t->setMagnification((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->exportSegment((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: { quint32 _r = _t->getMagnification();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 29: _t->timerTimeout(); break;
        case 30: { bool _r = _t->getPlayMode();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 31: _t->setPlayMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: { quint32 _r = _t->getRange();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 33: _t->setCTMin((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 34: _t->setCTMax((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 35: { quint32 _r = _t->getCTMin();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 36: { quint32 _r = _t->getCTMax();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 37: _t->setCTType((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->setAGCOn(); break;
        case 39: _t->setAGCOff(); break;
        case 40: { QStringList _r = _t->getMetaData();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 41: { QString _r = _t->fName();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 42: _t->setCurrentFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->setStartFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->setEndFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: { quint32 _r = _t->nFrames();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 46: { quint32 _r = _t->sFrame();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 47: { quint32 _r = _t->eFrame();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 48: { quint32 _r = _t->cFrame();
            if (_a[0]) *reinterpret_cast< quint32*>(_a[0]) = std::move(_r); }  break;
        case 49: _t->begin(); break;
        case 50: _t->back(); break;
        case 51: _t->play(); break;
        case 52: _t->pause(); break;
        case 53: _t->loop((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->forward(); break;
        case 55: _t->end(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint32> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::modelChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::fastModelChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::currentFrameChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::startFrameChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::endFrameChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::nFramesChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(QVector<quint32> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::newHistogram)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(QVector<QRgb> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::newColourTable)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::fNameInfoChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::playModeChanged)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::ctTypeChanged)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::ctMinChanged)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::ctMaxChanged)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (ReadImage::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ReadImage::agcState)) {
                *result = 13;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        ReadImage *_t = static_cast<ReadImage *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->getModel(); break;
        case 1: *reinterpret_cast< QStringList*>(_v) = _t->getFastModel(); break;
        case 2: *reinterpret_cast< QString*>(_v) = _t->fName(); break;
        case 3: *reinterpret_cast< quint32*>(_v) = _t->nFrames(); break;
        case 4: *reinterpret_cast< quint32*>(_v) = _t->cFrame(); break;
        case 5: *reinterpret_cast< quint32*>(_v) = _t->sFrame(); break;
        case 6: *reinterpret_cast< quint32*>(_v) = _t->eFrame(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->getPlayMode(); break;
        case 8: *reinterpret_cast< quint32*>(_v) = _t->getRange(); break;
        case 9: *reinterpret_cast< quint32*>(_v) = _t->getCTMin(); break;
        case 10: *reinterpret_cast< quint32*>(_v) = _t->getCTMax(); break;
        case 11: *reinterpret_cast< bool*>(_v) = _t->getCTMax(); break;
        case 12: *reinterpret_cast< quint32*>(_v) = _t->getMagnification(); break;
        case 13: *reinterpret_cast< QString*>(_v) = _t->IFileName(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        ReadImage *_t = static_cast<ReadImage *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0:
            if (_t->m_model != *reinterpret_cast< QStringList*>(_v)) {
                _t->m_model = *reinterpret_cast< QStringList*>(_v);
                Q_EMIT _t->modelChanged();
            }
            break;
        case 1:
            if (_t->n_model != *reinterpret_cast< QStringList*>(_v)) {
                _t->n_model = *reinterpret_cast< QStringList*>(_v);
                Q_EMIT _t->fastModelChanged();
            }
            break;
        case 4: _t->setCurrentFrame(*reinterpret_cast< quint32*>(_v)); break;
        case 5: _t->setStartFrame(*reinterpret_cast< quint32*>(_v)); break;
        case 6: _t->setEndFrame(*reinterpret_cast< quint32*>(_v)); break;
        case 7: _t->setPlayMode(*reinterpret_cast< bool*>(_v)); break;
        case 9: _t->setCTMin(*reinterpret_cast< quint32*>(_v)); break;
        case 10: _t->setCTMax(*reinterpret_cast< quint32*>(_v)); break;
        case 11: _t->setCTType(*reinterpret_cast< bool*>(_v)); break;
        case 12: _t->setMagnification(*reinterpret_cast< quint32*>(_v)); break;
        case 13: _t->openIFileName(*reinterpret_cast< QString*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject ReadImage::staticMetaObject = {
    { &QQuickPaintedItem::staticMetaObject, qt_meta_stringdata_ReadImage.data,
      qt_meta_data_ReadImage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ReadImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ReadImage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ReadImage.stringdata0))
        return static_cast<void*>(this);
    return QQuickPaintedItem::qt_metacast(_clname);
}

int ReadImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 56)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 56;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 56)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 56;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 14;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 14;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ReadImage::modelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ReadImage::fastModelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ReadImage::currentFrameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ReadImage::startFrameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ReadImage::endFrameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ReadImage::nFramesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ReadImage::newHistogram(QVector<quint32> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ReadImage::newColourTable(QVector<QRgb> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ReadImage::fNameInfoChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void ReadImage::playModeChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void ReadImage::ctTypeChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void ReadImage::ctMinChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ReadImage::ctMaxChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ReadImage::agcState(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
