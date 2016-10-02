#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingsdialog.h"
#include "recordpreview.h"
#include "emu_data_editor.h"


#include <QMessageBox>
#include <QFileDialog>




#include <QDebug>
#include <QProgressBar>
#include <QDesktopWidget>
#include <QDateTime>
#include <QTimer>

#include <stdarg.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);


    this->move(QApplication::desktop()->rect().x() + 10,this->y()+2);


        console = new Console;
        console->setEnabled(false);


        serial = new QSerialPort(this);
        settings = new SettingsDialog;

          ui->actionConnect->setEnabled(true);
          ui->actionDisconnect->setEnabled(false);
          ui->actionQuit->setEnabled(true);
          ui->actionConfigure->setEnabled(true);


          initActionsConnections();

            connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

            connect(serial, SIGNAL(readyRead()), this, SLOT(incoming_data_listener()));






           ui->dockWidget->setVisible(false);

           ui->centralWidget->setVisible(false);

           ui->actionClear->setVisible(false);

          this->addDockWidget(Qt::RightDockWidgetArea,ui->dock_central);


          last_cmd.listener_slot = NULL;
          //serial rc buffer start frash
          last_cmd.processed=true;
          rc_buffer.clear();

          cmd_line_separator="\n>> ";



          dir_default = DEFAULT_DIR;

          QDir dir;

          if(!dir.exists(dir.toNativeSeparators(dir_default)))
              dir.mkdir(dir.toNativeSeparators(dir_default));

          map_keys();

          installEventFilter(this);



}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject *object, QEvent *event)
{


    if (event->type() == QEvent::FocusOut)
    {
        if (object == this)
        {

        }
    }


    if (event->type() == QEvent::FocusIn)
    {
        if (object == ui->dockWidget_sen_cmd)
        {

        }

    }
    return false;
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About"),
                       tr("COMMING UP"));
}


void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

}



void MainWindow::msg(Uint16 type, QString msg)
{
    QString title;

    QMessageBox msgBox;
    msgBox.setWindowIcon(QMainWindow::windowIcon());
    msgBox.setWindowTitle(QMainWindow::windowTitle());

    switch (type) {
    case MSG_ERROR:
        msgBox.setText(tr("Error"));
        msgBox.setIcon(QMessageBox::Critical);
        break;
    case MSG_INFO:
        msgBox.setText(tr("Information"));
        msgBox.setIcon(QMessageBox::Information);
    default:

        break;
    }



 msgBox.setInformativeText(msg);

 msgBox.exec();

}

QString MainWindow::uint8_to_str(Uint8 val)
{
    const char cnv[] = "0123456789ABCDEF";

    return QString(cnv[(val>>4)&0x0F]) + QString(cnv[(val>>0)&0x0F]);
}

void MainWindow::on_actionClear_triggered()
{
    ui->terminal->clear();
    rc_buffer.clear();
}

void MainWindow::on_actionOpen_triggered()
{
    QDir   default_dir;
    QString SelectedFilesName;


    if(default_dir.exists(default_dir.toNativeSeparators(dir_default)))
        default_dir.mkdir(default_dir.toNativeSeparators(dir_default));




    SelectedFilesName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("Comma Separated Values (*.csv)"));


    loadFile(SelectedFilesName);

}

void MainWindow::loadFile(QString file_name){

    QString  type = NULL;
    QVector<double> measurement_values;
    QVector<double> history_amp;
    QVector<double> history_dfdt;
    QVector<double> time;

    int    length = 0;
    int    result = 0;


    if (file_name.isEmpty()) return;

        QFile file(file_name);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }



        QTextStream stream(&file);
        QStringList values;
        QString line = stream.readLine();

        values = line.split(',');
        if(values.length() == 2)
        type = values.at(1);


        line = stream.readLine();
        values.clear();
        values = line.split(',');
        if(values.length()==2)
        length = values.at(1).toInt();


            line = stream.readLine();

            for(int ii=0;ii< length;ii++){
                line = stream.readLine();
                values = line.split(',');

                if(values.length() < 2) result = -1;
                else{


                time.append(values[0].toDouble());

                if(time[ii]!=ii)result = -1;

                measurement_values.append(values[1].toDouble());

                if(values.length() == 3){
                 history_amp.append(values[2].toDouble());
                 history_dfdt.append(values[3].toDouble());
                }
                }

            }


            if(result == -1){

                 QMessageBox::StandardButton reply;
                 reply = QMessageBox::question(this, "#Error",
                                               "The file is corrupted, do you want to open anyway",
                                               QMessageBox::Yes|QMessageBox::No);
                 if (reply == QMessageBox::Yes)
                  result = 0;

            }


       stream.flush();
       file.close();




        if(result == 0){

        recordPreview* prv = new recordPreview(type,/*offset*/&measurement_values,&history_amp,&history_dfdt,this);

        prv->show();
        prv->raise();
        prv->activateWindow();

        if(this->x() < (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()+this->width()+ 50),50);
        else if(this->x() > (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()-this->width()- prv->width()- 10),50);
       }

}

