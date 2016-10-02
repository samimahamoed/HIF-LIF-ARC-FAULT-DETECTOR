#ifndef LIGHT_SEN_PRV_H
#define LIGHT_SEN_PRV_H
/*!
  \file   light_sen_prv.h

  \brief  Private light_sen module header file  
*/

#include "..\..\includes\allheaders.h" 

//!Internal event message, initiates periodic supervision
#define X_MSG_LIGHT_SENSOR_SV  				            (X_MSG_FREE)

//! Internal supervison timer
#define X_NTF_LIGHT_SENSOR_SV_TIMER                                 (X_NTF_FREE + 2)


//! Timeout
#define SV_TIMER_PERIOD       					    1000  //1 sec



#define FALSE_POSITIVE_TOLERANCE_LIMIT                              (2E8) - 1
#define MAX_SIGNAL_TO_THRESHOLD_GAP                                 100



typedef struct
{

      x_notify_light_sen_sv_report               sv_status_ntf;

     //! Internal event for processing light module suppervission task
      x_event         				 sv_task_event;

     //! Timer notification to initiate light module suppervission task
      x_notify                                   sv_timer_ntf;


     //!  stores user configuration details
      configured_t                               configured;

     /*!disturbance record storage , details starting form the first event
        detection to the duration of observation record length
      */
      module_disturbance_record_t                *disturbance_record;


      //!holds details about the module current state
      module_status_t                            module_status;

     //!holds diagonesis data on the module operation
      diagonesis_records_light_t                       diagonesis_records;

     //!supervission error flags
      sv_error_flags_ut                       sv_errors_flags;

}light_sen_data_t;



#endif //LIGHT_SEN_PRV_H

