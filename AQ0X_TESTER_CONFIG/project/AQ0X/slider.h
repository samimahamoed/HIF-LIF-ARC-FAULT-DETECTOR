/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include "typedef.h"
#include "emu_data_editor.h"

#define BG_STYLE_NORMAL           "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8f8f8f, stop:1 #8f8f8f);"
#define BG_STYLE__SELECTION_MODE  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3333FF, stop:1 #3333FF);"
#define BG_STYLE__HAS_FOCUS       "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #75FF47, stop:1 #75FF47);"
class Slider : public QSlider
{
    Q_OBJECT

signals:
   void action_set_value(double val,Uint8 type);
   void update_curve_signal();
   void selection(bool clear);

public:
    explicit Slider(Uint16 id, QWidget *parent = 0);




    union
    {
        Uint8 all;
        struct
        {


            Uint8 point_selected:1;
            Uint8 point_selection_mode:1;
            Uint8 point_deselection_mode:1;
            Uint8 point_focuse_en:1;

        }bits;


    }flags;





public slots:
       void  edit_point_selection   (bool status);
       void  edit_point_deselection (bool status);
       void  adjust_value           (Int16 val);
       void  set_value              (double val, Uint8 type);
       void  update_value           ();
       void  set_status             (Uint8 select);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void moved(int value);
private:
    Uint16 id;
    QString current_style;

    emu_data_editor * parent;

};

#endif // CONSOLE_H
