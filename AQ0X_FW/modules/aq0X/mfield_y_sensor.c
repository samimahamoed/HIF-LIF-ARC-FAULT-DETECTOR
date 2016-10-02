/*! \file   mfield_x_sensor.c

    \brief  Module responsible for mfield_x sensor element supervision


*/


#include "..\..\includes\allheaders.h"
#include "..\..\includes_prv\aq0X\mfield_x_sensor_prv.h"




static void             mfield_x_sen_sensor_element_sv_task(void);
static void             mfield_x_sen_threshold_adjustment_task(void);
static void             mfield_x_sen_mark_false_postives(void);
static void             mfield_x_sen_diagonesis_record(void);


//! Internal mfield_x_sen module data
mfield_x_sen_data_t mfield_x_sen_data;
x_task_id        mfield_x_sen_id;


Uint32           mfield_x_event_trigger_timestamp;


/*!

    \brief  module Init function



*/
 static void mfield_x_sen_init(void)
{


    memset(&mfield_x_sen_data,0,sizeof(mfield_x_sen_data));

    // Prepare notifications and events to be used later on

    mfield_x_sen_data.sv_timer_ntf.message                             = X_NTF_MFIELD_SENSOR_SV_TIMER ;
    mfield_x_sen_data.sv_timer_ntf.task_id          		         = mfield_x_sen_id;


    mfield_x_sen_data.sv_task_event.message			         = X_MSG_MFIELD_SENSOR_SV;
    mfield_x_sen_data.sv_task_event.task_id				 = mfield_x_sen_id;

    // Notifications and events sent to Algorithm module
    mfield_x_sen_data.sv_status_ntf.notify.message          	         = X_NTF_MFIELD_X_SENSOR_SV_ALART;
    mfield_x_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;

}





/*!
    \brief  mfield_x sensor module task function.

    \param [in] event - system distributed event


*/

void mfield_x_sen_taskx(x_event *event)
{
    switch(event->message)
    {


        case X_MSG_MFIELD_SENSOR_SV:
        {


            mfield_x_sen_sensor_element_sv_task();

            if(mfield_x_sen_data.diagonesis_records.signal_history.amp.total_observation >0)
                mfield_x_sen_data.diagonesis_records.signal_history.amp.avg =
                        mfield_x_sen_data.diagonesis_records.signal_history.amp.accumulated/
                        mfield_x_sen_data.diagonesis_records.signal_history.amp.total_observation;

            if(mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation >0){
                mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.avg =
                        mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated/
                        mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation;

                mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.avg - mfield_x_sen_data.configured.amp_treshold.set_value);
            }
            else
            mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg
                    = (mfield_x_sen_data.diagonesis_records.signal_history.amp.avg - mfield_x_sen_data.configured.amp_treshold.set_value);


              mfield_x_sen_data.diagonesis_records.signal_history.amp.accumulated                 = 0;
              mfield_x_sen_data.diagonesis_records.signal_history.amp.total_observation           = 0;
              mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated         = 0;
              mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation   = 0;



             mfield_x_sen_threshold_adjustment_task();

            if(mfield_x_sen_data.sv_errors_flags.flags_all!=0){
             mfield_x_sen_data.sv_status_ntf.message.error_flags = mfield_x_sen_data.sv_errors_flags;
             mfield_x_sen_data.sv_status_ntf.message.config = mfield_x_sen_data.configured;

             // Send notification to Algorithm module about detected error
             x_send_notify(&mfield_x_sen_data.sv_status_ntf.notify);
             mfield_x_sen_data.sv_errors_flags.flags_all =0;
            }

        }break;



        default:
        {
            ASSERT(0);
        }
    }
}



/*!
    \brief  mfield_x module notification function.

    \param [in] notify - system distributed notification


*/

