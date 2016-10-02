#ifndef ALGORITHM_H
#define ALGORITHM_H
/*! \file   algorithm.h

    \brief  Sub definitions  ( for algorithm.h)
    
    
*/

#include "dsp.h"


#define RANGE_MAX_8BIT                                          ((2E8)  -(1))
#define RANGE_MAX_16BIT                                         ((2E16) -(1))
#define RANGE_MAX_32BIT                                         ((2E32) -(1))


//#define PI                                                      (3.141592653589793)




#define __COMPARE(difference,tolerance)                         ((abs(difference)<= tolerance) ? (0) : (((difference)> 0)?(1):(-1)))

#define __COMPARE_U(difference,tolerance)                       (((difference)<= tolerance) ? (0) : 1)

#define __COMPARE_ZERO(difference)                              ((difference == 0) ? (0) : (((difference)> 0)?(1):(-1)))


#define __1ST_ORDER_DERIVATIVE(final_val,init_val)              ((final_val-init_val))


#define __PROBABLITY(event,obs)                                 (100L*((event)/(obs)))


Uint16                                                          crc16(unsigned char *puchMsg,Uint16 usDataLen);




//! Supply  - 
//! Measured in real system 990 & 1000
#define SUPPLY_LOW                                              990

//! Supply  -
#define SUPPLY_HIGH                                             1000



#define SV_CURREN_OUTPUT_LEVEL                                  4



/*! external interrupts source identification number
 MIN : 0x0001
 MAX : 0x00FF
 */
#define IRQ_SOURCE_SERIAL                                       0x0001
/*! internal software interrupts source identification number
 MIN : 0x0100
 MAX : 0xFF00
*/
#define IRQ_SOURCE_HIF_EVENT                                    0x0100
#define IRQ_DR_READY                                            0x0200




#define EVENT_SV_ALARM                                          0
#define SV_ALERT                                                1
#define DR_RECHECK                                              2
#define HIF_EVENT_ALARM                                         3
#define EVENT_CLEARED                                           4

#define DR_RECHECK_TIME                                         (POST_ACT_RECORD_BUFF_SIZE + 10)


#define CONFIG_DEFAULT_FREQ                                     50
#define CONFIG_DEFAULT_RATED_CURRENT                            2000
#define CONFIG_DEFAULT_SHORT_CKT_CURRENT                        45000
#define CONFIG_DEFAULT_MIN_INSTALLATION_DISTANCE                1

#define CONFIG_DEFAULT_IR_LEVEL_MAX                             300
#define CONFIG_DEFAULT_UV_LEVEL_MAX                             300
#define CONFIG_DEFAULT_LUX_LEVEL_MAX                            300
#define CONFIG_DEFAULT_PROBABLITY_MAX                           90
#define CONFIG_DEFAULT_PROBABLITY_MIN                           10
#define CONFIG_DEFAULT_NO_OF_OBSERVATION                        4

#define CONFIG__DEFAULT_MIN_DFDT                                3     //12mV




//! Sensor/input scan frequency - 10kHz, 100us period
#define SCAN_FREQ                                               10000L

#define FAST_TIMER_SAMPLING_RATE                                100E-6        //timer rate in second

#define FAST_TIMER_SAMPLING_RATE_XU                             100           //timer rate in us


//common commands
#define CMD_CONFIG                                              0xE000
#define CMD_DEVICE_ADDRESS                                      0xE101
#define CMD_SIGNAL_THRESHOLDS                                   0xE203
#define CMD_INSTALLATION_DITAILS                                0xE302
#define CMD_EVENT_ANALYSIS                                      0xE404
#define CMD_RUN_TIME_OPTIONS                                    0xE505
#define CMD_FACTORY_DEFAULT                                     0xE106
#define CMD_COMMIT                                              0xE107

#define CMD_EMULATION_SETTING                                   0xE008
#define CMD_LOAD_IR_DATA                                        0xE609
#define CMD_LOAD_UV_DATA                                        0xE60A
#define CMD_LOAD_MAGNETIC_FIELD_X_DATA                          0xE60B
#define CMD_LOAD_MAGNETIC_FIELD_Y_DATA                          0xE60C
#define CMD_LOAD_HIF_REFERENCE_DATA                             0xE60D

#define CMD_EMU_MODE_SET                                        0xE10D
#define CMD_EMU_MODE_RESET                                      0xE10E
#define CMD_CLEAR_EMU_DATA                                      0xE10F
#define CMD_MISCELLANEOUS                                       0xE01A
#define CMD_DEVICE_RESET                                        0xE11B
#define CMD_CLEAR_EEPROM                                        0xE11C
#define CMD_SUPERVISION_STATUS                                  0xE11D

