#ifndef RECORDPREVIEW_H
#define RECORDPREVIEW_H


#include "typedef.h"
#include "define.h"
#include "storage_structure.h"
#include <QVector>
#include <QMainWindow>

namespace Ui {
class recordPreview;
}

class recordPreview : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit recordPreview(QString type, QVector<Uint16> data, QWidget *parent = 0);
    explicit recordPreview(QString type, QVector<Uint16> data, event_data_t event_data, QWidget *parent = 0);
    explicit recordPreview(QString type, QVector<double> *data1, QVector<double> *data2=NULL, QVector<double> *data3=NULL, QWidget *parent = 0);

    ~recordPreview();

    double max(QVector<double> val);

private slots:


    void on_actionCreateCSV_triggered();

    void on_actionData_Plot_pdf_triggered();

    void on_actionData_plot_img_triggered();

    void on_actionWinDataPlot_toggled(bool arg1);

    void on_actionWinEventPlot_toggled(bool arg1);

private:
    Ui::recordPreview *ui;

    QString fileName;

    QVector<Uint16> rc_data;

    QVector<double> measured_values;
    QVector<double> history_amp;
    QVector<double> history_dfdt;

    QVector<double> time;

    event_data_t event_history;
    QString dir_default;




    void plotCurve(QVector<double> xvalue,QVector<double> yvalue);

    void plotPressureCurve(QVector<double> xvalue,QVector<double> yvalue);

    void plotBoth(QVector<double> xvalue, QVector<double> yvalue, QVector<double> y2value);

    void saveEventRecored(QString FileName);

    void plot_curve(QVector<double> xvalue, QVector<double> yvalue,Uint16 yrange);

    void plot_curve(QVector<double> xvalue, QVector<double> yvalue, Uint16 yrange, QVector<double> event_data);

    void plot_event(QVector<double> xvalue, QVector<double> yvalue, Uint16 yrange);
};

#endif // RECORDPREVIEW_H
