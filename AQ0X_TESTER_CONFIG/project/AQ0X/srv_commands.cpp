#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "typedef.h"
#include "recordpreview.h"


#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>
#include <QProgressBar>
#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>

/*!
    \brief  Function map command values to string
            used for console command and display purpose



*/
void MainWindow::map_keys()
{

    map_cmd.insert("CONFIG",CMD_CONFIG);
    map_cmd.insert("DEVICE ADDRESS",CMD_DEVICE_ADDRESS);
    map_cmd.insert("INSTALLATION DITAILS",CMD_INSTALLATION_DITAILS);
    map_cmd.insert("SIGNAL THRESHOLDS",CMD_SIGNAL_THRESHOLDS);
    map_cmd.insert("EVENT ANALYSIS OPTIONS",CMD_EVENT_ANALYSIS);
    map_cmd.insert("RUN TIME OPTIONS",CMD_RUN_TIME_OPTIONS);
    map_cmd.insert("LOAD HIF REFERENCE DATA",CMD_LOAD_HIF_REFERENCE_DATA);
    map_cmd.insert("FACTORY DEFAULT",CMD_FACTORY_DEFAULT);
    map_cmd.insert("COMMIT",CMD_COMMIT);
    map_cmd.insert("EMULATION SETTING",CMD_EMULATION_SETTING);
    map_cmd.insert("LOAD IR DATA",CMD_LOAD_IR_DATA);
    map_cmd.insert("LOAD UV DATA",CMD_LOAD_UV_DATA);
    map_cmd.insert("LOAD MAGNETIC FIELD X DATA",CMD_LOAD_MAGNETIC_FIELD_X_DATA);
    map_cmd.insert("LOAD MAGNETIC FIELD Y DATA",CMD_LOAD_MAGNETIC_FIELD_Y_DATA);
    map_cmd.insert("EMU MODE SET",CMD_EMU_MODE_SET);
    map_cmd.insert("EMU MODE RESET",CMD_EMU_MODE_RESET);
    map_cmd.insert("CLEAR EMU DATA",CMD_CLEAR_EMU_DATA);
    map_cmd.insert("GET DATA",CMD_GET_DATA);
    map_cmd.insert("DEVICE DATA",CMD_DEVICE_DATA);
    map_cmd.insert("IR DISTURBANCE RECORD",CMD_IR_DISTURBANCE_RECORD);
    map_cmd.insert("UV DISTURBANCE RECORD",CMD_UV_DISTURBANCE_RECORD);
    map_cmd.insert("HIF_DIAGNOSIS_RECORD",CMD_HIF_DIAGNOSIS_RECORD);
    map_cmd.insert("MFIELD X  DISTURBANCE RECORD",CMD_MFIELD_X_DISTURBANCE_RECORD);
    map_cmd.insert("MFIELD Y  DISTURBANCE RECORD",CMD_MFIELD_Y_DISTURBANCE_RECORD);
    map_cmd.insert("IR CURRENT RECORD",CMD_IR_CURRENT_RECORD);
    map_cmd.insert("UV CURRENT RECORD",CMD_UV_CURRENT_RECORD);
    map_cmd.insert("MFIELD X  CURRENT RECORD",CMD_MFIELD_X_CURRENT_RECORD);
    map_cmd.insert("MFIELD Y  CURRENT RECORD",CMD_MFIELD_Y_CURRENT_RECORD);
    map_cmd.insert("MISCELLANEOUS",CMD_MISCELLANEOUS);
    map_cmd.insert("DEVICE RESET",CMD_DEVICE_RESET);
    map_cmd.insert("CLEAR EEPROM",CMD_CLEAR_EEPROM);
    map_cmd.insert("SUPERVISION STATUS",CMD_SUPERVISION_STATUS);


    map_size.insert(CMD_DEVICE_DATA,sizeof(device_data_t));
    map_size.insert(CMD_IR_DISTURBANCE_RECORD,sizeof(module_disturbance_record_t));
    map_size.insert(CMD_UV_DISTURBANCE_RECORD,sizeof(module_disturbance_record_t));
    map_size.insert(CMD_MFIELD_X_DISTURBANCE_RECORD,sizeof(module_disturbance_record_t));
    map_size.insert(CMD_MFIELD_Y_DISTURBANCE_RECORD,sizeof(module_disturbance_record_t));
    map_size.insert(CMD_IR_CURRENT_RECORD,sizeof(pre_act_record_buffer_t));
    map_size.insert(CMD_UV_CURRENT_RECORD,sizeof(pre_act_record_buffer_t));
    map_size.insert(CMD_MFIELD_X_CURRENT_RECORD,sizeof(pre_act_record_buffer_t));
    map_size.insert(CMD_MFIELD_Y_CURRENT_RECORD,sizeof(pre_act_record_buffer_t));
    map_size.insert(CMD_HIF_DIAGNOSIS_RECORD,sizeof(hif_diagonesis_records_t));



}



