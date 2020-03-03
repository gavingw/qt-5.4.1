/****************************************************************************
** Meta object code from reading C++ file 'picturewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/picturewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'picturewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_pictureWidget_t {
    QByteArrayData data[6];
    char stringdata[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_pictureWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_pictureWidget_t qt_meta_stringdata_pictureWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "pictureWidget"
QT_MOC_LITERAL(1, 14, 25), // "deal_picture_views_signal"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 16), // "show_big_picture"
QT_MOC_LITERAL(4, 58, 12), // "on_usb_mount"
QT_MOC_LITERAL(5, 71, 13) // "on_usb_umount"

    },
    "pictureWidget\0deal_picture_views_signal\0"
    "\0show_big_picture\0on_usb_mount\0"
    "on_usb_umount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_pictureWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    1,   35,    2, 0x0a /* Public */,
       4,    0,   38,    2, 0x0a /* Public */,
       5,    0,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void pictureWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        pictureWidget *_t = static_cast<pictureWidget *>(_o);
        switch (_id) {
        case 0: _t->deal_picture_views_signal(); break;
        case 1: _t->show_big_picture((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_usb_mount(); break;
        case 3: _t->on_usb_umount(); break;
        default: ;
        }
    }
}

const QMetaObject pictureWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_pictureWidget.data,
      qt_meta_data_pictureWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *pictureWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *pictureWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_pictureWidget.stringdata))
        return static_cast<void*>(const_cast< pictureWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int pictureWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
