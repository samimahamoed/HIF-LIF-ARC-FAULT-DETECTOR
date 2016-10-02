#include "mainwindow.h"
#include "typedef.h"
#include "define.h"

Int8 MainWindow::make8(QString buf, Uint16 idx)
{
    qint8 result;
    result = ((HEX2VAL((qint8)buf[idx+0].toLatin1())<<4)|(HEX2VAL((qint8)buf[idx+1].toLatin1()))<<0);
    return result;
}


Int16  MainWindow::make16(QString buf, Uint16 idx)
{
    qint16 result = ((HEX2VAL((qint16)buf[idx+0].toLatin1())<<12)|
            (HEX2VAL((qint16)buf[idx+1].toLatin1())<<8)|
            (HEX2VAL((qint16)buf[idx+2].toLatin1())<<4)|
            (HEX2VAL((qint16)buf[idx+3].toLatin1())<<0));

    return result;
}