void mfield_x_sen_notifyx(x_notify *notify)
{


    switch(notify->message)
    {
        case X_NTF_INIT:
        {
            mfield_x_sen_init();
        }break;


        case X_NTF_MFIELD_X_SENSOR_CONFIG:
        {

            // configuration details from algorithm module
            x_notify_mfield_x_sen_configure * notify_configure = (x_notify_mfield_x_sen_configure *)notify;


            mfield_x_sen_data.configured = notify_configure->message.configured;
            mfield_x_sen_data.disturbance_record = notify_configure->message.disturbance_record;

           mfield_x_sen_data.module_status.projected.angular_freq             = 2*PI*mfield_x_sen_data.configured.sys_freq;
           mfield_x_sen_data.module_status.projected.samples_per_one_forth_period       = (Uint16)(SCAN_FREQ/(Float32)(mfield_x_sen_data.configured.sys_freq));
           mfield_x_sen_data.module_status.projected.samples_per_half_period  = mfield_x_sen_data.module_status.projected.samples_per_one_forth_period/2;
           mfield_x_sen_data.module_status.projected.samples_per_period_1_4  = mfield_x_sen_data.module_status.projected.samples_per_one_forth_period/4;
            // Clear supervison state ( so errors will be reported if module have internal error )
          mfield_x_sen_data.sv_errors_flags.flags_all = 0;

          x_delete_timer(&mfield_x_sen_data.sv_timer_ntf);

          if(mfield_x_sen_data.configured.element_status_sv_en)
          x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;


         case X_NTF_CLR_ALARM:
        {

           // Algorithm module alarm reset request
             mfield_x_sen_data.module_status.current.state = STATE_0;
             mfield_x_sen_mark_false_postives();


        }break;


        case X_NTF_MFIELD_X_SENSOR_SV_TIMER:
        {
           x_send_event(&mfield_x_sen_data.sv_task_event);
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

 Int8 mfield_x_compare(Float32 num1, Float32 num2, Float32 tolerance){

   Int8    result = -2;
   Float32 difference = num1 - num2;

   if(abs(difference) <= tolerance)
    result = 0;
   if(difference > 0/*num1 > num2*/)
    result = 1;
   if(difference < 0/*num1 < num2*/)
    result = -1;
   return result;
}


/*!

    \brief  Claculate first order derivative

*/
 Float32 mfield_x_sen_first_order_derivative(Uint16 final_val,Uint16 init_val){
    return ((Float32)final_val-(Float32)init_val);
}


/*!

    \brief  Claculates the event probablity

    \param [in] event , positive event count value
    \param [in] obs   , total number of observation

*/
Bool mfield_x_sen_calc_probablity(Uint16 event,Uint16 obs,Uint16 threshold){

    return ((100L*((Float32)event/(Float32)obs)) > threshold)?TRUE:FALSE;
}



/*!

    \brief peak detector
           returns true when the monitored signal change polarity

*/
static Bool mfield_x_peak_detected(void){


       if(
          (mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.previous ==0)
               &&
          (mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current  ==0)
          )
           return FALSE;


       if(
       (mfield_x_sen_data.module_status.previous.slop_polarity >= 0)
       &&
       (mfield_x_sen_data.module_status.current.slop_polarity < 0)
       &&
       (abs(mfield_x_sen_data.module_status.previous.change_per_time)< mfield_x_sen_data.configured.dfdt_treshold_min)
       &&
       (abs(mfield_x_sen_data.module_status.current.change_per_time)< mfield_x_sen_data.configured.dfdt_treshold_min)
       )
       {
        return TRUE;
       }
        else
        return FALSE;

}


/*!

    \brief  Supervise the status of the mfield_x sensor


*/



event_report_t mfield_x_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;
    mfield_x_sen_data.module_status.previous = mfield_x_sen_data.module_status.current;

    mfield_x_sen_data.module_status.current.signal_level = adc_value;

    mfield_x_sen_data.module_status.current.change_per_time = mfield_x_sen_first_order_derivative(
                                            mfield_x_sen_data.module_status.current.signal_level,
                                            mfield_x_sen_data.module_status.previous.signal_level
                                            );

    mfield_x_sen_data.module_status.current.slop_polarity =
                 mfield_x_compare(mfield_x_sen_data.module_status.current.change_per_time,0,0);


    if(logv.trip_active == TRUE){
	 /*!
           \req  REQ-1b:  "A valid trip is issued,
           for few handres of microseconds after trip data will be
           saved at provided location.

    */

    ///@{
      if(mfield_x_sen_data.disturbance_record->post_act_data.head <
        ((data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en==FALSE)?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH)))
      mfield_x_sen_data.disturbance_record->post_act_data.buffer.all[mfield_x_sen_data.disturbance_record->post_act_data.head++] = adc_value;
    ///@}
    }else if(!logv.data_tx_on_progress.bits.mfield_x){

     /*!
           \req  REQ-1a:  "For every sample, pre activation data is saved
           in a circular buffer at the provided storage location
    */

    ///@{
    mfield_x_sen_data.disturbance_record->pre_act_data.buffer[mfield_x_sen_data.disturbance_record->pre_act_data.head] = adc_value;
    mfield_x_sen_data.disturbance_record->pre_act_data.head
            = (mfield_x_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;

    if(mfield_x_sen_data.disturbance_record->pre_act_data.elapsed == TRUE)
    mfield_x_sen_data.disturbance_record->pre_act_data.tail
            = (mfield_x_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;
    else
    if(mfield_x_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    mfield_x_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;
    ///@}
    }



    mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;


    if(mfield_x_sen_data.module_status.current.change_per_time > mfield_x_sen_data.configured.dfdt_treshold_impulse)
        mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse = 1;
    else
    if(mfield_x_sen_data.module_status.current.change_per_time  > mfield_x_sen_data.configured.dfdt_treshold_max)
         mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max= 1;


    if(mfield_x_sen_data.module_status.current.signal_level > mfield_x_sen_data.configured.amp_treshold.set_value)
        mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;



   mfield_x_sen_diagonesis_record();

    switch(mfield_x_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
            mfield_x_sen_data.disturbance_record->observation_cnt    = 1;
            mfield_x_sen_data.disturbance_record->event_positive_cnt = 1;



            mfield_x_sen_data.disturbance_record->event_data.timestamp.event_detected = logv.tick;

             if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
             {
                  //! impulse or fast rising pulse detected, => state transition to state 2
                 mfield_x_sen_data.module_status.current.state = STATE_2;
             }
             else if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
             {
                //!overcurrent detected, => state transition to state 1
                mfield_x_sen_data.module_status.current.state = STATE_1;

             }else if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
             {
                 //!fast rising signal possible overcurrent fault, => state transition to state 3
               mfield_x_sen_data.module_status.current.state = STATE_3;
             }

                 mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.initial
                = mfield_x_sen_data.module_status.current.state;

            }

             mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.final
                = mfield_x_sen_data.module_status.current.state;

        }break;

         case STATE_1:
        {
            mfield_x_sen_data.disturbance_record->observation_cnt++;


           if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 mfield_x_sen_data.disturbance_record->event_positive_cnt++;
           }



               if(mfield_x_sen_data.disturbance_record->observation_cnt
                >
                mfield_x_sen_data.configured.min_number_of_observation
               ){


                   if(mfield_x_sen_calc_probablity(
                           mfield_x_sen_data.disturbance_record->event_positive_cnt
                          ,mfield_x_sen_data.disturbance_record->observation_cnt
                          ,mfield_x_sen_data.configured.probablity_treshold_max
                           )
                    ){
                         mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = mfield_x_sen_data.module_status.current.state;

                         mfield_x_sen_data.module_status.current.state = HOT;
                         mfield_x_sen_data.disturbance_record->post_act_data.head = 0;



                     }else{

                         mfield_x_sen_data.module_status.current.state = STATE_0;
                        // mfield_x_sen_mark_false_postives();
                         memset(&mfield_x_sen_data.disturbance_record->event_data.timestamp,0,
                                 sizeof(mfield_x_sen_data.disturbance_record->event_data.timestamp));

                    }
             }

        }break;

         case STATE_2: //!impulse
        {
            mfield_x_sen_data.disturbance_record->observation_cnt++;


            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! This is expected event after sharp rise in signal
                //! strong field  detected => state transition to state 1
                 mfield_x_sen_data.disturbance_record->event_positive_cnt++;

                 mfield_x_sen_data.module_status.current.state = STATE_1;
            }
            else
             if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max){
                //! This is another possible event after sharp rise in signal
                //! waveform is below threshold.
                //! however this may indicate HIF event
               mfield_x_sen_data.disturbance_record->event_positive_cnt++;

               mfield_x_sen_data.module_status.current.state = STATE_3;

             }
             else{
               mfield_x_sen_data.module_status.current.state = STATE_0;

               memset(&mfield_x_sen_data.disturbance_record->event_data.timestamp,0,
                                 sizeof(mfield_x_sen_data.disturbance_record->event_data.timestamp));
            }
        }break;


        case STATE_3: //!fast rising signal
        {
            mfield_x_sen_data.disturbance_record->observation_cnt++;

            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
               mfield_x_sen_data.disturbance_record->event_positive_cnt++;
            else  if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! strong field  detected state transition to state 1
               mfield_x_sen_data.disturbance_record->event_positive_cnt++;
               mfield_x_sen_data.module_status.current.state = STATE_1;
            }



               if(mfield_x_sen_data.disturbance_record->observation_cnt
                >=
                mfield_x_sen_data.configured.min_number_of_observation
               ){

                   if(mfield_x_sen_calc_probablity(
                             mfield_x_sen_data.disturbance_record->event_positive_cnt
                            ,mfield_x_sen_data.disturbance_record->observation_cnt
                            ,mfield_x_sen_data.configured.probablity_treshold_max)
                     ){
                        mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = mfield_x_sen_data.module_status.current.state;
                         mfield_x_sen_data.module_status.current.state = HOT;
                         mfield_x_sen_data.disturbance_record->post_act_data.head = 0;
                       }
                       else
                       {
                             mfield_x_sen_data.module_status.current.state = STATE_0;
                             //mfield_x_sen_mark_false_postives();
                             memset(&mfield_x_sen_data.disturbance_record->event_data.timestamp,0,
                             sizeof(mfield_x_sen_data.disturbance_record->event_data.timestamp));
                       }
             }

        }break;

        case HOT:
        {

            if(mfield_x_sen_data.module_status.current.change_per_time < 0)
            {
               //! for negative slop state will not change
               status.fields.state = mfield_x_sen_data.module_status.current.state;
               return status;

            }



             mfield_x_sen_data.disturbance_record->observation_cnt++;



            if(
              (mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
              ){

                 mfield_x_sen_data.disturbance_record->event_positive_cnt++;
            }else
            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 mfield_x_sen_data.disturbance_record->event_positive_cnt++;

                mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.final
                = STATE_1;
            }



             if(
                    mfield_x_sen_data.disturbance_record->observation_cnt
                  - mfield_x_sen_data.disturbance_record->event_positive_cnt
                  >(mfield_x_sen_data.module_status.projected.samples_per_one_forth_period)
               )
                 mfield_x_sen_data.module_status.current.state = STATE_0;


             if(mfield_x_sen_data.disturbance_record->observation_cnt > 100){
             //Prevents overflow
              mfield_x_sen_data.disturbance_record->observation_cnt     -= mfield_x_sen_data.disturbance_record->event_positive_cnt;

              mfield_x_sen_data.disturbance_record->event_positive_cnt   =0;




             }

        }break;


        default:
        {
            ASSERT(0);
        }
    }


    status.fields.state = mfield_x_sen_data.module_status.current.state;
    return status;
}



