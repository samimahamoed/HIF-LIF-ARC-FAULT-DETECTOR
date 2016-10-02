#include "mainwindow.h"
#include "recordpreview.h"
#include "ui_recordpreview.h"
#include <QFileDialog>
#include <QDesktopServices>
#include <QDateTime>
#include <QMessageBox>
#include <typedef.h>
#include <math.h>

recordPreview::recordPreview(QString type, QVector<Uint16> data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::recordPreview)
{
    ui->setupUi(this);
    this->setWindowTitle(type);

    dir_default = ((MainWindow *)parent)->dir_default;

    ui->centralwidget->setVisible(false);
    ui->dockWidgetPlot->setWidget(ui->curve_data);

    ui->actionWinDataPlot->setChecked(true);

    ui->actionWinEventPlot->setChecked(false);
    ui->actionWinEventPlot->setEnabled(false);

    ui->dockWidgetPlot_event_history->setVisible(false);

    this->rc_data = data;


    measured_values.resize(data.length());
    time.resize(data.length());
    history_dfdt.resize(data.length());

    
    for (int var = 0; var < data.length(); ++var) {
        time[var]=var;
        measured_values[var]=(rc_data[var]*ADC_VAL_MAPPING_FACTOR) - (type.contains("MFIELD")?1.65:0);

        if(var > 0)
            history_dfdt[var] = abs(measured_values[var])- abs(measured_values[var-1]);
    }

    plot_curve(time,measured_values,
              (type.contains("MFIELD")?WAVEFORM_PLUS_MINUS_1_65:WAVEFORM_0_TO_3_3));

}


recordPreview::recordPreview(QString type,QVector<Uint16> data, event_data_t event_data, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::recordPreview)
{
    ui->setupUi(this);
    this->setWindowTitle(type);

    ui->centralwidget->setVisible(false);
    ui->dockWidgetPlot->setWidget(ui->curve_data);
    ui->dockWidgetPlot_event_history->setWidget(ui->curve_dfdt);

    ui->actionWinDataPlot->setChecked(true);
    ui->actionWinEventPlot->setChecked(true);

    this->rc_data       = data;
    this->event_history  = event_data;

    measured_values.resize(data.length());
    history_amp.resize(data.length());
    history_dfdt.resize(data.length());
    time.resize(data.length());



    for (int var = 0; var < data.length(); ++var) {
        time[var]=var;
        measured_values[var]=(rc_data[var]*ADC_VAL_MAPPING_FACTOR)-(type.contains("MFIELD")?1.65:0);
    }



    //TODO: history amp shows when the module is activated
    /*
    for (int var = 0; var < MAX_EVENT_OBSERVATION_LENGTH; ++var) {
        history_amp[var + PRE_ACT_RECORD_BUFF_SIZE + event_history.offset]
                =(event_history.ignal_level[var]*ADC_VAL_MAPPING_FACTOR) - (type.contains("MFIELD")?1.65:0);

    }

    */
    plot_curve(time,measured_values,
              (type.contains("MFIELD")?WAVEFORM_PLUS_MINUS_1_65:WAVEFORM_0_TO_3_3),
              history_amp
              );



}


recordPreview::recordPreview(QString type, QVector<double> *data1, QVector<double> *data2, QVector<double> *data3, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::recordPreview)
{
    ui->setupUi(this);
    this->setWindowTitle(type);

    ui->centralwidget->setVisible(false);
    ui->dockWidgetPlot->setWidget(ui->curve_data);
    ui->dockWidgetPlot_event_history->setWidget(ui->curve_dfdt);


    ui->actionWinDataPlot->setChecked(true);

    measured_values.resize(data1->length());

    for (int var = 0; var < data1->length(); ++var)
      measured_values[var] = data1->at(var);

   // memmove(&measured_values,data1,sizeof(measured_values));

    time.resize(data1->length());
    for (int var = 0; var < data1->length(); ++var)
        time[var]=var;

    if((data2->length() > 0) && (data3->length()>0)){
        ui->actionWinEventPlot->setChecked(true);

        history_amp.resize(data2->length());
        memmove(&history_amp,data2,sizeof(history_amp));
        history_dfdt.resize(data3->length());
        memmove(&history_dfdt,data3,sizeof(history_dfdt));




        plot_curve(time,measured_values,
                  (type.contains("MFIELD")?WAVEFORM_PLUS_MINUS_1_65:WAVEFORM_0_TO_3_3),
                  history_amp
                  );



        plot_event(time,history_dfdt,max(history_dfdt));

    }else{
        ui->actionWinEventPlot->setChecked(false);
        ui->actionWinEventPlot->setEnabled(false);

        ui->dockWidgetPlot_event_history->setVisible(false);

        plot_curve(time,measured_values,
                  type.contains("MFIELD")?WAVEFORM_PLUS_MINUS_1_65:WAVEFORM_0_TO_3_3);


    }

}


