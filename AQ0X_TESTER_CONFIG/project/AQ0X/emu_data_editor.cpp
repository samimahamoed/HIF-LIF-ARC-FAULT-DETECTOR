#include "mainwindow.h"
#include "emu_data_editor.h"
#include "ui_emu_data_editor.h"
#include "slider.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QDateTime>
#include <QMessageBox>
#include <QKeyEvent>


emu_data_editor ::emu_data_editor (QString type,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::emu_data_editor )
{

    ui->setupUi(this);
    this->setWindowTitle(type);

    dir_default = ((MainWindow *)parent)->dir_default;

    win_title = type;
    if(type.contains("MAGNETIC FIELD")){
        signal_type = WAVEFORM_PLUS_MINUS_1_65;
        signal_amplitude = 1.65;
    }else{
        signal_type = WAVEFORM_0_TO_3_3;
        signal_amplitude = 3.3;
    }

    signal_freq      = CONFIG_DEFAULT_FREQ;
    signal_phase     = 0;//!defalt values


    ui->centralwidget->setVisible(false);
    ui->dockWidgetCurve_plot->setWidget(ui->plot_curve);
    ui->dockWidgetCurve_plot_fft->setWidget(ui->plot_curve_fft);

    this->addDockWidget(Qt::TopDockWidgetArea,ui->dockWidgetCurve_plot_fft);


    ui->actionWinEditor->setChecked(true);
    ui->actionWinPlot->setChecked(true);



    for (int var = 0; var < EMU_MODE_DATA_LENGTH; ++var) {
        emu_data_xvalue.append(var);
        emu_data_yvalue.append(0);
    }

    plot_curve(emu_data_xvalue,emu_data_yvalue);

    memset(fft_result,0,FFT_SIZE);
    memset(fft_inputs,0,FFT_SIZE);


    transformer.setSize(FFT_SIZE);
    if(!transformer.setWindowFunction("Rectangular"))
        qCritical()<<"unable to set window funtion : emu_data_editor";

    transformer.forwardTransform(fft_inputs,fft_result);


    spectrum_magnitude_max = 0;
    spectrum_magnitude_min = 0;



    for (int var = 0; var < FFT_SIZE; ++var) {

        emu_data_freq_value.append(
                    (var)*signal_freq*
                    ((signal_type == WAVEFORM_PLUS_MINUS_1_65)?1:2));

        qDebug()<<"f("<<var<<") = "<< emu_data_freq_value[var]<<";";
        emu_data_spectrum_magnitude.append((double)fft_result[var]);

        if(fft_result[var] > spectrum_magnitude_max)
            spectrum_magnitude_max = fft_result[var];
        if(fft_result[var] < spectrum_magnitude_min)
            spectrum_magnitude_min = fft_result[var];

    }







    plot_curve_fft(emu_data_freq_value,emu_data_spectrum_magnitude);
    installEventFilter(this);
    gui_init(EMU_MODE_DATA_LENGTH/2);



}


emu_data_editor ::~emu_data_editor ()
{
    delete ui;
}


bool emu_data_editor::eventFilter(QObject *obj, QEvent *event)
{


    if(obj!=this)
        return false;

        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            switch (keyEvent->key()) {
            case Qt::Key_Shift:
                    emit edit_point_selection(true);
                  return false;
                 break;


            case Qt::Key_Plus:

                    emit adjust_value(10);
                    update_curve();
                    return false;
                 break;

            case Qt::Key_Minus:

                    emit adjust_value(-10);
                    update_curve();
                    return false;
                break;
            case Qt::Key_Left:


                    return false;
                break;

            case Qt::Key_Clear:
                    emit edit_point_deselection(true);
                    return false;
                     break;


            case Qt::Key_Control:
                    emit edit_point_deselection(true);
                    return false;
                 break;


            case Qt::Key_Right:


            case Qt::Key_Enter:

                 return true;
                break;
            case Qt::Key_Return:


                 return true;
                break;

            default:


                return false;

               break;

            }

        } else  if (event->type() == QEvent::KeyRelease) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);


            switch (keyEvent->key()) {

            case Qt::Key_Shift:
                   emit edit_point_selection(false);
                  return false;
                 break;

            case Qt::Key_Clear:
                    emit edit_point_deselection(false);
                    return false;
                     break;


            case Qt::Key_Control:
                    emit edit_point_deselection(false);
                    return false;
                     break;

            case Qt::Key_Delete:

                    return false;
                     break;
            case Qt::Key_Left:

            case Qt::Key_Right:
            case Qt::Key_Up:
            case Qt::Key_Down:
                // skip processing
                break;
            case Qt::Key_Enter:

                 return true;
                break;
            case Qt::Key_Return:


                 return true;
                break;

            default:


                return false;

               break;

            }

        } else {
            return false;
        }

        return false;
}



