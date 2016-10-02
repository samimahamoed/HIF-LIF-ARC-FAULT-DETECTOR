#include "slider.h"
#include "storage_structure.h"
#include <QScrollBar>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QMenu>
#include <QLineEdit>
#include <QPushButton>
#include <QWidgetAction>
#include <QDoubleSpinBox>
#include <math.h>

Slider::Slider(Uint16 id,QWidget *parent)
    : QSlider(parent)
{

    this->id = id;
    this->parent = (emu_data_editor *)parent;


    setMaximumWidth(40/*item width*/);
    setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);

    current_style = "QSlider { width: 5px; border: none; }"
            "QSlider::groove:veritcal {"
            "border: none;"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:1 #000000);"

            "margin: 0 0 0 0;}"
            "QSlider::handle:veritcal {"
            "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
            "border: none;"
            "width: 5px;"
            "height: 5px;"
            "margin: 2px 2px 2px 2px;"
            "border-radius: 2px;}";

     setStyleSheet(current_style);

    // this->setSliderDown(true);

    flags.all = 0;

    connect(parent,SIGNAL(edit_point_selection(bool)),this,SLOT(edit_point_selection(bool)));
    connect(parent,SIGNAL(edit_point_deselection(bool)),this,SLOT(edit_point_deselection(bool)));
    connect(parent,SIGNAL(adjust_value(Int16)),this,SLOT(adjust_value(Int16)));
    connect(parent,SIGNAL(action_set_value(double,Uint8)),this,SLOT(set_value(double,Uint8)));
    connect(this,SIGNAL(action_set_value(double,Uint8)),parent,SLOT(action_set_value_triggered(double,Uint8)));
    connect(this,SIGNAL(selection(bool)),parent,SLOT(selection(bool)));
    connect(parent,SIGNAL(set_status(Uint8)),this,SLOT(set_status(Uint8)));
    connect(this,SIGNAL(sliderMoved(int)),this,SLOT(moved(int)));
    connect(parent,SIGNAL(update_value()),this,SLOT(update_value()));
    installEventFilter(this);
}

void Slider::edit_point_selection(bool status)
{
    flags.bits.point_selection_mode = status;

}

void Slider::edit_point_deselection(bool status)
{
    flags.bits.point_deselection_mode = status;

}

void Slider::adjust_value(Int16 val)
{
    if(flags.bits.point_selected == 1){


    Int16 new_value = this->value()+val;
    if((new_value >=0)&&(new_value <=this->maximum())){
        set_value(new_value,SET_TYPE_ADJUST);

    }

    }




}


void Slider::update_value(){

    set_value(this->value(),SET_TYPE_ADJUST);

}

void Slider::moved(int value){

    if(isSliderDown()){
    set_value(value,SET_TYPE_ADJUST);
    this->parent->update_curve();
    }
}

