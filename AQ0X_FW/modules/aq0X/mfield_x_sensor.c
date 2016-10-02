/*! \file   mfield_x_sensor.c

    \brief  Module responsible for mfield x axis sensor element supervision


*/



#include "..\..\includes\allheaders.h"
#include "..\..\includes_prv\aq0X\mfield_x_sensor_prv.h"


static void             mfield_x_sen_sensor_element_sv_task(void);
static void             mfield_x_sen_mark_false_postives(void);
static Bool             mfield_x_zero_cross(void);
static void             mfield_x_sen_diagonesis_record(void);




//! Internal mfield_x_sen module data
mfield_x_sen_data_t mfield_x_sen_data;
x_task_id           mfield_x_sen_id;





/*!

    \brief  module Init function



*/

static void mfield_x_sen_init(void)
{


    memset(&mfield_x_sen_data,0,sizeof(mfield_x_sen_data));

    // Prepare notifications and events to be used later on

    mfield_x_sen_data.sv_timer_ntf.message                               = X_NTF_MFIELD_SENSOR_SV_TIMER ;
    mfield_x_sen_data.sv_timer_ntf.task_id          		         = mfield_x_sen_id;


    mfield_x_sen_data.sv_task_event.message			     = X_MSG_MFIELD_SENSOR_SV;
    mfield_x_sen_data.sv_task_event.task_id				 = mfield_x_sen_id;

    // Notifications and events sent to Algorithm module
    mfield_x_sen_data.sv_status_ntf.notify.message          	         = X_NTF_MFIELD_X_SENSOR_SV_ALART;
    mfield_x_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;

}





/*!
    \brief  module task function.

    \param [in] event - system distributed event


*/