void MainWindow::on_btn_execute_cmd_clicked(bool checked)
{



    serial_msg_t     current_cmd;


    memset(&current_cmd,0,sizeof(current_cmd));
    current_cmd.attachment = NULL;
    current_cmd.multi_address = false;
    current_cmd.vPos = 0;

    if( ui->btn_execute_cmd->text().contains("Reset")){


        if(!last_cmd.processed){
            msg(MSG_INFO,"Please wait, while previous command is processing");
            return;
        }

        cancel_processing_cmd(true);
        return;
    }else{

        if(!last_cmd.processed){
            msg(MSG_INFO,"Please wait, while previous command is processing");
            return;
        }

    }

    if(
       ui->combo_Ch->currentText().contains("-")
    ||
       ui->combo_Sen->currentText().contains("-")
    )
    {
        msg(MSG_ERROR,"Incomplete command, All fields are required ");
        return;
    }




    current_cmd.cmd= map_cmd[ui->cmd_list->currentItem()->text(0)];

    current_cmd.arg[0]= ui->combo_Ch->currentText().toShort();
    current_cmd.arg[1]= ui->combo_Sen->currentText().toShort();

    try{
       if(current_cmd.cmd==CMD_DEVICE_ADDRESS){

           if(ui->combo_Sen->currentText().contains("Multiple Address")){
               msg(MSG_ERROR,"The slelected command can not be used for multiple address");
               return;
           }

           QString Question =
                   QString("The command sets the address of any sensor connected to the selected channel,")
                   +QString("please make sure only a single sensor is ")
                   +QString("connected to the channel unless otherwise required.\n")
                   +QString("Are you sure you want to continue with this action?");

           QMessageBox msgBox;
           msgBox.setWindowIcon(QMainWindow::windowIcon());
           msgBox.setWindowTitle(QMainWindow::windowTitle());
           msgBox.setText(Question);
           msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
           if(msgBox.exec() == QMessageBox::No)return;

       }


   } catch (...) {
        msg(MSG_ERROR,"Unknown command, Make sure you have selected the command correctly ");
   }


    Uint16 cmd = (((current_cmd.cmd&0x00F0)>>4)==0xE)?0xE:
                 (((current_cmd.cmd&0x0F00)>>8)==1)?1:
                 (((current_cmd.cmd&0x0F00)>>8)==6)?6:                
                 current_cmd.cmd;





    strcpy(current_cmd.cmd_type,ui->cmd_list->currentItem()->text(0).toLatin1());

    switch (cmd) {
    case CMD_INSTALLATION_DITAILS:


            current_cmd.arg[2]         |= ui->cb_config_rated_current->isChecked()?0x0001:0;
            current_cmd.arg[2]         |= ui->cb_config_sh_ckt_current->isChecked()?0x0002:0;
            current_cmd.arg[2]         |= ui->cb_config_freq->isChecked()?0x0004:0;
            current_cmd.arg[2]         |= ui->cb_config_min_distance->isChecked()?0x0008:0;

            current_cmd.arg[3]          = 4;

            current_cmd.arg[4]          = ui->dSB_rated_current->value();
            current_cmd.arg[5]          = ui->dSB_sh_ckt_current->value();
            current_cmd.arg[6]          = ui->dSB_freq->value();
            current_cmd.arg[7]          = ui->dSB_min_distance->value();

            current_cmd.arg_cnt         = 8;

            current_cmd.listener_slot   ="1srv_cmds_general_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

        break;


    case CMD_SIGNAL_THRESHOLDS:


            current_cmd.arg[2] |= ui->cb_config_ir_max->isChecked()?0x0001:0;
            current_cmd.arg[2] |= ui->cb_config_uv_max->isChecked()?0x0002:0;

            current_cmd.arg[3]  = 2;

            current_cmd.arg[4]  = ui->dSB_ir_max->value();
            current_cmd.arg[5]  = ui->dSB_uv_max->value();

            current_cmd.arg_cnt = 6;

            current_cmd.listener_slot ="1srv_cmds_general_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

        break;


    case CMD_EVENT_ANALYSIS:


            current_cmd.arg[2]     |= ui->cb_config_observation_length->isChecked()?0x0001:0;
            current_cmd.arg[2]     |= ui->cb_config_probablity_max->isChecked()?0x0002:0;
            current_cmd.arg[2]     |= ui->cb_config_probablity_min->isChecked()?0x0004:0;
            current_cmd.arg[2]     |= ui->cb_config_criteria->isChecked()?0x0008:0;

            current_cmd.arg[3]      = 4;

            current_cmd.arg[4]      = ui->dSB_observation_length->value();
            current_cmd.arg[5]      = ui->dSB_probablity_max->value();
            current_cmd.arg[6]      = ui->dSB_probablity_min->value();

            current_cmd.arg[7]     |= ui->cb_config_active_modules_ir->isChecked()?0x0001:0;
            current_cmd.arg[7]     |= ui->cb_config_active_modules_uv->isChecked()?0x0002:0;
            current_cmd.arg[7]     |= ui->cb_config_active_modules_mf->isChecked()?0x0004:0;
            current_cmd.arg[8]      = ui->combo_alarm_condition->currentText().toInt();



            current_cmd.listener_slot   ="1srv_cmds_general_msg_handler(QString)";

            current_cmd.arg_cnt     = 9;


            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

        break;


    case CMD_RUN_TIME_OPTIONS:


            current_cmd.arg[2]   = 0xFFFF;//dummy data
            current_cmd.arg[3]   = 1;

            current_cmd.arg[4]  |= ui->cb_config_threshold_adjust->isChecked()?0x0001:0;
            current_cmd.arg[4]  |= ui->cb_config_sv_ir->isChecked()?0x0002:0;
            current_cmd.arg[4]  |= ui->cb_config_sv_uv->isChecked()?0x0004:0;
            current_cmd.arg[4]  |= ui->cb_config_sv_mfield->isChecked()?0x0008:0;

            current_cmd.arg_cnt  = 1 ;
            current_cmd.listener_slot ="1srv_cmds_general_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

        break;

   {
    case 0xE/*data fetching or request msg*/:

            current_cmd.data_download.offset = 0;
            current_cmd.arg[2] = current_cmd.data_download.offset;

            current_cmd.data_download.total_size.copied = 0;

            current_cmd.data_download.total_size.expected = map_size[current_cmd.cmd];

            current_cmd.data_download.storage_location =
                    (void *)malloc(current_cmd.data_download.total_size.expected);


            current_cmd.arg[3]
                    = ((MAX_ATTACHMENT_LENGTH-(current_cmd.data_download.total_size.expected))<0)?
                        MAX_ATTACHMENT_LENGTH:current_cmd.data_download.total_size.expected;

            current_cmd.arg_cnt    = 4;

            current_cmd.listener_slot ="1srv_cmds_data_download_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

    break;
}

{
    case 6/*data transfer messages*/:

            current_cmd.arg[2]              = 0; //Inital offset
            current_cmd.attachment_length   = EMU_MODE_DATA_LENGTH*sizeof(Uint16);
            current_cmd.arg[3]              = ((MAX_ATTACHMENT_LENGTH-(current_cmd.attachment_length))<0)?
                                               MAX_ATTACHMENT_LENGTH:current_cmd.attachment_length;

            if(emu_data_buffer.valid == true){

            current_cmd.attachment = (void *)emu_data_buffer.data;
            }else{

                qCritical() <<"Unexpected result considered emulation data validation error "
                            <<"in MainWindow::on_btn_execute_cmd_clicked";

            return;
            }

            current_cmd.arg_cnt  = 4;

            current_cmd.listener_slot   ="1srv_cmds_data_transfer_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

    break;
 }
 {

    case 1/*zero argument messages*/:

            current_cmd.arg_cnt  = 2;

            current_cmd.listener_slot ="1srv_cmds_general_msg_handler(QString)";

            if(!QString(last_cmd.listener_slot).isNull())
            disconnect(this, SIGNAL(response_ready(QString)), this, last_cmd.listener_slot);

            connect(this, SIGNAL(response_ready(QString)), this, current_cmd.listener_slot);

    break;
}
    default:

        break;
    }






    if(ui->combo_Sen->currentText().contains("Multiple Address")){


          qint8 start_address = ui->spinBox_start->value();
          qint8 end_address   = ui->spinBox_end->value();
          QString data_dir_name = QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate);
          //! TODO: allow user to set this value

          data_dir_name.replace(':',"-").replace(' ',"_").replace(',',"");
          if((end_address-start_address) < 0){
              msg(MSG_ERROR,"  Incorrect sensor address range, acceptable range is between 1 and 255");
             return;
          }

          multi_cmds.clear();

          //ui->sAreaVLayout->addWidget(ui->label_Separator);

          for (int var = start_address; var <= end_address; ++var) {

           mult_sen_t target_sen;
           Uint16 width = 40;                   //! label width
           target_sen.address = var;

           target_sen.gui.label = new QLabel("SEN " + QString::number(var));
           target_sen.gui.label->setFixedWidth(width);



           if(cmd == 0xE){
           target_sen.gui.btn = new QPushButton("Open");
           target_sen.gui.btn->setEnabled(false);
           target_sen.gui.btn->setFixedWidth(40 /*btn width*/);
           target_sen.gui.btn->setCheckable(true);
           connect(target_sen.gui.btn, SIGNAL(clicked(bool)), this, SLOT(data_open(bool)));
           target_sen.gui.btnCreated = true;
           width +=40;
           }else{
               target_sen.gui.btnCreated=false;
           }



           target_sen.gui.pBar  = new QProgressBar();
           target_sen.gui.pBar->setTextVisible(false);
           target_sen.gui.pBar->setMaximumWidth(ui->gBox_range->width() - width);
           target_sen.gui.hLayout = new QHBoxLayout();
           target_sen.gui.hLayout->addWidget(target_sen.gui.label);
           target_sen.gui.hLayout->addWidget(target_sen.gui.pBar);

          if(cmd == 0xE)
           target_sen.gui.hLayout->addWidget(target_sen.gui.btn);

           target_sen.varified = false;
           target_sen.gui.hLayout->sizeHint();
           target_sen.data_dir_name = data_dir_name;
           target_sen.data_file_name = "";
           multi_cmds.append(target_sen);

           ui->sAreaVLayout->addLayout(target_sen.gui.hLayout);

           /*ui->dockWidget_sen_cmd->resize(ui->dockWidget_sen_cmd->width(),
                                        600
                                         );*/



          }

          current_cmd.multi_address = true;
          current_cmd.vPos = 0;

          multi_cmds[current_cmd.vPos].gui.pBar->setValue(20);

          current_cmd.arg[1] = start_address;

     }else{

        last_cmd.multi_address = false;
        last_cmd.vPos = 0;
     }



       if(srv_cmds_execute(current_cmd))
          ui->btn_execute_cmd->setText("Reset");

    return;

}


