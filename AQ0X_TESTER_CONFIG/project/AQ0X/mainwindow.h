
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QStringListModel>
#include <QTreeWidgetItem>
#include "typedef.h"
#include "emu_data_editor.h"
#include "storage_structure.h"

#define PRESSURE_DATA_INDEX 104
#define DATA_END_INDEX 204
#define DATA_SIZE 100
#define RESOLUTION_MAPING_FACTOR  (5.0/255.0)
#define DEFAULT_DIR             "../DEFAULT_DIR/"
#define EVENT_RECORD_FILE       "event_records.txt"

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void msg(Uint16 type, QString msg);
    QString uint8_to_str(Uint8 val);

    void ListFiles();

    qint8 make8(QString buf, Uint16 idx);

    qint16 make16(QString buf, Uint16 idx);


    Uint16 crc16(Uint8 *puchMsg, Uint16 usDataLen);

    void set_emu_data(emu_data_editor *editor, Uint16 * data, Uint16 size);


    QString save_to_file(QString dir, QString type, Uint16 offset, QVector<double> *data1,
                      QVector<double> *data2=NULL, QVector<double> *data3=NULL);


    QString eventRecoredList;

    QString dir_default;

signals:
    void response_ready(QString);



public slots:
    void data_open(bool checked);

    void srv_cmds_data_download_msg_handler(QString data);

    void srv_cmds_data_transfer_msg_handler(QString data);

    void srv_cmds_general_msg_handler(QString data);




private slots:
    void about();

    void openSerialPort();

    void closeSerialPort();

    void serial_supervisor();

    void incoming_data_listener();

    void handleError(QSerialPort::SerialPortError error);

    void on_actionClear_triggered();

    void on_actionOpen_triggered();

    void on_actionCommands_triggered(bool checked);

    void on_dockWidget_sen_cmd_visibilityChanged(bool visible);

    void on_btn_execute_cmd_clicked(bool checked);

    void on_combo_Sen_activated(const QString &arg1);

    void on_dockWidget_sen_cmd_destroyed(QObject *arg1);

    void on_cmd_list_itemSelectionChanged();

    void on_btn_save_sys_info_clicked();

    void on_stacked_widget_views_currentChanged(int arg1);

    void on_btn_set_emu_data_clicked();

    void on_btn_back_emu_data_clicked();

    void on_btn_nxt_emu_data_clicked();

    void on_btn_cancel_clicked();

    void on_btn_back_thresholds_clicked();

    void on_btn_cancel_sys_info_clicked();

    void on_btn_back_analisis_clicked();

    void on_btn_cancel_options_clicked();

    void on_btn_save_thresholds_clicked();

    void on_btn_save_analisis_clicked();

    void on_btn_save_options_clicked();


private:
    void initActionsConnections();

    void updateDeviceStatus();

    void map_keys();

protected:


protected slots:
    bool eventFilter(QObject *object, QEvent *event);
private:
    Ui::MainWindow *ui;
    Console *console;
    SettingsDialog *settings;
    QSerialPort *serial;
    int state;
    int countRx;
    QString deviceID;

    Uint8  error;
    Uint8  eepromUsageUpper;
    Uint8  eepromUsageLower;
    Uint16 eepromUsage;

    QByteArray          rc_buffer;

    struct{
    emu_data_editor *   current_editor;
    QString             sen_type;
    bool                valid;
    bool                processed;
    Uint16              cmd;
    Uint16              data[EMU_MODE_DATA_LENGTH] __attribute__((packed, aligned(2)));
    }emu_data_buffer;

    QString EventRecordFileName;
    QByteArray cmd_line_separator;

    QStringListModel *model;

    QMap<QString, Uint16> map_cmd;

    QMap<Uint16, Uint16> map_size;

    QMap<QString, Uint8> map_trip_criterion;
    
    serial_msg_t     last_cmd;

    QVector<mult_sen_t> multi_cmds;
    qint8 vPos;



    void display_data(QByteArray rc_data);
    bool comm_Write(char *msg);
    Int8 protocol_send_cmd(serial_msg_t current_cmd, Uint16 *resp, Uint16 wait_resp);

    void Success(...);
    void Failure(...);

    bool srv_cmds_execute(serial_msg_t current_cmd, Uint16 *resp=NULL, Uint16 wait_resp=0);

    //bool save_record(sen_nov_data_u data, mult_sen_t *det);
    void loadFile(QString file_name);
    void reset_form();
    void cancel_processing_cmd(bool checked);
    void cmd_cancel();
    void retransmission(QString error);
    void record_preview(serial_msg_t current_cmd);
    bool msg_to_mem(char *msg, void *dest, Uint16 size);
};

#endif // MAINWINDOW_H
