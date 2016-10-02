
/*! \file   cmd_sensor.cpp

    \brief  ....

*/


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "record_preview.h"

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/

void MainWindow::on_actionToolReadSensor_triggered(bool checked)
{
    if(checked){

        ui->dockWidget_ReadSensor->setWindowFlags(Qt::Window);
        ui->dockWidget_ReadSensor->show();
        ui->dockWidget_ReadSensor->raise();
        ui->dockWidget_ReadSensor->activateWindow();

      //  if(!ui->dockWidget_SelectSensor->isVisible())
           // ui->dockWidget_ReadSensor->setEnabled(false);
    }else{
        ui->dockWidget_ReadSensor->close();
    }


}


void MainWindow::on_actionSensor_Select_triggered(bool checked)
{

    if(checked){

        ui->dockWidget_SelectSensor->setWindowFlags(Qt::Window);
        ui->dockWidget_SelectSensor->show();
        ui->dockWidget_SelectSensor->raise();
        ui->dockWidget_SelectSensor->activateWindow();

        ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_select_ch_readSen));

        ui->readSen_ch_no_selection->setChecked(true);
        ui->readSen_ch_no_selection->setVisible(false);

        if(ui->dockWidget_ReadSensor->isVisible())
            ui->dockWidget_ReadSensor->setEnabled(true);
    }else{
        ui->dockWidget_SelectSensor->close();
    }
}



//! CHANNEL SELECTION
/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/

void MainWindow::on_readSen_ch1_clicked(bool checked)
{
    if(checked){
         ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->pageReadSenSelect_option_sen));
         ui->readSen_no_selection->setChecked(true);
         ui->readSen_no_selection->setVisible(false);
   }
}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_ch3_clicked(bool checked)
{
    if(checked){
        ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->pageReadSenSelect_option_sen));
        ui->readSen_no_selection->setChecked(true);
        ui->readSen_no_selection->setVisible(false);
  }
}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_ch2_clicked(bool checked)
{
    if(checked){
        if(checked){
             ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->pageReadSenSelect_option_sen));
             ui->readSen_no_selection->setChecked(true);
             ui->readSen_no_selection->setVisible(false);
       }

   }
}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_ch4_clicked(bool checked)
{
   if(checked){
       if(checked){
            ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->pageReadSenSelect_option_sen));
            ui->readSen_no_selection->setChecked(true);
            ui->readSen_no_selection->setVisible(false);
      }
   }
}


/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_sen_1_clicked(bool checked)
{

    if(checked){
         ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_selection_preview));

          ui->label_selection_preview->setText(selectedSenToStr());
    }


}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_sen_2_clicked(bool checked)
{

    if(checked){
         ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_selection_preview));
     ui->label_selection_preview->setText(selectedSenToStr());
    }

}

/*!
     \brief  Function

     \param [in]
     \param [in,out]
     \param [in,out]

*/
void MainWindow::on_readSen_sen_3_clicked(bool checked)
{

    if(checked){
         ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_selection_preview));

         ui->label_selection_preview->setText(selectedSenToStr());
     }

}


void MainWindow::on_readSen_back_senSelect_clicked()
{
    ui->readSen_no_selection->setChecked(true);
    ui->readSen_no_selection->setVisible(false);

  ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->pageReadSenSelect_option_sen));

}


void MainWindow::on_readSen_back_chSelect_clicked()
{
    ui->readSen_ch_no_selection->setChecked(true);
    ui->readSen_ch_no_selection->setVisible(false);

    ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_select_ch_readSen));

}

void MainWindow::on_readSenGet_btn_clicked()
{

       if((ui->readSenSelect_cmd_in->text().toLong() >= 0)&& !ui->readSenSelect_cmd_in->text().isEmpty() &&  !ui->readSenSelect_cmd_in->text().isNull())
           sen_cmd((ui->readSenSelect_cmd_in->text().toLong()));

}


void MainWindow::sen_cmd(int arg_val)
{

                int arg[1];
                 int resp;

                arg[0] = 0;            
                arg[0] |= (arg_val<<8);


                qDebug()<<"arg[0]---"<<arg[0];
                if(ui->readSen_ch1->isChecked())
                arg[0] |= (0x0010);
                else if(ui->readSen_ch2->isChecked())
                arg[0] |= (0x0020);
                else if(ui->readSen_ch3->isChecked())
                arg[0] |= (0x0030);
                else if(ui->readSen_ch4->isChecked())
                arg[0] |= (0x0040);

                if(ui->readSen_sen_1->isChecked())
                arg[0] |= (0x0001);
                else if(ui->readSen_sen_2->isChecked())
                arg[0] |= (0x0002);
                else if(ui->readSen_sen_3->isChecked())
                arg[0] |= (0x0003);



                 if(execute_command(SM_SEN_CONTROL_CMD,arg,1,&resp,true,NULL) == 0)
                 {
                     qDebug()<<"Success send";

                    Success();
                 }
                 else
                 {
                    Failure();
                 }


}