recordPreview::~recordPreview()
{
    delete ui;
}


double recordPreview::max(QVector<double> val){
    double result = 0;
    for (int var = 0; var < val.length(); ++var)
        result = (result > val[var])?result:val[var];

    return result;
}

void recordPreview ::plot_curve(QVector<double> xvalue, QVector<double> yvalue, Uint16 yrange)
{

  ui->curve_data->addGraph();
  ui->curve_data->setColor(QColor("black"));

  ui->curve_data->graph(0)->setPen(QPen(Qt::green));
  ui->curve_data->graph(0)->setData(xvalue, yvalue);

  ui->curve_data->xAxis->setLabel("Sampling time (us)");
  ui->curve_data->yAxis->setLabel("Measured value (V)");

  ui->curve_data->xAxis->setLabelColor(QColor("black"));
  ui->curve_data->yAxis->setLabelColor(QColor("black"));

  ui->curve_data->xAxis->setGridPen(QPen(Qt::gray));
  ui->curve_data->yAxis->setGridPen(QPen(Qt::gray));
  ui->curve_data->xAxis2->setGridPen(QPen(Qt::gray));
  ui->curve_data->yAxis2->setGridPen(QPen(Qt::gray));

  ui->curve_data->xAxis->setTickPen(QPen(Qt::gray));
  ui->curve_data->yAxis->setTickPen(QPen(Qt::gray));

  ui->curve_data->xAxis->setBasePen(QPen(Qt::gray));
  ui->curve_data->yAxis->setBasePen(QPen(Qt::gray));

  ui->curve_data->xAxis->setTickLabelColor(QColor("gray"));
  ui->curve_data->yAxis->setTickLabelColor(QColor("gray"));

  ui->curve_data->xAxis->setTicks(true);



  ui->curve_data->xAxis->setSubGrid(true);
  ui->curve_data->xAxis->setTickStep(1);

  ui->curve_data->xAxis->setSubGridPen(QPen(Qt::gray));
  ui->curve_data->yAxis->setSubGridPen(QPen(Qt::gray));

  ui->curve_data->xAxis2->setVisible(true);
  ui->curve_data->xAxis2->setTickLabels(false);
  ui->curve_data->yAxis2->setVisible(true);
  ui->curve_data->yAxis2->setTickLabels(false);
  ui->curve_data->xAxis2->setBasePen(QPen(Qt::gray));
  ui->curve_data->yAxis2->setBasePen(QPen(Qt::gray));

   ui->curve_data->xAxis->setGrid(true);
   ui->curve_data->yAxis->setGrid(true);


  ui->curve_data->xAxis->setRange(0, xvalue.length());
  if(yrange == WAVEFORM_PLUS_MINUS_1_65)
  ui->curve_data->yAxis->setRange(-1.65, 1.65);
  else
  ui->curve_data->yAxis->setRange(0, 3.34);
}



void recordPreview ::plot_curve(QVector<double> xvalue,QVector<double> yvalue, Uint16 yrange,QVector<double> event_data)
{
    QVector<double> section;

    section.clear();
    for (int var = 0; var < xvalue.length(); ++var) {
        if(var < PRE_ACT_RECORD_BUFF_SIZE)
        section.append(yvalue[var]);
        else
        section.append(0);
    }



    ui->curve_data->addGraph();
    ui->curve_data->graph(0)->setPen(QPen(Qt::lightGray));

    ui->curve_data->graph(0)->setData(xvalue, section);
    ui->curve_data->graph(0)->setBrush(QBrush(Qt::yellow));

    ui->curve_data->addGraph();
    ui->curve_data->graph(1)->setPen(QPen(Qt::green));
    ui->curve_data->graph(1)->setData(xvalue, yvalue);

     ui->curve_data->setColor(QColor("black"));

    ui->curve_data->xAxis->setLabel("time:  t x"+QString::number(FAST_TIMER_SAMPLING_RATE_XU)+"us");
    ui->curve_data->yAxis->setLabel("Measured Value (V)");

    ui->curve_data->xAxis->setLabelColor(QColor("gray"));
    ui->curve_data->yAxis->setLabelColor(QColor("gray"));

    ui->curve_data->xAxis->setGridPen(QPen(Qt::gray));
    ui->curve_data->yAxis->setGridPen(QPen(Qt::gray));
    ui->curve_data->xAxis2->setGridPen(QPen(Qt::gray));
    ui->curve_data->yAxis2->setGridPen(QPen(Qt::gray));

    ui->curve_data->xAxis->setTickPen(QPen(Qt::gray));
    ui->curve_data->yAxis->setTickPen(QPen(Qt::gray));

    ui->curve_data->xAxis->setBasePen(QPen(Qt::gray));
    ui->curve_data->yAxis->setBasePen(QPen(Qt::gray));

    ui->curve_data->xAxis->setTickLabelColor(QColor("gray"));
    ui->curve_data->yAxis->setTickLabelColor(QColor("gray"));

    ui->curve_data->xAxis->setTicks(true);



    ui->curve_data->xAxis->setSubGrid(true);
    ui->curve_data->xAxis->setTickStep(FAST_TIMER_SAMPLING_RATE_XU);

    ui->curve_data->xAxis->setSubGridPen(QPen(Qt::gray));
    ui->curve_data->yAxis->setSubGridPen(QPen(Qt::gray));

    ui->curve_data->xAxis2->setVisible(true);
    ui->curve_data->xAxis2->setTickLabels(false);
    ui->curve_data->yAxis2->setVisible(true);
    ui->curve_data->yAxis2->setTickLabels(false);
    ui->curve_data->xAxis2->setBasePen(QPen(Qt::gray));
    ui->curve_data->yAxis2->setBasePen(QPen(Qt::gray));

     ui->curve_data->xAxis->setGrid(true);
     ui->curve_data->yAxis->setGrid(true);

  ui->curve_data->xAxis->setRange(0, xvalue.length());
  if(yrange == WAVEFORM_PLUS_MINUS_1_65)
  ui->curve_data->yAxis->setRange(-1.65, 1.65);
  else
  ui->curve_data->yAxis->setRange(0, 3.34);
}


