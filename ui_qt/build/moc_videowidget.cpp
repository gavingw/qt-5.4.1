/****************************************************************************
** Meta object code from reading C++ file 'videowidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/videowidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'videowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_videoWidget_t {
    QByteArrayData data[12];
    char stringdata[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_videoWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_videoWidget_t qt_meta_stringdata_videoWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "videoWidget"
QT_MOC_LITERAL(1, 12, 10), // "play_video"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 3), // "pos"
QT_MOC_LITERAL(4, 28, 9), // "showError"
QT_MOC_LITERAL(5, 38, 8), // "filename"
QT_MOC_LITERAL(6, 47, 12), // "add_shot_pic"
QT_MOC_LITERAL(7, 60, 3), // "pix"
QT_MOC_LITERAL(8, 64, 9), // "file_path"
QT_MOC_LITERAL(9, 74, 12), // "deleteThread"
QT_MOC_LITERAL(10, 87, 12), // "on_usb_mount"
QT_MOC_LITERAL(11, 100, 13) // "on_usb_umount"

    },
    "videoWidget\0play_video\0\0pos\0showError\0"
    "filename\0add_shot_pic\0pix\0file_path\0"
    "deleteThread\0on_usb_mount\0on_usb_umount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_videoWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       4,    1,   47,    2, 0x0a /* Public */,
       6,    3,   50,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x0a /* Public */,
      10,    0,   58,    2, 0x0a /* Public */,
      11,    0,   59,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QPixmap, QMetaType::QString, QMetaType::Int,    7,    8,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void videoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        videoWidget *_t = static_cast<videoWidget *>(_o);
        switch (_id) {
        case 0: _t->play_video((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->showError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->add_shot_pic((*reinterpret_cast< QPixmap(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->deleteThread(); break;
        case 4: _t->on_usb_mount(); break;
        case 5: _t->on_usb_umount(); break;
        default: ;
        }
    }
}

const QMetaObject videoWidget::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_videoWidget.data,
      qt_meta_data_videoWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *videoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *videoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_videoWidget.stringdata))
        return static_cast<void*>(const_cast< videoWidget*>(this));
    return QDialog::qt_metacast(_clname);
}

int videoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
struct qt_meta_stringdata_ThumbnailThread_t {
    QByteArrayData data[8];
    char stringdata[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThumbnailThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThumbnailThread_t qt_meta_stringdata_ThumbnailThread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ThumbnailThread"
QT_MOC_LITERAL(1, 16, 17), // "thumbnailFinished"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 8), // "shot_pic"
QT_MOC_LITERAL(4, 44, 9), // "file_path"
QT_MOC_LITERAL(5, 54, 3), // "pos"
QT_MOC_LITERAL(6, 58, 15), // "thumbnailFailed"
QT_MOC_LITERAL(7, 74, 8) // "filename"

    },
    "ThumbnailThread\0thumbnailFinished\0\0"
    "shot_pic\0file_path\0pos\0thumbnailFailed\0"
    "filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThumbnailThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       6,    1,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPixmap, QMetaType::QString, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void ThumbnailThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThumbnailThread *_t = static_cast<ThumbnailThread *>(_o);
        switch (_id) {
        case 0: _t->thumbnailFinished((*reinterpret_cast< QPixmap(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->thumbnailFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ThumbnailThread::*_t)(QPixmap , QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThumbnailThread::thumbnailFinished)) {
                *result = 0;
            }
        }
        {
            typedef void (ThumbnailThread::*_t)(const QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ThumbnailThread::thumbnailFailed)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject ThumbnailThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ThumbnailThread.data,
      qt_meta_data_ThumbnailThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ThumbnailThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThumbnailThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ThumbnailThread.stringdata))
        return static_cast<void*>(const_cast< ThumbnailThread*>(this));
    return QThread::qt_metacast(_clname);
}

int ThumbnailThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ThumbnailThread::thumbnailFinished(QPixmap _t1, QString _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ThumbnailThread::thumbnailFailed(const QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
