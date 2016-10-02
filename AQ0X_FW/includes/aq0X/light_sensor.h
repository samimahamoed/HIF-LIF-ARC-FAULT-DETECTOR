#ifndef LIGHT_SEN_H
#define LIGHT_SEN_H

/*! \file   light_sen.h

    \brief  light_sen module public header file
    
    
*/


//! Notification from Algorithm module 
#define X_NTF_LIGHT_SENSOR_CONFIG                                  (X_NTF_FREE + 13)



//TODO:find out the gain HW task
#define LIGHT_SEN_GAIN                                             1

typedef struct
{
    Uint16     adc_val;
    Float32    ana_val;

}analog_sample_light_t;




//! Notification containing module operation configuration details ( from Algorithm module)
typedef struct _x_notify_light_sen_configure
{
    x_notify           notify;

    struct
    {
       configured_t  configured;
       module_disturbance_record_t   *disturbance_record;
    }message;


}x_notify_light_sen_configure;


//! Notification structure from light_sen module to Algorithm module about supervision result
typedef struct _x_notify_light_sen_sv_report
{
    x_notify            notify;

    struct
    {
      sv_error_flags_ut      error_status_flags;
      configured_t           config;
    }message;

}x_notify_light_sen_sv_report;



//! ir_sen module handle
extern x_task_id light_sen_id;


void light_sen_taskx(x_event *event);
void light_sen_notifyx(x_notify *notify);


#endif   //LIGHT_SEN_H












