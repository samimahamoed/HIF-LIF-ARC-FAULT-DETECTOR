#ifndef MFIELD_Z_SEN_PRV_H
#define MFIELD_Z_SEN_PRV_H

/*!
 *
  \file   mfield_z_sen_prv.h

  \brief  Private mfield_z_sen module header file
*/

//!Internal event message, initiates periodic supervision
#define X_MSG_MFIELD_SENSOR_SV  				    (X_MSG_FREE)

//! Internal supervison timer
#define X_NTF_MFIELD_SENSOR_SV_TIMER                                (X_NTF_FREE + 2)


//! Timeout
#define SV_TIMER_PERIOD       					    1000  //1 sec





typedef struct
{

       x_notify_mfield_sen_sv_report             sv_status_ntf;

     //! Internal event for processing mfield module suppervission task
      x_event         				 sv_task_event;

     //! Timer notification to initiate mfield module suppervission task
      x_notify                                   sv_timer_ntf;


     //!  stores user configuration details
      configured_t                               configured;

     /*!disturbance record storage , details starting form the first event
        detection to the duration of observation record length
      */
      module_disturbance_record_t                *disturbance_record;


      //!holds details about the module current state
      module_status_mfield_t                     module_status;

     //!holds diagonesis data on the module operation
      diagonesis_records_mfield_t                diagonesis_records;

     //!supervission error flags
      sv_error_flags_ut                       sv_errors_flags;


}mfield_z_sen_data_t;


extern mfield_z_sen_data_t mfield_z_sen_data;

Float32                 mfield_z_sen_first_order_derivative(Int16 final_val,Int16 init_val,Float32 period);
Float32                 mfield_z_sen_calc_probablity(Uint16 event,Uint16 obs);
void                    mfield_z_sen_sensor_element_sv_task(void);
void                    mfield_z_sen_false_positive_correction_task(void);
void                    mfield_z_sen_mark_false_postives(void);
Float32                 mfield_z_input_signal(void);
Bool                    mfield_z_zero_cross(void);
Bool                    mfield_z_observation_time_window_expired(void);



#endif //MFIELD_Z_SEN_PRV_H


