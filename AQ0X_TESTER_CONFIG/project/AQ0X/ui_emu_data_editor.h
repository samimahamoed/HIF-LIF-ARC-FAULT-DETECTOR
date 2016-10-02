/********************************************************************************
** Form generated from reading UI file 'emu_data_editor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMU_DATA_EDITOR_H
#define UI_EMU_DATA_EDITOR_H

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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_emu_data_editor
{
public:
    QAction *actionMakePDF;
    QAction *actionMakeImg;
    QAction *actionWinPlot;
    QAction *actionWinEditor;
    QAction *actionOpen;
    QAction *actionCSV;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuSave;
    QMenu *menuSaveAs;
    QMenu *menuWindows;
    QStatusBar *statusbar;
    QDockWidget *dockWidgetCurve_plot;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QCustomPlot *plot_curve;
    QToolBar *toolBar;
    QDockWidget *dockWidgetCurve_edit;
    QWidget *dockWidgetContents_4;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QScrollArea *sArea__editor;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_edit_points;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btn_save;
    QDockWidget *dockWidgetCurve_plot_fft;
    QWidget *dockWidgetContents_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QCustomPlot *plot_curve_fft;

    void setupUi(QMainWindow *emu_data_editor)
    {
        if (emu_data_editor->objectName().isEmpty())
            emu_data_editor->setObjectName(QStringLiteral("emu_data_editor"));
        emu_data_editor->setWindowModality(Qt::NonModal);
        emu_data_editor->resize(613, 547);
        QIcon icon;
        icon.addFile(QStringLiteral(":/logo/logo"), QSize(), QIcon::Normal, QIcon::Off);
        emu_data_editor->setWindowIcon(icon);
        actionMakePDF = new QAction(emu_data_editor);
        actionMakePDF->setObjectName(QStringLiteral("actionMakePDF"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/logo/icons/PDF-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMakePDF->setIcon(icon1);
        actionMakeImg = new QAction(emu_data_editor);
        actionMakeImg->setObjectName(QStringLiteral("actionMakeImg"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/logo/icons/image_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionMakeImg->setIcon(icon2);
        actionWinPlot = new QAction(emu_data_editor);
        actionWinPlot->setObjectName(QStringLiteral("actionWinPlot"));
        actionWinPlot->setCheckable(true);
        actionWinEditor = new QAction(emu_data_editor);
        actionWinEditor->setObjectName(QStringLiteral("actionWinEditor"));
        actionWinEditor->setCheckable(true);
        actionOpen = new QAction(emu_data_editor);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/icon/icons/open_file-icon.gif"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon3);
        actionCSV = new QAction(emu_data_editor);
        actionCSV->setObjectName(QStringLiteral("actionCSV"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/icon/icons/csv_file"), QSize(), QIcon::Normal, QIcon::Off);
        actionCSV->setIcon(icon4);
        centralwidget = new QWidget(emu_data_editor);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        emu_data_editor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(emu_data_editor);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 613, 26));
        menuSave = new QMenu(menubar);
        menuSave->setObjectName(QStringLiteral("menuSave"));
        menuSaveAs = new QMenu(menuSave);
        menuSaveAs->setObjectName(QStringLiteral("menuSaveAs"));
        menuSaveAs->setIcon(icon1);
        menuWindows = new QMenu(menubar);
        menuWindows->setObjectName(QStringLiteral("menuWindows"));
        emu_data_editor->setMenuBar(menubar);
        statusbar = new QStatusBar(emu_data_editor);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        emu_data_editor->setStatusBar(statusbar);
        dockWidgetCurve_plot = new QDockWidget(emu_data_editor);
        dockWidgetCurve_plot->setObjectName(QStringLiteral("dockWidgetCurve_plot"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/icon/Arcflash"), QSize(), QIcon::Normal, QIcon::Off);
        dockWidgetCurve_plot->setWindowIcon(icon5);
        dockWidgetCurve_plot->setAllowedAreas(Qt::TopDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        gridLayout = new QGridLayout(dockWidgetContents);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        plot_curve = new QCustomPlot(dockWidgetContents);
        plot_curve->setObjectName(QStringLiteral("plot_curve"));
        plot_curve->setStyleSheet(QLatin1String("QWidget{\n"
"border: 1px solid   #999999;\n"
"border-radius:10px;\n"
"border-style: inset;\n"
"background-color:#7E7E7E;\n"
"}"));

        horizontalLayout->addWidget(plot_curve);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        dockWidgetCurve_plot->setWidget(dockWidgetContents);
        emu_data_editor->addDockWidget(static_cast<Qt::DockWidgetArea>(4), dockWidgetCurve_plot);
        toolBar = new QToolBar(emu_data_editor);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        emu_data_editor->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidgetCurve_edit = new QDockWidget(emu_data_editor);
        dockWidgetCurve_edit->setObjectName(QStringLiteral("dockWidgetCurve_edit"));
        dockWidgetCurve_edit->setMinimumSize(QSize(453, 200));
        dockWidgetCurve_edit->setWindowIcon(icon5);
        dockWidgetCurve_edit->setAllowedAreas(Qt::BottomDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        gridLayout_5 = new QGridLayout(dockWidgetContents_4);
        gridLayout_5->setSpacing(0);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sArea__editor = new QScrollArea(dockWidgetContents_4);
        sArea__editor->setObjectName(QStringLiteral("sArea__editor"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush2(QColor(255, 255, 220, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush1);
        sArea__editor->setPalette(palette);
        sArea__editor->setStyleSheet(QStringLiteral(""));
        sArea__editor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        sArea__editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        sArea__editor->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 607, 125));
        gridLayout_4 = new QGridLayout(scrollAreaWidgetContents);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        horizontalLayout_edit_points = new QHBoxLayout();
        horizontalLayout_edit_points->setSpacing(0);
        horizontalLayout_edit_points->setObjectName(QStringLiteral("horizontalLayout_edit_points"));

        gridLayout_4->addLayout(horizontalLayout_edit_points, 0, 0, 1, 1);

        sArea__editor->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(sArea__editor);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        btn_save = new QPushButton(dockWidgetContents_4);
        btn_save->setObjectName(QStringLiteral("btn_save"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(btn_save->sizePolicy().hasHeightForWidth());
        btn_save->setSizePolicy(sizePolicy);
        btn_save->setMinimumSize(QSize(42, 0));
        btn_save->setMaximumSize(QSize(200, 16777215));
        btn_save->setStyleSheet(QLatin1String(" QPushButton{\n"
"     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n"
"                                 stop: 0 #D3D3D3, stop: 0.4 #7E7E7E,\n"
"                                 stop: 0.5 #7E7E7E, stop: 1.0 #7E7E7E);\n"
"     \n"
"/*     border: 1px solid #C4C4C3;\n"
" border-top: 1px solid #C2C7CB;*/\n"
"   /*  border-bottom-color: #C2C7CB;*/\n"
"     border-top-left-radius: 0px;\n"
"     border-top-right-radius: 0px;\n"
"    border-bottom-left-radius: 10px;\n"
"     border-bottom-right-radius: 10px;\n"
"     min-width: 8ex;\n"
"     padding: 8px;\n"
"     margin-right:2px;\n"
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
" QTabWidget::t"
                        "ab-bar {\n"
