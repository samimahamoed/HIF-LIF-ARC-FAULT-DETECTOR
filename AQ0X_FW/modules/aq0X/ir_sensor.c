/*! \file   ir_sensor.c

    \brief  Module responsible for ir sensor element supervision


*/


#include "..\..\includes\allheaders.h" 
#include "..\..\includes_prv\aq0X\ir_sensor_prv.h"




static void             ir_sen_sensor_element_sv_task(void);
static void             ir_sen_threshold_adjustment_task(void);
static void             ir_sen_mark_false_postives(void);
static void             ir_sen_diagonesis_record(void);


//! Internal ir_sen module data
ir_sen_data_t ir_sen_data;
x_task_id        ir_sen_id;


Uint32           ir_event_trigger_timestamp;


/*!

    \brief  module Init function



*/
 static void ir_sen_init(void)
{


    memset(&ir_sen_data,0,sizeof(ir_sen_data));

    // Prepare notifications and events to be used later on

    ir_sen_data.sv_timer_ntf.message                             = X_NTF_IR_SENSOR_SV_TIMER ;
    ir_sen_data.sv_timer_ntf.task_id          		         = ir_sen_id;


    ir_sen_data.sv_task_event.message			         = X_MSG_IR_SENSOR_SV;
    ir_sen_data.sv_task_event.task_id				 = ir_sen_id;

    // Notifications and events sent to Algorithm module
    ir_sen_data.sv_status_ntf.notify.message          	         = X_NTF_IR_SENSOR_SV_ALART;
    ir_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;

}





/*!
    \brief  ir sensor module task function.

    \param [in] event - system distributed event


*/

void ir_sen_taskx(x_event *event)
{
    switch(event->message)
    {


        case X_MSG_IR_SENSOR_SV:
        {

            srv_wdg_kick();
            ir_sen_sensor_element_sv_task();

            if(ir_sen_data.diagonesis_records.signal_history.amp.total_observation >0)
                ir_sen_data.diagonesis_records.signal_history.amp.avg =
                        (ir_sen_data.diagonesis_records.signal_history.amp.accumulated/
                        ir_sen_data.diagonesis_records.signal_history.amp.total_observation);

            if(ir_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation >0){
                ir_sen_data.diagonesis_records.signal_history.amp_exceded.avg =
                        (ir_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated/
                        ir_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation);
                 
                ir_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (ir_sen_data.diagonesis_records.signal_history.amp_exceded.avg - ir_sen_data.configured.amp_treshold.set_value);
            }
            else
            ir_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (ir_sen_data.diagonesis_records.signal_history.amp.avg - ir_sen_data.configured.amp_treshold.set_value);


              ir_sen_data.diagonesis_records.signal_history.amp.accumulated                 = 0;
              ir_sen_data.diagonesis_records.signal_history.amp.total_observation           = 0;
              ir_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated         = 0;
              ir_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation   = 0;


           
             ir_sen_threshold_adjustment_task();

            if((ir_sen_data.sv_errors_flags.flags_all!=0)  || (ir_sen_data.configured.amp_treshold.adjusted == 1)){
             ir_sen_data.sv_status_ntf.message.error_flags = ir_sen_data.sv_errors_flags;
             ir_sen_data.sv_status_ntf.message.config = ir_sen_data.configured;

             // Send notification to Algorithm module about detected error or about change in threshold values
             x_send_notify(&ir_sen_data.sv_status_ntf.notify);
             ir_sen_data.sv_errors_flags.flags_all =0;
             ir_sen_data.configured.amp_treshold.adjusted = 0;
            }



        }break;



        default:
        {
            ASSERT(0);
        }
    }
}



/*!
    \brief  ir module notification function.

    \param [in] notify - system distributed notification


*/

