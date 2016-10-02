#ifndef MFIELD_SEN_H
#define MFIELD_SEN_H
/*! \file   mfield_sen.h

    \brief  mfield_sen module public header file


*/

//! Notification from Algorithm module configuration data update
#define X_NTF_MFIELD_X_SENSOR_CONFIG                                (X_NTF_FREE  + 13)

#ifdef MFIELD_Y
#define X_NTF_MFIELD_Y_SENSOR_CONFIG                                (X_NTF_FREE +  14)
#endif



//TODO: adjust to 2 Meg
//! Sensor Sensitivity = 1mV/V/gauss => S(3.3V) = 3.3mV/gauss
//! With gain of 83.3 for full dynamic range +/- 6 gauss
//! produces about a 0.274V/gauss transfer characteristic
//! centered at half supply (1.65 volts).
#define MFIELD_SEN_GAIN                                             0.274



//! amplifier output at zero magnetic field
#define MFIELD_ADC_QUIESCENT_LEVEL                                  512

#define MFIELD_SEN_NOISE_DENSITY_LEVEL                              0


#define REDUCTION_FACTOR          100

#define EXCEDED_AMP_TH            0x00
#define EXCEDED_DFDT_IMPULS       0x02
#define EXCEDED_DFDT_MAX          0x04
#define EXCEDED_DFDT_MIN          0x08



#define SINUSOID                               0
#define MIN_ACCEPTABLE_CORRELATION             0.90
#define PRE_ACTIVATION_RECORD_LENGTH_MFIELD    100
#define OBSERVATION_TIME_WINDOW                (1/2*FAST_TIMER_SAMPLING_RATE_XU + 100)


typedef struct
{
    Int16      adc_val;
 //   Float32    ana_val;
    Float32    calculated_pick;
    Int8       polarity;
}analog_sample_mfield_t;



typedef struct{
        Float32                dl_impulse;
        Float32                dl_treshold_max;
        Float32                dl_treshold_min;
        analog_sample_mfield_t amp_treshold_max;
}calc_tresholds_mfield_t;






//! Notification containing module operation configuration details ( from Algorithm module)
typedef struct _x_notify_mfield_sen_configure
{
    x_notify           notify;

    struct
    {
       configured_t                   configured;
       module_disturbance_record_t   *disturbance_record;
    
    }message;


}x_notify_mfield_sen_configure;


//! Notification structure from mfield_sen module to Algorithm module about supervision error
typedef struct _x_notify_mfield_sen_sv_report
{
    x_notify            notify;

    struct
    {
      sv_error_flags_ut   error_status_flags;
    }message;

}x_notify_mfield_sen_sv_report;


typedef struct{
        Int16                  signal_level;
        Float32                change_per_time;
        Uint8                  state;
        Int8                   polarity;
}state_data_mfield_t;
typedef struct{
   Uint16                     time;
   state_data_mfield_t        current;

   state_data_mfield_t        previous;
   
    struct{
       Float32                amplitude;
       Float32                waveform;
       Float32                angular_freq;
       Uint16                 samples_per_period;
       Uint16                 samples_per_period_1_2;
       Uint16                 samples_per_period_1_4;
       Uint16                 samples_per_period_1_8;
    }projected;


    
}module_status_mfield_t;



typedef struct{

    struct{

        struct{

        Uint16                 max;
        Uint16                 min;
        //from the current signal level to threshold value
        Float32                accumulated;
        Uint16                 total_observation;
        Float32                avg;
        }amp;


        struct{
            Uint8                  total_observation;
            Uint32                 accumulated;
            Float32                avg;
        }amp_exceded;

    }signal_history;


     /*! Counts and stores false positive details, the module uses the
        information to adjust the threshold values in order to reduce
        false positive occurrence (i.e. if the module is running in
        automatic adjust mode)*/
        struct{
            struct{
                Uint16                 cnt;

                Float32                avg;
            }dfdt_exceded;

            struct{
                Uint16                 cnt;
        
                Float32                avg;
            }amp_exceded;
        }false_alarm;


}diagonesis_records_mfield_t;



event_report_t mfield_x_sen_get_status(Uint16 adc_value,Bool save_event);
event_report_t mfield_y_sen_get_status(Uint16 adc_value,Bool save_event);



//! mfield_x_sen module handle
extern x_task_id mfield_x_sen_id;
void mfield_x_sen_taskx(x_event *event);
void mfield_x_sen_notifyx(x_notify *notify);


//! mfield_y_sen module handle
extern x_task_id mfield_y_sen_id;
void mfield_y_sen_taskx(x_event *event);
void mfield_y_sen_notifyx(x_notify *notify);



#endif   //MFIELD_SEN_H









