/****************************************************************************
** Meta object code from reading C++ file 'music_player.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/music_player.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'music_player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Music_Player_t {
    QByteArrayData data[22];
    char stringdata[363];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Music_Player_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Music_Player_t qt_meta_stringdata_Music_Player = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Music_Player"
QT_MOC_LITERAL(1, 13, 25), // "on_preMovieButton_clicked"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 26), // "on_nextMovieButton_clicked"
QT_MOC_LITERAL(4, 67, 21), // "on_playButton_clicked"
QT_MOC_LITERAL(5, 89, 21), // "on_muteButton_clicked"
QT_MOC_LITERAL(6, 111, 22), // "on_btnMenu_Min_clicked"
QT_MOC_LITERAL(7, 134, 22), // "on_btnMenu_Max_clicked"
QT_MOC_LITERAL(8, 157, 24), // "on_btnMenu_Close_clicked"
QT_MOC_LITERAL(9, 182, 15), // "durationChanged"
QT_MOC_LITERAL(10, 198, 8), // "duration"
QT_MOC_LITERAL(11, 207, 15), // "positionChanged"
QT_MOC_LITERAL(12, 223, 8), // "progress"
QT_MOC_LITERAL(13, 232, 18), // "updateDurationInfo"
QT_MOC_LITERAL(14, 251, 11), // "currentInfo"
QT_MOC_LITERAL(15, 263, 9), // "setVolume"
QT_MOC_LITERAL(16, 273, 11), // "timerUpdate"
QT_MOC_LITERAL(17, 285, 25), // "on_timeout_mouserMoveTime"
QT_MOC_LITERAL(18, 311, 4), // "seek"
QT_MOC_LITERAL(19, 316, 7), // "seconds"
QT_MOC_LITERAL(20, 324, 17), // "on_video_complete"
QT_MOC_LITERAL(21, 342, 20) // "on_otherInfo_clicked"

    },
    "Music_Player\0on_preMovieButton_clicked\0"
    "\0on_nextMovieButton_clicked\0"
    "on_playButton_clicked\0on_muteButton_clicked\0"
    "on_btnMenu_Min_clicked\0on_btnMenu_Max_clicked\0"
    "on_btnMenu_Close_clicked\0durationChanged\0"
    "duration\0positionChanged\0progress\0"
    "updateDurationInfo\0currentInfo\0setVolume\0"
    "timerUpdate\0on_timeout_mouserMoveTime\0"
    "seek\0seconds\0on_video_complete\0"
    "on_otherInfo_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Music_Player[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    0,   97,    2, 0x08 /* Private */,
       6,    0,   98,    2, 0x08 /* Private */,
       7,    0,   99,    2, 0x08 /* Private */,
       8,    0,  100,    2, 0x08 /* Private */,
       9,    1,  101,    2, 0x08 /* Private */,
      11,    1,  104,    2, 0x08 /* Private */,
      13,    1,  107,    2, 0x08 /* Private */,
      15,    1,  110,    2, 0x08 /* Private */,
      16,    0,  113,    2, 0x08 /* Private */,
      17,    0,  114,    2, 0x08 /* Private */,
      18,    1,  115,    2, 0x08 /* Private */,
      20,    0,  118,    2, 0x08 /* Private */,
      21,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong,   10,
    QMetaType::Void, QMetaType::LongLong,   12,
    QMetaType::Void, QMetaType::LongLong,   14,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Music_Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Music_Player *_t = static_cast<Music_Player *>(_o);
        switch (_id) {
        case 0: _t->on_preMovieButton_clicked(); break;
        case 1: _t->on_nextMovieButton_clicked(); break;
        case 2: _t->on_playButton_clicked(); break;
        case 3: _t->on_muteButton_clicked(); break;
        case 4: _t->on_btnMenu_Min_clicked(); break;
        case 5: _t->on_btnMenu_Max_clicked(); break;
        case 6: _t->on_btnMenu_Close_clicked(); break;
        case 7: _t->durationChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->positionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 9: _t->updateDurationInfo((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 10: _t->setVolume((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->timerUpdate(); break;
        case 12: _t->on_timeout_mouserMoveTime(); break;
        case 13: _t->seek((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_video_complete(); break;
        case 15: _t->on_otherInfo_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject Music_Player::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Music_Player.data,
      qt_meta_data_Music_Player,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Music_Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Music_Player::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Music_Player.stringdata))
        return static_cast<void*>(const_cast< Music_Player*>(this));
    return QDialog::qt_metacast(_clname);
}

int Music_Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