#define CMD_GET_DATA                                            0xE0E0
#define CMD_DEVICE_DATA                                         0xE1E1
#define CMD_IR_DISTURBANCE_RECORD                               0xE1E2
#define CMD_UV_DISTURBANCE_RECORD                               0xE1E3
#define CMD_MFIELD_X_DISTURBANCE_RECORD                         0xE1E4
#define CMD_MFIELD_Y_DISTURBANCE_RECORD                         0xE1E5
#define CMD_IR_CURRENT_RECORD                                   0xE1E6
#define CMD_UV_CURRENT_RECORD                                   0xE1E7
#define CMD_MFIELD_X_CURRENT_RECORD                             0xE1E8
#define CMD_MFIELD_Y_CURRENT_RECORD                             0xE1E9
#define CMD_HIF_DIAGNOSIS_RECORD                                0xE1EA



#define ACK                                                     0x0006
#define RC_ERROR                                                0x0008
#define CMD_ERROR                                               0x0007




#define STATE_0                                                 0
#define STATE_1                                                 1
#define STATE_2                                                 2
#define STATE_3                                                 3
#define HOT                                                     4

//! Notification to Algorithm  module about persistent error
#define X_NTF_IR_SENSOR_SV_ALART			            (X_NTF_FREE + 0)

//! Notification to Algorithm  module about persistent error
#define X_NTF_LIGHT_SENSOR_SV_ALART			            (X_NTF_FREE + 1)

//! Notification to Algorithm  module about persistent error
#define X_NTF_MFIELD_X_SENSOR_SV_ALART			            (X_NTF_FREE + 2)

#define X_NTF_MFIELD_Y_SENSOR_SV_ALART			            (X_NTF_FREE + 3)

#define X_NTF_MFIELD_Z_SENSOR_SV_ALART			            (X_NTF_FREE + 4)

#define X_NTF_UV_SENSOR_SV_ALART                                    (X_NTF_FREE + 5)

//! Notification from Algorithm to clear alarm recognized as false positive
#define X_NTF_CLR_ALARM_IR_SEN                                      (X_NTF_FREE + 7)


#define X_NTF_CLR_ALARM_UV_SEN                                      (X_NTF_FREE + 8)


#define X_NTF_CLR_ALARM_MFIELD_SEN                                  (X_NTF_FREE + 9)


//TODO: use address detection to suppervise supply also
#define X_NTF_ALG_ANALOG_SUPPLY_ALERT                               (X_NTF_FREE + 10)




 typedef struct configured
 {

        Float32                         dfdt_treshold_impulse;
        Float32                         dfdt_treshold_max;
        Float32                         dfdt_treshold_min;
        Uint16                          sys_freq;
        struct{
            Uint16      set_value;
            Uint16      max;
            Uint16      min;
            Bool        adjusted;
        }amp_treshold;
      
        Uint16                          min_number_of_observation;
        Float32                         probablity_treshold_min;
        Float32                         probablity_treshold_max;

        Bool                            threshold_adjust_manual;
        Bool                            element_status_sv_en;
        hif_diagonesis_records_t*       hif_diagonesis_records;
  }configured_t;

 typedef union{
       Uint8  all;
       struct{
        Uint8  itoi:1;
        Uint8  ptop:1;
        Uint8  correlation:1;
       }bits;
 }hif_checklist_t;



//! Notification structure 
typedef struct _x_notify_module_sv_status_report_t
{
    x_notify            notify;

    struct
    {
      sv_error_flags_ut   error_flags;
      configured_t        config;
    }message;

}x_notify_module_sv_status_report_t;


typedef union{
    Uint8 all;
        struct{
            Uint8                   state:3;
            Uint8                   active_signal:1;
            Uint8                   hif_data_ready:1;
            Uint8                   post_act_data_ready:1;
        }fields;
}event_report_t;

typedef struct
{
    Uint8                 event_count;
    Uint32                tick;
    Bool                  trip_active;
    Bool                  wr;


    union{
       Uint8  all;
       struct{
        Uint8  wr:1;
        Uint8  ready:1;
        Uint8  loaded:1;
       }bits;
    }disturbance_record;

    
    union{
       Uint8  all;
       struct{
        Uint8  ir:1;
        Uint8  uv:1;
        Uint8  mfield_x:1;
        Uint8  mfield_y:1;

        Uint8  ir_l:1;
        Uint8  uv_l:1;
        Uint8  mfield_x_l:1;
        Uint8  mfield_y_l:1;
       }bits;
    }data_tx_on_progress;

    struct{
        Bool    set;
        Uint16  cnt;
        Uint16  type;
    }sv_delay;

}logv_t;



Float32         algorithm_get_mean(Uint16 * vector,Uint16 SIZE);
void 		msg_to_mem(void* dest,Uint8* msg,Uint16 size,Uint8 base);
void 		algorithm_message_arrived_ntf_isr(Uint16 id);
void 		algorithm_taskx(x_event *event);
void 		algorithm_notifyx(x_notify *notify);

//! ALgorithm module handle
extern x_task_id algorithm_id;
extern logv_t           logv;




#endif //ALGORITHM_H