void MainWindow::data_open(bool checked){

    for (int var = 0; var < multi_cmds.length(); ++var)
        if(multi_cmds[var].gui.btn->isChecked())
            loadFile(multi_cmds[var].data_file_name);


    for (int var = 0; var < multi_cmds.length(); ++var)
        multi_cmds[var].gui.btn->setChecked(false);



}



void MainWindow::on_dockWidget_sen_cmd_destroyed(QObject *arg1)
{
  reset_form();
}

void MainWindow::reset_form(){

    if(multi_cmds.length()>0){
        last_cmd.multi_address=false;
        for (int var = 0; var < multi_cmds.length(); ++var) {
            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.label);
            delete multi_cmds[var].gui.label;

            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.pBar);
            delete multi_cmds[var].gui.pBar;

            if(multi_cmds[var].gui.btnCreated){
            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.btn);
            delete multi_cmds[var].gui.btn;
            }

            delete multi_cmds[var].gui.hLayout;

        }


    ui->sAreaVLayout->removeWidget(ui->gBox_range);
    ui->spinBox_start->setValue(1);
    ui->spinBox_end->setValue(255);

    ui->sArea_progress_view->parentWidget()->adjustSize();
    ui->sArea_progress_view->setVisible(false);
    ui->dockWidget_sen_cmd->adjustSize();
    }
    ui->btn_execute_cmd->setText("Execute Command");
}