void emu_data_editor::gui_init(Uint16 points)
{
          edit_point_sets.clear();

          for (int var = 0; var < points; ++var) {

           emu_data_edit_point_t edit_point;

           edit_point.val = 0;

           edit_point.gui.slider  = new Slider(var,this);
           edit_point.gui.slider->setMaximum(ADC_RESOLUTION);

           edit_point.gui.vLayout = new QVBoxLayout();
           edit_point.gui.vLayout->addWidget(edit_point.gui.slider);



           edit_point.gui.vLayout->sizeHint();



           edit_point_sets.append(edit_point);

           ui->horizontalLayout_edit_points->addLayout(edit_point.gui.vLayout);
           ui->horizontalLayout_edit_points->sizeHint();




          }


          emit action_set_value(0,SET_TYPE_DEFAULT);

          update_curve();





}





void emu_data_editor ::update_curve(){

     ui->plot_curve->graph(0)->setData(emu_data_xvalue, emu_data_yvalue);

     Int16 head = 0;

     for (int var = 0; var < FFT_SIZE; ++var) {
         fft_inputs[var] = (float)emu_data_yvalue[head];
         head = (head + 1)%EMU_MODE_DATA_LENGTH;
     }

     transformer.forwardTransform(fft_inputs, fft_result);

     emu_data_spectrum_magnitude.clear();


     spectrum_magnitude_max = 0;
     spectrum_magnitude_min = 0;

     for (int var = 0; var < FFT_SIZE; ++var) {

         emu_data_spectrum_magnitude.append((float)fft_result[var]);


         if(fft_result[var] > spectrum_magnitude_max)
             spectrum_magnitude_max = fft_result[var];
         if(fft_result[var] < spectrum_magnitude_min)
             spectrum_magnitude_min = fft_result[var];
     }

#if(0)

     for (int var = 0; var < FFT_SIZE; ++var) {
          qDebug()<<"A("<<var+1<<") = "<< fft_result[var]<<";";
     }

     for (int var = 0; var < FFT_SIZE; ++var) {
          qDebug()<<"B("<<var+1<<") = "<< fft_inputs[var];
     }
#endif



     ui->plot_curve_fft->graph(0)->setData(emu_data_freq_value,emu_data_spectrum_magnitude);
     ui->plot_curve_fft->yAxis->setRange(spectrum_magnitude_min - 2,
                                         spectrum_magnitude_max + 2);



     ui->plot_curve->setColor(QColor("black"));
     ui->plot_curve->replot();
     ui->plot_curve_fft->replot();


}

void emu_data_editor::selection(bool clear)
{

    emit set_status(clear?2/*deselect action*/:1/*select action*/);

}