void MainWindow::ListFiles(){

    QStringList nameFilter("*.csv");

    QDir directory;
    directory.setPath(directory.toNativeSeparators(dir_default));

    if(!directory.exists()){
        if(!directory.mkdir(directory.toNativeSeparators(dir_default))){
            msg(MSG_ERROR,"Couldn't open the Directory " + directory.absolutePath() + "for reading");
        return;
        }
    }

    QStringList eventFiles = directory.entryList(nameFilter);
    model->setStringList(eventFiles);
}



void MainWindow::on_actionCommands_triggered(bool checked)
{
    if(!last_cmd.processed){
        msg(MSG_INFO,"Please wait, while previous command is processing");
        ui->actionCommands->setChecked(!checked);
        return;
    }

    if(checked){

        ui->sArea_progress_view->setVisible(false);



        ui->dockWidget_sen_cmd->setWindowFlags(Qt::Window);
        ui->dockWidget_sen_cmd->show();
        ui->dockWidget_sen_cmd->raise();
        ui->dockWidget_sen_cmd->activateWindow();

        if(this->x() > (ui->dockWidget_sen_cmd->width() + 50))
        ui->dockWidget_sen_cmd->move(QApplication::desktop()->rect().x()+10,this->y());
        else if(((QApplication::desktop()->rect().width())- this->x())> (ui->dockWidget_sen_cmd->width() + 50))
        ui->dockWidget_sen_cmd->move((this->x() + this->width() + 50),ui->dockWidget_sen_cmd->y());


        QModelIndex index = ui->cmd_list->model()->index(0,0,QModelIndex());
        ui->cmd_list->setCurrentIndex(index);
        ui->combo_Ch->setCurrentIndex(0);
        ui->combo_Sen->setCurrentIndex(0);
        ui->stacked_widget_views->setCurrentIndex(0);



        if(ui->actionConnect->isEnabled()/*not connected*/)
          ui->dockWidget_sen_cmd->setEnabled(false);
        else

            ui->dockWidget_sen_cmd->setEnabled(true);



    }else{
        ui->dockWidget_sen_cmd->close();
    }

}


void MainWindow::on_dockWidget_sen_cmd_visibilityChanged(bool visible)
{
    if(!visible)
        ui->actionCommands->setChecked(false);
    else
       ui->actionCommands->setChecked(true);
}