void MainWindow::on_readSen_getSr_no_clicked(bool checked)
{

     int arg[1];
     int * resp;

    arg[0] = 0;
    arg[0] |= (SR_NUM_REQUEST_CMD<<8);

    if(ui->readSen_ch1->isChecked())
    arg[0] |= (0x0010);
    else if(ui->readSen_ch2->isChecked())
    arg[0] |= (0x0020);
    else if(ui->readSen_ch3->isChecked())
    arg[0] |= (0x0030);
    else if(ui->readSen_ch4->isChecked())
    arg[0] |= (0x0040);

    if(ui->readSen_sen_1->isChecked())
    arg[0] |= (0x0001);
    else if(ui->readSen_sen_2->isChecked())
    arg[0] |= (0x0002);
    else if(ui->readSen_sen_3->isChecked())
    arg[0] |= (0x0003);

     if(execute_command(SM_SEN_CONTROL_CMD,arg,1,resp,true,NULL) == 0)
     {

          msg_u msg;
          msg.msg_all= make8(replay.toLatin1(),0);


          if(msg.msg_fields.cmd != SR_NUM_REQUEST_CMD){
              ui->terminal_win_read_cmd->textCursor().insertText(".");

              return;
          }


           replay = replay.remove(":00}");
           replay = replay.remove(0,2);

          if(replay.length()!=SERIAL_NUMBER_LENGTH){
             //   emit ui->readSen_getSr_no->clicked(true);

           }

          Uint32 sr_no = replay.toLong();

          ui->terminal_win_read_cmd->textCursor().insertText("\nResponse :");
          ui->terminal_win_read_cmd->textCursor().insertText("CH"+ QString::number(((arg[0]&0x00F0)>>4)) + " SEN" + QString::number(msg.msg_fields.sender_address) + "\n" );
          ui->terminal_win_read_cmd->textCursor().insertText(">> Serial Number :" +QString::number(sr_no)+"\n");


        Success();
     }
     else
     {
        Failure();
     }


}



void MainWindow::on_readSen_getSTATUS_Flag_clicked(bool checked)
{
    if(!checked)
        return;
       sen_cmd(STATUS_FLAG_REQUEST);
}



