/********************************************************************************
** Form generated from reading UI file 'recordpreview.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDPREVIEW_H
#define UI_RECORDPREVIEW_H

#include <../qcustomplot.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_recordPreview
{
public:
    QAction *actionCreateCSV;
    QAction *actionWinDataPlot;
    QAction *actionData_Plot_pdf;
    QAction *actionEvent_Plot_pdf;
    QAction *actionData_plot_img;
    QAction *actionEvent_Plot_img;
    QAction *actionWinEventPlot;
    QWidget *centralwidget;
    QDockWidget *dockWidgetPlot_event_history;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_4;
    QCustomPlot *curve_dfdt;
    QMenuBar *menubar;
    QMenu *menuSave;
    QMenu *menuSave_As;
    QMenu *menuPDF;
    QMenu *menuImage;
    QMenu *menuWindows;
    QStatusBar *statusbar;
    QDockWidget *dockWidgetPlot;
    QWidget *dockWidgetContents_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QCustomPlot *curve_data;
    QToolBar *toolBar;

    void setupUi(QMainWindow *recordPreview)
    {
        if (recordPreview->objectName().isEmpty())
            recordPreview->setObjectName(QStringLiteral("recordPreview"));
        recordPreview->setWindowModality(Qt::NonModal);
        recordPreview->resize(800, 600);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/logo"), QSize(), QIcon::Normal, QIcon::Off);
        recordPreview->setWindowIcon(icon);
        actionCreateCSV = new QAction(recordPreview);
        actionCreateCSV->setObjectName(QStringLiteral("actionCreateCSV"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/logo/icons/csv_file_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCreateCSV->setIcon(icon1);
        actionCreateCSV->setIconVisibleInMenu(true);
        actionWinDataPlot = new QAction(recordPreview);
        actionWinDataPlot->setObjectName(QStringLiteral("actionWinDataPlot"));
        actionWinDataPlot->setCheckable(true);
        actionData_Plot_pdf = new QAction(recordPreview);
        actionData_Plot_pdf->setObjectName(QStringLiteral("actionData_Plot_pdf"));
        actionEvent_Plot_pdf = new QAction(recordPreview);
        actionEvent_Plot_pdf->setObjectName(QStringLiteral("actionEvent_Plot_pdf"));
        actionData_plot_img = new QAction(recordPreview);
        actionData_plot_img->setObjectName(QStringLiteral("actionData_plot_img"));
        actionEvent_Plot_img = new QAction(recordPreview);
        actionEvent_Plot_img->setObjectName(QStringLiteral("actionEvent_Plot_img"));
        actionWinEventPlot = new QAction(recordPreview);
        actionWinEventPlot->setObjectName(QStringLiteral("actionWinEventPlot"));
        centralwidget = new QWidget(recordPreview);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        dockWidgetPlot_event_history = new QDockWidget(centralwidget);
        dockWidgetPlot_event_history->setObjectName(QStringLiteral("dockWidgetPlot_event_history"));
        dockWidgetPlot_event_history->setGeometry(QRect(590, 0, 91, 509));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icon/Arcflash"), QSize(), QIcon::Normal, QIcon::Off);
        dockWidgetPlot_event_history->setWindowIcon(icon2);
        dockWidgetPlot_event_history->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        gridLayout_4 = new QGridLayout(dockWidgetContents_4);
        gridLayout_4->setSpacing(0);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        curve_dfdt = new QCustomPlot(dockWidgetContents_4);
        curve_dfdt->setObjectName(QStringLiteral("curve_dfdt"));
        curve_dfdt->setStyleSheet(QLatin1String("QWidget{\n"
"border: 1px solid   #999999;\n"
"border-radius:10px;\n"
"border-style: inset;\n"
"background-color:#7E7E7E;\n"
"}"));

        horizontalLayout_4->addWidget(curve_dfdt);


        gridLayout_4->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        dockWidgetPlot_event_history->setWidget(dockWidgetContents_4);
        recordPreview->setCentralWidget(centralwidget);
        menubar = new QMenuBar(recordPreview);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menuSave = new QMenu(menubar);
        menuSave->setObjectName(QStringLiteral("menuSave"));
        menuSave_As = new QMenu(menuSave);
        menuSave_As->setObjectName(QStringLiteral("menuSave_As"));
        menuPDF = new QMenu(menuSave_As);
        menuPDF->setObjectName(QStringLiteral("menuPDF"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/icons/PDF-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuPDF->setIcon(icon3);
        menuImage = new QMenu(menuSave_As);
        menuImage->setObjectName(QStringLiteral("menuImage"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/icons/image_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        menuImage->setIcon(icon4);
        menuWindows = new QMenu(menubar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        recordPreview->setMenuBar(menubar);
        statusbar = new QStatusBar(recordPreview);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        recordPreview->setStatusBar(statusbar);
        dockWidgetPlot = new QDockWidget(recordPreview);
        dockWidgetPlot->setObjectName(QStringLiteral("dockWidgetPlot"));
        dockWidgetPlot->setWindowIcon(icon2);
        dockWidgetPlot->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        gridLayout_3 = new QGridLayout(dockWidgetContents_3);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        curve_data = new QCustomPlot(dockWidgetContents_3);
        curve_data->setObjectName(QStringLiteral("curve_data"));
        curve_data->setStyleSheet(QLatin1String("QWidget{\n"
"border: 1px solid   #999999;\n"
"border-radius:10px;\n"
"border-style: inset;\n"
"background-color:#7E7E7E;\n"
"}"));

        horizontalLayout_3->addWidget(curve_data);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        dockWidgetPlot->setWidget(dockWidgetContents_3);
        recordPreview->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidgetPlot);
        toolBar = new QToolBar(recordPreview);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        recordPreview->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuSave->menuAction());
        menubar->addAction(menuWindows->menuAction());
        menuSave->addAction(menuSave_As->menuAction());
        menuSave_As->addAction(actionCreateCSV);
        menuSave_As->addAction(menuPDF->menuAction());
        menuSave_As->addAction(menuImage->menuAction());
        menuPDF->addAction(actionData_Plot_pdf);
        menuPDF->addAction(actionEvent_Plot_pdf);
        menuImage->addAction(actionData_plot_img);
        menuImage->addAction(actionEvent_Plot_img);
        menuWindows->addAction(actionWinDataPlot);
        menuWindows->addAction(actionWinEventPlot);
        toolBar->addAction(actionCreateCSV);

        retranslateUi(recordPreview);

        QMetaObject::connectSlotsByName(recordPreview);
    } // setupUi

    void retranslateUi(QMainWindow *recordPreview)
    {
        recordPreview->setWindowTitle(QApplication::translate("recordPreview", "MainWindow", 0));
        actionCreateCSV->setText(QApplication::translate("recordPreview", "CSV", 0));
        actionCreateCSV->setIconText(QApplication::translate("recordPreview", "Create CSV", 0));
#ifndef QT_NO_TOOLTIP
        actionCreateCSV->setToolTip(QApplication::translate("recordPreview", "Creates CSV using both light and pressure data", 0));
#endif // QT_NO_TOOLTIP
        actionWinDataPlot->setText(QApplication::translate("recordPreview", "Data plot", 0));
        actionData_Plot_pdf->setText(QApplication::translate("recordPreview", "Data Plot", 0));
        actionEvent_Plot_pdf->setText(QApplication::translate("recordPreview", "Event Plot", 0));
        actionData_plot_img->setText(QApplication::translate("recordPreview", "Data plot", 0));
        actionEvent_Plot_img->setText(QApplication::translate("recordPreview", "Event plot", 0));
        actionWinEventPlot->setText(QApplication::translate("recordPreview", "Event plot", 0));
        dockWidgetPlot_event_history->setWindowTitle(QApplication::translate("recordPreview", "Plot:change per time ", 0));
        menuSave->setTitle(QApplication::translate("recordPreview", "File", 0));
        menuSave_As->setTitle(QApplication::translate("recordPreview", "Save As", 0));
        menuPDF->setTitle(QApplication::translate("recordPreview", "PDF", 0));
        menuImage->setTitle(QApplication::translate("recordPreview", "Image", 0));
        menuWindows->setTitle(QApplication::translate("recordPreview", "Windows", 0));
        dockWidgetPlot->setWindowTitle(QApplication::translate("recordPreview", "Plot:Data", 0));
        toolBar->setWindowTitle(QApplication::translate("recordPreview", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class recordPreview: public Ui_recordPreview {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDPREVIEW_H
