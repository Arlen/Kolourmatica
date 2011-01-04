/****************************************************************************
** Meta object code from reading C++ file 'ConversionConsole.hpp'
**
** Created: Mon Jan 3 04:02:42 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gui/ConversionConsole.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConversionConsole.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConversionConsole[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x08,
      44,   18,   18,   18, 0x08,
      69,   18,   18,   18, 0x08,
      94,   18,   18,   18, 0x08,
     121,   18,   18,   18, 0x08,
     146,   18,   18,   18, 0x08,
     173,   18,   18,   18, 0x08,
     200,   18,   18,   18, 0x08,
     227,   18,   18,   18, 0x08,
     253,   18,   18,   18, 0x08,
     279,   18,   18,   18, 0x08,
     306,   18,   18,   18, 0x08,
     331,   18,   18,   18, 0x08,
     363,   18,   18,   18, 0x08,
     389,   18,   18,   18, 0x08,
     414,   18,   18,   18, 0x08,
     448,   18,   18,   18, 0x08,
     474,   18,   18,   18, 0x08,
     504,   18,   18,   18, 0x08,
     534,   18,   18,   18, 0x08,
     562,   18,   18,   18, 0x08,
     588,   18,   18,   18, 0x08,
     619,   18,   18,   18, 0x08,
     636,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConversionConsole[] = {
    "ConversionConsole\0\0convertFrom_XYZ_To_all()\0"
    "convertFrom_xyY_To_all()\0"
    "convertFrom_Lab_To_all()\0"
    "convertFrom_LCHab_To_all()\0"
    "convertFrom_Luv_To_all()\0"
    "convertFrom_LCHuv_To_all()\0"
    "convertFrom_Adobe_To_all()\0"
    "convertFrom_Apple_To_all()\0"
    "convertFrom_Best_To_all()\0"
    "convertFrom_Beta_To_all()\0"
    "convertFrom_Bruce_To_all()\0"
    "convertFrom_CIE_To_all()\0"
    "convertFrom_ColorMatch_To_all()\0"
    "convertFrom_Don4_To_all()\0"
    "convertFrom_ECI_To_all()\0"
    "convertFrom_EktaSpacePS5_To_all()\0"
    "convertFrom_NTSC_To_all()\0"
    "convertFrom_PALSECAM_To_all()\0"
    "convertFrom_ProPhoto_To_all()\0"
    "convertFrom_SMPTEC_To_all()\0"
    "convertFrom_sRGB_To_all()\0"
    "convertFrom_WideGamut_To_all()\0"
    "setRefWhite(int)\0setAdaptationMethod(int)\0"
};

const QMetaObject ConversionConsole::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConversionConsole,
      qt_meta_data_ConversionConsole, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConversionConsole::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConversionConsole::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConversionConsole::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConversionConsole))
        return static_cast<void*>(const_cast< ConversionConsole*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConversionConsole::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: convertFrom_XYZ_To_all(); break;
        case 1: convertFrom_xyY_To_all(); break;
        case 2: convertFrom_Lab_To_all(); break;
        case 3: convertFrom_LCHab_To_all(); break;
        case 4: convertFrom_Luv_To_all(); break;
        case 5: convertFrom_LCHuv_To_all(); break;
        case 6: convertFrom_Adobe_To_all(); break;
        case 7: convertFrom_Apple_To_all(); break;
        case 8: convertFrom_Best_To_all(); break;
        case 9: convertFrom_Beta_To_all(); break;
        case 10: convertFrom_Bruce_To_all(); break;
        case 11: convertFrom_CIE_To_all(); break;
        case 12: convertFrom_ColorMatch_To_all(); break;
        case 13: convertFrom_Don4_To_all(); break;
        case 14: convertFrom_ECI_To_all(); break;
        case 15: convertFrom_EktaSpacePS5_To_all(); break;
        case 16: convertFrom_NTSC_To_all(); break;
        case 17: convertFrom_PALSECAM_To_all(); break;
        case 18: convertFrom_ProPhoto_To_all(); break;
        case 19: convertFrom_SMPTEC_To_all(); break;
        case 20: convertFrom_sRGB_To_all(); break;
        case 21: convertFrom_WideGamut_To_all(); break;
        case 22: setRefWhite((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: setAdaptationMethod((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
