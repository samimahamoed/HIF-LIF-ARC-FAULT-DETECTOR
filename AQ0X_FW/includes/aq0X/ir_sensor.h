#ifndef IR_SEN_H
#define IR_SEN_H

/*! \file   ir_sen.h

    \brief  ir_sen module public header file


*/


//! Notification from Algorithm module to load data from nov
#define X_NTF_IR_SENSOR_CONFIG                                  (X_NTF_FREE + 11)

//TODO:find out the gain HW task
#define IR_SEN_GAIN                                             1

#define IR_SEN_NOISE_DENSITY_LEVEL                              0.05


#define DC                                                      0
#define HIF                                                     1


#define FP_TOLERANCE                                            10
#define MAX_STOT_GAP                                            100


typedef struct
{
    Uint16     adc_val;
    Float32    ana_val;

}analog_sample_ir_t;

//! Notification containing module operation configuration details ( from Algorithm module)
typedef struct _x_notify_ir_sen_configure
{
    x_notify           notify;

    struct
    {
       configured_t                   configured;
       module_disturbance_record_t   *disturbance_record;
    }message;


}x_notify_ir_sen_configure;




//! Notification structure from ir_sen module to Algorithm module about supervision result
typedef struct _x_notify_ir_sen_sv_report
{
    x_notify            notify;

    struct
    {
      sv_error_flags_ut      error_status_flags;
      configured_t           config;
    }message;

}x_notify_ir_sen_sv_report;

typedef struct{

        struct{
       
        struct{

        Uint16                 max;
        Uint16                 min;
        //from the current signal level to threshold value
        
        Uint16                 total_observation;
        Float32                accumulated;
        Float32                avg;
        }amp;


        struct{
            Uint16                 total_observation;
            Float32                accumulated;
            Float32                avg;
        }amp_exceded;

        struct{
        Float32                avg;
        }th_gap;

 
        
        struct{
        struct{
            struct{
               Int8 previous;
               Int8 current;
            }projected_slop;
                         
        Uint16                 total_observation;
        Uint16                 rising;
        Uint16                 falling;
        Uint16                 zero;
        }change_counter;


        struct{
        Bool                   armed;
        Uint16                 value;
        Uint16                 total_observation;
        Uint16                 positive_event;
        }ptop_counter;

        struct{
        Bool                   armed;
        Uint16                 value;
        Uint16                 total_observation;
        Uint16                 positive_event;
        }itoi_counter;
        }time;



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
                Uint16                 avg;
            }amp_exceded;
        }false_alarm;

}diagonesis_records_light_t;

typedef struct
{
       Uint16                 signal_level;
       Int16                  change_per_time;
       Int8                   slop_polarity;
       Uint8                  state;
}state_data_light_t;


typedef struct{
  Uint16                     time;
  state_data_light_t         current;
  state_data_light_t         previous;

    struct{
       Float32                 amplitude;
       Float32                 angular_freq;
       Uint16                  samples_per_period;
       Uint16                  samples_per_period_1_2;
       Uint16                  samples_per_period_1_4;
       Uint16                  samples_per_period_1_8;
       Bool                    waveform;
    }projected;
}module_status_t;


extern Uint32 ir_event_trigger_timestamp;

event_report_t ir_sen_get_status(Uint16 adc_value,Bool save_event);



//! ir_sen module handle
extern x_task_id ir_sen_id;
void 		ir_sen_taskx(x_event *event);
void 		ir_sen_notifyx(x_notify *notify);
Uint8 		ir_sen_hif_check(void);
void            ir_record_reset(Uint8 type);



#endif //IR_SEN_H






