#ifndef IR_SEN_PRV_H
#define IR_SEN_PRV_H

/*!
  \file   ir_sen_prv.h

  \brief  Private ir_sen module header file
*/

#include "..\..\includes\allheaders.h" 

//!Internal event message, initiates periodic supervision
#define X_MSG_IR_SENSOR_SV  				            	        (X_MSG_FREE + 2)

//! Internal supervison timer
#define X_NTF_IR_SENSOR_SV_TIMER                        			(X_NTF_FREE + 12)


//! Timeout
#define SV_TIMER_PERIOD       			                    		1000  //2 sec
#define SV_TIMER_LED_ON_PERIOD       			            		10    //10 ms





typedef struct
{

      x_notify_module_sv_status_report_t         sv_status_ntf;

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


      //!holds details about the module current status
      module_status_t                            module_status;


     //!holds diagonesis data on the module operation
      diagonesis_records_light_t                 diagonesis_records;

     //!supervission error flags
      sv_error_flags_ut                          sv_errors_flags;

}ir_sen_data_t;


extern ir_sen_data_t ir_sen_data;

Float32         ir_sen_first_order_derivative(Uint16 final_val,Uint16 init_val);
Float32         ir_sen_calc_probablity(Uint16 event,Uint16 obs);
Int8            ir_compare(Float32 num, Float32 given_num, Float32 tolerance);



#endif //IR_SEN_PRV_H