void MainWindow::on_cmd_list_itemSelectionChanged()
{

    ui->stacked_widget_views->setCurrentIndex(((map_cmd[ui->cmd_list->currentItem()->text(0)])&0x0F00)>>8);

    if(emu_data_buffer.valid == false || ui->stacked_widget_views->currentIndex()!=1)
    emu_data_buffer.current_editor = NULL;

    ui->btn_set_emu_data->setStyleSheet(NORMAL);
    ui->btn_set_emu_data->setText("Add Data");



}


void MainWindow::on_stacked_widget_views_currentChanged(int arg1)
{

    if(arg1 < 2 || arg1 > 5)
        return;

    switch (arg1) {
        case 3/*INSTALLATION DITAILS*/:

             ui->dSB_rated_current->setValue(CONFIG_DEFAULT_RATED_CURRENT);
             ui->dSB_sh_ckt_current->setValue(CONFIG_DEFAULT_SHORT_CKT_CURRENT);
             ui->dSB_freq->setValue(CONFIG_DEFAULT_FREQ);
             ui->dSB_min_distance->setValue(CONFIG_DEFAULT_MIN_INSTALLATION_DISTANCE);

             ui->cb_config_rated_current->setChecked(true);
             ui->cb_config_sh_ckt_current->setChecked(true);
             ui->cb_config_freq->setChecked(true);
             ui->cb_config_min_distance->setChecked(true);
        break;

        case 2/*SIGNAL THRESHOLDS*/:
            ui->dSB_ir_max->setValue(100);
            ui->dSB_uv_max->setValue(100);
        break;

        case 4/*EVENT ANALYSIS OPTIONS*/:
            ui->dSB_observation_length->setValue(CONFIG_DEFAULT_NO_OF_OBSERVATION);
            ui->dSB_probablity_max->setValue(CONFIG_DEFAULT_PROBABLITY_MAX);
            ui->dSB_probablity_min->setValue(CONFIG_DEFAULT_PROBABLITY_MIN);
            ui->combo_alarm_condition->setCurrentIndex(0);
            ui->cb_config_active_modules_ir->setChecked(true);
            ui->cb_config_active_modules_uv->setChecked(true);
            ui->cb_config_active_modules_mf->setChecked(true);
        break;

        case 5 /*RUN TIME OPTIONS*/:
             ui->cb_config_threshold_adjust->setChecked(false);
             ui->cb_config_sv_ir->setChecked(false);
             ui->cb_config_sv_uv->setChecked(false);
             ui->cb_config_sv_mfield->setChecked(false);
        break;


    default:
        break;
    }

    if(last_cmd.processed /*TODO:?*/){
    ui->combo_Sen->setCurrentIndex(0);
    ui->combo_Ch->setCurrentIndex(0);
    ui->btn_execute_cmd->setText("Execute Command");
    }
}



