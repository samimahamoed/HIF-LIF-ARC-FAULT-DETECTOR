/*! \file   light_sensor.c

    \brief  Module responsible for light sensor element supervision 
    
    
*/


#include "..\..\includes\allheaders.h" 
#include "..\..\includes_prv\aq0X\light_sensor_prv.h"


//! Internal light_sen module data
light_sen_data_t light_sen_data;
x_task_id        light_sen_id;



/*!

    \brief  Init function for analog module


    
*/
static void light_sen_init(void)
{


    memset(&light_sen_data,0,sizeof(light_sen_data));

    // Prepare notifications and events to be used later on

    light_sen_data.sv_timer_ntf.message                                  = X_NTF_LIGHT_SENSOR_SV_TIMER ;
    light_sen_data.sv_timer_ntf.task_id          		         = light_sen_id;


    light_sen_data.sv_task_event.message			         = X_MSG_LIGHT_SENSOR_SV;
    light_sen_data.sv_task_event.task_id				 = light_sen_id;

    // Notifications and events sent to Algorithm module
    light_sen_data.sv_status_ntf.notify.message          	         = X_NTF_LIGHT_SENSOR_SV_ALART;
    light_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;





    // Schedule timer for periodic supervison scan
    x_schedule_timer(&light_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
}





/*!
    \brief  light sensor module task function.
    
    \param [in] event - system distributed event


*/

void light_sen_taskx(x_event *event)
{
    switch(event->message)
    {

    
        case X_MSG_LIGHT_SENSOR_SV:
        {

            light_sen_sensor_element_sv_task();

            light_sen_false_positive_adjustment_task();

            if(light_sen_data.sv_errors_flags.flags_all!=0){
             light_sen_data.sv_status_ntf.message.error_status_flags.flags_all = light_sen_data.sv_errors_flags.flags_all;
             // Send notification to Algorithm module about detected error
             x_send_notify(&light_sen_data.sv_status_ntf.notify);
             light_sen_data.sv_errors_flags.flags_all =0;
            }
            
        }break;
      

        
        default:
        {
            ASSERT(0);
        }        
    }
}



/*!
    \brief  light module notification function.
    
    \param [in] notify - system distributed notification


*/

void light_sen_notifyx(x_notify *notify)
{

    
    switch(notify->message)
    {    
        case X_NTF_INIT:
        {
            light_sen_init();
        }break;


        case X_NTF_LIGHT_SENSOR_CONFIG:
        {

            // configuration details from algorithm module
            x_notify_light_sen_configure * notify_configure = (x_notify_light_sen_configure *)notify;

           
            light_sen_data.configured = notify_configure->message.configured;
            light_sen_data.disturbance_record = notify_configure->message.disturbance_record;

            // Clear supervison state ( so errors will be reported if module have internal error )
             light_sen_data.sv_errors_flags.flags_all = 0;

          x_delete_timer(&light_sen_data.sv_timer_ntf);

          if(light_sen_data.configured.element_status_sv_en)
          x_schedule_timer(&light_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;


         case X_NTF_CLR_ALARM:
        {

           // Algorithm module alarm reset request
             light_sen_data.module_status.current.state = STATE_0;
             light_sen_mark_false_postives();


        }break;

        
        case X_NTF_LIGHT_SENSOR_SV_TIMER:
        {
           x_send_event(&light_sen_data.sv_task_event);
        }break;
        
       
        default:
        {
            ASSERT(0);
        }
    }

}


/*!

    \brief  Claculate first order derivative

    \param ad_value

*/
Float32 light_sen_first_order_derivative(Uint16 final_val,Uint16 init_val,Float32 period){
    return (Float32)(final_val-init_val)/period;
}


/*!

    \brief  Claculate the probablity that the event is true

    \param ad_value

*/
Float32 light_sen_calc_probablity(void){

    return (Float32)light_sen_data.disturbance_record->event_positive_cnt
            /
           (Float32)light_sen_data.disturbance_record->observation_cnt;

}



/*!

    \brief  Supervise the status of the light sensor

    \param ad_value

*/



event_report_t light_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;
    status.fields.active_signal= FALSE;

    light_sen_data.module_status.previous = light_sen_data.module_status.current;

    light_sen_data.module_status.current.signal_level = adc_value;

    light_sen_data.module_status.current.change_per_time = light_sen_first_order_derivative(
                                            light_sen_data.module_status.current.signal_level,
                                            light_sen_data.module_status.previous.signal_level,
                                            FAST_TIMER_SAMPLING_RATE
                                            );


    if(light_sen_data.module_status.current.change_per_time > 0)
        status.fields.active_signal = TRUE;

 
    if(logv.trip_active == TRUE){
    if(light_sen_data.disturbance_record->post_act_data.head <
        ((data_non_volatile.disturbance_record.light.emu_data_en==FALSE)?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH)))
      light_sen_data.disturbance_record->post_act_data.buffer.all[light_sen_data.disturbance_record->post_act_data.head++] = adc_value;
    }else{
   //! For every sample, pre activation data is saved in a circular buffer at the provided storage location
    light_sen_data.disturbance_record->pre_act_data.buffer[light_sen_data.disturbance_record->pre_act_data.head] = adc_value;
    light_sen_data.disturbance_record->pre_act_data.head
            = (light_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;

    if(light_sen_data.disturbance_record->pre_act_data.elapsed == TRUE)
    light_sen_data.disturbance_record->pre_act_data.tail
            = (light_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;
    else
    if(light_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    light_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;
    }

    light_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;
    if(light_sen_data.module_status.current.change_per_time > light_sen_data.configured.dfdt_treshold_max)
        light_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max = 1;
    if(light_sen_data.module_status.current.signal_level > light_sen_data.configured.amp_treshold_max)
        light_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;






 
    switch(light_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
              light_sen_data.disturbance_record->event_data.event_history    = 0;

            light_sen_data.disturbance_record->observation_cnt    = 1;
            light_sen_data.disturbance_record->event_positive_cnt = 1;

            light_sen_data.disturbance_record->event_data.buffer.signal_level[0] = light_sen_data.module_status.current.signal_level;
            light_sen_data.disturbance_record->event_data.buffer.change_per_time[0] = light_sen_data.module_status.current.change_per_time;
            light_sen_data.disturbance_record->event_data.buffer.head_ptr++;

            light_sen_data.disturbance_record->event_data.timestamp = logv.tick;
            light_sen_data.disturbance_record->event_data.event_probability = light_sen_calc_probablity();

            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                light_sen_data.module_status.current.state = STATE_2;
            }else{
              light_sen_data.module_status.current.state = STATE_1;
              light_sen_data.disturbance_record->event_data.event_history++;
            }
            }


        }break;

         case STATE_1:
        {

             light_sen_data.disturbance_record->observation_cnt++;

            if(light_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            light_sen_data.disturbance_record->event_data.buffer.signal_level[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.signal_level;
            light_sen_data.disturbance_record->event_data.buffer.change_per_time[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.change_per_time;
            light_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.all!=0)
             //! because both events are expected after fast rise in signal, we mark as positive event
                 light_sen_data.disturbance_record->event_positive_cnt++;

            light_sen_data.disturbance_record->event_data.event_probability = light_sen_calc_probablity();

            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                light_sen_data.module_status.current.state = STATE_2;
            }else
                 light_sen_data.disturbance_record->event_data.event_history++;

       
            if(light_sen_data.disturbance_record->observation_cnt
                >=
                light_sen_data.configured.min_number_of_observation
               ){
               
                   if(light_sen_data.disturbance_record->event_data.event_probability
                    >
                    light_sen_data.configured.probablity_treshold_max
                    ){
                         light_sen_data.module_status.current.state = HOT;
                         light_sen_data.disturbance_record->post_act_data.head=0;
                     }else{
                         light_sen_data.module_status.current.state = STATE_0;
                         light_sen_mark_false_postives();
                    }
             }
             
        }break;

          case STATE_2:
        {
            light_sen_data.disturbance_record->observation_cnt++;
            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
                light_sen_data.disturbance_record->event_positive_cnt++;


            if(light_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            light_sen_data.disturbance_record->event_data.buffer.signal_level[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.signal_level;
            light_sen_data.disturbance_record->event_data.buffer.change_per_time[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.change_per_time;
            light_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

              if(light_sen_data.disturbance_record->observation_cnt
                >=
                light_sen_data.configured.min_number_of_observation
               ){

                   if(light_sen_data.disturbance_record->event_data.event_probability
                    >
                    light_sen_data.configured.probablity_treshold_max
                    ){
                         light_sen_data.module_status.current.state = HOT;
                         light_sen_data.disturbance_record->post_act_data.head=0;
                     }else{
                         light_sen_data.module_status.current.state = STATE_0;
                         light_sen_mark_false_postives();
                    }
             }


         }break;



         case HOT:
        {

             light_sen_data.disturbance_record->observation_cnt++;

            if(light_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            light_sen_data.disturbance_record->event_data.buffer.signal_level[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.signal_level;
            light_sen_data.disturbance_record->event_data.buffer.change_per_time[light_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = light_sen_data.module_status.current.change_per_time;
            light_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            if(light_sen_data.disturbance_record->event_data.exceeded_threshold.all != 0)
                light_sen_data.disturbance_record->event_positive_cnt++;
            

             light_sen_data.disturbance_record->event_data.event_probability = light_sen_calc_probablity();



             if(light_sen_data.disturbance_record->event_data.event_probability < light_sen_data.configured.probablity_treshold_min){
                 light_sen_data.module_status.current.state = STATE_0;
                 light_sen_mark_false_postives();

             }

             if(light_sen_data.disturbance_record->observation_cnt > RANGE_MAX_16BIT - 1
                ||
                light_sen_data.disturbance_record->event_positive_cnt > RANGE_MAX_16BIT - 1
                ){
             //Prevents overflow
              light_sen_data.disturbance_record->observation_cnt    /=10;
              light_sen_data.disturbance_record->event_positive_cnt /=10;
             }

        }break;




        default:
        {
            ASSERT(0);
        }
    }

  status.fields.state = light_sen_data.module_status.current.state;

  return status;

}



/*!

    \brief  Supervises light sensors element by measuring sensor respond
            for different illumination level

*/

void light_sen_sensor_element_sv_task(void)
{




}

/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/


void light_sen_false_positive_adjustment_task(void){


    if(
     (light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt < FALSE_POSITIVE_TOLERANCE_LIMIT)
          &&
     (light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt < FALSE_POSITIVE_TOLERANCE_LIMIT)
     ){
      light_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH  = 0;
      light_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH = 0;
      memset(&light_sen_data.diagonesis_records.false_alarm,0,sizeof(light_sen_data.diagonesis_records.false_alarm));
      return;

    }

      if(light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt > FALSE_POSITIVE_TOLERANCE_LIMIT){
          light_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH = 1;

         if(!light_sen_data.configured.threshold_adjust_manual)
          //TODO: varifiy this will correct the problem
          light_sen_data.configured.dfdt_treshold_max +=
          light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.recorded_avg +
          light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.recorded_avg*0.01;

      }


      if(light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt > FALSE_POSITIVE_TOLERANCE_LIMIT){
          light_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH = 1;
          light_sen_data.configured.amp_treshold_max +=
          light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.recorded_avg +
          light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.recorded_avg*0.001;

           memset(&light_sen_data.diagonesis_records.false_alarm,0,sizeof(light_sen_data.diagonesis_records.false_alarm));
      }


      //!TODO: if level also is lower than the min gap from the current signal level adjust down to the level



}


/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/

void light_sen_mark_false_postives(void){
        if(light_sen_data.disturbance_record->event_data.event_history == EXCEDED_AMP_TH){
               if(
                (light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt < RANGE_MAX_8BIT)
                &
                (light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value < RANGE_MAX_32BIT)
               )
               {
                     light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt++;
                     light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value
                     +=(light_sen_data.disturbance_record->event_data.buffer.signal_level[0]-light_sen_data.configured.amp_treshold_max);
                     light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.recorded_avg
                     = light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value/
                             (Float32)light_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt;
              }

             }else{
             if(
                (light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt < RANGE_MAX_8BIT)
                &
                (light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value < RANGE_MAX_32BIT)
               )
             {

                 light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt++;
                 light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value
                 +=(light_sen_data.disturbance_record->event_data.buffer.change_per_time[0]-light_sen_data.configured.dfdt_treshold_max);
                 light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.recorded_avg
                 = light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value/
                    (Float32)light_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt;

             }

             }

}
