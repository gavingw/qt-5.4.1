/****************************************************************************
** Meta object code from reading C++ file 'moviedesk1.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/moviedesk1.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moviedesk1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_movieDesk1_t {
    QByteArrayData data[13];
    char stringdata[220];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_movieDesk1_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_movieDesk1_t qt_meta_stringdata_movieDesk1 = {
    {
QT_MOC_LITERAL(0, 0, 10), // "movieDesk1"
QT_MOC_LITERAL(1, 11, 23), // "on_returnButton_clicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 27), // "on_mainCameraButton_clicked"
QT_MOC_LITERAL(4, 64, 21), // "on_viceButton_clicked"
QT_MOC_LITERAL(5, 86, 15), // "deal_fileDialog"
QT_MOC_LITERAL(6, 102, 1), // "a"
QT_MOC_LITERAL(7, 104, 16), // "deal_fileDialog1"
QT_MOC_LITERAL(8, 121, 9), // "open_file"
QT_MOC_LITERAL(9, 131, 22), // "on_videoButton_clicked"
QT_MOC_LITERAL(10, 154, 20), // "on_picButton_clicked"
QT_MOC_LITERAL(11, 175, 21), // "on_scanButton_clicked"
QT_MOC_LITERAL(12, 197, 22) // "on_musicButton_clicked"

    },
    "movieDesk1\0on_returnButton_clicked\0\0"
    "on_mainCameraButton_clicked\0"
    "on_viceButton_clicked\0deal_fileDialog\0"
    "a\0deal_fileDialog1\0open_file\0"
    "on_videoButton_clicked\0on_picButton_clicked\0"
    "on_scanButton_clicked\0on_musicButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_movieDesk1[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x0a /* Public */,
       3,    0,   65,    2, 0x0a /* Public */,
       4,    0,   66,    2, 0x0a /* Public */,
       5,    1,   67,    2, 0x0a /* Public */,
       7,    0,   70,    2, 0x0a /* Public */,
       8,    1,   71,    2, 0x0a /* Public */,
       9,    0,   74,    2, 0x08 /* Private */,
      10,    0,   75,    2, 0x08 /* Private */,
      11,    0,   76,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void movieDesk1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        movieDesk1 *_t = static_cast<movieDesk1 *>(_o);
        switch (_id) {
        case 0: _t->on_returnButton_clicked(); break;
        case 1: _t->on_mainCameraButton_clicked(); break;
        case 2: _t->on_viceButton_clicked(); break;
        case 3: _t->deal_fileDialog((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->deal_fileDialog1(); break;
        case 5: _t->open_file((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->on_videoButton_clicked(); break;
        case 7: _t->on_picButton_clicked(); break;
        case 8: _t->on_scanButton_clicked(); break;
        case 9: _t->on_musicButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject movieDesk1::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_movieDesk1.data,
      qt_meta_data_movieDesk1,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *movieDesk1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *movieDesk1::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_movieDesk1.stringdata))
        return static_cast<void*>(const_cast< movieDesk1*>(this));
    return QDialog::qt_metacast(_clname);
}

int movieDesk1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