void ir_sen_notifyx(x_notify *notify)
{


    switch(notify->message)
    {
        case X_NTF_INIT:
        {
            ir_sen_init();
        }break;


        case X_NTF_IR_SENSOR_CONFIG:
        {

            // configuration details from algorithm module
            x_notify_ir_sen_configure * notify_configure = (x_notify_ir_sen_configure *)notify;


            ir_sen_data.configured = notify_configure->message.configured;
            ir_sen_data.disturbance_record = notify_configure->message.disturbance_record;

           ir_sen_data.module_status.projected.angular_freq             = 2*PI*ir_sen_data.configured.sys_freq;
           ir_sen_data.module_status.projected.samples_per_period       = (Uint16)(SCAN_FREQ/(Float32)(ir_sen_data.configured.sys_freq));
           ir_sen_data.module_status.projected.samples_per_period_1_2  = ir_sen_data.module_status.projected.samples_per_period/2;
           ir_sen_data.module_status.projected.samples_per_period_1_4  = ir_sen_data.module_status.projected.samples_per_period/4;
           ir_sen_data.module_status.projected.samples_per_period_1_8  = ir_sen_data.module_status.projected.samples_per_period/8;
            // Clear supervison state ( so errors will be reported if module have internal error )
          ir_sen_data.sv_errors_flags.flags_all = 0;

          x_delete_timer(&ir_sen_data.sv_timer_ntf);

          if(ir_sen_data.configured.element_status_sv_en)
          x_schedule_timer(&ir_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;


         case X_NTF_CLR_ALARM_IR_SEN:
        {

           // Algorithm module alarm reset request
            // ir_sen_data.module_status.current.state = STATE_0;
             ir_sen_mark_false_postives();
             ir_sen_mark_false_postives();
             ir_sen_mark_false_postives();

        }break;


        case X_NTF_IR_SENSOR_SV_TIMER:
        {
           x_send_event(&ir_sen_data.sv_task_event);
        }break;


        default:
        {
            ASSERT(0);
        }
    }

}


/*!

    \brief

    \param

*/


 Int8 ir_compare(Float32 num1, Float32 num2, Float32 tolerance){


   Float32 difference = num1 - num2;
     
   if(abs(difference) <= tolerance)
    return  0;

   if(difference > 0/*num1 > num2*/)
    return 1;
  
   if(difference < 0/*num1 < num2*/)
    return -1;
    
   return -2;
}


/*!

    \brief  Claculate first order derivative

*/
 Float32 ir_sen_first_order_derivative(Uint16 final_val,Uint16 init_val){
    return ((Float32)final_val-(Float32)init_val);
}


/*!

    \brief  Claculates the event probablity

    \param [in] event , positive event count value
    \param [in] obs   , total number of observation

*/
Float32 ir_sen_calc_probablity(Uint16 event,Uint16 obs){
   
    return (100L*((Float32)event/(Float32)obs));
}



/*!

    \brief peak detector
           returns true when the monitored signal change polarity

*/
static Bool ir_peak_detected(void){


       if(
          (ir_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.previous ==0)
               &&
          (ir_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current  ==0)
          )
           return FALSE;


       if(
       (ir_sen_data.module_status.previous.slop_polarity >= 0)
       &&
       (ir_sen_data.module_status.current.slop_polarity < 0)
       &&
       (abs(ir_sen_data.module_status.previous.change_per_time)< ir_sen_data.configured.dfdt_treshold_min)
       &&
       (abs(ir_sen_data.module_status.current.change_per_time)< ir_sen_data.configured.dfdt_treshold_min)
       )
       {
        return TRUE;
       }
        else
        return FALSE;

}


/*!

    \brief  Supervise the status of the ir sensor


*/



event_report_t ir_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;

    status.all = 0;
    ir_sen_data.module_status.previous = ir_sen_data.module_status.current;

    ir_sen_data.module_status.current.signal_level = adc_value;
  
    ir_sen_data.module_status.current.change_per_time = __1ST_ORDER_DERIVATIVE(
                                            ir_sen_data.module_status.current.signal_level,
                                            ir_sen_data.module_status.previous.signal_level
                                            );

    ir_sen_data.module_status.current.slop_polarity =
                 __COMPARE_ZERO(ir_sen_data.module_status.current.change_per_time);


    if(data_non_volatile.disturbance_record.new_dr == FALSE){
    if(logv.disturbance_record.bits.wr == TRUE){
	 /*!
           \req  REQ-1b:  "A valid trip is issued,
           for few handres of microseconds, i.e. after trip data will be
           saved at provided location.

    */

          if(ir_sen_data.disturbance_record->post_act_data.head == 0)
              ir_sen_data.disturbance_record->event_data.timestamp.global_active = logv.tick-1;
    ///@{
      if(ir_sen_data.disturbance_record->post_act_data.head <
        ((data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en==FALSE)?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH)))
      ir_sen_data.disturbance_record->post_act_data.buffer.all[ir_sen_data.disturbance_record->post_act_data.head++] = adc_value;
      else
      status.fields.post_act_data_ready = TRUE;
    ///@}
    }else if(!logv.data_tx_on_progress.bits.ir){
       
     /*!
           \req  REQ-1a:  "For every sample, pre activation data is saved
           in a circular buffer at the provided storage location
    */

    ///@{
    ir_sen_data.disturbance_record->pre_act_data.buffer[ir_sen_data.disturbance_record->pre_act_data.head] = adc_value;
    ir_sen_data.disturbance_record->pre_act_data.head
            = (ir_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;

    if(ir_sen_data.disturbance_record->pre_act_data.elapsed == TRUE)
    ir_sen_data.disturbance_record->pre_act_data.tail = ir_sen_data.disturbance_record->pre_act_data.head;
           
    else
    if(ir_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    ir_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;
    ///@}
    }

    }

    ir_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;


    if(ir_sen_data.module_status.current.change_per_time > ir_sen_data.configured.dfdt_treshold_impulse)
        ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse = 1;
    else
    if(ir_sen_data.module_status.current.change_per_time  > ir_sen_data.configured.dfdt_treshold_max)
         ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max= 1;


    if(ir_sen_data.module_status.current.signal_level > ir_sen_data.configured.amp_treshold.set_value)
        ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;



   ir_sen_diagonesis_record();
			              
    switch(ir_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
            ir_sen_data.disturbance_record->observation_cnt    = 1;
            ir_sen_data.disturbance_record->event_positive_cnt = 1;




            ir_sen_data.disturbance_record->event_data.initial_amp =
                     ir_sen_data.module_status.current.signal_level;

             ir_sen_data.disturbance_record->event_data.timestamp.event_detected = logv.tick;

             if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
             {
                //!overcurrent detected, => state transition to state 1
                ir_sen_data.module_status.current.state = STATE_1;

             }else if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
             {
                  //! impulse or fast rising pulse detected, => state transition to state 2
                 ir_sen_data.module_status.current.state = STATE_2;
             }
             else if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
             {
               if(ir_sen_data.module_status.previous.change_per_time >= 0)
                 //!fast rising signal possible overcurrent fault, => state transition to state 3
               ir_sen_data.module_status.current.state = STATE_3;
             }

                 ir_sen_data.disturbance_record->event_data.state_transition.history.bits.initial
                = ir_sen_data.module_status.current.state;

            }


        }break;

         case STATE_1:
        {

            ir_sen_data.disturbance_record->observation_cnt++;


           if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 ir_sen_data.disturbance_record->event_positive_cnt++;
           }



               if(ir_sen_data.disturbance_record->observation_cnt
                >
                ir_sen_data.configured.min_number_of_observation
               ){


                    ir_sen_data.disturbance_record->event_data.event_probability =
                       __PROBABLITY(
                           ir_sen_data.disturbance_record->event_positive_cnt
                          ,ir_sen_data.disturbance_record->observation_cnt
                           );

                   if( ir_sen_data.disturbance_record->event_data.event_probability
                        >
                       ir_sen_data.configured.probablity_treshold_max
                     )
                   {
                       ir_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                         ir_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = ir_sen_data.module_status.current.state;

                         ir_sen_data.module_status.current.state = HOT;
                         ir_sen_data.disturbance_record->post_act_data.head = 0;
                         ir_sen_data.disturbance_record->post_act_data.head = 0;
                         ir_sen_data.disturbance_record->event_positive_cnt = 0;


                   }
                   else if( ir_sen_data.disturbance_record->event_data.event_probability
                            <
                           ir_sen_data.configured.probablity_treshold_min
                           )
                         {

                             ir_sen_data.module_status.current.state = STATE_0;
                             ir_sen_mark_false_postives();

                             memset(&ir_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(ir_sen_data.disturbance_record->event_data.timestamp));

                        }
             }

        }break;

         case STATE_2: //!impulse
        {
            ir_sen_data.disturbance_record->observation_cnt++;

            if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! This is expected event after sharp rise in signal
                //! strong field  detected => state transition to state 1
                 ir_sen_data.disturbance_record->event_positive_cnt++;

                 ir_sen_data.module_status.current.state = STATE_1;
            }
            else
             if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max){
                //! signal is still increasing fast, mark as postive
               ir_sen_data.disturbance_record->event_positive_cnt++;

               ir_sen_data.module_status.current.state = STATE_3;

             }
             else if(
                     (
                     ir_sen_data.module_status.current.change_per_time>
                     ir_sen_data.configured.dfdt_treshold_min
                     )
                     &&
                     (
                     ir_sen_data.disturbance_record->observation_cnt <
                     ir_sen_data.module_status.projected.samples_per_period_1_8
                     )
                     ){
                    //! Even though signal level is below threshold
                    //! it's still increasing, we mark as postive
                    //! algorithm module will filter the error if there is one
                    ir_sen_data.disturbance_record->event_positive_cnt++;
                   }
            


               if(ir_sen_data.disturbance_record->observation_cnt
                >=
                ir_sen_data.configured.min_number_of_observation
               ){

                    ir_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           ir_sen_data.disturbance_record->event_positive_cnt
                          ,ir_sen_data.disturbance_record->observation_cnt
                           );

                   if( ir_sen_data.disturbance_record->event_data.event_probability
                        >
                       ir_sen_data.configured.probablity_treshold_max
                     )
                   {

                         
                        if(
                            (
                            ir_sen_data.module_status.current.signal_level
                            - ir_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            IR_SEN_NOISE_DENSITY_LEVEL
                          ){
                         ir_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                         ir_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = ir_sen_data.module_status.current.state;

                         ir_sen_data.module_status.current.state = HOT;
                         ir_sen_data.disturbance_record->post_act_data.head = 0;
                         ir_sen_data.disturbance_record->event_positive_cnt = 0;
                           }




                   }
                   else if( ir_sen_data.disturbance_record->event_data.event_probability
                            <
                           ir_sen_data.configured.probablity_treshold_min
                           )
                         {

                             ir_sen_data.module_status.current.state = STATE_0;
                             ir_sen_mark_false_postives();

                             memset(&ir_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(ir_sen_data.disturbance_record->event_data.timestamp));

                        }
             }



        }break;


        case STATE_3: //!fast rising signal
        {
            ir_sen_data.disturbance_record->observation_cnt++;

            if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
               ir_sen_data.disturbance_record->event_positive_cnt++;
            else  if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! strong field  detected state transition to state 1
               ir_sen_data.disturbance_record->event_positive_cnt++;
               ir_sen_data.module_status.current.state = STATE_1;
            }



               if(ir_sen_data.disturbance_record->observation_cnt
                >=
                ir_sen_data.configured.min_number_of_observation
               ){

                    ir_sen_data.disturbance_record->event_data.event_probability =
                          __PROBABLITY(
                           ir_sen_data.disturbance_record->event_positive_cnt
                          ,ir_sen_data.disturbance_record->observation_cnt
                           );

                   if( ir_sen_data.disturbance_record->event_data.event_probability
                        >
                       ir_sen_data.configured.probablity_treshold_max
                     )
                   {
                        if(
                            (
                            ir_sen_data.module_status.current.signal_level
                            - ir_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            IR_SEN_NOISE_DENSITY_LEVEL
                          ){
                             ir_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                             ir_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                             = ir_sen_data.module_status.current.state;

                             ir_sen_data.module_status.current.state = HOT;
                             ir_sen_data.disturbance_record->post_act_data.head = 0;
                             ir_sen_data.disturbance_record->event_positive_cnt = 0;
                           }


                   }
                   else if( ir_sen_data.disturbance_record->event_data.event_probability
                            <
                           ir_sen_data.configured.probablity_treshold_min
                           )
                         {


                             ir_sen_data.module_status.current.state = STATE_0;
                             ir_sen_mark_false_postives();


                             memset(&ir_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(ir_sen_data.disturbance_record->event_data.timestamp));

                        }
             }

        }break;

        case HOT:
        {

             ir_sen_data.disturbance_record->observation_cnt++;

            if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 ir_sen_data.disturbance_record->event_positive_cnt++;

                ir_sen_data.disturbance_record->event_data.state_transition.history.bits.final
                = STATE_1;
            }


             if(
                  ir_sen_data.disturbance_record->observation_cnt
                  >
                  ir_sen_data.module_status.projected.samples_per_period
               ){


                    if(ir_sen_data.disturbance_record->event_positive_cnt == 0){
                       ir_sen_data.module_status.current.state = STATE_0;
                    }
                           ir_sen_data.disturbance_record->observation_cnt     = 0;
                           ir_sen_data.disturbance_record->event_positive_cnt   =0;
                }

   

        }break;


        default:
        {
            ASSERT(0);
        }
    }


    status.fields.state = ir_sen_data.module_status.current.state;
    return status;
}



