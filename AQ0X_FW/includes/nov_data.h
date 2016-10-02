/*! \file   nov_data.h

    \brief  Header for low level NOV support

*/
#ifndef NOV_DATA_H
#define NOV_DATA_H

#include "System.h"
#include "allheaders.h"


#define PRE_ACT_RECORD_BUFF_SIZE                                200
#define POST_ACT_RECORD_BUFF_SIZE                               800
#define EMU_MODE_DATA_LENGTH                                    200
#define MAX_EVENT_OBSERVATION_LENGTH                            200

//! sensor supervission replay status report structure
typedef union
{
    Uint16 flags_all;
    struct
    {
        Uint16 power_supply:1;
        Uint16 storage:1;
        Uint16 mfield_x_sensor_element:1;
#ifdef MFIELD_Y
        Uint16 mfield_y_sensor_element:1;
#endif
#if(DEVICE == AQ0X_V1)
        Uint16 light_sensor_element:1;
#endif
        Uint16 uv_sensor_element:1;
        Uint16 ir_sensor_element:1;

    }bits;


}global_error_flags_ut;

//! sensor supervission replay status report structure
typedef union
{
     Uint8 flags_all;
    struct
    {
        Uint8 ELEMENT_NOT_RESPONDING:1;
        Uint8 FALSE_POSITIVE:1;
        Uint8 FALSE_POSITIVE_AMP_TH:1;
        Uint8 FALSE_POSITIVE_DFDT_TH:1;
    }bits;

}sv_error_flags_ut;

//!  Supervision error handling - contains current variable state reported by other modules and global error status
typedef struct
{
  //! Combined supervision errors
  global_error_flags_ut     system_error_flags;

  struct{
#if(DEVICE == AQ0X_V1)
   sv_error_flags_ut        light;
#endif
   sv_error_flags_ut        ir;
   sv_error_flags_ut        uv;
   sv_error_flags_ut        mfield_x;
#ifdef MFIELD_Y
   sv_error_flags_ut        mfield_y;
#endif
  }error_flags;

}supervision_t;


#define DEFAULT_TRIP_CRITERION   0b00001101


//! Bit coded info about trip criteria
typedef  union{

    Uint16 all;

    union
    {
        Uint16 all;
        struct
        {
            Uint8 ir      :1;
            Uint8 uv      :1;
            Uint8 mfield_x:1;
            #ifdef MFIELD_Y
            Uint8 mfield_y:1;
            #else
            Uint8         :1;
            #endif
            Uint8         :1;
            Uint8         :1;
            Uint8         :1;
            Uint8         :1;
        }bits;
    }active_modules;

     union
    {
        Uint8 all;
        struct
        {

            Uint8 case_1  :1;
            Uint8 case_2  :1;
            Uint8 case_3  :1;
            Uint8 case_4  :1;
            Uint8         :1;
            Uint8         :1;
            Uint8         :1;
            Uint8         :1;
        }bits;
    }criterion;


}trip_criteria_ut;


typedef struct    __attribute__((packed, aligned(4)))
{
       Float32                mean_value;
       Float32                std_deviation;
       Uint16                 head;
       Uint16                 valid;
       Uint16                 buffer[MAX_EVENT_OBSERVATION_LENGTH];
}hif_diagonesis_records_t;

typedef struct
{
//! buffer ( circular)
Uint16   buffer[PRE_ACT_RECORD_BUFF_SIZE];

//! if set indicates that head is elapsed one or more time
Uint16    elapsed;
//! Transmit buffer head
Uint16   head;
//! Transmit buffer tail
Uint16    tail;


}pre_act_record_buffer_t;


typedef struct __attribute__((packed, aligned(4)))
{
//! buffer ( circular)
    union{
        Uint16 all[POST_ACT_RECORD_BUFF_SIZE];
        struct{
            Uint16 l[POST_ACT_RECORD_BUFF_SIZE - EMU_MODE_DATA_LENGTH];
            Uint16 h[EMU_MODE_DATA_LENGTH];
        }data;
    }buffer;
//! when set indicates that half the buffer is used for emulation data and vice versa
Uint16                      emu_data_en;

//! Transmit buffer head
Uint16   					head;
//! Transmit buffer tail
Uint16   					tail;

Uint32                     	timestamp;
}post_act_record_buffer_t;



