
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include "recordpreview.h"

#include "define.h"

#include <QDebug>
#include <QThread>
#include <QTimer>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>



/*! Table of CRC values for high–order byte */
static Uint8 table_crc_hi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

/*! Table of CRC values for low–order byte */
static Uint8 table_crc_lo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;


void MainWindow::openSerialPort()
{
    state=0;
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    if (serial->open(QIODevice::ReadWrite)) {
        if (serial->setBaudRate(p.baudRate)
                && serial->setDataBits(p.dataBits)
                && serial->setParity(p.parity)
                && serial->setStopBits(p.stopBits)
                && serial->setFlowControl(p.flowControl)) {

            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);

            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));


           rc_buffer.clear();

           ui->dockWidget->setWidget(ui->terminal);

           ui->dock_central->setVisible(false);
           ui->centralWidget->setVisible(false);
           ui->dockWidget->setVisible(true);

           this->addDockWidget(Qt::RightDockWidgetArea,ui->dockWidget);
           ui->actionClear->setVisible(true);
           ui->dockWidget_sen_cmd->setEnabled(true);

        } else {
            serial->close();
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
        }
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Configure error"));
    }
}

void MainWindow::closeSerialPort()
{
    serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));


    ui->dockWidget->setVisible(false);


   ui->centralWidget->setVisible(false);
   ui->dock_central->setVisible(true);

   ui->actionClear->setVisible(false);
      rc_buffer.clear();

}


void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

bool MainWindow::comm_Write(char *msg){

    QByteArray data =msg;
    if(serial->write(data)!=-1)
        return true;
    else
        return false;
}



void MainWindow::incoming_data_listener()
{

    QByteArray rc_data = serial->readLine();
    ui->terminal->putData(rc_data);
    rc_buffer.append(rc_data);


    if(rc_buffer.contains("{") && rc_buffer.contains("}\r\n") /*full msg*/)
    {

        QByteArray resp_message = rc_buffer;
        rc_buffer.clear();
        Uint16          frameIdx;
        bool            frameOk=false;
        QString         frame;
        Uint16          kk;



             if(resp_message.length() > 0)
             {
                  for( kk = 0; kk < resp_message.length();kk++)
                  {
                      Uint8 cc = resp_message[kk];
                      frameIdx++;

                      if(cc == '{'){
                                  frame    = cc;
                                  frameIdx = 0;
                      }else if( cc == '}')
                      {
                                  frame.append(cc);
                                  frameOk = true;
                                  break;
                      }
                      else
                      {

                                   if(( ((cc >= '0')&&(cc <= '9')) || ((cc >= 'A')&&(cc <= 'F')) || ( cc == ':') ))
                                   {
                                       frame.append(cc);
                                   }
                                   else
                                   {
                                       frameIdx = 0;
                                   }

                      }

                 }
            }
       



        if(frameOk == true)
        {

            /*! count reliable device response to make sure proper
             *  communication is available the resulting value will
             *  be used by supervission function */
            last_cmd.device_response_cnt++;
            Uint16  arg_cnt      =   0;
            Uint8   channel      =   0;
            Uint16  sensor_id    =   0;
            QString resp         =   NULL;

            unsigned int  resp_cmd     =   0;
            unsigned int  crc          =   0;


          QStringList replay     = frame.split(':');


          if(replay.length() == 3){
          QString     sub_frame  = replay.at(0);

          sub_frame.remove("{");

          sub_frame.remove(":");

          arg_cnt = sscanf(sub_frame.toLatin1(),"%04X",&resp_cmd);

          if(arg_cnt == 1 && resp_cmd == last_cmd.cmd)
              resp = replay.at(1);

          resp.remove(":");

          channel    = make8(resp.toLatin1(),0);

          sensor_id  = make8(resp.toLatin1(),2);


          if(channel != last_cmd.arg[0])
              resp.append("\n#Error:target sensor channel mismatch");
          if(sensor_id != last_cmd.arg[1])
              resp.append("\n#Error:target sensor id mismatch");

          QString substr = replay.at(2);
           substr.remove("}");
           substr.remove(":");


          if(sscanf(substr.toLatin1(),"%04X",&crc)==1)
              /*! short messages may use manual mode crc, the data is assumed
                 to be valid if the expected messages are received with
                 the correct sequence*/


              if((crc != 0XCCCC /*manula mode crc value*/)){
              if(crc != crc16((Uint8 *)frame.toLocal8Bit().data(),frame.length()-6))
                resp.append("\n#Error:crc mismatch");
              }
          }else
              resp = "\n#Error:Frame error ";


              emit response_ready(resp);
        }
    }

}


void MainWindow::serial_supervisor(){

    if(last_cmd.device_response_cnt == 0){
        cancel_processing_cmd(true);

        ui->terminal->textCursor().insertText(
       QString("\nNo response from the serial device ")
       + QString("\nCommand execution canceled")
                    );

        last_cmd.processed = true;
   }else if(last_cmd.processed == false){

        last_cmd.device_response_cnt = 0;

        QTimer::singleShot(30000, this, SLOT(serial_supervisor()));

    }
}



Uint16 MainWindow::crc16(Uint8 *puchMsg, Uint16 usDataLen)
{
    Uint8 crc_hi = 0xFF ;        /* high byte of CRC initialized */
    Uint8 crc_lo = 0xFF ;        /* low byte of CRC initialized */
    Uint16 table_index ;            /* index into CRC lookup table */

    while (usDataLen--)
    {
        table_index = crc_hi ^ *puchMsg++ ;            /* calculate the CRC */
        crc_hi = crc_lo ^ table_crc_hi[table_index] ;
        crc_lo = table_crc_lo[table_index] ;
    }
    return (crc_hi << 8 | crc_lo) ;
}




