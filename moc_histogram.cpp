/****************************************************************************
** Meta object code from reading C++ file 'histogram.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "histogram.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'histogram.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Histogram_t {
    QByteArrayData data[24];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Histogram_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Histogram_t qt_meta_stringdata_Histogram = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Histogram"
QT_MOC_LITERAL(1, 10, 8), // "histVals"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 7), // "histStr"
QT_MOC_LITERAL(4, 28, 18), // "colourTableChanged"
QT_MOC_LITERAL(5, 47, 12), // "setHistogram"
QT_MOC_LITERAL(6, 60, 16), // "QVector<quint32>"
QT_MOC_LITERAL(7, 77, 10), // "tHistogram"
QT_MOC_LITERAL(8, 88, 14), // "setColourTable"
QT_MOC_LITERAL(9, 103, 13), // "QVector<QRgb>"
QT_MOC_LITERAL(10, 117, 12), // "tColourTable"
QT_MOC_LITERAL(11, 130, 12), // "setImageSize"
QT_MOC_LITERAL(12, 143, 8), // "tArfSize"
QT_MOC_LITERAL(13, 152, 10), // "setiMinMax"
QT_MOC_LITERAL(14, 163, 7), // "tMinMax"
QT_MOC_LITERAL(15, 171, 13), // "setPixelScale"
QT_MOC_LITERAL(16, 185, 5), // "paint"
QT_MOC_LITERAL(17, 191, 9), // "QPainter*"
QT_MOC_LITERAL(18, 201, 7), // "painter"
QT_MOC_LITERAL(19, 209, 10), // "niceNumber"
QT_MOC_LITERAL(20, 220, 6), // "number"
QT_MOC_LITERAL(21, 227, 5), // "Round"
QT_MOC_LITERAL(22, 233, 11), // "myHistogram"
QT_MOC_LITERAL(23, 245, 10) // "waitCTable"

    },
    "Histogram\0histVals\0\0histStr\0"
    "colourTableChanged\0setHistogram\0"
    "QVector<quint32>\0tHistogram\0setColourTable\0"
    "QVector<QRgb>\0tColourTable\0setImageSize\0"
    "tArfSize\0setiMinMax\0tMinMax\0setPixelScale\0"
    "paint\0QPainter*\0painter\0niceNumber\0"
    "number\0Round\0myHistogram\0waitCTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Histogram[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       2,   86, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    0,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   63,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x0a /* Public */,
      11,    1,   69,    2, 0x0a /* Public */,
      13,    1,   72,    2, 0x0a /* Public */,
      15,    1,   75,    2, 0x0a /* Public */,
      16,    1,   78,    2, 0x0a /* Public */,
      19,    2,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, QMetaType::QPoint,   12,
    QMetaType::Void, QMetaType::QPoint,   14,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Int, QMetaType::Int, QMetaType::Bool,   20,   21,

 // properties: name, type, flags
      22, 0x80000000 | 6, 0x0009500a,
      23, 0x80000000 | 9, 0x0009500a,

 // properties: notify_signal_id
       0,
       0,

       0        // eod
};

void Histogram::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Histogram *_t = static_cast<Histogram *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->histVals((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->colourTableChanged(); break;
        case 2: _t->setHistogram((*reinterpret_cast< QVector<quint32>(*)>(_a[1]))); break;
        case 3: _t->setColourTable((*reinterpret_cast< QVector<QRgb>(*)>(_a[1]))); break;
        case 4: _t->setImageSize((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->setiMinMax((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 6: _t->setPixelScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->paint((*reinterpret_cast< QPainter*(*)>(_a[1]))); break;
        case 8: { int _r = _t->niceNumber((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
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
            using _t = void (Histogram::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Histogram::histVals)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Histogram::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Histogram::colourTableChanged)) {
                *result = 1;
                return;
            }
        }
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<quint32> >(); break;
        }
    }

#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
    } else if (_c == QMetaObject::WriteProperty) {
        Histogram *_t = static_cast<Histogram *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setHistogram(*reinterpret_cast< QVector<quint32>*>(_v)); break;
        case 1: _t->setColourTable(*reinterpret_cast< QVector<QRgb>*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject Histogram::staticMetaObject = {
    { &QQuickPaintedItem::staticMetaObject, qt_meta_stringdata_Histogram.data,
      qt_meta_data_Histogram,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Histogram::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Histogram::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Histogram.stringdata0))
        return static_cast<void*>(this);
    return QQuickPaintedItem::qt_metacast(_clname);
}

int Histogram::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QQuickPaintedItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void Histogram::histVals(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Histogram::colourTableChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
