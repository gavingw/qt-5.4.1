/****************************************************************************
** Meta object code from reading C++ file 'main_desktop.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/main_desktop.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_desktop.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_main_desktop_t {
    QByteArrayData data[15];
    char stringdata[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_main_desktop_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_main_desktop_t qt_meta_stringdata_main_desktop = {
    {
QT_MOC_LITERAL(0, 0, 12), // "main_desktop"
QT_MOC_LITERAL(1, 13, 11), // "timerUpdate"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 28), // "on_mouse_no_active_10_second"
QT_MOC_LITERAL(4, 55, 26), // "on_main_desktop_disvisible"
QT_MOC_LITERAL(5, 82, 23), // "on_main_desktop_visible"
QT_MOC_LITERAL(6, 106, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(7, 128, 23), // "on_recordButton_clicked"
QT_MOC_LITERAL(8, 152, 31), // "on_camera_change_Button_clicked"
QT_MOC_LITERAL(9, 184, 25), // "on_setFirstButton_clicked"
QT_MOC_LITERAL(10, 210, 22), // "on_movieButton_clicked"
QT_MOC_LITERAL(11, 233, 23), // "on_cameraButton_clicked"
QT_MOC_LITERAL(12, 257, 22), // "on_reverseLine_repaint"
QT_MOC_LITERAL(13, 280, 16), // "on_sdcard_delete"
QT_MOC_LITERAL(14, 297, 16) // "on_sdcard_insert"

    },
    "main_desktop\0timerUpdate\0\0"
    "on_mouse_no_active_10_second\0"
    "on_main_desktop_disvisible\0"
    "on_main_desktop_visible\0on_pushButton_clicked\0"
    "on_recordButton_clicked\0"
    "on_camera_change_Button_clicked\0"
    "on_setFirstButton_clicked\0"
    "on_movieButton_clicked\0on_cameraButton_clicked\0"
    "on_reverseLine_repaint\0on_sdcard_delete\0"
    "on_sdcard_insert"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_main_desktop[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x0a /* Public */,
       4,    0,   81,    2, 0x0a /* Public */,
       5,    0,   82,    2, 0x0a /* Public */,
       6,    0,   83,    2, 0x0a /* Public */,
       7,    0,   84,    2, 0x0a /* Public */,
       8,    0,   85,    2, 0x0a /* Public */,
       9,    0,   86,    2, 0x0a /* Public */,
      10,    0,   87,    2, 0x0a /* Public */,
      11,    0,   88,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void main_desktop::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        main_desktop *_t = static_cast<main_desktop *>(_o);
        switch (_id) {
        case 0: _t->timerUpdate(); break;
        case 1: _t->on_mouse_no_active_10_second(); break;
        case 2: _t->on_main_desktop_disvisible(); break;
        case 3: _t->on_main_desktop_visible(); break;
        case 4: _t->on_pushButton_clicked(); break;
        case 5: _t->on_recordButton_clicked(); break;
        case 6: _t->on_camera_change_Button_clicked(); break;
        case 7: _t->on_setFirstButton_clicked(); break;
        case 8: _t->on_movieButton_clicked(); break;
        case 9: _t->on_cameraButton_clicked(); break;
        case 10: _t->on_reverseLine_repaint(); break;
        case 11: _t->on_sdcard_delete(); break;
        case 12: _t->on_sdcard_insert(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject main_desktop::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_main_desktop.data,
      qt_meta_data_main_desktop,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *main_desktop::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *main_desktop::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_main_desktop.stringdata))
        return static_cast<void*>(const_cast< main_desktop*>(this));
    return QWidget::qt_metacast(_clname);
}

int main_desktop::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
