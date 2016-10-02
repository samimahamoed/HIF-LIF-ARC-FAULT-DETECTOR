#ifndef EMU_DATA_EDITOR_H
#define EMU_DATA_EDITOR_H

#include <QMainWindow>
#include "typedef.h"
#include "define.h"
#include "console.h"
#include "../fft/headers/qfouriertransformer.h"
#include <QVector>

#define     NO_OF_SLIDERS            100
#define     FFT_SIZE                 256
#define     MAX_SPECTRUM_MAGNITUDE   100

namespace Ui {
class emu_data_editor;
}

class emu_data_editor : public QMainWindow
{
    Q_OBJECT



signals:
     void edit_point_selection(bool status);
     void edit_point_deselection(bool status);
     void adjust_value(Int16 val);
     void set_value(Int16 val,Uint16 waveform);
     void action_set_value(double val,Uint8 type);
     void update_curve_signal();
     void update_value();
     void set_status(Uint8 select);
public:
    explicit emu_data_editor(QString type,QWidget *parent = 0);
    ~emu_data_editor();

    QString                          win_title;
    Uint8                            signal_type;
    double                           signal_freq;
    double                           signal_phase;
    double                           signal_amplitude;
    QVector<emu_data_edit_point_t>   edit_point_sets;
    QFourierTransformer              transformer;
    QVector<double>                  emu_data_yvalue;
    QVector<double>                  emu_data_xvalue;

    //dosent look good but this an effort to use existing libraries
    float                            fft_result[FFT_SIZE];
    float                            fft_inputs[FFT_SIZE];
    QVector<double>                  emu_data_freq_value;
    QVector<double>                  emu_data_spectrum_magnitude;
    float                            spectrum_magnitude_max;
    float                            spectrum_magnitude_min;
    QString dir_default;

public slots:
    void update_curve();
    void selection(bool clear);
private slots:


    void action_set_value_triggered(double val,Uint8 type);
    void on_actionWinEditor_toggled(bool arg1);

    void on_actionWinPlot_toggled(bool arg1);

    void on_actionOpen_triggered();

    void on_actionCSV_triggered();

    void on_btn_save_clicked();

    void on_actionMakeImg_triggered();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);
    //void mousePressEvent(QGraphicsSceneMouseEvent *event);
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    Ui::emu_data_editor *ui;

    void gui_init(Uint16 points);

    void plot_curve(QVector<double> xvalue,QVector<double> yvalue);

    void plot_curve_fft(QVector<double> xvalue,QVector<double> yvalue);

    void saveEventRecored(QString FileName);

    void loadFile(QString file_name);


    QString fileName;







};

#endif // EMU_DATA_EDITOR_H
