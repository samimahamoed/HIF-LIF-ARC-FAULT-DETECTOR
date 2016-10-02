/****************************************************************************
** Meta object code from reading C++ file 'recordpreview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PROJECT/AQ0X/recordpreview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recordpreview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_recordPreview_t {
    QByteArrayData data[8];
    char stringdata[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_recordPreview_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_recordPreview_t qt_meta_stringdata_recordPreview = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 28),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 32),
QT_MOC_LITERAL(4, 77, 32),
QT_MOC_LITERAL(5, 110, 28),
QT_MOC_LITERAL(6, 139, 4),
QT_MOC_LITERAL(7, 144, 29)
    },
    "recordPreview\0on_actionCreateCSV_triggered\0"
    "\0on_actionData_Plot_pdf_triggered\0"
    "on_actionData_plot_img_triggered\0"
    "on_actionWinDataPlot_toggled\0arg1\0"
    "on_actionWinEventPlot_toggled\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_recordPreview[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08,
       3,    0,   40,    2, 0x08,
       4,    0,   41,    2, 0x08,
       5,    1,   42,    2, 0x08,
       7,    1,   45,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void recordPreview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        recordPreview *_t = static_cast<recordPreview *>(_o);
        switch (_id) {
        case 0: _t->on_actionCreateCSV_triggered(); break;
        case 1: _t->on_actionData_Plot_pdf_triggered(); break;
        case 2: _t->on_actionData_plot_img_triggered(); break;
        case 3: _t->on_actionWinDataPlot_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_actionWinEventPlot_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject recordPreview::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_recordPreview.data,
      qt_meta_data_recordPreview,  qt_static_metacall, 0, 0}
};


const QMetaObject *recordPreview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *recordPreview::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_recordPreview.stringdata))
        return static_cast<void*>(const_cast< recordPreview*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int recordPreview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
