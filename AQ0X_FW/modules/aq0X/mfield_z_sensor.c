/*! \file   mfield_z_sensor.c

    \brief  Module responsible for mfield x axis sensor element supervision


*/




#if(DEVICE == AQ0X_V0)



#include "..\..\includes\allheaders.h"
#include "..\..\includes_prv\aq0X\mfield_z_sensor_prv.h"




//! Internal mfield_z_sen module data
mfield_z_sen_data_t mfield_z_sen_data;
x_task_id           mfield_z_sen_id;





/*!

    \brief  Init function for analog module



*/

static void mfield_z_sen_init(void)
{


    memset(&mfield_z_sen_data,0,sizeof(mfield_z_sen_data));

    // Prepare notifications and events to be used later on

    mfield_z_sen_data.sv_timer_ntf.message                               = X_NTF_MFIELD_SENSOR_SV_TIMER ;
    mfield_z_sen_data.sv_timer_ntf.task_id          		         = mfield_z_sen_id;


    mfield_z_sen_data.sv_task_event.message			         = X_MSG_MFIELD_SENSOR_SV;
    mfield_z_sen_data.sv_task_event.task_id				 = mfield_z_sen_id;

    // Notifications and events sent to Algorithm module
    mfield_z_sen_data.sv_status_ntf.notify.message          	         = X_NTF_MFIELD_Z_SENSOR_SV_ALART;
    mfield_z_sen_data.sv_status_ntf.notify.task_id          		 = algorithm_id;


    // Schedule timer for periodic supervison scan
    //x_schedule_timer(&mfield_z_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));
}





/*!
    \brief  mfield sensor module task function.

    \param [in] event - system distributed event


*/

void mfield_z_sen_taskx(x_event *event)
{
    switch(event->message)
    {

        case X_MSG_MFIELD_SENSOR_SV:
        {
            mfield_z_sen_sensor_element_sv_task();
            mfield_z_sen_false_positive_correction_task();

            if(mfield_z_sen_data.sv_errors_flags.flags_all!=0){
             mfield_z_sen_data.sv_status_ntf.message.error_status_flags = mfield_z_sen_data.sv_errors_flags;
             // Send notification to Algorithm module about error
             x_send_notify(&mfield_z_sen_data.sv_status_ntf.notify);
             mfield_z_sen_data.sv_errors_flags.flags_all =0;
            }

        }break;






        default:
        {
            ASSERT(0);
        }
    }
}



/*!
    \brief  mfield module notification function.

    \param [in] notify - system distributed notification


*/

