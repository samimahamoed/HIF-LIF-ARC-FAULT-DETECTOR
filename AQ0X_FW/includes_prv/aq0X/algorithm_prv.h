#ifndef ALGORITHM_PRV_H
#define ALGORITHM_PRV_H
/*! \file   Algorithm_prv.h

    \brief  Private Algorithm module header file
    
    
*/



//! Notification id for supervson timer 
#define X_NTF_SUPERVISION_TIMER                (X_NTF_LAST)

//! Notification id for installation timer
#define X_NTF_INSTALLATION_TIMER               (X_NTF_LAST-1)


//! Event id for wakeing main task
#define X_MSG_WAKE_EVENT                       (X_MSG_FREE)

//! Event id for configuration request
#define X_MSG_CONFIGURE_EVENT                  (X_MSG_FREE+1)

//! Supervison timeout - after the timeout SF state is cleared ( if all errors disappeared)
#define SUPERVISION_TIMER_PERIOD               2000

//! Installation timeout - 
#define INSTALLATION_TIMER_PERIOD              3000


#define  TYPE_IR_MF                             1
#define  TYPE_IR_MF_UV                          2

#define  TEST_MF                                3
#define  TEST_UV                                4




//! Algorithm module variables
typedef struct
{
    //! Private notification object- supervison timer
    x_notify                         supervision_timer;


    //! Notification message to clear alarm recognized as false positive
    x_notify                         clear_alarm_ir;

     //! Notification message to clear alarm recognized as false positive
    x_notify                         clear_alarm_uv;

     //! Notification message to clear alarm recognized as false positive
    x_notify                         clear_alarm_mfield_x;
   
    //! Notification to be sent for mfield_x modules configuration
    x_notify_mfield_sen_configure    mfield_sensor_x_configure;

#ifdef MFIELD_Y
    //! Notification to be sent for mfield_y modules configuration
    x_notify_mfield_sen_configure    mfield_sensor_y_configure;
#endif


#if(DEVICE == AQ0X_V1)
    //! Notification to be sent for light sensor modules configuration
    x_notify_light_sen_configure     light_sensor_configure;
#endif

    //! Notification to be sent for uv sensor modules configuration
    x_notify_light_sen_configure     ir_sensor_configure;

    //! Notification to be sent for ir sensor modules configuration
    x_notify_light_sen_configure     uv_sensor_configure;


    //! Event used in notifications for wakeing main task
    x_event                          wake_event;

    //! Event used to wake main task in case configuration request
    x_event                          config_event;


    //! module status collected evey 100us interval

  
    struct{

                event_report_t          mfield_x_sen;
                event_report_t          mfield_y_sen;

#if(DEVICE == AQ0X_V1)
                event_report_t          light_sen;
#endif
                event_report_t          ir_sen;
                event_report_t          uv_sen;
    
    }modules_state_sv;

    serial_msg_t                        serial_msg;

    struct{
        hif_checklist_t  hif_checklist;
    union{
      Uint8                 all;
      struct{
          Uint8 result      :1;
          Uint8 processing  :1;
          Uint8 ready       :1;
      }flags;

    }hif;

    struct{


            struct{
                Uint16                 cnt;
                Uint16                 delay;
                Uint16                 armed;
            }ir;

            struct{
                Uint16                 cnt;
                Uint16                 delay;
                Uint16                 armed;
            }uv;

             struct{
                Uint16                 cnt;
                Uint16                 delay;
                Uint16                 armed;
            }mfield_x;

    }false_alarm;

      Uint16  samples_per_period;
    }diagnosis;
   



    

    union{
        Uint16 all;
        struct{
            Uint16 external:8;
            Uint16 internal:8;
        }fields;
    }irq_src;
   
}algorithm_data_t;

extern algorithm_data_t algorithm_data;

Float32     algorithm_get_std_deviation(Uint16 * vector,Float32 mean_val,Uint16 SIZE);
Bool        algorithm_signal_proportionality(Uint16 * vector1,Uint16 * vector2,Uint16 SIZE);
void        algorithm_wake(Uint8 event);
void        algorithm_sv_scan_timer_init(void);

#endif //ALGORITHM_PRV_H