void MainWindow::on_btn_set_emu_data_clicked()
{

    if(ui->btn_set_emu_data->text().contains("Add Data")){
    emu_data_editor* editor = new emu_data_editor(ui->cmd_list->currentItem()->text(0),this);

    this->emu_data_buffer.current_editor = editor;
    this->emu_data_buffer.valid          = false;

    editor->show();
    editor->raise();
    editor->activateWindow();



    if(this->x() < (QApplication::desktop()->rect().width()/2))
    editor->move((this->x()+this->width()+ 50),50);
    else if(this->x() > (QApplication::desktop()->rect().width()/2))
    editor->move((this->x()-this->width()-editor->width()- 10),50);


 }else{
      emu_data_buffer.current_editor->show();
      emu_data_buffer.current_editor->raise();
      emu_data_buffer.current_editor->activateWindow();
 }


}


void MainWindow::set_emu_data(emu_data_editor *editor,Uint16* data,Uint16 size)

{
    if(editor != this->emu_data_buffer.current_editor){
      msg(MSG_INFO,
          QString("The selected editor is not valid.")+
          QString("\nYou must save the data to a file" )+
          QString(" to open it again with a new editor"));

        return;//this is zambi editor
    }



    memmove(this->emu_data_buffer.data,data,size);


    this->emu_data_buffer.processed = false;
    this->emu_data_buffer.valid = false;
    this->emu_data_buffer.current_editor = editor;
    ui->btn_set_emu_data->setStyleSheet(ACTIVE);
    ui->btn_set_emu_data->setText("Edit Data");

    this->show();
    this->raise();
    this->activateWindow();

    ui->dockWidget_sen_cmd->setWindowFlags(Qt::Window);
    ui->dockWidget_sen_cmd->show();
    ui->dockWidget_sen_cmd->raise();
    ui->dockWidget_sen_cmd->activateWindow();

}

void MainWindow::cmd_cancel(){
    ui->stacked_widget_views->setCurrentIndex(0);
    QModelIndex index = ui->cmd_list->model()->index(0,0,QModelIndex());
    ui->cmd_list->setCurrentIndex(index);

}