void emu_data_editor ::plot_curve(QVector<double> xvalue,QVector<double> yvalue)
{

  ui->plot_curve->addGraph();
  ui->plot_curve->setColor(QColor("black"));

  ui->plot_curve->graph(0)->setPen(QPen(Qt::green));
  ui->plot_curve->graph(0)->setData(xvalue, yvalue);

  ui->plot_curve->xAxis->setLabel("t = (n x 100us)");
  ui->plot_curve->yAxis->setLabel("A(t)(V)");

  ui->plot_curve->xAxis->setLabelColor(QColor("black"));
  ui->plot_curve->yAxis->setLabelColor(QColor("black"));

  ui->plot_curve->xAxis->setGridPen(QPen(Qt::gray));
  ui->plot_curve->yAxis->setGridPen(QPen(Qt::gray));
  ui->plot_curve->xAxis2->setGridPen(QPen(Qt::gray));
  ui->plot_curve->yAxis2->setGridPen(QPen(Qt::gray));

  ui->plot_curve->xAxis->setTickPen(QPen(Qt::gray));
  ui->plot_curve->yAxis->setTickPen(QPen(Qt::gray));

  ui->plot_curve->xAxis->setBasePen(QPen(Qt::gray));
  ui->plot_curve->yAxis->setBasePen(QPen(Qt::gray));

  ui->plot_curve->xAxis->setTickLabelColor(QColor("gray"));
  ui->plot_curve->yAxis->setTickLabelColor(QColor("gray"));

  ui->plot_curve->xAxis->setTicks(true);



  ui->plot_curve->xAxis->setSubGrid(true);
  ui->plot_curve->xAxis->setTickStep(1.0);

  ui->plot_curve->xAxis->setSubGridPen(QPen(Qt::gray));
  ui->plot_curve->yAxis->setSubGridPen(QPen(Qt::gray));

  ui->plot_curve->xAxis2->setVisible(true);
  ui->plot_curve->xAxis2->setTickLabels(false);
  ui->plot_curve->yAxis2->setVisible(true);
  ui->plot_curve->yAxis2->setTickLabels(false);
  ui->plot_curve->xAxis2->setBasePen(QPen(Qt::gray));
  ui->plot_curve->yAxis2->setBasePen(QPen(Qt::gray));

   ui->plot_curve->xAxis->setGrid(true);
   ui->plot_curve->yAxis->setGrid(true);


  ui->plot_curve->xAxis->setRange(0, EMU_MODE_DATA_LENGTH);
  if(this->signal_type == WAVEFORM_PLUS_MINUS_1_65)
  ui->plot_curve->yAxis->setRange(-1.65, 1.65);
  else
  ui->plot_curve->yAxis->setRange(0, 3.34);
}



void emu_data_editor ::plot_curve_fft(QVector<double> xvalue,QVector<double> yvalue)
{


  ui->plot_curve_fft->addGraph();
  ui->plot_curve_fft->setColor(QColor("black"));

  ui->plot_curve_fft->graph(0)->setPen(QPen(Qt::green));
  ui->plot_curve_fft->graph(0)->setData(xvalue, yvalue);

  ui->plot_curve_fft->xAxis->setLabel("f ");
  ui->plot_curve_fft->yAxis->setLabel("A(f)(V)");

  ui->plot_curve_fft->xAxis->setLabelColor(QColor("black"));
  ui->plot_curve_fft->yAxis->setLabelColor(QColor("black"));

  ui->plot_curve_fft->xAxis->setGridPen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis->setGridPen(QPen(Qt::gray));
  ui->plot_curve_fft->xAxis2->setGridPen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis2->setGridPen(QPen(Qt::gray));

  ui->plot_curve_fft->xAxis->setTickPen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis->setTickPen(QPen(Qt::gray));

  ui->plot_curve_fft->xAxis->setBasePen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis->setBasePen(QPen(Qt::gray));

  ui->plot_curve_fft->xAxis->setTickLabelColor(QColor("gray"));
  ui->plot_curve_fft->yAxis->setTickLabelColor(QColor("gray"));

  ui->plot_curve_fft->xAxis->setTicks(true);



  ui->plot_curve_fft->xAxis->setSubGrid(true);
  ui->plot_curve_fft->xAxis->setTickStep(1
                                         );

  ui->plot_curve_fft->xAxis->setSubGridPen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis->setSubGridPen(QPen(Qt::gray));

  ui->plot_curve_fft->xAxis2->setVisible(true);
  ui->plot_curve_fft->xAxis2->setTickLabels(false);
  ui->plot_curve_fft->yAxis2->setVisible(true);
  ui->plot_curve_fft->yAxis2->setTickLabels(false);
  ui->plot_curve_fft->xAxis2->setBasePen(QPen(Qt::gray));
  ui->plot_curve_fft->yAxis2->setBasePen(QPen(Qt::gray));

   ui->plot_curve_fft->xAxis->setGrid(true);
   ui->plot_curve_fft->yAxis->setGrid(true);



  ui->plot_curve_fft->xAxis->setRange(0,
                                       2000);
  ui->plot_curve_fft->yAxis->setRange(spectrum_magnitude_min - 2,
                                      spectrum_magnitude_max + 2);
}



