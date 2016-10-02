
/*!
  \file   mfield_x_sen_prv.h

  \brief  Private mfield_x_sen module header file
*/
#ifndef MFIELD_X_SEN_PRV_H
#define MFIELD_X_SEN_PRV_H


//!Internal event message, initiates periodic supervision
#define X_MSG_MFIELD_SENSOR_SV  				    (X_MSG_FREE + 3)

//! Internal supervison timer
#define X_NTF_MFIELD_SENSOR_SV_TIMER                                (X_NTF_FREE + 15)


//! Timeout
#define SV_TIMER_PERIOD       					    1000  //msec



typedef struct
{

       x_notify_mfield_sen_sv_report             sv_status_ntf;

     //! Internal event for processing mfield module suppervission task
      x_event         				  sv_task_event;

     //! Timer notification to initiate mfield module suppervission task
      x_notify                                    sv_timer_ntf;


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
      sv_error_flags_ut                          sv_errors_flags;


}mfield_x_sen_data_t;


extern mfield_x_sen_data_t mfield_x_sen_data;

Float32                 mfield_x_sen_first_order_derivative(Int16 final_val,Int16 init_val);
Float32                 mfield_x_sen_calc_probablity(Uint16 event,Uint16 obs);
Int8      	   	mfield_x_compare(Float32 num, Float32 given_num, Float32 tolerance);
#endif //MFIELD_X_SEN_PRV_H