void MainWindow::record_preview(serial_msg_t current_cmd){
    
    

    QString type = "CH-"
                   + QString::number(current_cmd.arg[0])
                   + " SEN-"+QString::number(current_cmd.arg[1])
                   + ":"+ QString::fromLatin1(current_cmd.cmd_type)
                   + ":"+QDateTime::currentDateTime().toString("dd.MM.yyyy");
    if(current_cmd.cmd == CMD_DEVICE_DATA){
       device_data_t* device_data = (device_data_t *)current_cmd.data_download.storage_location;


       ui->terminal->textCursor().insertText( QString("Device configuration data \n"));
       ui->terminal->textCursor().insertText( QString("----------------------------------- \n"));
       ui->terminal->textCursor().insertText( QString("\tDevice ID :")+QString::number(device_data->installation.device_address)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("Installation details -------------------------- \n"));
       ui->terminal->textCursor().insertText( QString("\tRated current :")+QString::number(device_data->installation.system_info.rated_current)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tRated short circuit current:")+QString::number(device_data->installation.system_info.sh_current)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tSignal Frequency:")+QString::number(device_data->installation.system_info.freq)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tMinimum conductor distance :")+QString::number(device_data->installation.system_info.min_distance)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("Threshold levels     ----------------------------------- \n"));
       ui->terminal->textCursor().insertText( QString("\tIR signal level threshold :")+QString::number(device_data->installation.thresholds.ir_level_max)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tUV signal level threshold :")+QString::number(device_data->installation.thresholds.uv_level_max)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("Event analysis       ----------------------------------- \n"));
       ui->terminal->textCursor().insertText( QString("\tAlarm/Trip condition :")+QString::number(device_data->installation.trip_criteria.criterion.all)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t  IR Module:")+
                                            ((device_data->installation.trip_criteria.active_modules.bits.ir?QString("Enabled"):QString("Disabled"))+QString("\n")));
       ui->terminal->textCursor().insertText( QString("\t  UV Module:")+
                                            ((device_data->installation.trip_criteria.active_modules.bits.uv?QString("Enabled"):QString("Disabled"))+QString("\n")));
       ui->terminal->textCursor().insertText( QString("\t  Magnetic field x axis Module:")+
                                            ((device_data->installation.trip_criteria.active_modules.bits.mfield_x?QString("Enabled"):QString("Disabled"))+QString("\n")));
#ifdef MFIELD_Y
       ui->terminal->textCursor().insertText( QString("\t  Magnetic field y axis Module:")+
                                            ((device_data->installation.trip_criteria.active_modules.bits.mfield_y?QString("Enabled"):QString("Disabled"))+QString("\n")));
#endif
       ui->terminal->textCursor().insertText( QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tMinimum minimum observation length :")+QString::number(device_data->installation.thresholds.min_number_of_observation)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tProbability maximum value :")+QString::number(device_data->installation.thresholds.probablity_max)+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tProbability minimum value:")+QString::number(device_data->installation.thresholds.probablity_min)+QString("\n"));


       ui->terminal->textCursor().insertText( QString("Run time options  ----------------------------------- \n"));

       ui->terminal->textCursor().insertText( QString("\tIR element supervision :")+
                                            (device_data->installation.options.bits.ir_element_sv_en?QString("Enabled"):QString("Disabled"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tUV element supervision :")+
                                            (device_data->installation.options.bits.uv_element_sv_en?QString("Enabled"):QString("Disabled"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tMagnetic field element supervision :")+
                                            (device_data->installation.options.bits.mfield_element_sv_en?QString("Enabled"):QString("Disabled"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tThreshold manual adjustment mode:")+
                                            (device_data->installation.options.bits.threshold_adjust_manual?QString("Enabled"):QString("Disabled"))+QString("\n"));


       if(!device_data->installation.is_valid)
       ui->terminal->textCursor().insertText( QString("\nwarning:: Configuration data is not marked as valid user data\n"));


       ui->terminal->textCursor().insertText( QString("Device supervision record \n"));
       ui->terminal->textCursor().insertText( QString("System Error flags  ----------------------------------- \n"));

       ui->terminal->textCursor().insertText( QString("\tIR sensor element :")+
                                            ((!device_data->sv_record.system_error_flags.bits.ir_sensor_element)?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tUV sensor element :")+
                                            ((!device_data->sv_record.system_error_flags.bits.uv_sensor_element)?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tMagnetic field x axis sensor element :")+
                                            ((!device_data->sv_record.system_error_flags.bits.mfield_x_sensor_element)?QString("TRUE"):QString("FALSE"))+QString("\n"));
#ifdef MFIELD_Y
       ui->terminal->textCursor().insertText( QString("\tMagnetic field y axis sensor element :")+
                                            ((!device_data->sv_record.system_error_flags.bits.mfield_y_sensor_element)?QString("TRUE"):QString("FALSE"))+QString("\n"));
#endif
       ui->terminal->textCursor().insertText( QString("\tPower_supply :")+
                                            ((!device_data->sv_record.system_error_flags.bits.power_supply)?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\tStorage :")+
                                            ((!device_data->sv_record.system_error_flags.bits.storage)?QString("TRUE"):QString("FALSE"))+QString("\n"));



       ui->terminal->textCursor().insertText( QString("False Positives flags  ----------------------------------- \n"));

       ui->terminal->textCursor().insertText( QString("\tIR Module :")+
                                            ((!device_data->sv_record.error_flags.ir.bits.FALSE_POSITIVE)?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            ((!device_data->sv_record.error_flags.ir.bits.FALSE_POSITIVE_AMP_TH)?QString("amplitude threshold "):QString(""))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            ((!device_data->sv_record.error_flags.ir.bits.FALSE_POSITIVE_DFDT_TH)?QString("dfdt threshold "):QString(""))+QString("\n"));

       ui->terminal->textCursor().insertText( QString("\tUV Module :")+
                                            ((!device_data->sv_record.error_flags.uv.bits.FALSE_POSITIVE)?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            ((!device_data->sv_record.error_flags.uv.bits.FALSE_POSITIVE_AMP_TH)?QString("amplitude threshold "):QString(""))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            ((!device_data->sv_record.error_flags.uv.bits.FALSE_POSITIVE_DFDT_TH)?QString("dfdt threshold"):QString(""))+QString("\n"));

       ui->terminal->textCursor().insertText( QString("\tMagnetic field x Module :")+
                                            (!device_data->sv_record.error_flags.mfield_x.bits.FALSE_POSITIVE?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            (!device_data->sv_record.error_flags.mfield_x.bits.FALSE_POSITIVE_AMP_TH?QString("amplitude threshold "):QString(""))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            (!device_data->sv_record.error_flags.mfield_x.bits.FALSE_POSITIVE_DFDT_TH?QString("dfdt threshold"):QString(""))+QString("\n"));
#ifdef MFIELD_Y
       ui->terminal->textCursor().insertText( QString("\tMagnetic field y Module :")+
                                            (!device_data->sv_record.error_flags.mfield_y.bits.FALSE_POSITIVE?QString("TRUE"):QString("FALSE"))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            (!device_data->sv_record.error_flags.mfield_y.bits.FALSE_POSITIVE_AMP_TH?QString("amplitude threshold "):QString(""))+QString("\n"));
       ui->terminal->textCursor().insertText( QString("\t\t")+
                                            (!device_data->sv_record.error_flags.mfield_y.bits.FALSE_POSITIVE_DFDT_TH?QString("dfdt threshold "):QString(""))+QString("\n"));
#endif


    }
    else if(current_cmd.data_download.total_size.copied == sizeof(module_disturbance_record_t))
    {
        
        module_disturbance_record_t* rc_data = 
                (module_disturbance_record_t*)current_cmd.data_download.storage_location;



        ui->terminal->textCursor().insertText(QString("\nEvent timestamp "));

        ui->terminal->textCursor().insertText(QString("\n      Detected   : ")
                           + QString::number(rc_data->event_data.timestamp.event_detected));
        ui->terminal->textCursor().insertText(QString("\n      Module Trip  : ")
                           + QString::number(rc_data->event_data.timestamp.module_active));
        ui->terminal->textCursor().insertText(QString("\n      Global Trip: ")
                           + QString::number(rc_data->event_data.timestamp.global_active));

        ui->terminal->textCursor().insertText(QString("\nEvent History : ")
                           + QString::number(rc_data->event_data.state_transition.history.all));
         //TODO: interpret history

        ui->terminal->textCursor().insertText(QString("\nCalculated probability: ")
                           + QString::number(rc_data->event_data.event_probability));
  

        ui->terminal->textCursor().insertText(
                      QString("\nthresholds                       ")
                    + QString("\n:Amplitude above maximum: ")
                    + QString::number(rc_data->event_data.exceeded_threshold.bits.amp_max)
                    + QString("\n:dfdt above maximum: ")
                    + QString::number(rc_data->event_data.exceeded_threshold.bits.dfdt_max)
                    + QString("\n:dfdt below minimum: ")
                    + QString::number(rc_data->event_data.exceeded_threshold.bits.dfdt_min)
                    + QString("\n:dfdt impulse: ")
                    + QString::number(rc_data->event_data.exceeded_threshold.bits.dfdt_impulse));



        Uint16 post_act_data_length =
                (rc_data->post_act_data.emu_data_en == 0)
                ?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH );



        QVector<Uint16> data( PRE_ACT_RECORD_BUFF_SIZE + post_act_data_length);



        for (int var = 0; var < PRE_ACT_RECORD_BUFF_SIZE; ++var) {
            
            data[var]= rc_data->pre_act_data.buffer[rc_data->pre_act_data.tail];
            rc_data->pre_act_data.tail = (rc_data->pre_act_data.tail +1)%PRE_ACT_RECORD_BUFF_SIZE;

        }



        if(rc_data->post_act_data.emu_data_en == 1)
            for (int var = 0; var < post_act_data_length; ++var){
                data[var + PRE_ACT_RECORD_BUFF_SIZE]= rc_data->post_act_data.buffer.data.l[var];

            }
        else
            for (int var = 0; var < post_act_data_length; ++var){
                data[var + PRE_ACT_RECORD_BUFF_SIZE]=
                rc_data->post_act_data.buffer.all[var];

            }


        event_data_t event_data;
        memmove(&event_data,&rc_data->event_data,sizeof(event_data));

        if(current_cmd.multi_address ==false){
        recordPreview* prv = new recordPreview(type,data,event_data,this);
        prv->show();
        prv->raise();
        prv->activateWindow();

        if(this->x() < (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()+this->width()+ 50),50);
        else if(this->x() > (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()-this->width()- prv->width()- 10),50);

        }else{

            QVector<double> measurement_values;
            QVector<double> history_amp;
            QVector<double> history_dfdt;



            history_amp.clear();
            history_dfdt.clear();

            for (int var = 0; var < data.length(); ++var)

                measurement_values[var]=((double)data[var])*ADC_VAL_MAPPING_FACTOR-type.contains("MFIELD")?1.65:0;

/*
            for (int var = 0; var < MAX_EVENT_OBSERVATION_LENGTH; ++var) {
                history_amp[var + PRE_ACT_RECORD_BUFF_SIZE ]
                        =((double)event_data.signal_level[var])*ADC_VAL_MAPPING_FACTOR - type.contains("MFIELD")?1.65:0;

                history_dfdt[var + PRE_ACT_RECORD_BUFF_SIZE]
                        =((double)event_data.change_per_time[var]);
            }
*/

           multi_cmds[vPos].data_file_name =  save_to_file(dir_default,type,event_data.offset,&measurement_values,&history_amp,&history_dfdt);

        }

   


    }else if(current_cmd.data_download.total_size.copied == sizeof(hif_diagonesis_records_t)){
        hif_diagonesis_records_t* rc_data = (hif_diagonesis_records_t*)current_cmd.data_download.storage_location;

        QVector<Uint16> data(MAX_EVENT_OBSERVATION_LENGTH);


        for (int var = 0; var < MAX_EVENT_OBSERVATION_LENGTH; ++var) {
            data[var]= rc_data->buffer[var];
        }





        if(current_cmd.multi_address ==false){
        recordPreview* prv = new recordPreview(type,data,this);
        prv->show();
        prv->raise();
        prv->activateWindow();

        if(this->x() < (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()+this->width()+ 50),50);
        else if(this->x() > (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()-this->width()- prv->width()- 10),50);

        }else{

            QVector<double> measurement_values(data.length());


            for (int var = 0; var < data.length(); ++var) {
                measurement_values[var]=((double)data[var])*ADC_VAL_MAPPING_FACTOR - type.contains("MFIELD")?1.65:0;
            }



            multi_cmds[vPos].data_file_name=save_to_file(dir_default,type,0,&measurement_values);

        }


    }else if(current_cmd.data_download.total_size.copied == sizeof(pre_act_record_buffer_t)){
        pre_act_record_buffer_t* rc_data = (pre_act_record_buffer_t*)current_cmd.data_download.storage_location;

        QVector<Uint16> data(PRE_ACT_RECORD_BUFF_SIZE);



        for (int var = 0; var < PRE_ACT_RECORD_BUFF_SIZE; ++var) {

            data[var]= rc_data->buffer[rc_data->tail];
            rc_data->tail = (rc_data->tail +1)%PRE_ACT_RECORD_BUFF_SIZE;


        }



        if(current_cmd.multi_address ==false){
        recordPreview* prv = new recordPreview(type,data,this);
        prv->show();
        prv->raise();
        prv->activateWindow();

        if(this->x() < (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()+this->width()+ 50),50);
        else if(this->x() > (QApplication::desktop()->rect().width()/2))
        prv->move((this->x()-this->width()- prv->width()- 10),50);

        }else{

            QVector<double> measurement_values(data.length());


            for (int var = 0; var < data.length(); ++var) {
                measurement_values[var]=(((double)data[var])*ADC_VAL_MAPPING_FACTOR - (type.contains("MFIELD")?1.65:0));
            }



            multi_cmds[vPos].data_file_name = save_to_file("..\\DATA\\",type,0,&measurement_values);

        }


    }


    free(current_cmd.data_download.storage_location);
}


QString MainWindow::save_to_file(QString dir,QString type,Uint16 offset,QVector<double> *data1,QVector<double> *data2,QVector<double> *data3)

{


    bool disturbance_record = (data1->length() == PRE_ACT_RECORD_BUFF_SIZE)?false:true;

    QString file_name = type;

    file_name.replace(':',"-").replace(' ',"_").replace(',',"").replace('.',"_");

    file_name = dir + file_name + ".csv";

    if (file_name.isEmpty()) return NULL;
    else {
        QFile file(file_name);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return NULL;
        }

        QTextStream stream(&file);

        stream << "Title,"+type+"\n";
        stream << "Record length,"+QString::number(data1->length())+"\n";
        if(disturbance_record){
        stream << "Event offset,"+QString::number(offset)+"\n";
        stream << "TIME, MEASURED DATA,EVENT_AMP,EVENT_DFDT\n";
        }else
        stream << "TIME, DATA\n";

        for(int ii=0;ii< data1->length();ii++)
           stream << QString::number(ii) +","+QString::number(data1->value(ii))
                     +
                     ((disturbance_record)?(
                     ","+ QString::number(data2->value(ii))
                     + ","+ QString::number(data3->value(ii))
                     ):"")
                     +"\n";



        file.flush();

     }

    return file_name;

}