void emu_data_editor ::saveEventRecored(QString FileName){

    QString EventRecordFileName;

            QDir default_dir;

            if(!default_dir.exists(default_dir.toNativeSeparators(dir_default)))
                default_dir.mkdir(default_dir.toNativeSeparators(dir_default));

            EventRecordFileName = default_dir.toNativeSeparators(dir_default)+ EVENT_RECORD_FILE;


       QFile outputFile(EventRecordFileName);

       if(outputFile.exists())
       outputFile.open(QIODevice::WriteOnly | QIODevice::Append);
       else
       outputFile.open(QIODevice::WriteOnly);

       if(!outputFile.isOpen()){
           ((MainWindow *)parent())->msg(1,"Error, unable to open " + EventRecordFileName + " ");

            return;
       }

       QTextStream outStream(&outputFile);

       outStream << FileName<<"\n";
       outputFile.close();
}





void emu_data_editor::action_set_value_triggered(double val, Uint8 type)
{

    if(type == SET_TYPE_SIGNAL_FREQ)
        signal_freq = val;
    else if(type == SET_TYPE_PHASE_SHIFT)
        signal_phase = val/M_PI;
    else if(type == SET_TYPE_SIGNAL_AMP)
        signal_amplitude = val;

    emit action_set_value(val,type);
    update_curve();
}



void emu_data_editor::on_actionWinEditor_toggled(bool arg1)
{
     ui->actionWinEditor->setChecked(arg1);

     if(arg1)
         ui->dockWidgetCurve_plot->show();
     else
         ui->dockWidgetCurve_plot->close();

}

void emu_data_editor::on_actionWinPlot_toggled(bool arg1)
{
     ui->actionWinPlot->setChecked(arg1);

     if(arg1)
         ui->dockWidgetCurve_edit->show();
     else
         ui->dockWidgetCurve_edit->close();
}

void emu_data_editor::on_actionOpen_triggered()
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

void emu_data_editor::loadFile(QString file_name){

    bool   sucess = true;

    if (file_name.isEmpty()) return;

        QFile file(file_name);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }


QTextStream stream(&file);
Uint16 arg_cnt;
unsigned int time;


       QString line = stream.readLine();


       for(unsigned int ii=0;ii<(EMU_MODE_DATA_LENGTH/2);ii++){

             line = stream.readLine();

             arg_cnt = sscanf(line.toLatin1(),"%u%*1c%u",&time,&(edit_point_sets[ii].val)
                              );


             if(arg_cnt!=2 ||time!= ii){

                 QMessageBox::StandardButton reply;
                 reply = QMessageBox::question(this, "Error Report",
                                               "This file is corrupted, do you want to open anyway",
                                               QMessageBox::Yes|QMessageBox::No);
                 if (reply == QMessageBox::No)
                  sucess = false;
                  break;
             }else {

                  edit_point_sets[ii].gui.slider->setValue(edit_point_sets[ii].val);
             }
             }

       stream.flush();
       file.close();





       emit update_value();
       update_curve();


}