typedef struct
{

    pre_act_record_buffer_t    pre_act_data;
    post_act_record_buffer_t   post_act_data;

    //Diagnostic counter - total observation count
    Uint16                      observation_cnt;
    //Diagnostic counter - positive event count
    Uint16                      event_positive_cnt;

    //holds disturbance details starting form event detetion for the duration of Observation record length
    struct __attribute__((packed, aligned(4)))
   {
     Uint16                     initial_amp;
    union{
        Uint16 all;
        struct{
            Uint16 dfdt_max:1;
            Uint16 amp_max:1;
            Uint16 dfdt_impulse:1;
            Uint16 dfdt_min:1;
        }bits;
    }exceeded_threshold;


    struct{
    union{
        Uint16 all;
        struct{
            Uint16 initial  :4;
            Uint16 mid      :4;
            Uint16 final    :4;
            Uint16          :4;
  
        }bits;
    }history;

    Uint16 cnt;

    }state_transition;


    struct{
     Uint32                    event_detected;
     Uint32                    module_active;
     Uint32                    global_active;
    }timestamp;



    Float32                    event_probability;

    

    }event_data;



}module_disturbance_record_t;


typedef struct __attribute__((packed, aligned(4)))
{
    struct{
        Uint16                              device_address;
        struct{
        //! max short circuit current
        Uint16                              sh_current;
        //! max rated circuit current
        Uint16                              rated_current;
        //! minimum distance for live conductor
        Uint16                              min_distance;
        //! system frequency
        Uint16                              freq;

        }system_info;


        struct{
        //! light intensity measurement threshold value
        Uint16                              lux_level_max;
        //! infrared intensity measurement threshold level
        Uint16                              ir_level_max;
        //! ultraviolet intensity measurement threshold level
        Uint16                              uv_level_max;

        //!the minimum number of event observation used by modules
        //!to validate probability calculation result
        Uint16                              min_number_of_observation;

        //! used as a threshold value, above which the probablity
        //of an event condisered to be true
        Uint16                              probablity_max;

        //! used as a threshold value, below which the probablity
        //of an event condisered to be false
        Uint16                              probablity_min;

        }thresholds;

        //!bit coded info about trip criteria
        trip_criteria_ut                    trip_criteria;

        //!FW sub modules operation mode options
        union
        {
           Uint16                           all;
           struct
           {
              Uint16                        threshold_adjust_manual :1;
              Uint16                        ir_element_sv_en        :1;
              Uint16                        uv_element_sv_en        :1;
              Uint16                        mfield_element_sv_en    :1;
              Uint16                                                :1;
              Uint16                                                :1;
              Uint16                                                :1;
              Uint16                                                :1;
           }bits;
        }options;


        //! Set to true when other fileds contain valid user configured data
        Uint16                             is_valid;
   }installation;

   //! supervision record
    supervision_t                          sv_record;


    Uint16                                 configured_data_crc;

    hif_diagonesis_records_t               ref_signal;

}device_data_t;

 typedef struct{
    module_disturbance_record_t            mfield_x;
    #ifdef MFIELD_Y
    module_disturbance_record_t            mfield_y;
    #endif
    module_disturbance_record_t            ir;
    module_disturbance_record_t            uv;
    hif_diagonesis_records_t               hif_diagonesis_records;
    Uint16                                 new_dr;
 }disturbance_record_t;

//! Data structure for variables stored in NOV
typedef struct __attribute__((packed, aligned(4)))
{
    //! Installed options
    device_data_t                          device_data;
    disturbance_record_t                   disturbance_record;
    Uint16                                 crc;
}data_non_volatile_t;


extern data_non_volatile_t  data_non_volatile __attribute__((far));

//extern __prog__ data_non_volatile_t                 data_non_volatile  __attribute__((space(prog)));



#endif //NOV_DATA_H


