#ifndef UV_SEN_H
#define UV_SEN_H

/*! \file   uv_sen.h

    \brief  uv_sen module public header file


*/

//! Notification from Algorithm module to load data from nov
#define X_NTF_UV_SENSOR_CONFIG                                  (X_NTF_FREE + 16)


//TODO:find out the gain HW task
#define UV_SEN_GAIN                                             1

#define UV_SEN_NOISE_DENSITY_LEVEL                              0.05




typedef struct
{
    Uint16     adc_val;
    Float32    ana_val;

}analog_sample_uv_t;

//! Notification containing module operation configuration details ( from Algorithm module)
typedef struct _x_notify_uv_sen_configure
{
    x_notify           notify;

    struct
    {
       configured_t  configured;

       module_disturbance_record_t   *disturbance_record;
    }message;


}x_notify_uv_sen_configure;




//! Notification structure from uv_sen module to Algorithm module about supervision result
typedef struct _x_notify_uv_sen_sv_report
{
    x_notify            notify;

    struct
    {
      sv_error_flags_ut      error_status_flags;
      configured_t           config;
    }message;

}x_notify_uv_sen_sv_report;

event_report_t uv_sen_get_status(Uint16 adc_value,Bool save_event);


//! mfield_x_sen module handle
extern x_task_id uv_sen_id;


void uv_sen_taskx(x_event *event);
void uv_sen_notifyx(x_notify *notify);

#endif //UV_SEN_H