void mfield_x_sen_taskx(x_event *event)
{
    switch(event->message)
    {

        case X_MSG_MFIELD_SENSOR_SV:
        {
            srv_wdg_kick();
            mfield_x_sen_sensor_element_sv_task();

            if(mfield_x_sen_data.sv_errors_flags.flags_all!=0){
             mfield_x_sen_data.sv_status_ntf.message.error_status_flags = mfield_x_sen_data.sv_errors_flags;
             // Send notification to Algorithm module about error
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
    \brief  module notification function.

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

              // Algorithm module configuration reading
              x_notify_mfield_sen_configure * notify_configure = (x_notify_mfield_sen_configure *)notify;

              // Module configuration
              mfield_x_sen_data.configured = notify_configure->message.configured;
              mfield_x_sen_data.disturbance_record = notify_configure->message.disturbance_record;


              mfield_x_sen_data.module_status.projected.angular_freq             = 2*PI*mfield_x_sen_data.configured.sys_freq;
              mfield_x_sen_data.module_status.projected.samples_per_period       = (Uint16)(SCAN_FREQ/(Float32)(mfield_x_sen_data.configured.sys_freq));
              mfield_x_sen_data.module_status.projected.samples_per_period_1_2   = mfield_x_sen_data.module_status.projected.samples_per_period/2;
              mfield_x_sen_data.module_status.projected.samples_per_period_1_4   = mfield_x_sen_data.module_status.projected.samples_per_period/4;
              mfield_x_sen_data.module_status.projected.samples_per_period_1_8   = mfield_x_sen_data.module_status.projected.samples_per_period/8;
              // Clear supervison state ( so errors will be reported if module have internal error )
              mfield_x_sen_data.sv_errors_flags.flags_all = 0;

              x_delete_timer(&mfield_x_sen_data.sv_timer_ntf);

              if(mfield_x_sen_data.configured.element_status_sv_en)
              x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;

         case X_NTF_CLR_ALARM_MFIELD_SEN:
        {

           // Algorithm module alarm reset request
           //  mfield_x_sen_data.module_status.current.state = STATE_0;
             mfield_x_sen_mark_false_postives();


        }break;


        case X_NTF_MFIELD_SENSOR_SV_TIMER:
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

    \brief  Claculate first order derivative

    \param ad_value

*/
Float32 mfield_x_sen_first_order_derivative(Int16 final_val,Int16 init_val){
     return ((Float32)final_val-(Float32)init_val);
}


/*!

    \brief  Claculates the event probablity

    \param [in] event , positive event count value
    \param [in] obs   , total number of observation

*/
Float32 mfield_x_sen_calc_probablity(Uint16 event,Uint16 obs){

    return (100L*((Float32)event/(Float32)obs));
}

/*!

    \brief zero cross detector
           returns true when the monitored signal change polarity

*/
static Bool mfield_x_zero_cross(void){

    if((mfield_x_sen_data.module_status.previous.polarity*mfield_x_sen_data.module_status.current.polarity)<=0)
    return TRUE;
    else
    return FALSE;

}


/*!

    \brief

    \param

*/


Int8 mfield_x_compare(Float32 num1, Float32 num2, Float32 tolerance){

     return __COMPARE((num1 - num2),tolerance);
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

    \brief  function used to identify the measured signal waveform

*/
#if(DEVICE == AQ0X_V1)
static Float32 mfield_x_input_signal(void){

    Float32 result;
    Float32 current_signal_level = 0;
    //!TODO: determine the frquency type and
              mfield_x_sen_data.module_status.projected.amplitude
              = mfield_x_sen_data.module_status.current.change_per_time/
                     (mfield_x_sen_data.module_status.projected.angular_freq
                      *cos(mfield_x_sen_data.module_status.projected.angular_freq*mfield_x_sen_data.module_status.time*(1E-6))
                      );

             current_signal_level = mfield_x_sen_data.module_status.projected.amplitude
               *sin(mfield_x_sen_data.module_status.projected.angular_freq*mfield_x_sen_data.module_status.time*(1E-6));

          result =  mfield_x_compare((Float32)(mfield_x_sen_data.module_status.current.signal_level)
                  ,current_signal_level,0.05/*TODO: user configurable*/);

          if(result == 0)
              mfield_x_sen_data.module_status.projected.positive_cnt++;

          return result;

}
#endif

/*!

    \brief  function supervise the status of the mfield sensor

*/
event_report_t mfield_x_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;
    mfield_x_sen_data.module_status.previous = mfield_x_sen_data.module_status.current;



    mfield_x_sen_data.module_status.current.signal_level = adc_value - MFIELD_ADC_QUIESCENT_LEVEL;

   // mfield_x_sen_data.sv_status.current_signal_level.ana_val = ADC_VAL_MAPPING_FACTOR*adc_value - ADC_QUIESCENT_LEVEL;

    mfield_x_sen_data.module_status.current.polarity = __COMPARE_ZERO(mfield_x_sen_data.module_status.current.signal_level);


    mfield_x_sen_data.module_status.current.signal_level = abs(mfield_x_sen_data.module_status.current.signal_level);


    if(mfield_x_zero_cross())
    mfield_x_sen_data.module_status.current.change_per_time = mfield_x_sen_data.module_status.current.signal_level;

    else
    mfield_x_sen_data.module_status.current.change_per_time = __1ST_ORDER_DERIVATIVE(
                                            mfield_x_sen_data.module_status.current.signal_level,
                                            mfield_x_sen_data.module_status.previous.signal_level

                                            );

    if(data_non_volatile.disturbance_record.new_dr == FALSE){
    if(logv.disturbance_record.bits.wr == TRUE){
	 /*!
           req  REQ-1b:  "when a valid trip is issued,for few handres of microseconds after trip data will be to the provided location.

        */
          if(mfield_x_sen_data.disturbance_record->post_act_data.head == 0)
            mfield_x_sen_data.disturbance_record->event_data.timestamp.global_active = logv.tick-1;

    ///@{
      if(mfield_x_sen_data.disturbance_record->post_act_data.head <
        ((data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en==FALSE)?POST_ACT_RECORD_BUFF_SIZE:(POST_ACT_RECORD_BUFF_SIZE-EMU_MODE_DATA_LENGTH)))
        mfield_x_sen_data.disturbance_record->post_act_data.buffer.all[mfield_x_sen_data.disturbance_record->post_act_data.head++] = adc_value;
        else
        status.fields.post_act_data_ready = TRUE;
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
            = mfield_x_sen_data.disturbance_record->pre_act_data.head;
    else
    if(mfield_x_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    mfield_x_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;
    ///@}
    }
    }


    mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;


    if(mfield_x_sen_data.module_status.current.change_per_time > mfield_x_sen_data.configured.dfdt_treshold_impulse)
        mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse = 1;
    else
    if(mfield_x_sen_data.module_status.current.change_per_time  > mfield_x_sen_data.configured.dfdt_treshold_max)
         mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max= 1;


    if(mfield_x_sen_data.module_status.current.signal_level > mfield_x_sen_data.configured.amp_treshold.set_value)
        mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;


 switch(mfield_x_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
            mfield_x_sen_data.disturbance_record->observation_cnt    = 1;
            mfield_x_sen_data.disturbance_record->event_positive_cnt = 1;



             mfield_x_sen_data.disturbance_record->event_data.initial_amp =
                     mfield_x_sen_data.module_status.current.signal_level;

             mfield_x_sen_data.disturbance_record->event_data.timestamp.event_detected = logv.tick;

             if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
             {
                //!overcurrent detected, => state transition to state 1
                mfield_x_sen_data.module_status.current.state = STATE_1;

             }else if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
             {
                  //! impulse or fast rising pulse detected, => state transition to state 2
                 mfield_x_sen_data.module_status.current.state = STATE_2;
             }
             else if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
             {
                 if(mfield_x_sen_data.module_status.previous.change_per_time >= 0)
                 //!fast rising signal possible overcurrent fault, => state transition to state 3
               mfield_x_sen_data.module_status.current.state = STATE_3;
             }

                 mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.initial
                = mfield_x_sen_data.module_status.current.state;

            }


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


                    mfield_x_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           mfield_x_sen_data.disturbance_record->event_positive_cnt
                          ,mfield_x_sen_data.disturbance_record->observation_cnt
                           );

                   if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                        >
                       mfield_x_sen_data.configured.probablity_treshold_max
                     )
                   {
                         mfield_x_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                         mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                         = mfield_x_sen_data.module_status.current.state;

                         mfield_x_sen_data.module_status.current.state = HOT;
                         mfield_x_sen_data.disturbance_record->post_act_data.head = 0;
                         mfield_x_sen_data.disturbance_record->post_act_data.head = 0;
                         mfield_x_sen_data.disturbance_record->event_positive_cnt = 0;


                   }
                   else if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                            <
                           mfield_x_sen_data.configured.probablity_treshold_min
                           )
                         {

                             mfield_x_sen_data.module_status.current.state = STATE_0;

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
                //! signal is still increasing fast, mark as postive
               mfield_x_sen_data.disturbance_record->event_positive_cnt++;
                //!=> state transition to state 3
               mfield_x_sen_data.module_status.current.state = STATE_3;

             }else if(
                     (
                     mfield_x_sen_data.module_status.current.change_per_time>
                     mfield_x_sen_data.configured.dfdt_treshold_min
                     )
                     &&
                     (
                     mfield_x_sen_data.disturbance_record->observation_cnt <
                     mfield_x_sen_data.module_status.projected.samples_per_period_1_8
                     )
                     ){
                    //! Even though signal level is below threshold
                    //! it's still increasing, we mark as postive
                    //! algorithm module will filter the error if there is one
                    mfield_x_sen_data.disturbance_record->event_positive_cnt++;
                   }




               if(mfield_x_sen_data.disturbance_record->observation_cnt
                >=
                mfield_x_sen_data.configured.min_number_of_observation
                 ){

                    mfield_x_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           mfield_x_sen_data.disturbance_record->event_positive_cnt
                          ,mfield_x_sen_data.disturbance_record->observation_cnt
                           );

                   if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                        >
                       mfield_x_sen_data.configured.probablity_treshold_max
                     )
                   {
                          if(
                            (
                            mfield_x_sen_data.module_status.current.signal_level
                            - mfield_x_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            MFIELD_SEN_NOISE_DENSITY_LEVEL
                          ){
                                 mfield_x_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                                 mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                                 = mfield_x_sen_data.module_status.current.state;

                                 mfield_x_sen_data.module_status.current.state = HOT;
                                 mfield_x_sen_data.disturbance_record->post_act_data.head = 0;
                                 mfield_x_sen_data.disturbance_record->event_positive_cnt = 0;
                           }

                   }
                   else if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                            <
                           mfield_x_sen_data.configured.probablity_treshold_min
                           )
                         {

                             mfield_x_sen_data.module_status.current.state = STATE_0;

                             memset(&mfield_x_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(mfield_x_sen_data.disturbance_record->event_data.timestamp));

                        }
             }



        }break;


        case STATE_3: //!fast rising signal
        {
            mfield_x_sen_data.disturbance_record->observation_cnt++;

            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max){

               mfield_x_sen_data.disturbance_record->event_positive_cnt++;
            }else  if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! strong field  detected state transition to state 1
               mfield_x_sen_data.disturbance_record->event_positive_cnt++;
               mfield_x_sen_data.module_status.current.state = STATE_1;
            }



               if(mfield_x_sen_data.disturbance_record->observation_cnt
                >=
                mfield_x_sen_data.configured.min_number_of_observation
               ){

                    mfield_x_sen_data.disturbance_record->event_data.event_probability =
                           __PROBABLITY(
                           mfield_x_sen_data.disturbance_record->event_positive_cnt
                          ,mfield_x_sen_data.disturbance_record->observation_cnt
                           );


                   if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                        >
                       mfield_x_sen_data.configured.probablity_treshold_max
                     )
                   {

                          if(
                            (
                            mfield_x_sen_data.module_status.current.signal_level
                            - mfield_x_sen_data.disturbance_record->event_data.initial_amp
                            )
                            >
                            MFIELD_SEN_NOISE_DENSITY_LEVEL
                          ){
                                 mfield_x_sen_data.disturbance_record->event_data.timestamp.module_active = logv.tick;
                                 mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.mid
                                 = mfield_x_sen_data.module_status.current.state;

                                 mfield_x_sen_data.module_status.current.state = HOT;
                                 mfield_x_sen_data.disturbance_record->post_act_data.head = 0;
                                 mfield_x_sen_data.disturbance_record->event_positive_cnt = 0;
                           }


                   }
                   else if( mfield_x_sen_data.disturbance_record->event_data.event_probability
                            <
                           mfield_x_sen_data.configured.probablity_treshold_min
                           )
                         {

                             mfield_x_sen_data.module_status.current.state = STATE_0;

                             memset(&mfield_x_sen_data.disturbance_record->event_data.timestamp,0,
                                     sizeof(mfield_x_sen_data.disturbance_record->event_data.timestamp));

                         }
             }

        }break;

        case HOT:
        {

             mfield_x_sen_data.disturbance_record->observation_cnt++;

            if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 mfield_x_sen_data.disturbance_record->event_positive_cnt++;

                mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.final
                = STATE_1;
            }


             if(
                  mfield_x_sen_data.disturbance_record->observation_cnt
                  >
                  mfield_x_sen_data.module_status.projected.samples_per_period
               ){

                 if(mfield_x_sen_data.disturbance_record->event_positive_cnt == 0){
                     mfield_x_sen_data.module_status.current.state = STATE_0;
                     mfield_x_sen_mark_false_postives();
                 }
                           mfield_x_sen_data.disturbance_record->observation_cnt     = 0;
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

    \brief  Supervises mfield sensors element by measuring sensor respond

*/

static void mfield_x_sen_sensor_element_sv_task(void)
{
        mfield_x_sen_data.sv_errors_flags.flags_all = 0;
        
        hwio_execute_gpio_task(HWIO_HMC10XX_PULSE_SET);
        Delay5us(4);
        hwio_execute_gpio_task(HWIO_HMC10XX_PULSE_RSET);

        if(mfield_x_sen_data.configured.element_status_sv_en)
        x_schedule_timer(&mfield_x_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
}



/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/

static void mfield_x_sen_mark_false_postives(void){

        if(mfield_x_sen_data.disturbance_record->event_data.state_transition.history.bits.final == STATE_1){
            mfield_x_sen_data.diagonesis_records.false_alarm.amp_exceded.cnt++;
        }else{
            mfield_x_sen_data.diagonesis_records.false_alarm.dfdt_exceded.cnt++;
       }
       
}



/*!

    \brief  collectes diagnosis data from sampled

*/

static void mfield_x_sen_diagonesis_record(void)
{


        if(mfield_x_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                    mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.total_observation++;
                    mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated =
                    mfield_x_sen_data.diagonesis_records.signal_history.amp_exceded.accumulated
                    +mfield_x_sen_data.module_status.current.signal_level;
        }


}





