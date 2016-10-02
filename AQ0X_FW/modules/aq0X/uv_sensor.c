/*! \file   uv_sensor.c

    \brief  Module responsible for uv sensor element supervision


*/


#include "..\..\includes\allheaders.h"
#include "..\..\includes_prv\aq0X\uv_sensor_prv.h"




static void             uv_sen_sensor_element_sv_task(void);
static void             uv_sen_threshold_adjustment_task(void);
static void             uv_sen_mark_false_postives(void);
static void             uv_sen_diagonesis_record(void);


//! Internal uv_sen module data
uv_sen_data_t uv_sen_data;
x_task_id        uv_sen_id;


Uint32           uv_event_trigger_timestamp;


/*!

    \brief  module Init function



*/
 static void uv_sen_init(void)
{


    memset(&uv_sen_data,0,sizeof(uv_sen_data));

    // Prepare notifications and events to be used later on

    uv_sen_data.sv_timer_ntf.message                             = X_NTF_UV_SENSOR_SV_TIMER ;
    uv_sen_data.sv_timer_ntf.task_id          		         = uv_sen_id;


    uv_sen_data.sv_task_event.message			         = X_MSG_UV_SENSOR_SV;
    uv_sen_data.sv_task_event.task_id				 = uv_sen_id;

    // Notifications and events sent to Algorithm module
    uv_sen_data.sv_status_ntf.notify.message          	         = X_NTF_UV_SENSOR_SV_ALART;
    uv_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;

}





/*!
    \brief  uv sensor module task function.

    \param [in] event - system distributed event


*/

void uv_sen_taskx(x_event *event)
{
    switch(event->message)
    {


        case X_MSG_UV_SENSOR_SV:
        {

            srv_wdg_kick();
            uv_sen_sensor_element_sv_task();

            if(uv_sen_data.diagonesis_records.signal_history.amp.total_observation >0)
                uv_sen_data.diagonesis_records.signal_history.amp.avg =
                        (uv_sen_data.diagonesis_records.signal_history.amp.accumulated/
                        uv_sen_data.diagonesis_records.signal_history.amp.total_observation);

            if(uv_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation >0){
                uv_sen_data.diagonesis_records.signal_history.amp_exceded.avg =
                        (uv_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated/
                        uv_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation);

                uv_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (uv_sen_data.diagonesis_records.signal_history.amp_exceded.avg - uv_sen_data.configured.amp_treshold.set_value);
            }
            else
            uv_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (uv_sen_data.diagonesis_records.signal_history.amp.avg - uv_sen_data.configured.amp_treshold.set_value);


              uv_sen_data.diagonesis_records.signal_history.amp.accumulated                 = 0;
              uv_sen_data.diagonesis_records.signal_history.amp.total_observation           = 0;
              uv_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated         = 0;
              uv_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation   = 0;



             uv_sen_threshold_adjustment_task();

            if((uv_sen_data.sv_errors_flags.flags_all!=0)  || (uv_sen_data.configured.amp_treshold.adjusted == 1)){
             uv_sen_data.sv_status_ntf.message.error_flags = uv_sen_data.sv_errors_flags;
             uv_sen_data.sv_status_ntf.message.config = uv_sen_data.configured;

             // Send notification to Algorithm module about detected error
             x_send_notify(&uv_sen_data.sv_status_ntf.notify);
             uv_sen_data.sv_errors_flags.flags_all =0;
             uv_sen_data.configured.amp_treshold.adjusted = 0;
            }



        }break;



        default:
        {
            ASSERT(0);
        }
    }
}



/*!
    \brief  uv module notification function.

    \param [in] notify - system distributed notification


*/

