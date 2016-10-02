/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "console.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionQuit;
    QAction *actionConfigure;
    QAction *actionClear;
    QAction *actionAbout;
    QAction *actionOpen;
    QAction *actionCommands;
    QWidget *centralWidget;
    QDockWidget *dockWidget_sen_cmd;
    QWidget *dockWidgetContents_6;
    QGridLayout *gridLayout_2;
    QTreeWidget *cmd_list;
    QStackedWidget *stacked_widget_views;
    QWidget *page;
    QWidget *page_2;
    QGridLayout *gridLayout_4;
    QGroupBox *gBox_cmd;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QComboBox *combo_Ch;
    QComboBox *combo_Sen;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *sArea_progress_view;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_3;
    QVBoxLayout *sAreaVLayout;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_progress_view;
    QHBoxLayout *horizontalLayout_23;
    QPushButton *btn_cancel;
    QPushButton *btn_execute_cmd;
    QWidget *page_3;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QCheckBox *cb_config_ir_max;
    QSpinBox *dSB_ir_max;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *cb_config_uv_max;
    QSpinBox *dSB_uv_max;
    QLabel *label_5;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_22;
    QPushButton *btn_back_thresholds;
    QPushButton *btn_save_thresholds;
    QWidget *page_4;
    QLabel *label_6;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QCheckBox *cb_config_min_distance;
    QSpinBox *dSB_min_distance;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_8;
    QHBoxLayout *horizontalLayout_9;
    QCheckBox *cb_config_rated_current;
    QSpinBox *dSB_rated_current;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *cb_config_sh_ckt_current;
    QSpinBox *dSB_sh_ckt_current;
    QHBoxLayout *horizontalLayout_16;
    QCheckBox *cb_config_freq;
    QSpinBox *dSB_freq;
    QWidget *layoutWidget_3;
    QHBoxLayout *horizontalLayout_25;
    QPushButton *btn_cancel_sys_info;
    QPushButton *btn_save_sys_info;
    QWidget *page_5;
    QLabel *label_7;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout_7;
    QCheckBox *cb_config_active_modules_ir;
    QCheckBox *cb_config_active_modules_uv;
    QCheckBox *cb_config_active_modules_mf;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_10;
    QHBoxLayout *horizontalLayout_12;
    QCheckBox *cb_config_observation_length;
    QSpinBox *dSB_observation_length;
    QHBoxLayout *horizontalLayout_13;
    QCheckBox *cb_config_probablity_max;
    QSpinBox *dSB_probablity_max;
    QHBoxLayout *horizontalLayout_14;
    QCheckBox *cb_config_probablity_min;
    QSpinBox *dSB_probablity_min;
    QWidget *layoutWidget_4;
    QHBoxLayout *horizontalLayout_26;
    QPushButton *btn_back_analisis;
    QPushButton *btn_save_analisis;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_9;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *cb_config_criteria;
    QComboBox *combo_alarm_condition;
    QWidget *page_6;
    QLabel *label_8;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_12;
    QHBoxLayout *horizontalLayout_18;
    QCheckBox *cb_config_sv_mfield;
    QHBoxLayout *horizontalLayout_17;
    QCheckBox *cb_config_threshold_adjust;
    QHBoxLayout *horizontalLayout_19;
    QCheckBox *cb_config_sv_uv;
    QCheckBox *cb_config_sv_ir;
    QWidget *layoutWidget_5;
    QHBoxLayout *horizontalLayout_27;
    QPushButton *btn_cancel_options;
    QPushButton *btn_save_options;
    QWidget *page_7;
    QPushButton *btn_set_emu_data;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_24;
    QPushButton *btn_back_emu_data;
    QPushButton *btn_nxt_emu_data;
    QGroupBox *gBox_range;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QSpinBox *spinBox_start;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QSpinBox *spinBox_end;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    Console *terminal;
    QMenuBar *menuBar;
    QMenu *menuConnection;
    QMenu *menuTools;
    QMenu *menuHelp;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QDockWidget *dock_central;
    QWidget *dockWidgetContents_27;
    QGridLayout *gridLayout;
    QWidget *widget_20;
    QGridLayout *gridLayout_153;
    QLabel *centralLogo;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(419, 665);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/logo"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icon/connect"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/disconnect"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon2);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/exit"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon3);
        actionConfigure = new QAction(MainWindow);
        actionConfigure->setObjectName(QStringLiteral("actionConfigure"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/setting"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfigure->setIcon(icon4);
        actionConfigure->setText(QStringLiteral("Serial Configure"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icon/clear"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon5);
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/logo/icons/open_file-icon.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon6);
        actionCommands = new QAction(MainWindow);
        actionCommands->setObjectName(QStringLiteral("actionCommands"));
        actionCommands->setCheckable(true);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        dockWidget_sen_cmd = new QDockWidget(centralWidget);
        dockWidget_sen_cmd->setObjectName(QStringLiteral("dockWidget_sen_cmd"));
        dockWidget_sen_cmd->setGeometry(QRect(40, 0, 740, 671));
        sizePolicy.setHeightForWidth(dockWidget_sen_cmd->sizePolicy().hasHeightForWidth());
        dockWidget_sen_cmd->setSizePolicy(sizePolicy);
        dockWidget_sen_cmd->setMinimumSize(QSize(740, 344));
        dockWidget_sen_cmd->setMaximumSize(QSize(800, 800));
        dockWidget_sen_cmd->setAcceptDrops(false);
        dockWidget_sen_cmd->setFeatures(QDockWidget::AllDockWidgetFeatures);
        dockWidget_sen_cmd->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::RightDockWidgetArea|Qt::TopDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_6);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        cmd_list = new QTreeWidget(dockWidgetContents_6);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/icon/icons/options.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setIcon(0, icon7);
        cmd_list->setHeaderItem(__qtreewidgetitem);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/icon/Address"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/icon/sys_info"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/icon/icons/signal.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/icon/icons/calculation.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon12;
        icon12.addFile(QStringLiteral(":/icon/icons/system_options.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon13;
        icon13.addFile(QStringLiteral(":/icon/icons/default.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon14;
        icon14.addFile(QStringLiteral(":/icon/icons/upload.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon15;
        icon15.addFile(QStringLiteral(":/icon/icons/confirm.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon16;
        icon16.addFile(QStringLiteral(":/icon/Download"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon17;
        icon17.addFile(QStringLiteral(":/icon/icons/conditional.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon18;
        icon18.addFile(QStringLiteral(":/icon/icons/reset.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon19;
        icon19.addFile(QStringLiteral(":/icon/icons/clear_data.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        QIcon icon20;
        icon20.addFile(QStringLiteral(":/icon/icons/sv_status.png"), QSize(), QIcon::Normal, QIcon::Off);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(cmd_list);
        __qtreewidgetitem1->setIcon(0, icon4);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem2->setIcon(0, icon8);
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem3->setIcon(0, icon9);
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem4->setIcon(0, icon10);
        QTreeWidgetItem *__qtreewidgetitem5 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem5->setIcon(0, icon11);
        QTreeWidgetItem *__qtreewidgetitem6 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem6->setIcon(0, icon12);
        QTreeWidgetItem *__qtreewidgetitem7 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem7->setIcon(0, icon13);
        QTreeWidgetItem *__qtreewidgetitem8 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem8->setIcon(0, icon14);
        QTreeWidgetItem *__qtreewidgetitem9 = new QTreeWidgetItem(__qtreewidgetitem1);
        __qtreewidgetitem9->setIcon(0, icon15);
        QTreeWidgetItem *__qtreewidgetitem10 = new QTreeWidgetItem(cmd_list);
        __qtreewidgetitem10->setIcon(0, icon14);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        new QTreeWidgetItem(__qtreewidgetitem10);
        QTreeWidgetItem *__qtreewidgetitem11 = new QTreeWidgetItem(cmd_list);
        __qtreewidgetitem11->setIcon(0, icon16);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        new QTreeWidgetItem(__qtreewidgetitem11);
        QTreeWidgetItem *__qtreewidgetitem12 = new QTreeWidgetItem(cmd_list);
        __qtreewidgetitem12->setIcon(0, icon17);
        QTreeWidgetItem *__qtreewidgetitem13 = new QTreeWidgetItem(__qtreewidgetitem12);
        __qtreewidgetitem13->setIcon(0, icon18);
        QTreeWidgetItem *__qtreewidgetitem14 = new QTreeWidgetItem(__qtreewidgetitem12);
        __qtreewidgetitem14->setIcon(0, icon19);
        QTreeWidgetItem *__qtreewidgetitem15 = new QTreeWidgetItem(__qtreewidgetitem12);
        __qtreewidgetitem15->setIcon(0, icon20);
        cmd_list->setObjectName(QStringLiteral("cmd_list"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmd_list->sizePolicy().hasHeightForWidth());
        cmd_list->setSizePolicy(sizePolicy1);
        cmd_list->setMinimumSize(QSize(300, 0));
        cmd_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        cmd_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
        cmd_list->setAlternatingRowColors(false);
        cmd_list->setColumnCount(1);
        cmd_list->header()->setCascadingSectionResizes(true);
        cmd_list->header()->setHighlightSections(true);
        cmd_list->header()->setProperty("showSortIndicator", QVariant(false));

        gridLayout_2->addWidget(cmd_list, 0, 0, 2, 1);

        stacked_widget_views = new QStackedWidget(dockWidgetContents_6);
        stacked_widget_views->setObjectName(QStringLiteral("stacked_widget_views"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(stacked_widget_views->sizePolicy().hasHeightForWidth());
        stacked_widget_views->setSizePolicy(sizePolicy2);
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        stacked_widget_views->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        gridLayout_4 = new QGridLayout(page_2);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gBox_cmd = new QGroupBox(page_2);
        gBox_cmd->setObjectName(QStringLiteral("gBox_cmd"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(gBox_cmd->sizePolicy().hasHeightForWidth());
        gBox_cmd->setSizePolicy(sizePolicy3);
        gBox_cmd->setMinimumSize(QSize(389, 180));
        verticalLayout_6 = new QVBoxLayout(gBox_cmd);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBox_3 = new QGroupBox(gBox_cmd);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy4);
        groupBox_3->setMinimumSize(QSize(361, 100));
        groupBox_3->setMaximumSize(QSize(16777215, 91));
        layoutWidget = new QWidget(groupBox_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 30, 331, 51));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        combo_Ch = new QComboBox(layoutWidget);
        combo_Ch->setObjectName(QStringLiteral("combo_Ch"));
        combo_Ch->setEditable(false);

        horizontalLayout->addWidget(combo_Ch);

        combo_Sen = new QComboBox(layoutWidget);
        combo_Sen->setObjectName(QStringLiteral("combo_Sen"));
        combo_Sen->setEditable(true);

        horizontalLayout->addWidget(combo_Sen);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_6->addWidget(groupBox_3);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        sArea_progress_view = new QScrollArea(gBox_cmd);
        sArea_progress_view->setObjectName(QStringLiteral("sArea_progress_view"));
        sArea_progress_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        sArea_progress_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sArea_progress_view->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 340, 31));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        sAreaVLayout = new QVBoxLayout();
        sAreaVLayout->setSpacing(0);
        sAreaVLayout->setObjectName(QStringLiteral("sAreaVLayout"));

        gridLayout_3->addLayout(sAreaVLayout, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        sArea_progress_view->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(sArea_progress_view);

        verticalSpacer_progress_view = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout_5->addItem(verticalSpacer_progress_view);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        btn_cancel = new QPushButton(gBox_cmd);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));
        btn_cancel->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_23->addWidget(btn_cancel);

        btn_execute_cmd = new QPushButton(gBox_cmd);
        btn_execute_cmd->setObjectName(QStringLiteral("btn_execute_cmd"));
        btn_execute_cmd->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
" \n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_23->addWidget(btn_execute_cmd);


        verticalLayout_5->addLayout(horizontalLayout_23);


        verticalLayout_6->addLayout(verticalLayout_5);


        gridLayout_4->addWidget(gBox_cmd, 0, 0, 1, 1);

        stacked_widget_views->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        groupBox_4 = new QGroupBox(page_3);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 170, 376, 111));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(groupBox_4->sizePolicy().hasHeightForWidth());
        groupBox_4->setSizePolicy(sizePolicy5);
        groupBox_4->setMinimumSize(QSize(361, 100));
        groupBox_4->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_5 = new QGridLayout(groupBox_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        cb_config_ir_max = new QCheckBox(groupBox_4);
        cb_config_ir_max->setObjectName(QStringLiteral("cb_config_ir_max"));
        sizePolicy5.setHeightForWidth(cb_config_ir_max->sizePolicy().hasHeightForWidth());
        cb_config_ir_max->setSizePolicy(sizePolicy5);
        cb_config_ir_max->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
" "));

        horizontalLayout_4->addWidget(cb_config_ir_max);

        dSB_ir_max = new QSpinBox(groupBox_4);
        dSB_ir_max->setObjectName(QStringLiteral("dSB_ir_max"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(dSB_ir_max->sizePolicy().hasHeightForWidth());
        dSB_ir_max->setSizePolicy(sizePolicy6);
        dSB_ir_max->setMinimumSize(QSize(100, 0));
        dSB_ir_max->setMaximum(3000);

        horizontalLayout_4->addWidget(dSB_ir_max);


        gridLayout_5->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        cb_config_uv_max = new QCheckBox(groupBox_4);
        cb_config_uv_max->setObjectName(QStringLiteral("cb_config_uv_max"));
        sizePolicy5.setHeightForWidth(cb_config_uv_max->sizePolicy().hasHeightForWidth());
        cb_config_uv_max->setSizePolicy(sizePolicy5);
        cb_config_uv_max->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        horizontalLayout_5->addWidget(cb_config_uv_max);

        dSB_uv_max = new QSpinBox(groupBox_4);
        dSB_uv_max->setObjectName(QStringLiteral("dSB_uv_max"));
        sizePolicy6.setHeightForWidth(dSB_uv_max->sizePolicy().hasHeightForWidth());
        dSB_uv_max->setSizePolicy(sizePolicy6);
        dSB_uv_max->setMinimumSize(QSize(100, 0));
        dSB_uv_max->setMaximum(60000);

        horizontalLayout_5->addWidget(dSB_uv_max);


        gridLayout_5->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        label_5 = new QLabel(page_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(20, 30, 381, 131));
        label_5->setTextFormat(Qt::RichText);
        label_5->setScaledContents(true);
        label_5->setWordWrap(true);
        layoutWidget1 = new QWidget(page_3);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 590, 381, 36));
        horizontalLayout_22 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        horizontalLayout_22->setContentsMargins(0, 0, 0, 0);
        btn_back_thresholds = new QPushButton(layoutWidget1);
        btn_back_thresholds->setObjectName(QStringLiteral("btn_back_thresholds"));
        btn_back_thresholds->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_22->addWidget(btn_back_thresholds);

        btn_save_thresholds = new QPushButton(layoutWidget1);
        btn_save_thresholds->setObjectName(QStringLiteral("btn_save_thresholds"));
        btn_save_thresholds->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_22->addWidget(btn_save_thresholds);

        stacked_widget_views->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        label_6 = new QLabel(page_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 20, 381, 131));
        label_6->setTextFormat(Qt::RichText);
        label_6->setScaledContents(true);
        label_6->setWordWrap(true);
        groupBox_6 = new QGroupBox(page_4);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(0, 360, 376, 71));
        sizePolicy5.setHeightForWidth(groupBox_6->sizePolicy().hasHeightForWidth());
        groupBox_6->setSizePolicy(sizePolicy5);
        groupBox_6->setMinimumSize(QSize(361, 0));
        groupBox_6->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_7 = new QGridLayout(groupBox_6);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        cb_config_min_distance = new QCheckBox(groupBox_6);
        cb_config_min_distance->setObjectName(QStringLiteral("cb_config_min_distance"));
        sizePolicy5.setHeightForWidth(cb_config_min_distance->sizePolicy().hasHeightForWidth());
        cb_config_min_distance->setSizePolicy(sizePolicy5);
        cb_config_min_distance->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
""));

        horizontalLayout_8->addWidget(cb_config_min_distance);

        dSB_min_distance = new QSpinBox(groupBox_6);
        dSB_min_distance->setObjectName(QStringLiteral("dSB_min_distance"));
        sizePolicy6.setHeightForWidth(dSB_min_distance->sizePolicy().hasHeightForWidth());
        dSB_min_distance->setSizePolicy(sizePolicy6);
        dSB_min_distance->setMinimumSize(QSize(100, 0));
        dSB_min_distance->setMinimum(50);
        dSB_min_distance->setMaximum(2000);

        horizontalLayout_8->addWidget(dSB_min_distance);


        gridLayout_7->addLayout(horizontalLayout_8, 0, 0, 1, 1);

        groupBox_7 = new QGroupBox(page_4);
        groupBox_7->setObjectName(QStringLiteral("groupBox_7"));
        groupBox_7->setGeometry(QRect(0, 160, 376, 151));
        sizePolicy5.setHeightForWidth(groupBox_7->sizePolicy().hasHeightForWidth());
        groupBox_7->setSizePolicy(sizePolicy5);
        groupBox_7->setMinimumSize(QSize(361, 100));
        groupBox_7->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_8 = new QGridLayout(groupBox_7);
        gridLayout_8->setSpacing(6);
        gridLayout_8->setContentsMargins(11, 11, 11, 11);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        cb_config_rated_current = new QCheckBox(groupBox_7);
        cb_config_rated_current->setObjectName(QStringLiteral("cb_config_rated_current"));
        sizePolicy5.setHeightForWidth(cb_config_rated_current->sizePolicy().hasHeightForWidth());
        cb_config_rated_current->setSizePolicy(sizePolicy5);
        cb_config_rated_current->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
""));

        horizontalLayout_9->addWidget(cb_config_rated_current);

        dSB_rated_current = new QSpinBox(groupBox_7);
        dSB_rated_current->setObjectName(QStringLiteral("dSB_rated_current"));
        sizePolicy6.setHeightForWidth(dSB_rated_current->sizePolicy().hasHeightForWidth());
        dSB_rated_current->setSizePolicy(sizePolicy6);
        dSB_rated_current->setMinimumSize(QSize(100, 0));
        dSB_rated_current->setMaximum(3000);

        horizontalLayout_9->addWidget(dSB_rated_current);


        gridLayout_8->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        cb_config_sh_ckt_current = new QCheckBox(groupBox_7);
        cb_config_sh_ckt_current->setObjectName(QStringLiteral("cb_config_sh_ckt_current"));
        sizePolicy5.setHeightForWidth(cb_config_sh_ckt_current->sizePolicy().hasHeightForWidth());
        cb_config_sh_ckt_current->setSizePolicy(sizePolicy5);
        cb_config_sh_ckt_current->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_10->addWidget(cb_config_sh_ckt_current);

        dSB_sh_ckt_current = new QSpinBox(groupBox_7);
        dSB_sh_ckt_current->setObjectName(QStringLiteral("dSB_sh_ckt_current"));
        sizePolicy6.setHeightForWidth(dSB_sh_ckt_current->sizePolicy().hasHeightForWidth());
        dSB_sh_ckt_current->setSizePolicy(sizePolicy6);
        dSB_sh_ckt_current->setMinimumSize(QSize(100, 0));
        dSB_sh_ckt_current->setMaximum(60000);

        horizontalLayout_10->addWidget(dSB_sh_ckt_current);


        gridLayout_8->addLayout(horizontalLayout_10, 1, 0, 1, 1);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setSpacing(6);
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        cb_config_freq = new QCheckBox(groupBox_7);
        cb_config_freq->setObjectName(QStringLiteral("cb_config_freq"));
        sizePolicy5.setHeightForWidth(cb_config_freq->sizePolicy().hasHeightForWidth());
        cb_config_freq->setSizePolicy(sizePolicy5);
        cb_config_freq->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_16->addWidget(cb_config_freq);

        dSB_freq = new QSpinBox(groupBox_7);
        dSB_freq->setObjectName(QStringLiteral("dSB_freq"));
        sizePolicy6.setHeightForWidth(dSB_freq->sizePolicy().hasHeightForWidth());
        dSB_freq->setSizePolicy(sizePolicy6);
        dSB_freq->setMinimumSize(QSize(100, 0));
        dSB_freq->setMaximum(100);

        horizontalLayout_16->addWidget(dSB_freq);


        gridLayout_8->addLayout(horizontalLayout_16, 2, 0, 1, 1);

        layoutWidget_3 = new QWidget(page_4);
        layoutWidget_3->setObjectName(QStringLiteral("layoutWidget_3"));
        layoutWidget_3->setGeometry(QRect(0, 580, 381, 36));
        horizontalLayout_25 = new QHBoxLayout(layoutWidget_3);
        horizontalLayout_25->setSpacing(6);
        horizontalLayout_25->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_25->setObjectName(QStringLiteral("horizontalLayout_25"));
        horizontalLayout_25->setContentsMargins(0, 0, 0, 0);
        btn_cancel_sys_info = new QPushButton(layoutWidget_3);
        btn_cancel_sys_info->setObjectName(QStringLiteral("btn_cancel_sys_info"));
        btn_cancel_sys_info->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_25->addWidget(btn_cancel_sys_info);

        btn_save_sys_info = new QPushButton(layoutWidget_3);
        btn_save_sys_info->setObjectName(QStringLiteral("btn_save_sys_info"));
        btn_save_sys_info->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_25->addWidget(btn_save_sys_info);

        stacked_widget_views->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        label_7 = new QLabel(page_5);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 20, 381, 131));
        label_7->setTextFormat(Qt::RichText);
        label_7->setScaledContents(true);
        label_7->setWordWrap(true);
        groupBox_8 = new QGroupBox(page_5);
        groupBox_8->setObjectName(QStringLiteral("groupBox_8"));
        groupBox_8->setGeometry(QRect(10, 380, 371, 152));
        sizePolicy5.setHeightForWidth(groupBox_8->sizePolicy().hasHeightForWidth());
        groupBox_8->setSizePolicy(sizePolicy5);
        groupBox_8->setMinimumSize(QSize(40, 0));
        groupBox_8->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_6 = new QGridLayout(groupBox_8);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        cb_config_active_modules_ir = new QCheckBox(groupBox_8);
        cb_config_active_modules_ir->setObjectName(QStringLiteral("cb_config_active_modules_ir"));
        sizePolicy5.setHeightForWidth(cb_config_active_modules_ir->sizePolicy().hasHeightForWidth());
        cb_config_active_modules_ir->setSizePolicy(sizePolicy5);
        cb_config_active_modules_ir->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        verticalLayout_7->addWidget(cb_config_active_modules_ir);

        cb_config_active_modules_uv = new QCheckBox(groupBox_8);
        cb_config_active_modules_uv->setObjectName(QStringLiteral("cb_config_active_modules_uv"));
        sizePolicy5.setHeightForWidth(cb_config_active_modules_uv->sizePolicy().hasHeightForWidth());
        cb_config_active_modules_uv->setSizePolicy(sizePolicy5);
        cb_config_active_modules_uv->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        verticalLayout_7->addWidget(cb_config_active_modules_uv);

        cb_config_active_modules_mf = new QCheckBox(groupBox_8);
        cb_config_active_modules_mf->setObjectName(QStringLiteral("cb_config_active_modules_mf"));
        sizePolicy5.setHeightForWidth(cb_config_active_modules_mf->sizePolicy().hasHeightForWidth());
        cb_config_active_modules_mf->setSizePolicy(sizePolicy5);
        cb_config_active_modules_mf->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        verticalLayout_7->addWidget(cb_config_active_modules_mf);


        gridLayout_6->addLayout(verticalLayout_7, 0, 0, 1, 1);

        groupBox_9 = new QGroupBox(page_5);
        groupBox_9->setObjectName(QStringLiteral("groupBox_9"));
        groupBox_9->setGeometry(QRect(10, 120, 376, 151));
        sizePolicy5.setHeightForWidth(groupBox_9->sizePolicy().hasHeightForWidth());
        groupBox_9->setSizePolicy(sizePolicy5);
        groupBox_9->setMinimumSize(QSize(361, 100));
        groupBox_9->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_10 = new QGridLayout(groupBox_9);
        gridLayout_10->setSpacing(6);
        gridLayout_10->setContentsMargins(11, 11, 11, 11);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        cb_config_observation_length = new QCheckBox(groupBox_9);
        cb_config_observation_length->setObjectName(QStringLiteral("cb_config_observation_length"));
        sizePolicy5.setHeightForWidth(cb_config_observation_length->sizePolicy().hasHeightForWidth());
        cb_config_observation_length->setSizePolicy(sizePolicy5);
        cb_config_observation_length->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_12->addWidget(cb_config_observation_length);

        dSB_observation_length = new QSpinBox(groupBox_9);
        dSB_observation_length->setObjectName(QStringLiteral("dSB_observation_length"));
        sizePolicy6.setHeightForWidth(dSB_observation_length->sizePolicy().hasHeightForWidth());
        dSB_observation_length->setSizePolicy(sizePolicy6);
        dSB_observation_length->setMinimumSize(QSize(100, 0));
        dSB_observation_length->setMaximum(200);

        horizontalLayout_12->addWidget(dSB_observation_length);


        gridLayout_10->addLayout(horizontalLayout_12, 0, 0, 1, 1);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        cb_config_probablity_max = new QCheckBox(groupBox_9);
        cb_config_probablity_max->setObjectName(QStringLiteral("cb_config_probablity_max"));
        sizePolicy5.setHeightForWidth(cb_config_probablity_max->sizePolicy().hasHeightForWidth());
        cb_config_probablity_max->setSizePolicy(sizePolicy5);
        cb_config_probablity_max->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_13->addWidget(cb_config_probablity_max);

        dSB_probablity_max = new QSpinBox(groupBox_9);
        dSB_probablity_max->setObjectName(QStringLiteral("dSB_probablity_max"));
        sizePolicy6.setHeightForWidth(dSB_probablity_max->sizePolicy().hasHeightForWidth());
        dSB_probablity_max->setSizePolicy(sizePolicy6);
        dSB_probablity_max->setMinimumSize(QSize(100, 0));
        dSB_probablity_max->setMaximum(100);

        horizontalLayout_13->addWidget(dSB_probablity_max);


        gridLayout_10->addLayout(horizontalLayout_13, 1, 0, 1, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        cb_config_probablity_min = new QCheckBox(groupBox_9);
        cb_config_probablity_min->setObjectName(QStringLiteral("cb_config_probablity_min"));
        sizePolicy5.setHeightForWidth(cb_config_probablity_min->sizePolicy().hasHeightForWidth());
        cb_config_probablity_min->setSizePolicy(sizePolicy5);
        cb_config_probablity_min->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_14->addWidget(cb_config_probablity_min);

        dSB_probablity_min = new QSpinBox(groupBox_9);
        dSB_probablity_min->setObjectName(QStringLiteral("dSB_probablity_min"));
        sizePolicy6.setHeightForWidth(dSB_probablity_min->sizePolicy().hasHeightForWidth());
        dSB_probablity_min->setSizePolicy(sizePolicy6);
        dSB_probablity_min->setMinimumSize(QSize(100, 0));
        dSB_probablity_min->setMaximum(100);

        horizontalLayout_14->addWidget(dSB_probablity_min);


        gridLayout_10->addLayout(horizontalLayout_14, 2, 0, 1, 1);

        layoutWidget_4 = new QWidget(page_5);
        layoutWidget_4->setObjectName(QStringLiteral("layoutWidget_4"));
        layoutWidget_4->setGeometry(QRect(0, 580, 381, 36));
        horizontalLayout_26 = new QHBoxLayout(layoutWidget_4);
        horizontalLayout_26->setSpacing(6);
        horizontalLayout_26->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        horizontalLayout_26->setContentsMargins(0, 0, 0, 0);
        btn_back_analisis = new QPushButton(layoutWidget_4);
        btn_back_analisis->setObjectName(QStringLiteral("btn_back_analisis"));
        btn_back_analisis->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_26->addWidget(btn_back_analisis);

        btn_save_analisis = new QPushButton(layoutWidget_4);
        btn_save_analisis->setObjectName(QStringLiteral("btn_save_analisis"));
        btn_save_analisis->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_26->addWidget(btn_save_analisis);

        groupBox_12 = new QGroupBox(page_5);
        groupBox_12->setObjectName(QStringLiteral("groupBox_12"));
        groupBox_12->setGeometry(QRect(10, 280, 371, 74));
        sizePolicy5.setHeightForWidth(groupBox_12->sizePolicy().hasHeightForWidth());
        groupBox_12->setSizePolicy(sizePolicy5);
        groupBox_12->setMinimumSize(QSize(40, 0));
        groupBox_12->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_9 = new QGridLayout(groupBox_12);
        gridLayout_9->setSpacing(6);
        gridLayout_9->setContentsMargins(11, 11, 11, 11);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        cb_config_criteria = new QCheckBox(groupBox_12);
        cb_config_criteria->setObjectName(QStringLiteral("cb_config_criteria"));
        sizePolicy5.setHeightForWidth(cb_config_criteria->sizePolicy().hasHeightForWidth());
        cb_config_criteria->setSizePolicy(sizePolicy5);
        cb_config_criteria->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        horizontalLayout_6->addWidget(cb_config_criteria);

        combo_alarm_condition = new QComboBox(groupBox_12);
        combo_alarm_condition->setObjectName(QStringLiteral("combo_alarm_condition"));
        combo_alarm_condition->setEditable(true);

        horizontalLayout_6->addWidget(combo_alarm_condition);


        gridLayout_9->addLayout(horizontalLayout_6, 0, 0, 1, 1);

        stacked_widget_views->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QStringLiteral("page_6"));
        label_8 = new QLabel(page_6);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 20, 381, 131));
        label_8->setTextFormat(Qt::RichText);
        label_8->setScaledContents(true);
        label_8->setWordWrap(true);
        groupBox_11 = new QGroupBox(page_6);
        groupBox_11->setObjectName(QStringLiteral("groupBox_11"));
        groupBox_11->setGeometry(QRect(10, 30, 376, 191));
        sizePolicy5.setHeightForWidth(groupBox_11->sizePolicy().hasHeightForWidth());
        groupBox_11->setSizePolicy(sizePolicy5);
        groupBox_11->setMinimumSize(QSize(361, 100));
        groupBox_11->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_12 = new QGridLayout(groupBox_11);
        gridLayout_12->setSpacing(6);
        gridLayout_12->setContentsMargins(11, 11, 11, 11);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setSpacing(6);
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        cb_config_sv_mfield = new QCheckBox(groupBox_11);
        cb_config_sv_mfield->setObjectName(QStringLiteral("cb_config_sv_mfield"));
        sizePolicy5.setHeightForWidth(cb_config_sv_mfield->sizePolicy().hasHeightForWidth());
        cb_config_sv_mfield->setSizePolicy(sizePolicy5);
        cb_config_sv_mfield->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
""));

        horizontalLayout_18->addWidget(cb_config_sv_mfield);


        gridLayout_12->addLayout(horizontalLayout_18, 1, 0, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setSpacing(6);
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        cb_config_threshold_adjust = new QCheckBox(groupBox_11);
        cb_config_threshold_adjust->setObjectName(QStringLiteral("cb_config_threshold_adjust"));
        sizePolicy5.setHeightForWidth(cb_config_threshold_adjust->sizePolicy().hasHeightForWidth());
        cb_config_threshold_adjust->setSizePolicy(sizePolicy5);
        cb_config_threshold_adjust->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        horizontalLayout_17->addWidget(cb_config_threshold_adjust);


        gridLayout_12->addLayout(horizontalLayout_17, 0, 0, 1, 1);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setSpacing(6);
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        cb_config_sv_uv = new QCheckBox(groupBox_11);
        cb_config_sv_uv->setObjectName(QStringLiteral("cb_config_sv_uv"));
        sizePolicy5.setHeightForWidth(cb_config_sv_uv->sizePolicy().hasHeightForWidth());
        cb_config_sv_uv->setSizePolicy(sizePolicy5);
        cb_config_sv_uv->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        horizontalLayout_19->addWidget(cb_config_sv_uv);


        gridLayout_12->addLayout(horizontalLayout_19, 3, 0, 1, 1);

        cb_config_sv_ir = new QCheckBox(groupBox_11);
        cb_config_sv_ir->setObjectName(QStringLiteral("cb_config_sv_ir"));
        sizePolicy5.setHeightForWidth(cb_config_sv_ir->sizePolicy().hasHeightForWidth());
        cb_config_sv_ir->setSizePolicy(sizePolicy5);
        cb_config_sv_ir->setStyleSheet(QLatin1String(" QCheckBox{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
""));

        gridLayout_12->addWidget(cb_config_sv_ir, 2, 0, 1, 1);

        cb_config_sv_ir->raise();
        layoutWidget_5 = new QWidget(page_6);
        layoutWidget_5->setObjectName(QStringLiteral("layoutWidget_5"));
        layoutWidget_5->setGeometry(QRect(0, 580, 381, 36));
        horizontalLayout_27 = new QHBoxLayout(layoutWidget_5);
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        horizontalLayout_27->setContentsMargins(0, 0, 0, 0);
        btn_cancel_options = new QPushButton(layoutWidget_5);
        btn_cancel_options->setObjectName(QStringLiteral("btn_cancel_options"));
        btn_cancel_options->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_27->addWidget(btn_cancel_options);

        btn_save_options = new QPushButton(layoutWidget_5);
        btn_save_options->setObjectName(QStringLiteral("btn_save_options"));
        btn_save_options->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_27->addWidget(btn_save_options);

        stacked_widget_views->addWidget(page_6);
        page_7 = new QWidget();
        page_7->setObjectName(QStringLiteral("page_7"));
        btn_set_emu_data = new QPushButton(page_7);
        btn_set_emu_data->setObjectName(QStringLiteral("btn_set_emu_data"));
        btn_set_emu_data->setGeometry(QRect(20, 30, 371, 61));
        layoutWidget_2 = new QWidget(page_7);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 580, 381, 36));
        horizontalLayout_24 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        horizontalLayout_24->setContentsMargins(0, 0, 0, 0);
        btn_back_emu_data = new QPushButton(layoutWidget_2);
        btn_back_emu_data->setObjectName(QStringLiteral("btn_back_emu_data"));
        btn_back_emu_data->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_24->addWidget(btn_back_emu_data);

        btn_nxt_emu_data = new QPushButton(layoutWidget_2);
        btn_nxt_emu_data->setObjectName(QStringLiteral("btn_nxt_emu_data"));
        btn_nxt_emu_data->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 4px;\n"
"     border-top-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"  QPushButton:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
"  QPushButton:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QPushButton:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_24->addWidget(btn_nxt_emu_data);

        stacked_widget_views->addWidget(page_7);

        gridLayout_2->addWidget(stacked_widget_views, 0, 1, 2, 1);

        dockWidget_sen_cmd->setWidget(dockWidgetContents_6);
        gBox_range = new QGroupBox(centralWidget);
        gBox_range->setObjectName(QStringLiteral("gBox_range"));
        gBox_range->setGeometry(QRect(780, 100, 324, 81));
        sizePolicy4.setHeightForWidth(gBox_range->sizePolicy().hasHeightForWidth());
        gBox_range->setSizePolicy(sizePolicy4);
        gBox_range->setMinimumSize(QSize(40, 0));
        gBox_range->setMaximumSize(QSize(16777215, 100));
        verticalLayout_2 = new QVBoxLayout(gBox_range);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(7, 7, 7, 7);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(gBox_range);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        spinBox_start = new QSpinBox(gBox_range);
        spinBox_start->setObjectName(QStringLiteral("spinBox_start"));
        spinBox_start->setMaximum(254);
        spinBox_start->setValue(1);

        verticalLayout_3->addWidget(spinBox_start);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_4 = new QLabel(gBox_range);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_4->addWidget(label_4);

        spinBox_end = new QSpinBox(gBox_range);
        spinBox_end->setObjectName(QStringLiteral("spinBox_end"));
        spinBox_end->setMaximum(255);
        spinBox_end->setValue(255);

        verticalLayout_4->addWidget(spinBox_end);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_3);

        dockWidget = new QDockWidget(centralWidget);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setGeometry(QRect(210, 700, 356, 42));
        dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        terminal = new Console(dockWidgetContents);
        terminal->setObjectName(QStringLiteral("terminal"));
        terminal->setGeometry(QRect(10, 10, 71, 351));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 419, 26));
        menuConnection = new QMenu(menuBar);
        menuConnection->setObjectName(QStringLiteral("menuConnection"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QStringLiteral("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        toolBar->setCursor(QCursor(Qt::CrossCursor));
        toolBar->setMovable(true);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dock_central = new QDockWidget(MainWindow);
        dock_central->setObjectName(QStringLiteral("dock_central"));
        sizePolicy2.setHeightForWidth(dock_central->sizePolicy().hasHeightForWidth());
        dock_central->setSizePolicy(sizePolicy2);
        dock_central->setMinimumSize(QSize(91, 374));
        dock_central->setMaximumSize(QSize(524287, 524287));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(135, 135, 135, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        dock_central->setPalette(palette);
        dock_central->setAcceptDrops(false);
        dock_central->setStyleSheet(QStringLiteral(""));
        dock_central->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dock_central->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_27 = new QWidget();
        dockWidgetContents_27->setObjectName(QStringLiteral("dockWidgetContents_27"));
        gridLayout = new QGridLayout(dockWidgetContents_27);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget_20 = new QWidget(dockWidgetContents_27);
        widget_20->setObjectName(QStringLiteral("widget_20"));
        widget_20->setStyleSheet(QLatin1String(" QWidget{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     border-bottom-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
"}\n"
"\n"
"\n"
" QTabWidget::pane { /* The tab widget frame */\n"
"  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"   border-bottom-left-radius: 10px; \n"
"    border-bottom-right-radius: 10px;\n"
" border-top-right-radius: 10px;\n"
" }\n"
"\n"
" QTabWidget::tab-bar {\n"
"     left: 0px; /* m"
                        "ove to the right by 5px */\n"
" }\n"
"\n"
" QTabBar::tab {\n"
"\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 10px;\n"
"     border-top-right-radius: 4px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"\n"
" }\n"
"\n"
" QWidget:hover {\n"
"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #7E7E7E, stop: 0.4 #DDDDDD,\n"
"                                 stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\n"
" }\n"
"\n"
" QWidget:selected {\n"
"     border-color: #9B9B9B;\n"
"   /*  border-bottom-color: #C2C7CB;*/ /* same as pane color */\n"
" }\n"
"\n"
" QWidget:!selected {\n"
"     margin-top: 2px; /* make non-selected tabs look smaller */\n"
""
                        " }"));
        gridLayout_153 = new QGridLayout(widget_20);
        gridLayout_153->setSpacing(0);
        gridLayout_153->setContentsMargins(11, 11, 11, 11);
        gridLayout_153->setObjectName(QStringLiteral("gridLayout_153"));
        gridLayout_153->setContentsMargins(0, 0, 0, 0);
        centralLogo = new QLabel(widget_20);
        centralLogo->setObjectName(QStringLiteral("centralLogo"));
        sizePolicy1.setHeightForWidth(centralLogo->sizePolicy().hasHeightForWidth());
        centralLogo->setSizePolicy(sizePolicy1);
        centralLogo->setPixmap(QPixmap(QString::fromUtf8(":/logo/logo")));
        centralLogo->setScaledContents(true);

        gridLayout_153->addWidget(centralLogo, 0, 0, 1, 1);


        gridLayout->addWidget(widget_20, 0, 0, 1, 1);

        dock_central->setWidget(dockWidgetContents_27);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dock_central);

        menuBar->addAction(menuConnection->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuConnection->addAction(actionOpen);
        menuConnection->addAction(actionConnect);
        menuConnection->addAction(actionDisconnect);
        menuConnection->addAction(actionQuit);
        menuTools->addAction(actionConfigure);
        menuTools->addAction(actionClear);
        menuTools->addAction(actionCommands);
        menuHelp->addAction(actionAbout);
        toolBar->addAction(actionConnect);
        toolBar->addAction(actionDisconnect);
        toolBar->addSeparator();
        toolBar->addAction(actionConfigure);
        toolBar->addAction(actionClear);
        toolBar->addSeparator();
        toolBar->addAction(actionQuit);

        retranslateUi(MainWindow);

        stacked_widget_views->setCurrentIndex(5);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "DR_DOWNLOADER", 0));
        actionConnect->setText(QApplication::translate("MainWindow", "Connect", 0));
        actionDisconnect->setText(QApplication::translate("MainWindow", "Disconnect", 0));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0));
        actionClear->setText(QApplication::translate("MainWindow", "Clear", 0));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", 0));
        actionCommands->setText(QApplication::translate("MainWindow", "Single sensor commands ", 0));
        dockWidget_sen_cmd->setWindowTitle(QApplication::translate("MainWindow", "Commands", 0));
        QTreeWidgetItem *___qtreewidgetitem = cmd_list->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("MainWindow", "COMMAND OPTIONS  ", 0));

        const bool __sortingEnabled = cmd_list->isSortingEnabled();
        cmd_list->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = cmd_list->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("MainWindow", "CONFIG", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("MainWindow", "DEVICE ADDRESS", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("MainWindow", "INSTALLATION DITAILS", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = ___qtreewidgetitem1->child(2);
        ___qtreewidgetitem4->setText(0, QApplication::translate("MainWindow", "SIGNAL THRESHOLDS", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem1->child(3);
        ___qtreewidgetitem5->setText(0, QApplication::translate("MainWindow", "EVENT ANALYSIS OPTIONS", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem1->child(4);
        ___qtreewidgetitem6->setText(0, QApplication::translate("MainWindow", "RUN TIME OPTIONS", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem1->child(5);
        ___qtreewidgetitem7->setText(0, QApplication::translate("MainWindow", "FACTORY DEFAULT", 0));
        QTreeWidgetItem *___qtreewidgetitem8 = ___qtreewidgetitem1->child(6);
        ___qtreewidgetitem8->setText(0, QApplication::translate("MainWindow", "LOAD HIF REFERENCE DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem9 = ___qtreewidgetitem1->child(7);
        ___qtreewidgetitem9->setText(0, QApplication::translate("MainWindow", "COMMIT", 0));
        QTreeWidgetItem *___qtreewidgetitem10 = cmd_list->topLevelItem(1);
        ___qtreewidgetitem10->setText(0, QApplication::translate("MainWindow", "EMULATION SETTING", 0));
        QTreeWidgetItem *___qtreewidgetitem11 = ___qtreewidgetitem10->child(0);
        ___qtreewidgetitem11->setText(0, QApplication::translate("MainWindow", "LOAD IR DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem12 = ___qtreewidgetitem10->child(1);
        ___qtreewidgetitem12->setText(0, QApplication::translate("MainWindow", "LOAD UV DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem13 = ___qtreewidgetitem10->child(2);
        ___qtreewidgetitem13->setText(0, QApplication::translate("MainWindow", "LOAD MAGNETIC FIELD X DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem14 = ___qtreewidgetitem10->child(3);
        ___qtreewidgetitem14->setText(0, QApplication::translate("MainWindow", "LOAD MAGNETIC FIELD Y DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem15 = ___qtreewidgetitem10->child(4);
        ___qtreewidgetitem15->setText(0, QApplication::translate("MainWindow", "EMU MODE SET", 0));
        QTreeWidgetItem *___qtreewidgetitem16 = ___qtreewidgetitem10->child(5);
        ___qtreewidgetitem16->setText(0, QApplication::translate("MainWindow", "EMU MODE RESET", 0));
        QTreeWidgetItem *___qtreewidgetitem17 = ___qtreewidgetitem10->child(6);
        ___qtreewidgetitem17->setText(0, QApplication::translate("MainWindow", "CLEAR EMU DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem18 = cmd_list->topLevelItem(2);
        ___qtreewidgetitem18->setText(0, QApplication::translate("MainWindow", "GET DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem19 = ___qtreewidgetitem18->child(0);
        ___qtreewidgetitem19->setText(0, QApplication::translate("MainWindow", "DEVICE DATA", 0));
        QTreeWidgetItem *___qtreewidgetitem20 = ___qtreewidgetitem18->child(1);
        ___qtreewidgetitem20->setText(0, QApplication::translate("MainWindow", "IR DISTURBANCE RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem21 = ___qtreewidgetitem18->child(2);
        ___qtreewidgetitem21->setText(0, QApplication::translate("MainWindow", "UV DISTURBANCE RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem22 = ___qtreewidgetitem18->child(3);
        ___qtreewidgetitem22->setText(0, QApplication::translate("MainWindow", "MFIELD X  DISTURBANCE RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem23 = ___qtreewidgetitem18->child(4);
        ___qtreewidgetitem23->setText(0, QApplication::translate("MainWindow", "MFIELD Y  DISTURBANCE RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem24 = ___qtreewidgetitem18->child(5);
        ___qtreewidgetitem24->setText(0, QApplication::translate("MainWindow", "HIF_DIAGNOSIS_RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem25 = ___qtreewidgetitem18->child(6);
        ___qtreewidgetitem25->setText(0, QApplication::translate("MainWindow", "IR CURRENT RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem26 = ___qtreewidgetitem18->child(7);
        ___qtreewidgetitem26->setText(0, QApplication::translate("MainWindow", "UV CURRENT RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem27 = ___qtreewidgetitem18->child(8);
        ___qtreewidgetitem27->setText(0, QApplication::translate("MainWindow", "MFIELD X  CURRENT RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem28 = ___qtreewidgetitem18->child(9);
        ___qtreewidgetitem28->setText(0, QApplication::translate("MainWindow", "MFIELD Y  CURRENT RECORD", 0));
        QTreeWidgetItem *___qtreewidgetitem29 = cmd_list->topLevelItem(3);
        ___qtreewidgetitem29->setText(0, QApplication::translate("MainWindow", "MISCELLANEOUS", 0));
        QTreeWidgetItem *___qtreewidgetitem30 = ___qtreewidgetitem29->child(0);
        ___qtreewidgetitem30->setText(0, QApplication::translate("MainWindow", "DEVICE RESET", 0));
        QTreeWidgetItem *___qtreewidgetitem31 = ___qtreewidgetitem29->child(1);
        ___qtreewidgetitem31->setText(0, QApplication::translate("MainWindow", "CLEAR EEPROM", 0));
        QTreeWidgetItem *___qtreewidgetitem32 = ___qtreewidgetitem29->child(2);
        ___qtreewidgetitem32->setText(0, QApplication::translate("MainWindow", "SUPERVISION STATUS", 0));
        cmd_list->setSortingEnabled(__sortingEnabled);

        gBox_cmd->setTitle(QString());
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Target Sensor", 0));
        label->setText(QApplication::translate("MainWindow", "Channel", 0));
        label_2->setText(QApplication::translate("MainWindow", "Address", 0));
        combo_Ch->clear();
        combo_Ch->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "-", 0)
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "2", 0)
         << QApplication::translate("MainWindow", "3", 0)
         << QApplication::translate("MainWindow", "4", 0)
        );
        combo_Sen->clear();
        combo_Sen->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "-", 0)
         << QApplication::translate("MainWindow", "Multiple Address", 0)
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "2", 0)
         << QApplication::translate("MainWindow", "3", 0)
        );
        btn_cancel->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_execute_cmd->setText(QApplication::translate("MainWindow", "Execute Command", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "LIGHT SIGNAL THRESHOLDS ", 0));
        cb_config_ir_max->setText(QApplication::translate("MainWindow", "IR MAX ", 0));
        cb_config_uv_max->setText(QApplication::translate("MainWindow", "UV MAX", 0));
        label_5->setText(QApplication::translate("MainWindow", "<html><head/><body><p>The values set the target sensor light signal threshold levels.  </p><p align=\"justify\">Hence the action affects the device behavior make sure proper values are set and clear the corresponding checkbox to leave previous value unaltered, use GET DEVICE DATA command to obtain the current setting of target device. </p><p><br/></p></body></html>", 0));
        btn_back_thresholds->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_save_thresholds->setText(QApplication::translate("MainWindow", "Next >>", 0));
        label_6->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"justify\">The values provide the target sensor information\342\200\231s about the installation environment which is used to calculate threshold values. </p><p align=\"justify\">Hence the action affects the device behavior make sure proper values are set and clear the corresponding checkbox to leave previous value unaltered, use GET DEVICE DATA command to obtain the current setting of target device. </p><p><br/></p></body></html>", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "SENSOR INSTALLATION GEOMETRIC DETAILS ", 0));
        cb_config_min_distance->setText(QApplication::translate("MainWindow", "MIN CONDUCTOR DISTANCE (mm)", 0));
        groupBox_7->setTitle(QApplication::translate("MainWindow", "POWER LINE CURRENT", 0));
        cb_config_rated_current->setText(QApplication::translate("MainWindow", "RATED CURRENT (A)", 0));
        cb_config_sh_ckt_current->setText(QApplication::translate("MainWindow", "MAX SHORT CIRCUIT CURRENT (A)", 0));
        cb_config_freq->setText(QApplication::translate("MainWindow", "FREQUENCY (Hz)", 0));
        btn_cancel_sys_info->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_save_sys_info->setText(QApplication::translate("MainWindow", "Next >>", 0));
        label_7->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"justify\">Hence the action affects the device behavior make sure proper values are set and clear the corresponding checkbox to leave previous value unaltered, use GET DEVICE DATA command to obtain the current setting of target device. </p><p><br/></p></body></html>", 0));
        groupBox_8->setTitle(QApplication::translate("MainWindow", "ACTIVE MODULES", 0));
        cb_config_active_modules_ir->setText(QApplication::translate("MainWindow", "IR                ", 0));
        cb_config_active_modules_uv->setText(QApplication::translate("MainWindow", "UV            ", 0));
        cb_config_active_modules_mf->setText(QApplication::translate("MainWindow", "MAGNETIC FIELD", 0));
        groupBox_9->setTitle(QApplication::translate("MainWindow", "EVENT PROBABLITY ANALISIS", 0));
        cb_config_observation_length->setText(QApplication::translate("MainWindow", "OBSERVATION LENGTH", 0));
        cb_config_probablity_max->setText(QApplication::translate("MainWindow", "PROBABLITY MAX", 0));
        cb_config_probablity_min->setText(QApplication::translate("MainWindow", "PROBABLITY MIN", 0));
        btn_back_analisis->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_save_analisis->setText(QApplication::translate("MainWindow", "Next >>", 0));
        groupBox_12->setTitle(QApplication::translate("MainWindow", "ALARM CONDITION ", 0));
        cb_config_criteria->setText(QApplication::translate("MainWindow", "ALARM CRITERIA", 0));
        combo_alarm_condition->clear();
        combo_alarm_condition->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "DEFAULT", 0)
        );
        label_8->setText(QApplication::translate("MainWindow", "<html><head/><body><p><br/></p></body></html>", 0));
        groupBox_11->setTitle(QApplication::translate("MainWindow", "OPTIONS", 0));
        cb_config_sv_mfield->setText(QApplication::translate("MainWindow", "ENABLE MAGNETIC FIELD ELEMENT SUPERVISION", 0));
        cb_config_threshold_adjust->setText(QApplication::translate("MainWindow", "ENABLE SELF THRESHOLD ADJUST ", 0));
        cb_config_sv_uv->setText(QApplication::translate("MainWindow", "ENABLE UV ELEMENT SUPERVISION", 0));
        cb_config_sv_ir->setText(QApplication::translate("MainWindow", "ENABLE IR ELEMENT SUPERVISION", 0));
        btn_cancel_options->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_save_options->setText(QApplication::translate("MainWindow", "Next >>", 0));
        btn_set_emu_data->setText(QApplication::translate("MainWindow", "Add Data", 0));
        btn_back_emu_data->setText(QApplication::translate("MainWindow", "<< Cancel", 0));
        btn_nxt_emu_data->setText(QApplication::translate("MainWindow", "Next >>", 0));
        gBox_range->setTitle(QApplication::translate("MainWindow", "Select Range", 0));
        label_3->setText(QApplication::translate("MainWindow", "Start", 0));
        label_4->setText(QApplication::translate("MainWindow", "End", 0));
        menuConnection->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuTools->setTitle(QApplication::translate("MainWindow", "Tools", 0));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0));
        dock_central->setWindowTitle(QString());
        centralLogo->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