void mfield_z_sen_notifyx(x_notify *notify)
{


    switch(notify->message)
    {
        case X_NTF_INIT:
        {
            mfield_z_sen_init();
        }break;


        case X_NTF_MFIELD_Z_SENSOR_CONFIG:
        {

              // Algorithm module configuration reading
              x_notify_mfield_sen_configure * notify_configure = (x_notify_mfield_sen_configure *)notify;

              // Module configuration
              mfield_z_sen_data.configured = notify_configure->message.configured;
              mfield_z_sen_data.disturbance_record = notify_configure->message.disturbance_record;

              // Clear supervison state ( so errors will be reported if module have internal error )
              mfield_z_sen_data.sv_errors_flags.flags_all = 0;

              x_delete_timer(&mfield_z_sen_data.sv_timer_ntf);

              if(mfield_z_sen_data.configured.element_status_sv_en)
              x_schedule_timer(&mfield_z_sen_data.sv_timer_ntf,X_MS2TICK(SV_TIMER_PERIOD));

        }break;

         case X_NTF_CLR_ALARM:
        {
           // Algorithm module alarm reset request
             mfield_z_sen_data.module_status.current.state = STATE_0;
             mfield_z_sen_mark_false_postives();



        }break;


        case X_NTF_MFIELD_SENSOR_SV_TIMER:
        {
           x_send_event(&mfield_z_sen_data.sv_task_event);
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
Float32 mfield_z_sen_first_order_derivative(Int16 final_val,Int16 init_val,Float32 period){
    return (Float32)(final_val-init_val)/period;
}


/*!

    \brief  Claculates the event probablity

    \param ad_value

*/
Float32 mfield_z_sen_calc_probablity(Uint16 event,Uint16 obs){
    return (Float32)event/(Float32)obs;
}


/*!

    \brief  Claculate

    \param

*/
Bool mfield_z_zero_cross(void){

    if((mfield_z_sen_data.module_status.previous.polarity*mfield_z_sen_data.module_status.current.polarity)<=0)
         //!TODO: angular freq = 2*PI/(this_time - last_cross_time)
    return TRUE;
    else
    return FALSE;

}


/*!

    \brief  Claculate

    \param ad_value

*/


Int8 mfield_z_compare(Float32 num, Float32 given_num, Float32 tolerance){

   if(((given_num-given_num*tolerance) <= num) && (num <= (given_num + given_num*tolerance)))
       return 0;
   if(num < (given_num-given_num*tolerance))
       return -1;
   if(num > (given_num+given_num*tolerance))
       return 1;
   return -2;
}

/*!

    \brief  Claculate

    \param ad_value

*/
Bool mfield_z_observation_time_window_expired(void){

    return (mfield_z_sen_data.module_status.current.time <= OBSERVATION_TIME_WINDOW)?FALSE:TRUE;
}



/*!

    \brief  Claculate

    \param ad_value

*/
Float32 mfield_z_input_signal(void){

    Float32 result;
    Float32 current_signal_level = 0;
    //!TODO: determine the frquency type and
              mfield_z_sen_data.module_status.projected.amplitude
              = mfield_z_sen_data.module_status.current.change_per_time/
                     (mfield_z_sen_data.module_status.projected.angular_freq
                      *cos(mfield_z_sen_data.module_status.projected.angular_freq*mfield_z_sen_data.module_status.current.time)
                      );

             current_signal_level = mfield_z_sen_data.module_status.projected.amplitude
               *sin(mfield_z_sen_data.module_status.projected.angular_freq*mfield_z_sen_data.module_status.current.time);

          result =  mfield_z_compare((Float32)(mfield_z_sen_data.module_status.current.signal_level)
                  ,current_signal_level,0.05/*TODO: user configurable*/);

          if(result == 0)
              mfield_z_sen_data.module_status.projected.positive_cnt++;

          return result;

}

/*!

    \brief  Supervise the status of the mfield sensor

    \param ad_value

*/


#if(DEVICE != AQ0X)

event_report_t mfield_z_sen_get_status(Uint16 adc_value,Bool save_event)
{

    event_report_t status;
    status.fields.active_signal= TRUE;
    mfield_z_sen_data.module_status.previous = mfield_z_sen_data.module_status.current;

    mfield_z_sen_data.module_status.current.signal_level = adc_value - MFIELD_ADC_QUIESCENT_LEVEL;

   // mfield_z_sen_data.sv_status.current_signal_level.ana_val = ADC_VAL_MAPPING_FACTOR*adc_value - ADC_QUIESCENT_LEVEL;

    mfield_z_sen_data.module_status.current.polarity = mfield_z_compare(mfield_z_sen_data.module_status.current.signal_level,0,0);


    mfield_z_sen_data.module_status.current.signal_level *= mfield_z_sen_data.module_status.current.polarity;

    if(mfield_z_zero_cross()){
         mfield_z_sen_data.module_status.projected.observation_cnt=1;
         mfield_z_sen_data.module_status.projected.positive_cnt=1;
         mfield_z_sen_data.module_status.projected.zero_change_cnt=1;

         mfield_z_sen_data.module_status.current.time = 0;
         mfield_z_sen_data.module_status.current.change_per_time = mfield_z_sen_first_order_derivative(
                                            mfield_z_sen_data.module_status.current.signal_level,
                                            (-1*mfield_z_sen_data.module_status.previous.signal_level),
                                            FAST_TIMER_SAMPLING_RATE
                                            );
    }else{
        mfield_z_sen_data.module_status.current.time += FAST_TIMER_SAMPLING_RATE;

        mfield_z_sen_data.module_status.current.change_per_time = mfield_z_sen_first_order_derivative(
                                            mfield_z_sen_data.module_status.current.signal_level,
                                            mfield_z_sen_data.module_status.previous.signal_level,
                                            FAST_TIMER_SAMPLING_RATE
                                            );


         if(mfield_z_sen_data.module_status.projected.observation_cnt > RANGE_MAX_16BIT - 2){
         mfield_z_sen_data.module_status.projected.observation_cnt=1;
         mfield_z_sen_data.module_status.projected.positive_cnt=1;
         mfield_z_sen_data.module_status.projected.zero_change_cnt=1;
         }

    }

         mfield_z_sen_data.module_status.projected.observation_cnt++;
         if(mfield_z_sen_data.module_status.current.change_per_time <= 10/*tolerance value */)
                 mfield_z_sen_data.module_status.projected.zero_change_cnt++;

    if(logv.trip_active == TRUE){
   
     
    }else{
       //! For every sample, pre activation data is saved in a circular buffer at the provided storage location
   
    if(mfield_z_sen_data.disturbance_record->pre_act_data.elapsed == TRUE)
    mfield_z_sen_data.disturbance_record->pre_act_data.tail
            = (mfield_z_sen_data.disturbance_record->pre_act_data.head + 1)%PRE_ACT_RECORD_BUFF_SIZE;
    else
    if(mfield_z_sen_data.disturbance_record->pre_act_data.head == PRE_ACT_RECORD_BUFF_SIZE - 1)
    mfield_z_sen_data.disturbance_record->pre_act_data.elapsed = TRUE;


    }



    mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.all = 0;

    if(mfield_z_sen_data.module_status.current.change_per_time > mfield_z_sen_data.configured.dfdt_treshold_impulse)
        mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse = 1;


    if(mfield_z_sen_data.module_status.current.signal_level > mfield_z_sen_data.configured.amp_treshold_max)
        mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max = 1;



    mfield_z_sen_data.module_status.projected.waveform = mfield_z_input_signal();

    if(!mfield_z_observation_time_window_expired()){
        //this may never happen due to other field sources  ?
       if((mfield_z_sen_data.module_status.current.change_per_time < mfield_z_sen_data.configured.dfdt_treshold_min)
           &&
           (mfield_z_sen_data.module_status.current.time != 0)
           &&
          (mfield_z_sen_data.module_status.current.signal_level <= 10 /*TODO: user config*/)
          )
        mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_min = 1;

      if((mfield_z_sen_data.module_status.current.change_per_time  > mfield_z_sen_data.configured.dfdt_treshold_max)
       &&
       (mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse == 0)
        )
        mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max= 1;
    }



    switch(mfield_z_sen_data.module_status.current.state)
    {
        case STATE_0:
        {
            if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.all !=0){
            mfield_z_sen_data.disturbance_record->observation_cnt    = 1;
            mfield_z_sen_data.disturbance_record->event_positive_cnt = 1;

            if(save_event == TRUE){
            if((ir_event_trigger_timestamp == logv.tick))
                mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr = 0;
            else
                mfield_z_sen_data.disturbance_record->event_data.buffer.offset = mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr;
            }


            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            mfield_z_sen_data.disturbance_record->event_data.timestamp = logv.tick;



             if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
             {
                mfield_z_sen_data.disturbance_record->event_data.event_history = 0;
                //!overcurrent detected, => state transition to state 1
                mfield_z_sen_data.module_status.current.state = STATE_1;
             }else if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse)
             {
                 mfield_z_sen_data.disturbance_record->event_data.event_history =EXCEDED_DFDT_IMPULS;
                 //! impulse or fast rising pulse detected, => state transition to state 2
               mfield_z_sen_data.module_status.current.state = STATE_2;
             }else if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_min)
             {
                 mfield_z_sen_data.disturbance_record->event_data.event_history =EXCEDED_DFDT_MIN;
                 //! signal rising rate below expectation, => state transition to state 3
               mfield_z_sen_data.module_status.current.state = STATE_3;
             }else if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max)
             {
               mfield_z_sen_data.disturbance_record->event_data.event_history =EXCEDED_DFDT_MAX;
                 //! possible overcurrent fault, => state transition to state 4
               mfield_z_sen_data.module_status.current.state = STATE_4;
             }

            }else if(save_event == TRUE){
                   //TODO: may be this is not good ?
                    if(ir_event_trigger_timestamp == logv.tick)
                        mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr = 0;

                    if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
                    mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                            = mfield_z_sen_data.module_status.current.signal_level;
                    mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                            = mfield_z_sen_data.module_status.current.change_per_time;
                    mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
                    }


            }
        }break;

         case STATE_1:
        {
             mfield_z_sen_data.disturbance_record->observation_cnt++;

            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

             if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                 mfield_z_sen_data.disturbance_record->event_positive_cnt++;
             }


             mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                  ,mfield_z_sen_data.disturbance_record->observation_cnt);

             if(mfield_z_sen_data.disturbance_record->observation_cnt
                >=
                mfield_z_sen_data.configured.min_number_of_observation
               ){

                   if(mfield_z_sen_data.disturbance_record->event_data.event_probability
                    >
                    mfield_z_sen_data.configured.probablity_treshold_max
                    ){
                         mfield_z_sen_data.module_status.current.state = HOT;
                          mfield_z_sen_data.disturbance_record->post_act_data.head = 0;
                   }else{
                         mfield_z_sen_data.module_status.current.state = STATE_0;

                         mfield_z_sen_mark_false_postives();
                    }
             }

        }break;

         case STATE_2: //!impulse
        {
            mfield_z_sen_data.disturbance_record->observation_cnt++;

            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! This is expected event after sharp rise in signal
                //! strong field  detected => state transition to state 1
               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                    ,mfield_z_sen_data.disturbance_record->observation_cnt);
               mfield_z_sen_data.module_status.current.state = STATE_1;

               status.fields.state = mfield_z_sen_data.module_status.current.state;

               return status;
            }


             if(mfield_z_sen_data.module_status.projected.waveform == SINUSOID){
                //! This is expected event after sharp rise in signal
                //! waveform is sinusoid below threshold. however this indicates shouldering phenomena of arc fault current
               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                    ,mfield_z_sen_data.disturbance_record->observation_cnt);
               mfield_z_sen_data.module_status.current.state = STATE_4;
               status.fields.state = mfield_z_sen_data.module_status.current.state;
               return status;
             }



               mfield_z_sen_data.module_status.current.state = STATE_0;
               mfield_z_sen_mark_false_postives();
        }break;

         case STATE_3: //!many zeros
        {
            mfield_z_sen_data.disturbance_record->observation_cnt++;

            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

             if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){

                //!over valued level is critical the module state have to be switched to state 1

               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                    ,mfield_z_sen_data.disturbance_record->observation_cnt);
               mfield_z_sen_data.module_status.current.state = STATE_1;

               status.fields.state = mfield_z_sen_data.module_status.current.state;
               return status;
            }


             if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_impulse){
                //! This is expected event after many zero (shouldering) phenomenon
                //! => state transition to state 2
               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                    ,mfield_z_sen_data.disturbance_record->observation_cnt);
               mfield_z_sen_data.module_status.current.state = STATE_2;

               status.fields.state = mfield_z_sen_data.module_status.current.state;
               return status;
             }

             if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_min){
               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                    ,mfield_z_sen_data.disturbance_record->observation_cnt);
              status.fields.state = mfield_z_sen_data.module_status.current.state;
               return status;
             }

             if(mfield_z_observation_time_window_expired()){
               mfield_z_sen_data.module_status.current.state = STATE_0;
               status.fields.active_signal= FALSE;
               mfield_z_sen_mark_false_postives();
             }

        }break;

         case STATE_4: //!sinusoid singnal possible overcurrent fault
        {
            mfield_z_sen_data.disturbance_record->observation_cnt++;

            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max){
                //! strong field  detected state transition to state 1
               mfield_z_sen_data.disturbance_record->event_positive_cnt++;
               mfield_z_sen_data.module_status.current.state = STATE_1;
            }


              if(
                 (mfield_z_sen_data.module_status.projected.amplitude
                 >
                  mfield_z_sen_data.configured.amp_treshold_max
                 )
                 &
                 (mfield_z_sen_data.module_status.projected.waveform >= SINUSOID)
                 ||

                 mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max
                 ){

                  mfield_z_sen_data.disturbance_record->event_positive_cnt++;

                  }


             mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                   ,mfield_z_sen_data.disturbance_record->observation_cnt);

             if(mfield_z_sen_data.disturbance_record->observation_cnt
                >=
                mfield_z_sen_data.configured.min_number_of_observation
               ){
                   if(mfield_z_sen_data.disturbance_record->event_data.event_probability
                    >
                    mfield_z_sen_data.configured.probablity_treshold_max
                    ){
                         mfield_z_sen_data.module_status.current.state = HOT;
                         mfield_z_sen_data.disturbance_record->event_data.event_history++;
                         mfield_z_sen_data.disturbance_record->post_act_data.head = 0;
                   }else{
                         mfield_z_sen_data.module_status.current.state = STATE_0;
                         mfield_z_sen_mark_false_postives();
                    }
             }

        }break;

        case HOT:
        {
            mfield_z_sen_data.disturbance_record->observation_cnt++;


            if(mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr < EVENT_OBSERVATION_RECORD_LENGTH){
            mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.signal_level;
            mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr]
                    = mfield_z_sen_data.module_status.current.change_per_time;
            mfield_z_sen_data.disturbance_record->event_data.buffer.head_ptr++;
            }

            if(mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.amp_max)
                 mfield_z_sen_data.disturbance_record->event_positive_cnt++;
            else

              if(
                 (mfield_z_sen_data.module_status.projected.amplitude
                 >
                  mfield_z_sen_data.configured.amp_treshold_max
                 )
                 &
                 (mfield_z_sen_data.module_status.projected.waveform == SINUSOID)
                 ||
                 mfield_z_sen_data.disturbance_record->event_data.exceeded_threshold.bits.dfdt_max
                 )
              {
                    mfield_z_sen_data.disturbance_record->event_positive_cnt++;
              }


             mfield_z_sen_data.disturbance_record->event_data.event_probability = mfield_z_sen_calc_probablity(mfield_z_sen_data.disturbance_record->event_positive_cnt
                                                                                   ,mfield_z_sen_data.disturbance_record->observation_cnt);

             if(mfield_z_sen_data.disturbance_record->event_data.event_probability < mfield_z_sen_data.configured.probablity_treshold_min)
                 mfield_z_sen_data.module_status.current.state = STATE_0;

             if(mfield_z_sen_data.disturbance_record->observation_cnt > RANGE_MAX_16BIT - 2
                ||
                mfield_z_sen_data.disturbance_record->event_positive_cnt > RANGE_MAX_16BIT - 2
                ){
             //Prevents overflow
              mfield_z_sen_data.disturbance_record->observation_cnt    /=10;
              mfield_z_sen_data.disturbance_record->event_positive_cnt /=10;
             }

        }break;


        default:
        {
            ASSERT(0);
        }
    }


    status.fields.state = mfield_z_sen_data.module_status.current.state;
    return status;
}