"     left: 0px; /* move to the right by 5px */\n"
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
"     margin-top: 2px;"
                        " /* make non-selected tabs look smaller */\n"
" }"));

        horizontalLayout_5->addWidget(btn_save);


        verticalLayout->addLayout(horizontalLayout_5);


        horizontalLayout_4->addLayout(verticalLayout);


        gridLayout_5->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        dockWidgetCurve_edit->setWidget(dockWidgetContents_4);
        emu_data_editor->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidgetCurve_edit);
        dockWidgetCurve_plot_fft = new QDockWidget(emu_data_editor);
        dockWidgetCurve_plot_fft->setObjectName(QStringLiteral("dockWidgetCurve_plot_fft"));
        dockWidgetCurve_plot_fft->setWindowIcon(icon5);
        dockWidgetCurve_plot_fft->setAllowedAreas(Qt::TopDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        gridLayout_2 = new QGridLayout(dockWidgetContents_2);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        plot_curve_fft = new QCustomPlot(dockWidgetContents_2);
        plot_curve_fft->setObjectName(QStringLiteral("plot_curve_fft"));
        plot_curve_fft->setStyleSheet(QLatin1String("QWidget{\n"
"border: 1px solid   #999999;\n"
"border-radius:10px;\n"
"border-style: inset;\n"
"background-color:#7E7E7E;\n"
"}"));

        horizontalLayout_2->addWidget(plot_curve_fft);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        dockWidgetCurve_plot_fft->setWidget(dockWidgetContents_2);
        emu_data_editor->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetCurve_plot_fft);

        menubar->addAction(menuSave->menuAction());
        menubar->addAction(menuWindows->menuAction());
        menuSave->addAction(actionOpen);
        menuSave->addAction(menuSaveAs->menuAction());
        menuSaveAs->addAction(actionCSV);
        menuSaveAs->addAction(actionMakePDF);
        menuSaveAs->addAction(actionMakeImg);
        menuWindows->addAction(actionWinPlot);
        menuWindows->addAction(actionWinEditor);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionCSV);

        retranslateUi(emu_data_editor);

        QMetaObject::connectSlotsByName(emu_data_editor);
    } // setupUi

    void retranslateUi(QMainWindow *emu_data_editor)
    {
        emu_data_editor->setWindowTitle(QApplication::translate("emu_data_editor", "MainWindow", 0));
        actionMakePDF->setText(QApplication::translate("emu_data_editor", "PDF", 0));
        actionMakeImg->setText(QApplication::translate("emu_data_editor", "Image", 0));
        actionWinPlot->setText(QApplication::translate("emu_data_editor", "Plot", 0));
        actionWinEditor->setText(QApplication::translate("emu_data_editor", "Editor", 0));
        actionOpen->setText(QApplication::translate("emu_data_editor", "Open", 0));
        actionCSV->setText(QApplication::translate("emu_data_editor", "CSV", 0));
        menuSave->setTitle(QApplication::translate("emu_data_editor", "File", 0));
        menuSaveAs->setTitle(QApplication::translate("emu_data_editor", "SaveAs", 0));
        menuWindows->setTitle(QApplication::translate("emu_data_editor", "Windows", 0));
        dockWidgetCurve_plot->setWindowTitle(QApplication::translate("emu_data_editor", "Plot: Time domain", 0));
        toolBar->setWindowTitle(QApplication::translate("emu_data_editor", "toolBar", 0));
        dockWidgetCurve_edit->setWindowTitle(QApplication::translate("emu_data_editor", "Editor", 0));
        btn_save->setText(QApplication::translate("emu_data_editor", "Save", 0));
        dockWidgetCurve_plot_fft->setWindowTitle(QApplication::translate("emu_data_editor", "Plot:Frequency domain", 0));
    } // retranslateUi

};

namespace Ui {
    class emu_data_editor: public Ui_emu_data_editor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMU_DATA_EDITOR_H