/*!

    \brief  Supervises ir sensors element by measuring sensor respond
            for different illumination level

*/

static void ir_sen_sensor_element_sv_task(void)
{
    static Uint16 ad_value1;
    static Uint16 ad_value2;

#if(TEST_MODE)


    if(ir_sen_data.configured.element_status_sv_en){
    x_delete_timer(&ir_sen_data.sv_timer_ntf);
    x_schedule_timer(&ir_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    return;
#else

    if(hwio_execute_gpio_task(HWIO_SV_LED_PORT_STATUS) == 0){
    ad_value1  = ir_sen_data.module_status.current.signal_level;
    hwio_execute_gpio_task(HWIO_SV_LED_ON);


    x_delete_timer(&ir_sen_data.sv_timer_ntf);
    x_schedule_timer(&ir_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_LED_ON_PERIOD));
    }else{
     ir_sen_data.sv_errors_flags.flags_all = 0;
    ad_value2  = ir_sen_data.module_status.current.signal_level;
    hwio_execute_gpio_task(HWIO_SV_LED_OFF);


    if(
      (ir_sen_data.module_status.previous.change_per_time = 0)
      &&
      (ir_sen_data.module_status.current.change_per_time = 0)
      &&
      (ad_value2 <= ad_value1)
      )
        ir_sen_data.sv_errors_flags.bits.ELEMENT_NOT_RESPONDING = TRUE;



    if(ir_sen_data.configured.element_status_sv_en){
    x_delete_timer(&ir_sen_data.sv_timer_ntf);
    x_schedule_timer(&ir_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    }
#endif

}

/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/


 static void ir_sen_threshold_adjustment_task(void){




                if(ir_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt < FP_TOLERANCE){
                     ir_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 0;
                }else{
                     ir_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 1;
                }

                if(
                  (ir_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt < FP_TOLERANCE)
                 ){
                  ir_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH  = 0;

           
                  }


                if(ir_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt > FP_TOLERANCE){
                  ir_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH = 1;

                  ir_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt = 0;
   
                 if(ir_sen_data.configured.threshold_adjust_manual == 0){
                         ir_sen_data.configured.amp_treshold.set_value  +=
                         (ir_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(ir_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)
                         );

                         if(ir_sen_data.configured.amp_treshold.set_value > ir_sen_data.configured.amp_treshold.max){
                             ir_sen_data.configured.amp_treshold.set_value = ir_sen_data.configured.amp_treshold.max;
                            
                         }
                  
                      ir_sen_data.configured.dfdt_treshold_max
                        = ir_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     ir_sen_data.configured.dfdt_treshold_impulse= 10*ir_sen_data.configured.dfdt_treshold_max;
                     

                     ir_sen_data.configured.amp_treshold.adjusted = 1;

                 }


                }else

                if(
                  (ir_sen_data.configured.threshold_adjust_manual == 0)
                    &&
                  (ir_sen_data.diagonesis_records.signal_history.th_gap.avg < -1*ir_sen_data.configured.amp_treshold.min)
                  )
                  {


                      ir_sen_data.configured.amp_treshold.set_value  +=
                         (ir_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(ir_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)

                         );


                     if(ir_sen_data.configured.amp_treshold.set_value < ir_sen_data.configured.amp_treshold.min)
                        ir_sen_data.configured.amp_treshold.set_value = ir_sen_data.configured.amp_treshold.min;

                      ir_sen_data.configured.dfdt_treshold_max
                        = ir_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     ir_sen_data.configured.dfdt_treshold_impulse= 10*ir_sen_data.configured.dfdt_treshold_max;

                     ir_sen_data.configured.amp_treshold.adjusted = 1;

                  }


                   memset(&ir_sen_data.diagonesis_records.false_alarm,0,sizeof(ir_sen_data.diagonesis_records.false_alarm));
     



}


/*!

    \brief mark false positive occurrence

*/

 static void ir_sen_mark_false_postives(void){
        if(ir_sen_data.disturbance_record->event_data.state_transition.history.bits.final == STATE_1)
            ir_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt++;
        
        ir_sen_data.disturbance_record->event_data.state_transition.history.bits.final = 0;

}



/*!

    \brief  collectes diagnosis data from sampled signal

*/

 static void ir_sen_diagonesis_record(void)
 {

              if(ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed==1){
               ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value++;

               if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse==1 )
                {
                     ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation++;

                     if(
                        (__COMPARE((ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value
                                  -ir_sen_data.module_status.projected.samples_per_period_1_2),
                               5/*+/- 5*/
                          )==0)
                         )
                       {
                           ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                           ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event++;

                       }else
                          if((__COMPARE((ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value
                               -ir_sen_data.module_status.projected.samples_per_period),
                               5 /*+/- 5*/
                              )==0)
                            ){
                              ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                              ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event++;

                             }


                 }else{


                     if(
                       (ir_sen_data.module_status.current.state == STATE_0)
                       ||
                       (
                            ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value
                            >
                            (3*ir_sen_data.module_status.projected.samples_per_period)
                       )
                       )
                      {
                       ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation = 0;
                     }

                 }

         }else
         if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
         {
              //! impulse or fast rising pulse detected, launch diagnosis counter
                 ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed              = 1;
                 ir_sen_data.configured.hif_diagonesis_records->head                                = 0;
                 ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.value              = 0;
                 ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event     = 1;
                 ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation  = 1;


             //!  new peak is expected after this
                 ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed              = 0;

         }


             



             ir_sen_data.diagonesis_records.signal_history.amp.total_observation++;
             ir_sen_data.diagonesis_records.signal_history.amp.accumulated =
                     ir_sen_data.diagonesis_records.signal_history.amp.accumulated
                     +ir_sen_data.module_status.current.signal_level;



             if(ir_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                    ir_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation++;
                    ir_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated =
                     ir_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated
                     +ir_sen_data.module_status.current.signal_level;
             }


                                                  ir_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation++;
        
             if(ir_sen_data.module_status.current.change_per_time > 0)
                 ir_sen_data.diagonesis_records.signal_history.time.change_counter.rising++;
             else  if(ir_sen_data.module_status.current.change_per_time < 0)
             ir_sen_data.diagonesis_records.signal_history.time.change_counter.falling++;


             if(ir_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation ==8/*Observation length*/){

             if(ir_sen_data.diagonesis_records.signal_history.time.change_counter.rising > 5 /*>60%*/){
                  ir_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = 1;
             }else
             if(ir_sen_data.diagonesis_records.signal_history.time.change_counter.falling > 5 /*>60%*/){
              ir_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = -1;
             }else
              ir_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = 0;

                 ir_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation = 0;
                 ir_sen_data.diagonesis_records.signal_history.time.change_counter.rising            = 0;
                 ir_sen_data.diagonesis_records.signal_history.time.change_counter.falling           = 0;

           }





          if(ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed==1){
               ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value++;

               if(ir_peak_detected())
                {

                   ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation++;
                   if(
                      (__COMPARE((ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value
                           -ir_sen_data.module_status.projected.samples_per_period_1_2),
                           5
                      )==0)
                   )
                   {
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event++;

                   }
                   else
                   if(ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value
                           <
                      (ir_sen_data.module_status.projected.samples_per_period)
                   )
                   {

                    if(
                     ir_sen_data.module_status.previous.signal_level
                      >
                     ir_sen_data.diagonesis_records.signal_history.amp.max
                     )
                   {

                      /*!
                       Although, the peak detector function is designed to detect only
                       sinusoidal signal peak while ignoring other local peaks in the
                       signal due to harmonics. It is possible that false positive may
                       occur if the detected peak exhibit similar behavior as expected
                       one. In this situation, we simply switch the reference peak if
                       the new peak is higher in amplitude and it occurs within one
                       fourth of the period after the peak which is detected earlier.

                       */
                       ir_sen_data.diagonesis_records.signal_history.amp.max =
                       ir_sen_data.module_status.previous.signal_level;
           

                       ir_sen_data.configured.hif_diagonesis_records->head                                = 0;
                       ir_sen_data.configured.hif_diagonesis_records->mean_value                          = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value              = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event     = 1;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation  = 1;



                       if(ir_sen_data.configured.hif_diagonesis_records->head < MAX_EVENT_OBSERVATION_LENGTH){
                       ir_sen_data.configured.hif_diagonesis_records->buffer[ir_sen_data.configured.hif_diagonesis_records->head++]
                       = ir_sen_data.module_status.current.signal_level;
                       ir_sen_data.configured.hif_diagonesis_records->mean_value =ir_sen_data.module_status.previous.signal_level;
                       }


                    }
                   }
                 }
                 else
                 {


                    if(ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value
                    >
                    2*ir_sen_data.module_status.projected.samples_per_period
                    ){
                       //signal was inactive for 2 cycles
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed = 0;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event = 1;
                       ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation = 1;

                     }

                 }

               if(ir_sen_data.configured.hif_diagonesis_records->head < MAX_EVENT_OBSERVATION_LENGTH){
               ir_sen_data.configured.hif_diagonesis_records->buffer[ir_sen_data.configured.hif_diagonesis_records->head++]
               = ir_sen_data.module_status.current.signal_level;
               ir_sen_data.configured.hif_diagonesis_records->mean_value +=ir_sen_data.module_status.previous.signal_level;
               }


         }
         else
         if(ir_peak_detected())
         {

                  ir_sen_data.diagonesis_records.signal_history.amp.max  =
                         ir_sen_data.module_status.previous.signal_level;

                   ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed              = 1;
                   ir_sen_data.configured.hif_diagonesis_records->head                                = 0;
                   ir_sen_data.configured.hif_diagonesis_records->mean_value                          = 0;

                   ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.value              = 0;
                   ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event     = 1;
                   ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation  = 1;

               ir_sen_data.configured.hif_diagonesis_records->buffer[ir_sen_data.configured.hif_diagonesis_records->head++]
               = ir_sen_data.module_status.previous.signal_level;
               ir_sen_data.configured.hif_diagonesis_records->mean_value =ir_sen_data.module_status.previous.signal_level;

         }




 }


/*!

    \brief

*/

Uint8 ir_sen_hif_check(void){

    hif_checklist_t result;
    result.all = 0;

    if(ir_sen_data.configured.hif_diagonesis_records->head < PRE_ACT_RECORD_BUFF_SIZE)
        return result.all;

    ir_sen_data.configured.hif_diagonesis_records->mean_value /=PRE_ACT_RECORD_BUFF_SIZE;

    if(ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation > 0)
    if(__COMPARE_U(
            (
            ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation
            -
            ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event
            )
            ,2
            )==0
           )
        result.bits.itoi = 1;

    if(ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation > 0)
    if(__COMPARE_U(
          (
           ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation
          -
           ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event
          )
         ,0
          )==0
       )
        result.bits.ptop = 1;

    ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event      = 0;
    ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation   = 0;
    ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event      = 0;
    ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation   = 0;

    return result.all;



}


/*!

    \brief

*/

void ir_record_reset(Uint8 type){

    if(type == HIF){
            ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event      = 0;
            ir_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation   = 0;
            ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event      = 0;
            ir_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation   = 0;

            ir_sen_data.configured.hif_diagonesis_records->head = 0;
       }
}