void uv_sen_notifyx(x_notify *notify)
{


    switch(notify->message)
    {
        case X_NTF_INIT:
        {
            uv_sen_init();
        }break;


        case X_NTF_UV_SENSOR_CONFIG:
        {

            // configuration details from algorithm module
            x_notify_uv_sen_configure * notify_configure = (x_notify_uv_sen_configure *)notify;


            uv_sen_data.configured = notify_configure->message.configured;
            uv_sen_data.disturbance_record = notify_configure->message.disturbance_record;

           uv_sen_data.module_status.projected.angular_freq             = 2*PI*uv_sen_data.configured.sys_freq;
           uv_sen_data.module_status.projected.samples_per_period       = (Uint16)(SCAN_FREQ/(Float32)(uv_sen_data.configured.sys_freq));
           uv_sen_data.module_status.projected.samples_per_period_1_2  = uv_sen_data.module_status.projected.samples_per_period/2;
           uv_sen_data.module_status.projected.samples_per_period_1_4  = uv_sen_data.module_status.projected.samples_per_period/4;
           uv_sen_data.module_status.projected.samples_per_period_1_8  = uv_sen_data.module_status.projected.samples_per_period/8;
            // Clear supervison state ( so errors will be reported if module have internal error )
          uv_sen_data.sv_errors_flags.flags_all = 0;

          x_delete_timer(&uv_sen_data.sv_timer_ntf);

          if(uv_sen_data.configured.element_status_sv_en)
          x_schedule_timer(&uv_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;


        case X_NTF_CLR_ALARM_UV_SEN:
        {

           // Algorithm module alarm reset request
             //uv_sen_data.module_status.current.state = STATE_0;
             uv_sen_mark_false_postives();
             uv_sen_mark_false_postives();
             uv_sen_mark_false_postives();


        }break;


        case X_NTF_UV_SENSOR_SV_TIMER:
        {
           x_send_event(&uv_sen_data.sv_task_event);
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

 Int8 uv_compare(Float32 num1, Float32 num2, Float32 tolerance){


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
 Float32 uv_sen_first_order_derivative(Uint16 final_val,Uint16 init_val){
    return ((Float32)final_val-(Float32)init_val);
}


/*!

    \brief  Claculates the event probablity

    \param [in] event , positive event count value
    \param [in] obs   , total number of observation

*/
Float32 uv_sen_calc_probablity(Uint16 event,Uint16 obs){

    return (100L*((Float32)event/(Float32)obs));
}



/*!

    \brief peak detector
           returns true when the monitored signal change polarity

*/
static Bool uv_peak_detected(void){


       if(
          (uv_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.previous ==0)
               &&
          (uv_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current  ==0)
          )
           return FALSE;


       if(
       (uv_sen_data.module_status.previous.slop_polarity >= 0)
       &&
       (uv_sen_data.module_status.current.slop_polarity < 0)
       &&
       (abs(uv_sen_data.module_status.previous.change_per_time)< uv_sen_data.configured.dfdt_treshold_min)
       &&
       (abs(uv_sen_data.module_status.current.change_per_time)< uv_sen_data.configured.dfdt_treshold_min)
       )
       {
        return TRUE;
       }
        else
        return FALSE;

}


/*!

    \brief  Supervise the status of the uv sensor


*/



event_report_t uv_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;
    uv_sen_data.module_status.previous = uv_sen_data.module_status.current;

    uv_sen_data.module_status.current.signal_level = adc_value;

    uv_sen_data.module_status.current.change_per_time = __1ST_ORDER_DERIVATIVE(
                                            uv_sen_data.module_status.current.signal_level,
                                            uv_sen_data.module_status.previous.signal_level
                                            );

    uv_sen_data.module_status.current.slop_polarity =
                 __COMPARE_ZERO(uv_sen_data.module_status.current.change_per_time);


    if(data_non_volatile.disturbance_record.new_dr == FALSE){
    if(logv.disturbance_record.bits.wr == TRUE){
	 /*!
           \req  REQ-1b:  "A valid trip is issued,
           for few handres of microseconds after trip data will be
           saved at provided location.

    */
        if(uv_sen_data.disturbance_record->post_act_data.head == 0)
            uv_sen_data.disturbance_record->event_data.timestamp.global_active = logv.tick-1;
    ///@{
      if(uv_sen_data.disturbance_record->post_act_data.head <
        ((data_non_volatile.disturbance_record.uv.post_act_data.emu_data_en==FALSE)?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH)))
      uv_sen_data.disturbance_record->post_act_data.buffer.all[uv_sen_data.disturbance_record->post_act_data.head++] = adc_value;
      else
      status.fields.post_act_data_ready = TRUE;
    ///@}
    }else if(!logv.data_tx_on_progress.bits.uv){

     /*!
           \req  REQ-1a:  "For every sample, pre activation data is saved
           in a circular buffer at the provided storage location
    */

    ///@{
    uv_sen_data.disturbance_record->pre_act_data.buffer[uv_sen_data.disturbance_record->pre_act_data.head] = adc_value;
    uv_sen_data.disturbance_record->pre_act_data.head
            = (uv_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;

    if(uv_sen_data.disturbance_record->pre_act_data.elapsed == TRUE)
    uv_sen_data.disturbance_record->pre_act_data.tail
            = uv_sen_data.disturbance_record->pre_act_data.head;
    else
    if(uv_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    uv_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;
    ///@}
    }
    }


    uv_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;


    if(uv_sen_data.module_status.current.change_per_time > uv_sen_data.configured.dfdt_treshold_impulse)
        uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse = 1;
    else
    if(uv_sen_data.module_status.current.change_per_time  > uv_sen_data.configured.dfdt_treshold_max)
         uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max= 1;


    if(uv_sen_data.module_status.current.signal_level > uv_sen_data.configured.amp_treshold.set_value)
        uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;



   uv_sen_diagonesis_record();

    switch(uv_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
            uv_sen_data.disturbance_record->observation_cnt    = 1;
            uv_sen_data.disturbance_record->event_positive_cnt = 1;


                  uv_sen_data.disturbance_record->event_data.initial_amp =
                     uv_sen_data.module_status.current.signal_level;

             uv_sen_data.disturbance_record->event_data.timestamp.event_detected = logv.tick;

             if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
             {
                //!overcurrent detected, => state transition to state 1
                uv_sen_data.module_status.current.state = STATE_1;

             }else if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
             {
                  //! impulse or fast rising pulse detected, => state transition to state 2
                 uv_sen_data.module_status.current.state = STATE_2;
             }
             else if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
             {
               if(uv_sen_data.module_status.previous.change_per_time >= 0)
                 //!fast rising signal possible overcurrent fault, => state transition to state 3
               uv_sen_data.module_status.current.state = STATE_3;
             }

                 uv_sen_data.disturbance_record->event_data.state_transition.history.bits.initial
                = uv_sen_data.module_status.current.state;

            }


        }break;

         case STATE_1:
        {

            uv_sen_data.disturbance_record->observation_cnt++;


           if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 uv_sen_data.disturbance_record->event_positive_cnt++;
           }



               if(uv_sen_data.disturbance_record->observation_cnt
                >
                uv_sen_data.configured.min_number_of_observation
               ){


                    uv_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           uv_sen_data.disturbance_record->event_positive_cnt
                          ,uv_sen_data.disturbance_record->observation_cnt
                           );

                   if( uv_sen_data.disturbance_record->event_data.event_probability
                        >
                       uv_sen_data.configured.probablity_treshold_max
                     )
                   {
                         uv_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                         uv_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = uv_sen_data.module_status.current.state;

                         uv_sen_data.module_status.current.state = HOT;
                         uv_sen_data.disturbance_record->post_act_data.head = 0;
                         uv_sen_data.disturbance_record->post_act_data.head = 0;
                         uv_sen_data.disturbance_record->event_positive_cnt = 0;


                   }
                   else if( uv_sen_data.disturbance_record->event_data.event_probability
                            <
                           uv_sen_data.configured.probablity_treshold_min
                           )
                         {

                             uv_sen_data.module_status.current.state = STATE_0;

                             memset(&uv_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(uv_sen_data.disturbance_record->event_data.timestamp));

                        }
             }

        }break;

         case STATE_2: //!impulse
        {
            uv_sen_data.disturbance_record->observation_cnt++;

            if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! This is expected event after sharp rise in signal
                //! strong field  detected => state transition to state 1
                 uv_sen_data.disturbance_record->event_positive_cnt++;

                 uv_sen_data.module_status.current.state = STATE_1;
            }
            else
             if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max){
                //! signal is still increasing fast, mark as postive
               uv_sen_data.disturbance_record->event_positive_cnt++;

               uv_sen_data.module_status.current.state = STATE_3;

             }else if(
                     (
                     uv_sen_data.module_status.current.change_per_time>
                     uv_sen_data.configured.dfdt_treshold_min
                     )
                     &&
                     (
                     uv_sen_data.disturbance_record->observation_cnt <
                     uv_sen_data.module_status.projected.samples_per_period_1_8
                     )
                     ){
                    //! Even though signal level is below threshold
                    //! it's still increasing, we mark as postive
                    //! algorithm module will filter the error if there is one
                    uv_sen_data.disturbance_record->event_positive_cnt++;
                   }




               if(uv_sen_data.disturbance_record->observation_cnt
                >=
                uv_sen_data.configured.min_number_of_observation
               ){

                    uv_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           uv_sen_data.disturbance_record->event_positive_cnt
                          ,uv_sen_data.disturbance_record->observation_cnt
                           );

                   if( uv_sen_data.disturbance_record->event_data.event_probability
                        >
                       uv_sen_data.configured.probablity_treshold_max
                     )
                   {
                          if(
                            (
                            uv_sen_data.module_status.current.signal_level
                            - uv_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            UV_SEN_NOISE_DENSITY_LEVEL
                          ){
                                 uv_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                                 uv_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                                 = uv_sen_data.module_status.current.state;

                                 uv_sen_data.module_status.current.state = HOT;
                                 uv_sen_data.disturbance_record->post_act_data.head = 0;
                                 uv_sen_data.disturbance_record->event_positive_cnt = 0;
                           }


                   }
                   else if( uv_sen_data.disturbance_record->event_data.event_probability
                            <
                           uv_sen_data.configured.probablity_treshold_min
                           )
                         {

                             uv_sen_data.module_status.current.state = STATE_0;

                             memset(&uv_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(uv_sen_data.disturbance_record->event_data.timestamp));

                        }
             }



        }break;


        case STATE_3: //!fast rising signal
        {
            uv_sen_data.disturbance_record->observation_cnt++;

            if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
               uv_sen_data.disturbance_record->event_positive_cnt++;
            else  if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! strong field  detected state transition to state 1
               uv_sen_data.disturbance_record->event_positive_cnt++;
               uv_sen_data.module_status.current.state = STATE_1;
            }



               if(uv_sen_data.disturbance_record->observation_cnt
                >=
                uv_sen_data.configured.min_number_of_observation
               ){

                    uv_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           uv_sen_data.disturbance_record->event_positive_cnt
                          ,uv_sen_data.disturbance_record->observation_cnt
                           );

                   if( uv_sen_data.disturbance_record->event_data.event_probability
                        >
                       uv_sen_data.configured.probablity_treshold_max
                     )
                   {
                          if(
                            (
                            uv_sen_data.module_status.current.signal_level
                            - uv_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            UV_SEN_NOISE_DENSITY_LEVEL
                          ){
                                 uv_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                                 uv_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                                 = uv_sen_data.module_status.current.state;

                                 uv_sen_data.module_status.current.state = HOT;
                                 uv_sen_data.disturbance_record->post_act_data.head = 0;
                                 uv_sen_data.disturbance_record->event_positive_cnt = 0;
                           }



                   }
                   else if( uv_sen_data.disturbance_record->event_data.event_probability
                            <
                           uv_sen_data.configured.probablity_treshold_min
                           )
                         {

                             uv_sen_data.module_status.current.state = STATE_0;

                             memset(&uv_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(uv_sen_data.disturbance_record->event_data.timestamp));

                        }
             }

        }break;

        case HOT:
        {

             uv_sen_data.disturbance_record->observation_cnt++;

            if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 uv_sen_data.disturbance_record->event_positive_cnt++;

                uv_sen_data.disturbance_record->event_data.state_transition.history.bits.final
                = STATE_1;
            }


             if(
                  uv_sen_data.disturbance_record->observation_cnt
                  >
                  uv_sen_data.module_status.projected.samples_per_period
               ){

                 if(uv_sen_data.disturbance_record->event_positive_cnt == 0){
                     uv_sen_data.module_status.current.state = STATE_0; 
                 }
                           uv_sen_data.disturbance_record->observation_cnt     = 0;
                           uv_sen_data.disturbance_record->event_positive_cnt   =0;
                }



        }break;


        default:
        {
            ASSERT(0);
        }
    }


    status.fields.state = uv_sen_data.module_status.current.state;
    return status;
}