void MainWindow::on_btn_cancel_clicked()
{
    cmd_cancel();
}

void MainWindow::on_btn_back_thresholds_clicked()
{
    cmd_cancel();
}

void MainWindow::on_btn_cancel_sys_info_clicked()
{
    cmd_cancel();
}

void MainWindow::on_btn_back_analisis_clicked()
{
    cmd_cancel();
}

void MainWindow::on_btn_cancel_options_clicked()
{
    cmd_cancel();
}

void MainWindow::on_btn_back_emu_data_clicked()
{
    cmd_cancel();

}
void MainWindow::on_btn_save_thresholds_clicked()
{
     ui->stacked_widget_views->setCurrentIndex(1);
}

void MainWindow::on_btn_save_sys_info_clicked()
{
    ui->stacked_widget_views->setCurrentIndex(1);
}


void MainWindow::on_btn_save_analisis_clicked()
{
    ui->stacked_widget_views->setCurrentIndex(1);
}

void MainWindow::on_btn_save_options_clicked()
{
    ui->stacked_widget_views->setCurrentIndex(1);
}

void MainWindow::on_btn_nxt_emu_data_clicked()
{
    if(ui->btn_set_emu_data->text().contains("Add Data")){
        msg(MSG_ERROR,"Data is not yet added.\nyou can’t continue with this action");
        return;
    }

    if(emu_data_buffer.current_editor != NULL)
        this->emu_data_buffer.valid = true;
     ui->stacked_widget_views->setCurrentIndex(1);
}



void MainWindow::cancel_processing_cmd(bool checked)
{
    if(!checked){
        return;
    }
    if(last_cmd.multi_address){
    if(multi_cmds.length()>0){
        for (int var = 0; var < multi_cmds.length(); ++var) {
            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.label);
            delete multi_cmds[var].gui.label;

            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.pBar);
            delete multi_cmds[var].gui.pBar;

            if(multi_cmds[var].gui.btnCreated){
            multi_cmds[var].gui.hLayout->removeWidget(multi_cmds[var].gui.btn);
            delete multi_cmds[var].gui.btn;
            }

            ui->sAreaVLayout->removeItem(multi_cmds[var].gui.hLayout);

            delete multi_cmds[var].gui.hLayout;

        }
    ui->sArea_progress_view->parentWidget()->adjustSize();
    ui->dockWidget_sen_cmd->adjustSize();

    }

    last_cmd.multi_address=false;
   }

    last_cmd.processed = true;
    ui->btn_execute_cmd->setText("Execute Command");


}


bool MainWindow::srv_cmds_execute(serial_msg_t current_cmd, Uint16 *resp, Uint16 wait_resp)
{

      if(protocol_send_cmd(current_cmd,resp,wait_resp) == 0)
      {

      ui->terminal->textCursor().insertText(QString("Selected sensor\t- > ") +
                                            QString("Channel ") + QString::number(current_cmd.arg[0]) +
                                            QString(", Address ") + QString::number(current_cmd.arg[1])
                                            );
      ui->terminal->textCursor().insertText(QString("\nCommand \t\t- > ") +
                                            ui->cmd_list->currentItem()->text(0)

                                            );

      ui->terminal->textCursor().insertText(QString("\nWaiting for response...\n "));

      if(last_cmd.multi_address == true)
      multi_cmds[last_cmd.vPos].gui.pBar->setValue(
              multi_cmds[last_cmd.vPos].gui.pBar->value() +
              20);


      last_cmd.processed           = false;
      last_cmd.device_response_cnt = 0;
      last_cmd.last_response_cnt   = 0;
      QTimer::singleShot(30000, this, SLOT(serial_supervisor()));

      return true;
      }
      else
      {
         ui->terminal->textCursor().insertText(
        QString("\nCommunication error: serial connection problem "));
        cancel_processing_cmd(true);

       return false;
      }


}


void MainWindow::on_combo_Sen_activated(const QString &arg1)
{
    if(arg1.compare(QString("Multiple Address")) == 0){

        if(ui->sArea_progress_view->isVisible())
              return;


        ui->sArea_progress_view->setVisible(true);
        ui->sAreaVLayout->addWidget(ui->gBox_range);


        ui->sArea_progress_view->setMinimumHeight(450);

       }else{

        ui->sArea_progress_view->setVisible(false);


       }



}









