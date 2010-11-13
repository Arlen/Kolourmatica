/****************************************************************************
** Meta object code from reading C++ file 'ConversionConsole.hpp'
**
** Created: Sat Nov 13 02:29:04 2010
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
    "ConversionConsole\0\0ConvertFrom_XYZ_To_all()\0"
    "ConvertFrom_xyY_To_all()\0"
    "ConvertFrom_Lab_To_all()\0"
    "ConvertFrom_LCHab_To_all()\0"
    "ConvertFrom_Luv_To_all()\0"
    "ConvertFrom_LCHuv_To_all()\0"
    "ConvertFrom_Adobe_To_all()\0"
    "ConvertFrom_Apple_To_all()\0"
    "ConvertFrom_Best_To_all()\0"
    "ConvertFrom_Beta_To_all()\0"
    "ConvertFrom_Bruce_To_all()\0"
    "ConvertFrom_CIE_To_all()\0"
    "ConvertFrom_ColorMatch_To_all()\0"
    "ConvertFrom_Don4_To_all()\0"
    "ConvertFrom_ECI_To_all()\0"
    "ConvertFrom_EktaSpacePS5_To_all()\0"
    "ConvertFrom_NTSC_To_all()\0"
    "ConvertFrom_PALSECAM_To_all()\0"
    "ConvertFrom_ProPhoto_To_all()\0"
    "ConvertFrom_SMPTEC_To_all()\0"
    "ConvertFrom_sRGB_To_all()\0"
    "ConvertFrom_WideGamut_To_all()\0"
    "SetRefWhite(int)\0SetAdaptationMethod(int)\0"
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
        case 0: ConvertFrom_XYZ_To_all(); break;
        case 1: ConvertFrom_xyY_To_all(); break;
        case 2: ConvertFrom_Lab_To_all(); break;
        case 3: ConvertFrom_LCHab_To_all(); break;
        case 4: ConvertFrom_Luv_To_all(); break;
        case 5: ConvertFrom_LCHuv_To_all(); break;
        case 6: ConvertFrom_Adobe_To_all(); break;
        case 7: ConvertFrom_Apple_To_all(); break;
        case 8: ConvertFrom_Best_To_all(); break;
        case 9: ConvertFrom_Beta_To_all(); break;
        case 10: ConvertFrom_Bruce_To_all(); break;
        case 11: ConvertFrom_CIE_To_all(); break;
        case 12: ConvertFrom_ColorMatch_To_all(); break;
        case 13: ConvertFrom_Don4_To_all(); break;
        case 14: ConvertFrom_ECI_To_all(); break;
        case 15: ConvertFrom_EktaSpacePS5_To_all(); break;
        case 16: ConvertFrom_NTSC_To_all(); break;
        case 17: ConvertFrom_PALSECAM_To_all(); break;
        case 18: ConvertFrom_ProPhoto_To_all(); break;
        case 19: ConvertFrom_SMPTEC_To_all(); break;
        case 20: ConvertFrom_sRGB_To_all(); break;
        case 21: ConvertFrom_WideGamut_To_all(); break;
        case 22: SetRefWhite((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: SetAdaptationMethod((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