/*!

    \brief  Supervises mfield_x sensors element by measuring sensor respond
            for different illumination level

*/

static void mfield_x_sen_sensor_element_sv_task(void)
{
    static Uint16 ad_value1;
    static Uint16 ad_value2;

#if(TEST_MODE)


    if(mfield_x_sen_data.configured.element_status_sv_en){
    x_delete_timer(&mfield_x_sen_data.sv_timer_ntf);
    x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    return;
#else

    if(hwio_execute_gpio_task(HWIO_SV_LED_PORT_STATUS) == 0){
    ad_value1  = mfield_x_sen_data.module_status.current.signal_level;
    hwio_execute_gpio_task(HWIO_SV_LED_ON);


    x_delete_timer(&mfield_x_sen_data.sv_timer_ntf);
    x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_LED_ON_PERIOD));
    }else{
     mfield_x_sen_data.sv_errors_flags.flags_all = 0;
    ad_value2  = mfield_x_sen_data.module_status.current.signal_level;
    hwio_execute_gpio_task(HWIO_SV_LED_OFF);


    if(
      (mfield_x_sen_data.module_status.previous.change_per_time = 0)
      &&
      (mfield_x_sen_data.module_status.current.change_per_time = 0)
      &&
      (ad_value2 <= ad_value1)
      )
        mfield_x_sen_data.sv_errors_flags.bits.ELEMENT_NOT_RESPONDING = TRUE;



    if(mfield_x_sen_data.configured.element_status_sv_en){
    x_delete_timer(&mfield_x_sen_data.sv_timer_ntf);
    x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
    }
    }