/*!

    \brief  Supervises uv sensors element by measuring sensor respond
            for different illumination level

*/

static void uv_sen_sensor_element_sv_task(void)
{
    static Uint16 ad_value1;
    static Uint16 ad_value2;

#if(TEST_MODE)


    if(uv_sen_data.configured.element_status_sv_en){
    x_delete_timer(&uv_sen_data.sv_timer_ntf);
    x_schedule_timer(&uv_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    return;
#else

    if(hwio_execute_gpio_task(HWIO_SV_LED_PORT_STATUS) == 0){
    ad_value1  = uv_sen_data.module_status.current.signal_level;

    x_delete_timer(&uv_sen_data.sv_timer_ntf);
    x_schedule_timer(&uv_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_LED_ON_PERIOD));
    }else{
     uv_sen_data.sv_errors_flags.flags_all = 0;
    ad_value2  = uv_sen_data.module_status.current.signal_level;
  


    if(
      (uv_sen_data.module_status.previous.change_per_time = 0)
      &&
      (uv_sen_data.module_status.current.change_per_time = 0)
      &&
      (ad_value2 <= ad_value1)
      )
        uv_sen_data.sv_errors_flags.bits.ELEMENT_NOT_RESPONDING = TRUE;



    if(uv_sen_data.configured.element_status_sv_en){
    x_delete_timer(&uv_sen_data.sv_timer_ntf);
    x_schedule_timer(&uv_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    }
#endif

}

/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/


 static void uv_sen_threshold_adjustment_task(void){




                if(uv_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt < FP_TOLERANCE){
                     uv_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 0;
                }else{
                     uv_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 1;
                }

                if(
                  (uv_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt < FP_TOLERANCE)
                 ){
                  uv_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH  = 0;
                }


                if(uv_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt > FP_TOLERANCE){
                  uv_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH = 1;

                  uv_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt = 0;

                 if(uv_sen_data.configured.threshold_adjust_manual == 0){
                         uv_sen_data.configured.amp_treshold.set_value  +=
                         (uv_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(uv_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)
                         );

                         if(uv_sen_data.configured.amp_treshold.set_value > uv_sen_data.configured.amp_treshold.max){
                             uv_sen_data.configured.amp_treshold.set_value = uv_sen_data.configured.amp_treshold.max;
                             //!TODO: report the situation somehow
                         }

                      uv_sen_data.configured.dfdt_treshold_max
                        = uv_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     uv_sen_data.configured.dfdt_treshold_impulse= 10*uv_sen_data.configured.dfdt_treshold_max;


                     uv_sen_data.configured.amp_treshold.adjusted = 1;

                 }


                }else

                if(
                  (uv_sen_data.configured.threshold_adjust_manual == 0)
                    &&
                  (uv_sen_data.diagonesis_records.signal_history.th_gap.avg < -1*uv_sen_data.configured.amp_treshold.min)
                  )
                  {


                      uv_sen_data.configured.amp_treshold.set_value  +=
                         (uv_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(uv_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)

                         );


                     if(uv_sen_data.configured.amp_treshold.set_value < uv_sen_data.configured.amp_treshold.min)
                        uv_sen_data.configured.amp_treshold.set_value = uv_sen_data.configured.amp_treshold.min;

                      uv_sen_data.configured.dfdt_treshold_max
                        = uv_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     uv_sen_data.configured.dfdt_treshold_impulse= 10*uv_sen_data.configured.dfdt_treshold_max;

                     uv_sen_data.configured.amp_treshold.adjusted = 1;

                  }


                   memset(&uv_sen_data.diagonesis_records.false_alarm,0,sizeof(uv_sen_data.diagonesis_records.false_alarm));




}


/*!

    \brief mark false positive occurrence

*/

 static void uv_sen_mark_false_postives(void){
        if(uv_sen_data.disturbance_record->event_data.state_transition.history.bits.final == STATE_1){
            uv_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt++;
        }else{
            uv_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt++;
       }

}



/*!

    \brief  collectes diagnosis data from sampled signal

*/

 static void uv_sen_diagonesis_record(void)
 {

              
             uv_sen_data.diagonesis_records.signal_history.amp.total_observation++;
             uv_sen_data.diagonesis_records.signal_history.amp.accumulated =
                     uv_sen_data.diagonesis_records.signal_history.amp.accumulated
                     +uv_sen_data.module_status.current.signal_level;



             if(uv_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                    uv_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation++;
                    uv_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated =
                     uv_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated
                     +uv_sen_data.module_status.current.signal_level;
             }


 }


/*!

    \brief

*/

Uint8 uv_sen_hif_check(void){

    Uint8 result = 0;
return result;
    if(uv_sen_data.configured.hif_diagonesis_records->head < PRE_ACT_RECORD_BUFF_SIZE)
        return result;

    uv_sen_data.configured.hif_diagonesis_records->mean_value/=PRE_ACT_RECORD_BUFF_SIZE;

    if(uv_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation > 0)
    if(__COMPARE_U(
            (
            uv_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation
            -
            uv_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event
            )
            ,2
            )==0
           )
        result++;

    if(uv_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation > 0)
    if(__COMPARE_U(
          (
           uv_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation
          -
           uv_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event
          )
         ,0
          )==0
       )
        result++;

    uv_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event      = 0;
    uv_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation   = 0;
    uv_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event      = 0;
    uv_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation   = 0;

    return result;


}
