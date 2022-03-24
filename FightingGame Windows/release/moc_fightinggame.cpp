/****************************************************************************
** Meta object code from reading C++ file 'fightinggame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../FG/fightinggame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fightinggame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FightingGame_t {
    QByteArrayData data[10];
    char stringdata0[107];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FightingGame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FightingGame_t qt_meta_stringdata_FightingGame = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FightingGame"
QT_MOC_LITERAL(1, 13, 11), // "setMainMenu"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "setGame"
QT_MOC_LITERAL(4, 34, 7), // "setInfo"
QT_MOC_LITERAL(5, 42, 11), // "confirmQuit"
QT_MOC_LITERAL(6, 54, 16), // "changePlayerName"
QT_MOC_LITERAL(7, 71, 14), // "savePlayerName"
QT_MOC_LITERAL(8, 86, 10), // "setBackInv"
QT_MOC_LITERAL(9, 97, 9) // "setFWDInv"

    },
    "FightingGame\0setMainMenu\0\0setGame\0"
    "setInfo\0confirmQuit\0changePlayerName\0"
    "savePlayerName\0setBackInv\0setFWDInv"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FightingGame[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
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

void FightingGame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FightingGame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setMainMenu(); break;
        case 1: _t->setGame(); break;
        case 2: _t->setInfo(); break;
        case 3: _t->confirmQuit(); break;
        case 4: _t->changePlayerName(); break;
        case 5: _t->savePlayerName(); break;
        case 6: _t->setBackInv(); break;
        case 7: _t->setFWDInv(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject FightingGame::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_FightingGame.data,
    qt_meta_data_FightingGame,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FightingGame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FightingGame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FightingGame.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FightingGame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