void MainWindow::on_readSen_getData_clicked(bool checked)
{

    int arg[1];
    int * resp;

   arg[0] = 0;
   arg[0] |= (DATA_REQUEST_CMD<<8);


   qDebug()<<"arg[0]---"<<arg[0];
   if(ui->readSen_ch1->isChecked())
   arg[0] |= (0x0010);
   else if(ui->readSen_ch2->isChecked())
   arg[0] |= (0x0020);
   else if(ui->readSen_ch3->isChecked())
   arg[0] |= (0x0030);
   else if(ui->readSen_ch4->isChecked())
   arg[0] |= (0x0040);

   if(ui->readSen_sen_1->isChecked())
   arg[0] |= (0x0001);
   else if(ui->readSen_sen_2->isChecked())
   arg[0] |= (0x0002);
   else if(ui->readSen_sen_3->isChecked())
   arg[0] |= (0x0003);


    if(execute_command(SM_SEN_CONTROL_CMD,arg,1,resp,false,NULL) == 0)
    {
      disconnect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
      while(serial->waitForReadyRead(1000));
      char data[DATA_PACKET_SIZE];
      msg_u msg;

      int length = serial->readLine(data,DATA_PACKET_SIZE);
      QString rc_data = QString(data);


      if(!rc_data.startsWith("{A102:") && !rc_data.endsWith("}\r\n")){

          //re emit signal may be?
          return;
      }

      rc_data = rc_data.remove("{A102:");
      msg.msg_all= make8(rc_data.toLatin1(),0);

      rc_data = rc_data.remove(0,2);
      sen_nov_data_u sen_data;

       ui->terminal_win_read_cmd->textCursor().insertText("\n\n-------------------------------\n");
      sen_data.sen_nov_data.serial_number =(((Uint32)make16(rc_data.toLatin1() + 6,PTR_SERIAL_NUMBER)<<16)| (Uint32)make16(rc_data.toLatin1(),PTR_SERIAL_NUMBER+4));
      ui->terminal_win_read_cmd->textCursor().insertText("Device Serial Number :" + QString::number(sen_data.sen_nov_data.serial_number) + "\n");
      sen_data.sen_nov_data.event_timestamp = (((Uint32)make16(rc_data.toLatin1(),PTR_EVENT_TIMESTAMP)<<16)| (Uint32)make16(rc_data.toLatin1(),PTR_EVENT_TIMESTAMP+4));
      ui->terminal_win_read_cmd->textCursor().insertText("Event Time    :" + QString::number(sen_data.sen_nov_data.event_timestamp) + "\n");

      sen_data.sen_nov_data.device_address = make8(rc_data.toLatin1(),PTR_DEVICE_ADDRESS);
      ui->terminal_win_read_cmd->textCursor().insertText("Device Address  :"+QString::number(sen_data.sen_nov_data.device_address) + "\n");

      sen_data.sen_nov_data.error.error_all = make8(rc_data.toLatin1(),PTR_ERROR);
      ui->terminal_win_read_cmd->textCursor().insertText("Error Flags  :"+QString::number(sen_data.sen_nov_data.error.error_all).toLatin1().toHex() + "\n");


      sen_data.sen_nov_data.config_serial_num = make8(rc_data.toLatin1(),PTR_SR_NUM_CONFIGURED);
      ui->terminal_win_read_cmd->textCursor().insertText("Serial Number configered:"+QString((sen_data.sen_nov_data.config_serial_num == 1)?"TRUE":"FALSE") + "\n");
      sen_data.sen_nov_data.config_device_address = make8(rc_data.toLatin1(),PTR_DEVICE_ADDRESS_CONFIGURED);
      ui->terminal_win_read_cmd->textCursor().insertText("Device Address configered:"+QString((sen_data.sen_nov_data.config_device_address==1)?"TRUE":"FALSE") + "\n");
      sen_data.sen_nov_data.config_timestamp = make8(rc_data.toLatin1(),PTR_TIMESTAMP_CONFIGURED);
      ui->terminal_win_read_cmd->textCursor().insertText("Configered Time configered:"+QString((sen_data.sen_nov_data.config_timestamp==1)?"TRUE":"FALSE") + "\n");
      sen_data.sen_nov_data.config_self_test_dis = make8(rc_data.toLatin1(),PTR_SELF_TEST_EN_DIS);
      ui->terminal_win_read_cmd->textCursor().insertText("Device self test enable :"+QString((sen_data.sen_nov_data.config_serial_num==1)?"FALSE":"TRUE") + "\n");

      int ii = 0,cc = 0;
      for(int ii = 0;ii< DATA_SIZE;ii++){
          sen_data.sen_nov_data.light_samples[ii] = make8(rc_data.toLatin1(),PTR_LIGHT_DATA + cc );cc +=2;
      }


      for(int ii = 0;ii< DATA_SIZE;ii++){
          sen_data.sen_nov_data.light_samples[ii] = make8(rc_data.toLatin1(),PTR_PRESSURE_DATA + cc );cc +=2;
      }

      sen_data.sen_nov_data.reset_count = make16(rc_data.toLatin1(),PTR_RESET_COUNT);

      ui->terminal_win_read_cmd->textCursor().insertText("Reset Counter   :"+QString::number(sen_data.sen_nov_data.reset_count) + "\n");
      sen_data.sen_nov_data.eeprom_usage_count = make16(rc_data.toLatin1(),PTR_RESET_COUNT);
      ui->terminal_win_read_cmd->textCursor().insertText("EEPROM usage  :"+QString::number(sen_data.sen_nov_data.eeprom_usage_count) + "\n");
      sen_data.sen_nov_data.crc = make16(rc_data.toLatin1(),PTR_RESET_COUNT);

      //crc check


        Success();

        connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

        recordPreview* prv = new recordPreview();

        prv->readBuffer(sen_data,eventRecoredList);
        prv->show();
        prv->raise();
        prv->activateWindow();

    }
    else
    {
       Failure();
    }

}


QString MainWindow::selectedSenToStr()
{

        QString selection = "Selected Sensor\n";

        if(ui->readSen_ch1->isChecked())
            selection += "Channel: 1\n";
        else if(ui->readSen_ch2->isChecked())
            selection += "Channel: 2\n";
        else if(ui->readSen_ch3->isChecked())
            selection += "Channel: 3\n";
        else if(ui->readSen_ch4->isChecked())
            selection += "Channel: 4\n";

        if(ui->readSen_sen_1->isChecked())
        selection += "Sensor Address : 1\n";
        else if(ui->readSen_sen_2->isChecked())
        selection += "Sensor Address : 2\n";
        else if(ui->readSen_sen_3->isChecked())
        selection += "Sensor Address : 3\n";

        return selection;




}


void MainWindow::on_selectTarget_Btn_clicked()
{
    ui->readSen_ch_no_selection->setChecked(true);
    ui->readSen_ch_no_selection->setVisible(false);

    ui->stack_widget_target_sen_select_option->setCurrentIndex(ui->stack_widget_target_sen_select_option->indexOf(ui->page_select_ch_readSen));

}