void recordPreview ::plot_event(QVector<double> xvalue, QVector<double> yvalue, Uint16 yrange)
{

  ui->curve_dfdt->addGraph();
  ui->curve_dfdt->setColor(QColor("black"));

  ui->curve_dfdt->graph(0)->setPen(QPen(Qt::green));
  ui->curve_dfdt->graph(0)->setBrush(QBrush(Qt::yellow));
  ui->curve_dfdt->graph(0)->setData(xvalue, yvalue);

  ui->curve_dfdt->xAxis->setLabel("Sampling time (us)");
  ui->curve_dfdt->yAxis->setLabel("dv/dt         (v/s)");

  ui->curve_dfdt->xAxis->setLabelColor(QColor("black"));
  ui->curve_dfdt->yAxis->setLabelColor(QColor("black"));

  ui->curve_dfdt->xAxis->setGridPen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis->setGridPen(QPen(Qt::gray));
  ui->curve_dfdt->xAxis2->setGridPen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis2->setGridPen(QPen(Qt::gray));

  ui->curve_dfdt->xAxis->setTickPen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis->setTickPen(QPen(Qt::gray));

  ui->curve_dfdt->xAxis->setBasePen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis->setBasePen(QPen(Qt::gray));

  ui->curve_dfdt->xAxis->setTickLabelColor(QColor("gray"));
  ui->curve_dfdt->yAxis->setTickLabelColor(QColor("gray"));

  ui->curve_dfdt->xAxis->setTicks(true);



  ui->curve_dfdt->xAxis->setSubGrid(true);
  ui->curve_dfdt->xAxis->setTickStep(1.0);

  ui->curve_dfdt->xAxis->setSubGridPen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis->setSubGridPen(QPen(Qt::gray));

  ui->curve_dfdt->xAxis2->setVisible(true);
  ui->curve_dfdt->xAxis2->setTickLabels(false);
  ui->curve_dfdt->yAxis2->setVisible(true);
  ui->curve_dfdt->yAxis2->setTickLabels(false);
  ui->curve_dfdt->xAxis2->setBasePen(QPen(Qt::gray));
  ui->curve_dfdt->yAxis2->setBasePen(QPen(Qt::gray));

   ui->curve_dfdt->xAxis->setGrid(true);
   ui->curve_dfdt->yAxis->setGrid(true);


  ui->curve_dfdt->xAxis->setRange(0, xvalue.length());
  ui->curve_dfdt->yAxis->setRange(0, yrange);
}



