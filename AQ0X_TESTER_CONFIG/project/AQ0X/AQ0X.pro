#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T15:31:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport printsupport

TARGET = AQ0X
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    console.cpp \
    settingsdialog.cpp \
    recordpreview.cpp \
    ../qcustomplot.cpp \
    srv_protocol.cpp \
    srv_serial.cpp \
    emu_data_editor.cpp \
    slider.cpp \
    srv_commands.cpp \
    srv_miscellaneous.cpp \
    ../fft/sources/qcomplexnumber.cpp \
    ../fft/sources/qfouriercalculator.cpp \
    ../fft/sources/qfourierfixedcalculator.cpp \
    ../fft/sources/qfouriertransformer.cpp \
    ../fft/sources/qfouriervariablecalculator.cpp \
    ../fft/sources/qwindowfunction.cpp

HEADERS  += mainwindow.h \
    console.h \
    settingsdialog.h \
    recordpreview.h \
    typedef.h \
    define.h \
    ../qcustomplot.h \
    emu_data_editor.h \
    slider.h \
    storage_structure.h \
    common_define_aq0x.h \
    ../fft/headers/qcomplexnumber.h \
    ../fft/headers/qfouriercalculator.h \
    ../fft/headers/qfourierfixedcalculator.h \
    ../fft/headers/qfouriertransformer.h \
    ../fft/headers/qfouriervariablecalculator.h \
    ../fft/headers/qwindowfunction.h \
    ../fft/fftreal/Array.h \
    ../fft/fftreal/Array.hpp \
    ../fft/fftreal/def.h \
    ../fft/fftreal/DynArray.h \
    ../fft/fftreal/DynArray.hpp \
    ../fft/fftreal/FFTReal.h \
    ../fft/fftreal/FFTReal.hpp \
    ../fft/fftreal/FFTRealFixLen.h \
    ../fft/fftreal/FFTRealFixLen.hpp \
    ../fft/fftreal/FFTRealFixLenParam.h \
    ../fft/fftreal/FFTRealPassDirect.h \
    ../fft/fftreal/FFTRealPassDirect.hpp \
    ../fft/fftreal/FFTRealPassInverse.h \
    ../fft/fftreal/FFTRealPassInverse.hpp \
    ../fft/fftreal/FFTRealSelect.h \
    ../fft/fftreal/FFTRealSelect.hpp \
    ../fft/fftreal/FFTRealUseTrigo.h \
    ../fft/fftreal/FFTRealUseTrigo.hpp \
    ../fft/fftreal/OscSinCos.h \
    ../fft/fftreal/OscSinCos.hpp

FORMS    += mainwindow.ui \
    settingsdialog.ui \
    recordpreview.ui \
    emu_data_editor.ui

RESOURCES += \
    logo.qrc \
    icons.qrc

RC_FILE = app.rc
