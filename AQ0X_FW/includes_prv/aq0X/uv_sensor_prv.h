#ifndef UV_SEN_PRV_H
#define UV_SEN_PRV_H

/*!
  \file   uv_sen_prv.h

  \brief  Private uv_sen module header file
*/

#include "..\..\includes\allheaders.h"

//!Internal event message, initiates periodic supervision
#define X_MSG_UV_SENSOR_SV  				           (X_MSG_FREE + 5)

//! Internal supervison timer
#define X_NTF_UV_SENSOR_SV_TIMER                                   (X_NTF_FREE + 17)


//! Timeout
#define SV_TIMER_PERIOD       					    1000  //1 sec
#define SV_TIMER_LED_ON_PERIOD       			            10    //10 ms


#define FALSE_POSITIVE_TOLERANCE_LIMIT                              (2E8) - 1
#define MAX_SIGNAL_TO_THRESHOLD_GAP                                 100





typedef struct
{

     x_notify_module_sv_status_report_t          sv_status_ntf;

     //! Internal event for processing light module suppervission task
      x_event         				 sv_task_event;

     //! Timer notification to initiate light module suppervission task
      x_notify                                   sv_timer_ntf;


     //!  stores user configuration details
      configured_t                               configured;

     /*!disturbance record storage , details starting form the fuvst event
        detection to the duration of observation record length
      */
      module_disturbance_record_t                *disturbance_record;


      //!holds details about the module current state
      module_status_t                            module_status;

     //!holds diagonesis data on the module operation
      diagonesis_records_light_t                 diagonesis_records;

     //!supervission error flags
      sv_error_flags_ut                       sv_errors_flags;

}uv_sen_data_t;




extern uv_sen_data_t uv_sen_data;


Float32                 uv_sen_first_order_derivative(Uint16 final_val,Uint16 init_val);
Float32                 uv_sen_calc_probablity(Uint16 event,Uint16 obs);
Int8      	   	uv_compare(Float32 num, Float32 given_num, Float32 tolerance);


#endif //UV_SEN_PRV_H