void recordPreview::saveEventRecored(QString FileName){

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

void recordPreview::on_actionCreateCSV_triggered()
{

    QDir default_dir;

    if(!default_dir.exists(default_dir.toNativeSeparators(dir_default)))
        default_dir.mkdir(default_dir.toNativeSeparators(dir_default));

    QString DefaultFileName = this->windowTitle();



    DefaultFileName.replace(':',"-").replace(' ',"_").replace('.',"_");

    DefaultFileName = default_dir.toNativeSeparators(dir_default)+ DefaultFileName;

    QString SelectedFilesName = QFileDialog::getSaveFileName(
        this,
        tr("Save As"),
        DefaultFileName,
        tr("Comma Separated Values (*.csv)"));



    if (SelectedFilesName.isEmpty()) return;
    else {
        QFile file(SelectedFilesName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QTextStream stream(&file);

        stream << "Title,"+this->windowTitle()+"\n";

        stream << "Record length,"+QString::number(measured_values.length())+"\n";


        stream << "TIME, MEASURED DATA\n";



        for(int ii=0;ii< measured_values.length();ii++)
           stream << QString::number(ii) +","+QString::number(measured_values[ii])+"\n";



        file.close();



        try {
            QDesktopServices::openUrl(QUrl("file:"+ SelectedFilesName));

        } catch (...) {

        }
    }


}

void recordPreview::on_actionData_Plot_pdf_triggered()
{
    QDir default_dir;

    if(!default_dir.exists(default_dir.toNativeSeparators(dir_default)))
        default_dir.mkdir(default_dir.toNativeSeparators(dir_default));

    QString DefaultFileName = this->windowTitle();



    DefaultFileName.replace(':',"-").replace(' ',"_").replace('.',"_");

    DefaultFileName = default_dir.toNativeSeparators(dir_default)+ DefaultFileName;



    QString SelectedFilesName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        DefaultFileName,
                                                         tr("PDF (*.pdf);"));


    if(SelectedFilesName == NULL)
        return;

    ui->curve_data->graph(0)->setPen(QPen(Qt::black));
    /*
    ui->curve_data->graph(1)->setPen(QPen(Qt::black));
    ui->curve_data->graph(1)->setPen(QPen(Qt::DotLine));
    ui->curve_data->graph(1)->setBrush(QBrush(Qt::gray));
    */
    ui->curve_data->setColor(QColor("white"));
    ui->curve_data->replot();

      if(
        !ui->curve_data->savePdf(SelectedFilesName
                                ,false,800,600))
            ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");

       ui->curve_data->graph(0)->setPen(QPen(Qt::green));
      /*
       ui->curve_data->graph(1)->setPen(QPen(Qt::yellow));
       ui->curve_data->graph(1)->setBrush(QBrush(Qt::yellow));
       */
       ui->curve_data->setColor(QColor("black"));
       ui->curve_data->replot();

       if(!QDesktopServices::openUrl(QUrl(SelectedFilesName)))
           ((MainWindow *)parent())->msg(MSG_ERROR,"Couldn't open the file"+SelectedFilesName);

}

void recordPreview::on_actionData_plot_img_triggered()
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

    ui->curve_data->graph(0)->setPen(QPen(Qt::green));
    /*
    ui->curve_data->graph(1)->setPen(QPen(Qt::red));
    ui->curve_data->graph(1)->setPen(QPen(Qt::DotLine));
    ui->curve_data->graph(1)->setBrush(QBrush(Qt::gray));
    */

    ui->curve_data->setColor(QColor("white"));
    ui->curve_data->replot();




    if(SelectedFilesName.indexOf('.')==-1)
        SelectedFilesName.append(".jpg");

    if((SelectedFilesName.indexOf('.')==SelectedFilesName.length()-4)){
    if(SelectedFilesName.endsWith(".jpg")){
       if(
        !ui->curve_data->saveJpg(SelectedFilesName
                                ,800,600))
            ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");
    }else

    if(SelectedFilesName.endsWith(".png")){
           if(
            !ui->curve_data->savePng(SelectedFilesName
                                    ,800,600))
                ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");

    }else

    if(SelectedFilesName.endsWith(".bmp")){
                  if(
                   !ui->curve_data->saveBmp(SelectedFilesName
                                           ,800,600))
                       ((MainWindow *)parent())->msg(MSG_ERROR,"Error: Couldn't save data");

     }

    }
     ui->curve_data->graph(0)->setPen(QPen(Qt::green));

     /*
     ui->curve_data->graph(1)->setPen(QPen(Qt::yellow));
     ui->curve_data->graph(1)->setBrush(QBrush(Qt::yellow));
     */
     ui->curve_data->setColor(QColor("black"));
     ui->curve_data->replot();

       if(!QDesktopServices::openUrl(QUrl(SelectedFilesName)))
           ((MainWindow *)parent())->msg(MSG_ERROR,"Couldn't open the file"+SelectedFilesName);

}

void recordPreview::on_actionWinDataPlot_toggled(bool arg1)
{
    ui->dockWidgetPlot->setVisible(arg1);
    ui->actionWinDataPlot->setChecked(arg1);
}

void recordPreview::on_actionWinEventPlot_toggled(bool arg1)
{
    ui->dockWidgetPlot_event_history->setVisible(arg1);
    ui->actionWinEventPlot->setChecked(arg1);
}