#endif

}

/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/


 static void mfield_x_sen_threshold_adjustment_task(void){




                if(mfield_x_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt < FP_TOLERANCE){
                     mfield_x_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 0;
                }else{
                     mfield_x_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_DFDT_TH        = 1;
                }

                if(
                  (mfield_x_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt < FP_TOLERANCE)
                 ){
                  mfield_x_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH  = 0;
                }


                if(mfield_x_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt > FP_TOLERANCE){
                  mfield_x_sen_data.sv_errors_flags.bits.FALSE_POSITIVE_AMP_TH = 1;

                  mfield_x_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt = 0;

                 if(mfield_x_sen_data.configured.threshold_adjust_manual == 0){
                         mfield_x_sen_data.configured.amp_treshold.set_value  +=
                         (mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)
                         );

                         if(mfield_x_sen_data.configured.amp_treshold.set_value > mfield_x_sen_data.configured.amp_treshold.max){
                             mfield_x_sen_data.configured.amp_treshold.set_value = mfield_x_sen_data.configured.amp_treshold.max;
                             //!TODO: report the situation somehow
                         }

                      mfield_x_sen_data.configured.dfdt_treshold_max
                        = mfield_x_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     mfield_x_sen_data.configured.dfdt_treshold_impulse= 10*mfield_x_sen_data.configured.dfdt_treshold_max;



                 }


                }else

                if(
                  (mfield_x_sen_data.configured.threshold_adjust_manual == 0)
                    &&
                  ((mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg) < -1*MAX_STOT_GAP)
                  )
                  {


                      mfield_x_sen_data.configured.amp_treshold.set_value  +=
                         (mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg
                          +
                         abs(mfield_x_sen_data.diagonesis_records.signal_history.th_gap.avg*0.01)

                         );


                     if(mfield_x_sen_data.configured.amp_treshold.set_value < mfield_x_sen_data.configured.amp_treshold.min)
                        mfield_x_sen_data.configured.amp_treshold.set_value = mfield_x_sen_data.configured.amp_treshold.min;

                      mfield_x_sen_data.configured.dfdt_treshold_max
                        = mfield_x_sen_data.configured.amp_treshold.set_value*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


                     mfield_x_sen_data.configured.dfdt_treshold_impulse= 10*mfield_x_sen_data.configured.dfdt_treshold_max;

                  }


                   memset(&mfield_x_sen_data.diagonesis_records.false_alarm,0,sizeof(mfield_x_sen_data.diagonesis_records.false_alarm));




}


