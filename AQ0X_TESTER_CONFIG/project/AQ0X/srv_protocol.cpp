/*! \file   protocol.cpp

    \brief  ....

*/


#include "mainwindow.h"
#include "define.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QThread>
#include <QMessageBox>



/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/


Int8 MainWindow::protocol_send_cmd(serial_msg_t current_cmd, Uint16 *resp, Uint16 wait_resp)
{




   Int16   result = -1;
   char    message[400];


   Uint16  ii;
   Uint16  kk;
   QByteArray resp_message;
   QString frame;
   Uint16  frameIdx;
   bool frameOk = false;



   frame.clear();
   resp_message.clear();


   last_cmd = current_cmd;


   Uint16 cmd = (((current_cmd.cmd&0x00F0)>>4)== 0xE)?0xE:
                (((current_cmd.cmd&0x0F00)>>8)==1)?1:
                (((current_cmd.cmd&0x0F00)>>8)==6)?6:            
                current_cmd.cmd;


 switch(cmd)
   {
       case 1/*zero argument messages*/:
       {

       sprintf(message,"[%04X:%02X%02X:",current_cmd.cmd,current_cmd.arg[0],current_cmd.arg[1]);

       sprintf(message+11,"%04X]",crc16((Uint8*)message,10));



       }break;
       
       
       case 0xE/*data download request message*/:
       {
    
       sprintf(message,"[%04X:%02X%02X%04X%04X:",current_cmd.cmd,current_cmd.arg[0],current_cmd.arg[1],
                                                 current_cmd.arg[2],current_cmd.arg[3]);
    
       sprintf(message+19,"%04X]",crc16((Uint8*)message,18));

    
       }break;


       case 6 /*data transfer*/:
       {

       const char cnv[] = "0123456789ABCDEF";

       memset(message,0,sizeof(message));

       sprintf(message,"[%04X:%02X%02X%04X%04X",current_cmd.cmd,
                                            current_cmd.arg[0],current_cmd.arg[1],
                                            current_cmd.arg[2],current_cmd.arg[3]);

       Uint16 var;
       Uint16 cc = 0;
       Uint16 offset = current_cmd.arg[2];

       Uint8 * data = (Uint8 *)(current_cmd.attachment);

       for (var = 18; (var-18) < current_cmd.arg[3]*2;) {

          Uint16 val = (((Uint16)(*(data + offset + cc++)))>>0)|
                  (((Uint16)(*(data + offset + cc++)))<<8);
          sprintf(message+var,"%04X",val);

          var+=4;




       }

       message[var++]=':';

       sprintf(message+var,"%04X]",crc16((Uint8*)message,var-1));




       }break;



       case CMD_INSTALLATION_DITAILS:
       {
       sprintf(message,"[%04X:%02X%02X%04X%04X%04X%04X%04X%04X:CCCC]",current_cmd.cmd,
                                    current_cmd.arg[0],current_cmd.arg[1],
                                    current_cmd.arg[2],current_cmd.arg[3],
                                    current_cmd.arg[4],current_cmd.arg[5],
                                    current_cmd.arg[6],current_cmd.arg[7]

                                    );

       sprintf(message+35,"%04X]",crc16((Uint8*)message,34));

       }break;



       case CMD_SIGNAL_THRESHOLDS:
       {
       sprintf(message,"[%04X:%02X%02X%04X%04X%04X%04X:CCCC]",current_cmd.cmd,
                                    current_cmd.arg[0],current_cmd.arg[1],
                                    current_cmd.arg[2],current_cmd.arg[3],
                                    current_cmd.arg[4],current_cmd.arg[5]
                                    );


       sprintf(message+27,"%04X]",crc16((Uint8*)message,26));

       }break;



       case CMD_EVENT_ANALYSIS:
       {
       sprintf(message,"[%04X:%02X%02X%04X%04X%04X%04X%04X%04X%04X:CCCC]",current_cmd.cmd,
                                    current_cmd.arg[0],current_cmd.arg[1],
                                    current_cmd.arg[2],current_cmd.arg[3],
                                    current_cmd.arg[4],current_cmd.arg[5],
                                    current_cmd.arg[6],current_cmd.arg[7]

                                    );

       sprintf(message+39,"%04X]",crc16((Uint8*)message,38));

       }break;



   case CMD_RUN_TIME_OPTIONS:
       {
       sprintf(message,"[%04X:%02X%02X%04X%04X%04X:CCCC]",current_cmd.cmd,
                                    current_cmd.arg[0],current_cmd.arg[1],
                                    current_cmd.arg[2],current_cmd.arg[3],
                                    current_cmd.arg[4]

                                    );

       sprintf(message+23,"%04X]",crc16((Uint8*)message,22));

       }break;


       default:
       {

           return -1;
       }
   }



   for( ii = 0; ii < MAX_RE_TX_ATTEMPT;ii++)
   {
      serial->clear();
      if(comm_Write(message)){

          ui->terminal->textCursor().insertText(cmd_line_separator + QString(message) + "\n");

          if(wait_resp==0)
              return 0;

          result = 0;
          disconnect(serial, SIGNAL(readyRead()), this, SLOT(incoming_data_listener()));
              rc_buffer.clear();
              if(serial->waitForReadyRead(wait_resp) == true)
              {


                     bool dataReady = false;
                     resp_message.clear();
                     resp_message.append(serial->readAll());

                     if(rc_buffer.contains("}\r\n"))
                         dataReady = true;


                   while (!dataReady) {

                       QThread::msleep(wait_resp);
                       /*
                       if(serial->bytesAvailable()>0){
                        resp_message.append(serial->readAll());
                       }*/

                       if(rc_buffer.contains("}\r\n"))
                           dataReady = true;

                   }
                    resp_message = rc_buffer;


                   frameOk = false;

                   if(resp_message.length() > 0)
                   {
                        for( kk = 0; kk < resp_message.length();kk++)
                        {
                            qint8 cc = resp_message[kk];


                            if(cc == '{')
                                frame = cc;
                            else if( cc == '}')
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
             }


   }

      connect(serial, SIGNAL(readyRead()), this, SLOT(incoming_data_listener()));
      return result;
}

   return -1;
}

void MainWindow::Success(...)
{

}

void MainWindow::Failure(...)
{

}


void MainWindow::srv_cmds_general_msg_handler(QString data){

uint  channel   = 0;
uint  sensor_id = 0;
uint  arg       = 0;

    if(data.contains("#Error"))
    {

         QString result =  QString("\n#Error: data verification error \n ");
         ui->terminal->textCursor().insertText(result+"\n"+data);


             if(last_cmd.multi_address){

                 QString Question =
                         QString("The response from the selected sensor (SEN address = ")
                         +QString::number(last_cmd.arg[1])
                         +QString(") contains error .\n")
                         +QString("Do you want to run the command again ?");

                 QMessageBox msgBox;
                 msgBox.setWindowIcon(QMainWindow::windowIcon());
                 msgBox.setWindowTitle(QMainWindow::windowTitle());
                 msgBox.setText(Question);
                 msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


                 if(msgBox.exec() == QMessageBox::No){
                 last_cmd.processed = true;
                 multi_cmds[last_cmd.vPos].varified = true;
                 multi_cmds[last_cmd.vPos].gui.pBar->setStyleSheet(PBAR_FAILURE);
                 if((last_cmd.vPos < (multi_cmds.length() - 1)))
                     last_cmd.vPos++;
                 else{

                     return;
                  }
                 }


                 last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;


                 multi_cmds[last_cmd.vPos].gui.pBar->setValue(20);

                 srv_cmds_execute(last_cmd/*now current cmd*/);

             }else{
                 last_cmd.processed = true;
             }

             return;
     }



    if(sscanf(data.toLatin1(),"%02X%02X%04X",&channel,&sensor_id,&arg) == 3){

       if(arg == ACK){

                    last_cmd.processed = true;
                    QString result =  QString("Command execution verified\n");
                    ui->terminal->textCursor().insertText(result);

                    if(last_cmd.multi_address){
                         multi_cmds[last_cmd.vPos].gui.pBar->setValue(100);
                         multi_cmds[last_cmd.vPos].varified = true;
                     if(last_cmd.vPos < (multi_cmds.length()-1)){
                        last_cmd.vPos++;
                        last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;
                        srv_cmds_execute(last_cmd/*now current cmd*/);
                    }else
                        ui->btn_execute_cmd->setEnabled(true);

                    }




       }
       else if(arg == ARG_PROCESSING_CMD_WAIT_MSG)
       {

                QString result =  QString("Processing command, Please Wait ...\n ");
                ui->terminal->textCursor().insertText(result);

       }
       if(arg == ARG_SEN_COMM_ERROR_NO_RESPONSE)
       {

            QString result =  QString("#Communication Error, Sensor not responding\n ");
            ui->terminal->textCursor().insertText(result);


                if(last_cmd.multi_address){

                    QString Question =
                            QString("Communication error, the selected sensor (SEN address = ")
                            +QString::number(multi_cmds[last_cmd.vPos].address)
                            +QString(") is not responding.\n")
                            +QString("Do you want to try again ?");

                    QMessageBox msgBox;
                    msgBox.setWindowIcon(QMainWindow::windowIcon());
                    msgBox.setWindowTitle(QMainWindow::windowTitle());
                    msgBox.setText(Question);
                    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


                    if(msgBox.exec() == QMessageBox::No){
                    last_cmd.processed = true;
                    multi_cmds[last_cmd.vPos].varified = true;
                    multi_cmds[last_cmd.vPos].gui.pBar->setStyleSheet(PBAR_FAILURE);
                    if((last_cmd.vPos < (multi_cmds.length() - 1)))
                        last_cmd.vPos++;
                    else{

                        return;
                     }
                    }


                    last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;


                    multi_cmds[last_cmd.vPos].gui.pBar->setValue(20);

                    srv_cmds_execute(last_cmd/*now current cmd*/);

                }else{
                    last_cmd.processed = true;
                }
        }




     }


}


void MainWindow::srv_cmds_data_transfer_msg_handler(QString data){

    unsigned int  channel               = 0;
    unsigned int  sensor_id             = 0;
    unsigned int  arg                   = 0;
    unsigned int  copied_data_length    = 0;

        if(data.contains("#Error"))
        {
             bool repeat = true;
             QString result =  QString("\n#Error: data verification error \n ");
             ui->terminal->textCursor().insertText(result+"\n"+data);

             QString Question =
                     QString("The response from the selected sensor (SEN address = ")
                     +QString::number(last_cmd.arg[1])
                     +QString(") contains error .\n")
                     +QString("Do you want to run the command again ?");

             QMessageBox msgBox;
             msgBox.setWindowIcon(QMainWindow::windowIcon());
             msgBox.setWindowTitle(QMainWindow::windowTitle());
             msgBox.setText(Question);
             msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


             if(msgBox.exec() == QMessageBox::No)
                      repeat = false;

                 if(last_cmd.multi_address){

                     if(!repeat){
                     last_cmd.processed = true;
                     multi_cmds[last_cmd.vPos].varified = true;
                     multi_cmds[last_cmd.vPos].gui.pBar->setStyleSheet(PBAR_FAILURE);
                     last_cmd.arg[2]=0;//reset offset to zero for new data transmission
                     last_cmd.arg[3]= ((MAX_ATTACHMENT_LENGTH-(last_cmd.attachment_length))<0)?
                                          MAX_ATTACHMENT_LENGTH:last_cmd.attachment_length;
                     if((last_cmd.vPos < (multi_cmds.length() - 1)))
                         last_cmd.vPos++;
                     else{

                         return;
                      }
                     }


                     last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;

                     multi_cmds[last_cmd.vPos].gui.pBar->setValue(20);

                     srv_cmds_execute(last_cmd/*now current cmd*/);

                 }else{

                     if(repeat){
                        //simply repeat the previous command
                        srv_cmds_execute(last_cmd/*now current cmd*/);
                        return;
                     }


                     last_cmd.processed = true;

                     //!TODO: clean up procedure 
                 }


            return;
         }

        if(data.length() == 8 && sscanf(data.toLatin1(),"%02X%02X%04X",&channel,&sensor_id,&arg) == 3)
        {

            srv_cmds_general_msg_handler(data);
            return;
        }


        if(sscanf(data.toLatin1(),"%02X%02X%04X%04X",&channel,&sensor_id,&arg,&copied_data_length) == 4)
        {

           if(arg == ACK){

               if(copied_data_length == last_cmd.attachment_length){
                   last_cmd.processed = true;
                   QString result =  QString("Command execution verified\n");
                   ui->terminal->textCursor().insertText(result);

                   if(last_cmd.multi_address){
                        multi_cmds[last_cmd.vPos].gui.pBar->setValue(100);
                        multi_cmds[last_cmd.vPos].varified = true;
                    if(last_cmd.vPos < (multi_cmds.length()-1)){
                       last_cmd.vPos++;
                       last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;
                       last_cmd.arg[2] = 0;//data transfer starts fresh
                       last_cmd.arg[3]= ((MAX_ATTACHMENT_LENGTH-(last_cmd.attachment_length))<0)?
                                          MAX_ATTACHMENT_LENGTH:last_cmd.attachment_length;
                       srv_cmds_execute(last_cmd/*now current cmd*/);
                   }else
                       ui->btn_execute_cmd->setEnabled(true);

                   }


               }else if(copied_data_length < last_cmd.attachment_length){
                   Uint16 remaining_data_length = last_cmd.attachment_length - copied_data_length;

                   QString result =  QString("data transfer verification\n");
                   ui->terminal->textCursor().insertText(result);

                   result =  QString("Copied data length :"+QString::number(copied_data_length)+" byte\n");
                   ui->terminal->textCursor().insertText(result);

                   result =  QString("Remaining data length "+QString::number(remaining_data_length)+" byte\n");
                   ui->terminal->textCursor().insertText(result);




                   //TODO: check copid data length is valid  (very important)

                   last_cmd.arg[2] = copied_data_length;  //sets current offset

                   last_cmd.arg[3] = ((MAX_ATTACHMENT_LENGTH-remaining_data_length)<0)?
                                          MAX_ATTACHMENT_LENGTH:remaining_data_length;

                   srv_cmds_execute(last_cmd/*now current cmd*/);
               }else{


                   qWarning() <<"Unexpected result, considered as data transfer error"
                              <<"in MainWindow::srv_cmds_data_transfer_msg_handler";
               }

           }else{

               qWarning() <<"Unexpected argument received, considered as data transfer error"
                          <<"in MainWindow::srv_cmds_data_transfer_msg_handler";

           }



         }



}


void MainWindow::srv_cmds_data_download_msg_handler(QString data){

    
    uint  channel              = 0;
    uint  sensor_id            = 0;
    uint  arg                  = 0;
    uint  copied_data_length   = 0;
    char attachment[2*MAX_ATTACHMENT_LENGTH];

    memset(attachment,0,sizeof(attachment));

        if(data.contains("#Error"))
        {
             bool repeat = true;
             QString result =  QString("\n#Error: data verification error \n ");
             ui->terminal->textCursor().insertText(result+"\n"+data);

             QString Question =
                     QString("The response from the selected sensor (SEN address = ")
                     +QString::number(last_cmd.arg[1])
                     +QString(") contains error .\n")
                     +QString("Do you want to run the command again ?");

             QMessageBox msgBox;
             msgBox.setWindowIcon(QMainWindow::windowIcon());
             msgBox.setWindowTitle(QMainWindow::windowTitle());
             msgBox.setText(Question);
             msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


             if(msgBox.exec() == QMessageBox::No)
                      repeat = false;

                 if(last_cmd.multi_address){

                     if(!repeat){
                     last_cmd.processed = true;
                     multi_cmds[last_cmd.vPos].varified = true;
                     multi_cmds[last_cmd.vPos].gui.pBar->setStyleSheet(PBAR_FAILURE);
                     
                     last_cmd.arg[2]=0;//reset offset to zero for new data transmission
                       
                     last_cmd.data_download.total_size.copied       = 0;
                     
                     last_cmd.data_download.offset                  = 0;
                     
                     last_cmd.arg[3]              
                             = ((MAX_ATTACHMENT_LENGTH-(last_cmd.data_download.total_size.expected))<0)?
                                 MAX_ATTACHMENT_LENGTH:last_cmd.data_download.total_size.expected;
                     
                     if((last_cmd.vPos < (multi_cmds.length() - 1)))
                         last_cmd.vPos++;
                     else{
                         //!TODO: clean up procedure 
                         return;
                      }
                     }


                     last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;

                     multi_cmds[last_cmd.vPos].gui.pBar->setValue(20);

                     srv_cmds_execute(last_cmd/*now current cmd*/);

                 }else{

                     if(repeat){
                        //simply repeat the previous command
                        srv_cmds_execute(last_cmd/*now current cmd*/);
                        return;
                     }


                     last_cmd.processed = true;

                     //!TODO: clean up procedure 
                 }


            return;
         }

        if((data.length() == 8) && sscanf(data.toLatin1(),"%02X%02X%04X",&channel,&sensor_id,&arg) == 3)
        {

            srv_cmds_general_msg_handler(data);
            return;
        }
       
        

        if(sscanf(data.toLatin1(),"%02X%02X%04X%04X%s",&channel,&sensor_id,&arg,&copied_data_length,attachment) == 5)
        {

            //TODO:
           /*
           if(copied_data_length != sizeof(attachment)){

               retransmission("#Error: Received data  validation ");
               return;
           }*/


           if(arg == ACK){

               if(copied_data_length-last_cmd.data_download.offset == last_cmd.arg[3]/*requested data length*/)
               {
               if(msg_to_mem(
                      attachment,
                      (((Uint8 *)last_cmd.data_download.storage_location)+last_cmd.data_download.offset),
                      last_cmd.arg[3]
                          )){
                   last_cmd.data_download.total_size.copied = copied_data_length;

                  }else
                  if(last_cmd.data_download.total_size.copied+last_cmd.arg[3] != copied_data_length){
                    Failure();
                  }else{
                   
                   qCritical() <<"Unable to copy received data to memory "
                               <<"in MainWindow::srv_cmds_data_download_msg_handler";
                
                   exit(1);
                   
                  }
               }else{
                   
                  retransmission("#Error: Received data  validation ");
                  return;
               }
               
               if(copied_data_length == last_cmd.data_download.total_size.expected){
                   last_cmd.processed = true;
                   QString result =  QString("Command execution verified\n");
                   ui->terminal->textCursor().insertText(result);

                   
                   record_preview(last_cmd);
                   
                   if(last_cmd.multi_address){
                        multi_cmds[last_cmd.vPos].gui.pBar->setValue(100);
                        multi_cmds[last_cmd.vPos].varified = true;
                    if(last_cmd.vPos < (multi_cmds.length()-1)){
                       last_cmd.vPos++;
                       last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;
                       last_cmd.arg[2] = 0;//data transfer starts fresh
                       
                       last_cmd.data_download.total_size.copied     = 0;
                       
                       last_cmd.data_download.offset                = 0;
                       
                       last_cmd.arg[3]              
                               = ((MAX_ATTACHMENT_LENGTH-(last_cmd.data_download.total_size.expected))<0)?
                                   MAX_ATTACHMENT_LENGTH:last_cmd.data_download.total_size.expected;

                       srv_cmds_execute(last_cmd/*now current cmd*/);
                   }else
                       ui->btn_execute_cmd->setEnabled(true);

                   }

               }else if(copied_data_length < last_cmd.data_download.total_size.expected){
                   Uint16 remaining_data_length 
                           = last_cmd.data_download.total_size.expected 
                           - last_cmd.data_download.total_size.copied;

                   last_cmd.data_download.offset = last_cmd.data_download.total_size.copied;
                   last_cmd.arg[2] = last_cmd.data_download.offset;  //sets current offset

                   last_cmd.arg[3] = ((MAX_ATTACHMENT_LENGTH-remaining_data_length)<0)?
                                          MAX_ATTACHMENT_LENGTH:remaining_data_length;

                   srv_cmds_execute(last_cmd/*now current cmd*/);
               }else{

                   
                   qWarning() <<"Unexpected result, considered as data transfer error"
                              <<"in MainWindow::srv_cmds_data_transfer_msg_handler";
                   
                   
                   retransmission("#Error: Received data  validation");
                   return;
               }

           }else{

               qWarning() <<"Unexpected argument received, considered as data transfer error"
                          <<"in MainWindow::srv_cmds_data_transfer_msg_handler";
               
               
               retransmission("#Error: Received data  validation");
               return;

           }



         }


    

}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/

void MainWindow::retransmission(QString error){

    bool repeat = true;
    
    Uint16 cmd = (((last_cmd.cmd&0x00F0)>>4)==0xE)?0xE:
                 (((last_cmd.cmd&0x0F00)>>8)==1)?1:
                 (((last_cmd.cmd&0x0F00)>>8)==6)?6:
                 last_cmd.cmd;
    
    ui->terminal->textCursor().insertText(error);

    QString Question =
            QString("The response from the selected sensor (SEN address = ")
            +QString::number(last_cmd.arg[1])
            +QString(") contain critical error .\n")
            +QString("one possible cause is communication error, Do you want to run the command again ?");

    QMessageBox msgBox;
    msgBox.setWindowIcon(QMainWindow::windowIcon());
    msgBox.setWindowTitle(QMainWindow::windowTitle());
    msgBox.setText(Question);
    msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);


    if(msgBox.exec() == QMessageBox::No)
             repeat = false;

        if(last_cmd.multi_address){

            if(!repeat){
            last_cmd.processed = true;
            multi_cmds[last_cmd.vPos].varified = true;
            multi_cmds[last_cmd.vPos].gui.pBar->setStyleSheet(PBAR_FAILURE);
            
            
            if(cmd == 6 || cmd == 0xE){
            last_cmd.arg[2]=0;//reset offset to zero for new data transmission
              
            if(cmd == 6){
            last_cmd.arg[3] = ((MAX_ATTACHMENT_LENGTH-(last_cmd.attachment_length))<0)?
                              MAX_ATTACHMENT_LENGTH:last_cmd.attachment_length;

                
            }else{
            last_cmd.data_download.total_size.copied     = 0;
            
            last_cmd.data_download.offset   = 0;
            
            last_cmd.arg[3]              
                    = ((MAX_ATTACHMENT_LENGTH-(last_cmd.data_download.total_size.expected))<0)?
                        MAX_ATTACHMENT_LENGTH:last_cmd.data_download.total_size.expected;
            }
            }
            if((last_cmd.vPos < (multi_cmds.length() - 1)))
                last_cmd.vPos++;
            else{
                //!TODO: clean up procedure 
                return;
             }
            }


            last_cmd.arg[1] = multi_cmds[last_cmd.vPos].address;

            multi_cmds[last_cmd.vPos].gui.pBar->setValue(20);

            srv_cmds_execute(last_cmd/*now current cmd*/);

        }else{

            if(repeat){
               //simply repeats the previous command
               srv_cmds_execute(last_cmd/*now current cmd*/);
               return;
            }


            last_cmd.processed = true;

            //!TODO: clean up procedure 
        }

    

}


bool MainWindow::msg_to_mem(char * msg,void* dest,Uint16 size){

    bool result = true;
    try {

        Uint8* dst8= (Uint8*)dest;
        Uint16 cc   = 0;

        for (int var = 0; var < size; ++var) {
            *(dst8+var)= make8(msg,cc);
            cc +=2;
        }


    } catch (...) {
        result = false;
    }


    return result;

}