void emu_data_editor::on_actionCSV_triggered()
{

    QDir default_dir;



    if(default_dir.exists(default_dir.toNativeSeparators(dir_default)))
        default_dir.mkdir(default_dir.toNativeSeparators(dir_default));

    QString DefaultFileName =  default_dir.toNativeSeparators(dir_default)+(win_title
                                                                            +"_File_Saved_At_" +
                               QDateTime::currentDateTime().toString(Qt::SystemLocaleLongDate)
                               );


    DefaultFileName.replace(':',"-").replace(' ',"_");


    QString SelectedFilesName = QFileDialog::getSaveFileName(
        this,
        tr("Save As"),
        DefaultFileName,
        tr("Comma Separated Values (*.csv)"));


    if(SelectedFilesName == NULL)
        return;

     fileName = SelectedFilesName;
    if (fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QTextStream stream(&file);

        stream << "TIME, DATA\n";

        for(int ii=0;ii < (EMU_MODE_DATA_LENGTH/2)/*data edit points*/;ii++)
           stream << QString::number(ii) +","+QString::number(edit_point_sets[ii].gui.slider->value())+"\n";



        file.close();



        try {
            QDesktopServices::openUrl(QUrl("file:"+ fileName));

        } catch (...) {

        }
    }


}



void emu_data_editor::on_btn_save_clicked()
{

    Uint16 data[EMU_MODE_DATA_LENGTH];
    double adjust = windowTitle().contains("MAGNETIC FIELD")?1.65:0;


    for (int var = 0; var < emu_data_yvalue.length(); ++var) {
         data[var]=((emu_data_yvalue[var]+adjust)/ADC_VAL_MAPPING_FACTOR);
    }

    ((MainWindow *)this->parent())->set_emu_data(this,data,sizeof(data));

}

void emu_data_editor::on_actionMakeImg_triggered()
{

    QDir default_dir;

    if(!default_dir.exists(default_dir.toNativeSeparators(dir_default)))
        default_dir.mkdir(default_dir.toNativeSeparators(dir_default));

    QString DefaultFileName = this->windowTitle();



    DefaultFileName.replace(':',"-").replace(' ',"_").replace('.',"_");

    DefaultFileName = default_dir.toNativeSeparators(dir_default)+ DefaultFileName;

    QString SelectedFilesName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        DefaultFileName,
                                                         tr("Images (*.png *.jpg *.bmp)"));

    if(SelectedFilesName == NULL)
        return;

    ui->plot_curve->graph(0)->setPen(QPen(Qt::green));
    /*
    ui->plot_curve->graph(1)->setPen(QPen(Qt::red));
    ui->plot_curve->graph(1)->setPen(QPen(Qt::DotLine));
    ui->plot_curve->graph(1)->setBrush(QBrush(Qt::gray));
    */

    ui->plot_curve->setColor(QColor("white"));
    ui->plot_curve->replot();




    if(SelectedFilesName.indexOf('.')==-1)
        SelectedFilesName.append(".jpg");

    if((SelectedFilesName.indexOf('.')==SelectedFilesName.length()-4)){
    if(SelectedFilesName.endsWith(".jpg")){
       if(
        !ui->plot_curve->saveJpg(SelectedFilesName
                                ,800,600))
            ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");
    }else

    if(SelectedFilesName.endsWith(".png")){
           if(
            !ui->plot_curve->savePng(SelectedFilesName
                                    ,800,600))
                ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");

    }else

    if(SelectedFilesName.endsWith(".bmp")){
                  if(
                   !ui->plot_curve->saveBmp(SelectedFilesName
                                           ,800,600))
                       ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");

     }

    }
     ui->plot_curve->graph(0)->setPen(QPen(Qt::green));

     /*
     ui->plot_curve->graph(1)->setPen(QPen(Qt::yellow));
     ui->plot_curve->graph(1)->setBrush(QBrush(Qt::yellow));
     */
     ui->plot_curve->setColor(QColor("black"));
     ui->plot_curve->replot();

       if(!QDesktopServices::openUrl(QUrl(SelectedFilesName)))
           ((MainWindow *)parent())->msg(MSG_ERROR,"Couldn't open the file"+SelectedFilesName);

}