/*!

    \brief mark false positive occurrence

*/

 static void mfield_x_sen_mark_false_postives(void){
        if(mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.final == STATE_1){
            mfield_x_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt++;
        }else{
            mfield_x_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt++;
       }

}



/*!

    \brief  collectes diagnosis data from sampled signal

*/

 static void mfield_x_sen_diagonesis_record(void)
 {

              if(mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed==1){
               mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value++;

               if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse==1 )
                {
                     mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation++;

                     if(
                        (mfield_x_compare(mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value,
                               (mfield_x_sen_data.module_status.projected.samples_per_half_period),
                               5/*+/- 5*/
                          )==0)
                         )
                       {
                           mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                           mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event++;

                       }else
                          if((mfield_x_compare(mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value,
                               mfield_x_sen_data.module_status.projected.samples_per_one_forth_period,
                               5 /*+/- 5*/
                          )==0)
                        ){
                          mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                          mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event++;

                         }


                 }else{

                    if(mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value
                    >
                    3*mfield_x_sen_data.module_status.projected.samples_per_one_forth_period
                    ){
                       mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event = 1;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation = 1;
                     }

                 }

         }else
         if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
         {
              //! impulse or fast rising pulse detected, launch diagnosis counter
                 mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.armed              = 1;
                 mfield_x_sen_data.configured.hif_diagonesis_records->head                                = 0;
                 mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.value              = 0;
                 mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event     = 1;
                 mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation  = 1;


             //!  new peak is expected after this
                 mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed              = 0;

         }






             mfield_x_sen_data.diagonesis_records.signal_history.amp.total_observation++;
             mfield_x_sen_data.diagonesis_records.signal_history.amp.accumulated =
                     mfield_x_sen_data.diagonesis_records.signal_history.amp.accumulated
                     +mfield_x_sen_data.module_status.current.signal_level;



             if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                    mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation++;
                    mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated =
                     mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated
                     +mfield_x_sen_data.module_status.current.signal_level;
             }


              mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation++;

             if(mfield_x_sen_data.module_status.current.change_per_time > 0)
                 mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.rising++;
             else  if(mfield_x_sen_data.module_status.current.change_per_time < 0)
             mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.falling++;


             if(mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation ==8/*Observation length*/){

             if(mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.rising > 5 /*>60%*/){
                  mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = 1;
             }else
             if(mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.falling > 5 /*>60%*/){
              mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = -1;
             }else
              mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.projected_slop.current = 0;

                 mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.total_observation = 0;
                 mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.rising            = 0;
                 mfield_x_sen_data.diagonesis_records.signal_history.time.change_counter.falling           = 0;

           }





          if(mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed==1){
               mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value++;

               if(mfield_x_peak_detected())
                {

                   mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation++;
                   if(
                      (mfield_x_compare(mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value,
                           (mfield_x_sen_data.module_status.projected.samples_per_half_period),
                           5
                      )==0)
                   )
                   {
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event++;

                   }
                   else
                   if(mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value
                           <
                      (mfield_x_sen_data.module_status.projected.samples_per_one_forth_period)
                   )
                   {

                    if(
                     mfield_x_sen_data.module_status.previous.signal_level
                      >
                     mfield_x_sen_data.diagonesis_records.signal_history.amp.max
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
                       mfield_x_sen_data.diagonesis_records.signal_history.amp.max =
                       mfield_x_sen_data.module_status.previous.signal_level;


                       mfield_x_sen_data.configured.hif_diagonesis_records->head                                = 0;
                       mfield_x_sen_data.configured.hif_diagonesis_records->mean_value                          = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value              = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event     = 1;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation  = 1;



                       if(mfield_x_sen_data.configured.hif_diagonesis_records->head < MAX_EVENT_OBSERVATION_LENGTH){
                       mfield_x_sen_data.configured.hif_diagonesis_records->buffer[mfield_x_sen_data.configured.hif_diagonesis_records->head++]
                       = mfield_x_sen_data.module_status.current.signal_level;
                       mfield_x_sen_data.configured.hif_diagonesis_records->mean_value =mfield_x_sen_data.module_status.previous.signal_level;
                       }


                    }
                   }
                 }
                 else
                 {


                    if(mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value
                    >
                    2*mfield_x_sen_data.module_status.projected.samples_per_one_forth_period
                    ){
                       //signal was inactive for 2 cycles
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed = 0;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event = 1;
                       mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation = 1;

                     }

                 }

               if(mfield_x_sen_data.configured.hif_diagonesis_records->head < MAX_EVENT_OBSERVATION_LENGTH){
               mfield_x_sen_data.configured.hif_diagonesis_records->buffer[mfield_x_sen_data.configured.hif_diagonesis_records->head++]
               = mfield_x_sen_data.module_status.current.signal_level;
               mfield_x_sen_data.configured.hif_diagonesis_records->mean_value +=mfield_x_sen_data.module_status.previous.signal_level;
               }


         }
         else
         if(mfield_x_peak_detected())
         {

                  mfield_x_sen_data.diagonesis_records.signal_history.amp.max  =
                         mfield_x_sen_data.module_status.previous.signal_level;

                   mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.armed              = 1;
                   mfield_x_sen_data.configured.hif_diagonesis_records->head                                = 0;
                   mfield_x_sen_data.configured.hif_diagonesis_records->mean_value                          = 0;

                   mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.value              = 0;
                   mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event     = 1;
                   mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation  = 1;

               mfield_x_sen_data.configured.hif_diagonesis_records->buffer[mfield_x_sen_data.configured.hif_diagonesis_records->head++]
               = mfield_x_sen_data.module_status.previous.signal_level;
               mfield_x_sen_data.configured.hif_diagonesis_records->mean_value =mfield_x_sen_data.module_status.previous.signal_level;

         }




 }


/*!

    \brief

*/

Uint8 mfield_x_sen_hif_check(void){

    Uint8 result = 0;
return result;
    if(mfield_x_sen_data.configured.hif_diagonesis_records->head < PRE_ACT_RECORD_BUFF_SIZE)
        return result;

    mfield_x_sen_data.configured.hif_diagonesis_records->mean_value/=PRE_ACT_RECORD_BUFF_SIZE;

    if(mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation > 0)
    if(mfield_x_compare(
            mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event
           ,mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation
           ,1
            )
           )
        result++;

    if(mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation > 0)
    if(mfield_x_compare(
          mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event
         ,mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation
         ,1
          )
       )
        result++;

    mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.positive_event      = 0;
    mfield_x_sen_data.diagonesis_records.signal_history.time.itoi_counter.total_observation   = 0;
    mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.positive_event      = 0;
    mfield_x_sen_data.diagonesis_records.signal_history.time.ptop_counter.total_observation   = 0;

    return result;

}