#endif
/*!

    \brief  Supervises mfield sensors element by measuring sensor respond
 for different illumination level

*/

void mfield_z_sen_sensor_element_sv_task(void)
{


}

/*!

    \brief  checks the number of false positives within supervision time window

*/

void mfield_z_sen_false_positive_correction_task(void){



}



/*!

    \brief  checks the number of false positives within supervision (100ms | 5 cycle) time window

*/

void mfield_z_sen_mark_false_postives(void){
        if(mfield_z_sen_data.disturbance_record->event_data.event_history == EXCEDED_AMP_TH){
               if(
                (mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt < RANGE_MAX_8BIT)
                &
                (mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value < RANGE_MAX_32BIT)
               )
               {
                     mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt++;
                     mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value
                     +=(mfield_z_sen_data.disturbance_record->event_data.buffer.signal_level[0]-mfield_z_sen_data.configured.amp_treshold_max);
                     mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.recorded_avg
                     = mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.accumulated_value/
                             (Float32)mfield_z_sen_data.diagonesis_records.false_alarm.amp_max_exceded.cnt;
              }

             }else if(mfield_z_sen_data.disturbance_record->event_data.event_history > EXCEDED_AMP_TH){
             if(
                (mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt < RANGE_MAX_8BIT)
                &
                (mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value < RANGE_MAX_32BIT)
               )
             {

                 mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt++;
                 mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value
                 +=(mfield_z_sen_data.disturbance_record->event_data.buffer.change_per_time[0]-mfield_z_sen_data.configured.dfdt_treshold_max);
                 mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.recorded_avg
                 = mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.accumulated_value/
                    (Float32)mfield_z_sen_data.diagonesis_records.false_alarm.dfdt_max_exceded.cnt;

             }

             }

}

#endif