void Slider::set_value(double val, Uint8 type)
{


    switch (type) {
    case SET_TYPE_DC_VALUE:
     {
        if(this->flags.bits.point_selected == 1){
            if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
            this->setValue(2*(val)/ADC_VAL_MAPPING_FACTOR);
            else
            this->setValue((val)/ADC_VAL_MAPPING_FACTOR);

        this->parent->emu_data_yvalue[this->id]= val;

        Uint16 interval = (Uint16)(SCAN_FREQ/(2*this->parent->signal_freq));

        if((this->id + interval) < this->parent->emu_data_yvalue.length())
        this->parent->emu_data_yvalue[this->id + interval]=
        (this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        ?(-1*val):val;
        }


        break;
    }
    case SET_TYPE_AC_AMPLITUDE:
      {

        if(this->flags.bits.point_selected == 1){
        double freq    = this->parent->signal_freq;
        double phase   = this->parent->signal_phase;
        double new_val = val*sin(2*M_PI*freq*this->id*FAST_TIMER_SAMPLING_RATE - phase);


        Uint16 interval = (Uint16)(SCAN_FREQ/(2*this->parent->signal_freq));

        this->parent->emu_data_yvalue[this->id]= new_val;

       if((this->id + interval) < this->parent->emu_data_yvalue.length())
        this->parent->emu_data_yvalue[this->id + interval]=
        (this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        ?(-1*new_val):new_val;


        if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        this->setValue((2*new_val)/ADC_VAL_MAPPING_FACTOR);
        else
        this->setValue((new_val)/ADC_VAL_MAPPING_FACTOR);
        }

    break;
     }
    case SET_TYPE_DEFAULT:
    {
        double amp     = this->parent->signal_amplitude;
        double freq    = this->parent->signal_freq;
        double phase   = this->parent->signal_phase;
        double new_val = amp*sin(2*M_PI*freq*this->id*FAST_TIMER_SAMPLING_RATE - phase);


        Uint16 interval = (Uint16)(SCAN_FREQ/(2*this->parent->signal_freq));

        this->parent->emu_data_yvalue[this->id]= new_val;

        if((this->id + interval) < this->parent->emu_data_yvalue.length())
        this->parent->emu_data_yvalue[this->id + interval]=
        (this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        ?(-1*new_val):new_val;



        if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        this->setValue((2*new_val)/ADC_VAL_MAPPING_FACTOR);
        else
        this->setValue((new_val)/ADC_VAL_MAPPING_FACTOR);


    break;
    }

    case SET_TYPE_ADJUST:
    {
        this->setValue(val);

        double new_val;



        if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        new_val = (val/2)*ADC_VAL_MAPPING_FACTOR;
        else
        new_val = val*ADC_VAL_MAPPING_FACTOR;


        Uint16 interval = (Uint16)(SCAN_FREQ/(2*this->parent->signal_freq));

        this->parent->emu_data_yvalue[this->id]= new_val;

        if((this->id + interval) < this->parent->emu_data_yvalue.length())
        this->parent->emu_data_yvalue[this->id + interval]=
        (this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        ?(-1*new_val):new_val;

        //emit update_curve_signal();

    break;
    }
    default:
        break;
    }




}

void Slider::keyPressEvent(QKeyEvent *e)
{


    if(flags.bits.point_focuse_en==1){

    if(e->key() == Qt::Key_Up){
        Int16 new_value;
        if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        new_value= (this->value()/2)+10;
        else
        new_value= (this->value())+10;
        if((new_value <=this->maximum())){
            set_value(new_value,SET_TYPE_ADJUST);
            this->parent->update_curve();
        }
    }else if(e->key() == Qt::Key_Down){
        Int16 new_value;
        if(this->parent->signal_type ==WAVEFORM_PLUS_MINUS_1_65)
        new_value= (this->value()/2)-10;
        else
        new_value= (this->value())-10;
        if((new_value >=0)){
            set_value(new_value,SET_TYPE_ADJUST);
            this->parent->update_curve();
        }
    }
    }

    QSlider::keyPressEvent(e);
}

bool Slider::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::FocusOut)
    {
        if (object == this)
        {
               flags.bits.point_focuse_en = 0;
               setStyleSheet(current_style);
        }
    }
    if (event->type() == QEvent::FocusIn)
    {
        if (object == this)
        {
            flags.bits.point_focuse_en = 1;

            setStyleSheet("QSlider { width: 5px; border: none; }"
                         "QSlider::groove:veritcal {"
                         "border: none;"
                         "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:1 #000000);"

                         "margin: 0 0 0 0;}"
                         "QSlider::handle:veritcal {"
                         "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #75FF47, stop:1 #75FF47);"
                         "border: none;"
                         "width: 5px;"
                         "height: 5px;"
                         "margin: 2px 2px 2px 2px;"
                         "border-radius: 2px;}");
        }
    }
    return false;
}


void Slider::enterEvent(QEvent *e)
{

    QSlider::enterEvent(e);
}

void Slider::leaveEvent(QEvent *e)
{
    set_status(0);
    QSlider::leaveEvent(e);
}


void Slider::mousePressEvent(QMouseEvent *e)
{
   QSlider::mousePressEvent(e);
}

void Slider::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Slider::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu menu;

    QMenu   *waveform = menu.addMenu("Waveform");
             //waveform->setIcon(QIcon(":icons\signal"));

    QMenu   *dc = waveform->addMenu("DC (Constant value)");
    QDoubleSpinBox *value_dc = new QDoubleSpinBox();

    if(this->parent->signal_type == WAVEFORM_PLUS_MINUS_1_65){
    value_dc->setMaximum(50);
    value_dc->setMinimum(-50);
    }else{
    value_dc->setMaximum(100);
    value_dc->setMinimum(0);
    }
    QWidgetAction *action_dc = new QWidgetAction(this);
    action_dc->setDefaultWidget(value_dc);
    dc->addAction(action_dc);
    QAction *set_dc = dc->addAction("Set Value");

    QMenu   *ac = waveform->addMenu("AC (Sinusoid)");
    QDoubleSpinBox *value_ac = new QDoubleSpinBox();


    if(this->parent->signal_type == WAVEFORM_PLUS_MINUS_1_65){
    value_ac->setMaximum(50);
    value_ac->setMinimum(-50);
    }else{
    value_ac->setMaximum(100);
    value_ac->setMinimum(0);
    }
    QWidgetAction *action_ac = new QWidgetAction(this);
    action_ac->setDefaultWidget(value_ac);
    ac->addAction(action_ac);
    QAction *set_ac = ac->addAction("Set Value");


    QAction *select_all = menu.addAction("Select all");
    QAction *clear_all = menu.addAction("Clear selection");
    QAction *reset = menu.addAction("Reset curve");

    QAction *selectedAction = menu.exec(e->globalPos());

    if(selectedAction == set_dc)
    {

        emit action_set_value(value_dc->value(),SET_TYPE_DC_VALUE);

    }if(selectedAction == set_ac)
    {
         emit action_set_value(value_ac->value(),SET_TYPE_AC_AMPLITUDE);
    }else
    if(selectedAction == select_all)
    {
         emit selection(false);
    }else if(selectedAction == clear_all)
    {
         emit selection(true);
    }
    else if(selectedAction == reset)
    {
         emit action_set_value(0,SET_TYPE_DEFAULT);
    }


}


void Slider::set_status(Uint8 select){

    if (flags.bits.point_selection_mode==1 || select == 1)
    {
       flags.bits.point_selected=1;
     current_style = "QSlider { width: 5px; border: none; }"
                  "QSlider::groove:veritcal {"
                  "border: none;"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:1 #000000);"

                  "margin: 0 0 0 0;}"
                  "QSlider::handle:veritcal {"
                  "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #FF0000, stop:1 #FF0000);"
                  "border: none;"
                  "width: 5px;"
                  "height: 5px;"
                  "margin: 2px 2px 2px 2px;"
                  "border-radius: 2px;}";
     setStyleSheet(current_style);
    }else if (flags.bits.point_deselection_mode==1 || select==2)
    {
       flags.bits.point_selected=0;

       current_style = "QSlider { width: 5px; border: none; }"
                    "QSlider::groove:veritcal {"
                    "border: none;"
                    "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:1 #000000);"

                    "margin: 0 0 0 0;}"
                    "QSlider::handle:veritcal {"
                    "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #b4b4b4, stop:1 #8f8f8f);"
                    "border: none;"
                    "width: 5px;"
                    "height: 5px;"
                    "margin: 2px 2px 2px 2px;"
                    "border-radius: 2px;}";

       setStyleSheet(current_style);
    }



}



