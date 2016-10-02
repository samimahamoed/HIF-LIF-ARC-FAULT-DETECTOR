/*! \file   algorithm.c

    \page  Module1 Algorithm module
 * 
    \brief  This is main module which controls high level functionality of the sensor


    Algorithm module is topmost module in the firmware. It coordinates all other module functions 
    and validates alart reported by other sensor element monitoring modules

    
    
*/


#include "..\..\includes\allheaders.h" 
#include "..\..\includes_prv\aq0X\algorithm_prv.h"
#include "..\..\includes_prv\aq0X\srv_hwio_prv.h"




#include "dsp.h"
#include <string.h>


#define NETIVE_ARITHMETIC                  1


static Float32 algorithm_liner_correlation(hif_diagonesis_records_t * vector,Uint16 SIZE);
static void    algorithm_data_mark_false_postives(void);

algorithm_data_t algorithm_data;
x_task_id        algorithm_id;

logv_t           logv;




/*!

    \brief  calculates mean value

*/
#if(NETIVE_ARITHMETIC)
Float32 algorithm_get_mean(Uint16 * vector,Uint16 SIZE)
{
    
    
    register int result asm("A");
    result = __builtin_clr();
    Int16 cc;

    for(cc = 0;cc<SIZE;cc++)
        result = __builtin_add(result,*(vector+cc),0);
 
    cc = __builtin_sac(result,0);


    
    Float32 mean_val = (Float32)cc/(Float32)SIZE ;
    

    return  (mean_val);

}
#else

Float32 algorithm_get_mean(Uint16 * vector,Uint16 SIZE)
{
    Int16 cc;
    long double result=0;


    for(cc = 0;cc<SIZE;cc++)
        result += *(vector+cc);

    return  result/SIZE;

}


#endif


/*!

    \brief  calculates the standard deviation of the vector

*/

int meanVal __attribute__((space(ymemory),far()));

#if(DATA_TYPE==FRACTIONAL)
Float32     algorithm_get_std_deviation(Uint16 * vector,Float32 mean_val,Uint16 SIZE)
{
    Uint16 cc;

    fractional *vect                = (fractional *)vector;

    fractional  uval                = (fractional)(mean_val);

    Int16  tmp                      = 0;

    Int32  variance                 = 0;
  
    for(cc = 0;cc < SIZE ; cc++){
          tmp       = ((*(vect+cc)) - uval);
          variance += __builtin_mulss(tmp,tmp);
    }

    Float32 result =  sqrt(((Float32)variance)/((Float32)SIZE));
  
    return result;
}
#elif(NETIVE_ARITHMETIC)
Float32 algorithm_get_std_deviation(Int16 * vector,Int16* mean_val,Uint16 SIZE)
{
    Int16 cc;
    Int16 distance;
    register Int16 result     asm("A");
    register int result_acc   asm("B");


     for(cc = 0;cc<200;cc++){

        result      = __builtin_ed(distance,
                                &vector,2,
                                &mean_val,0, &distance);

        result_acc  = __builtin_addab(result_acc,result);
      }



        result_acc  = __builtin_addab(result_acc,result);

        result_acc = __builtin_sftac(result_acc,1);
    
        cc = __builtin_sac(result_acc,0);
   
        return sqrt((Float32)cc);
}
#else

Float32 algorithm_get_std_deviation(hif_diagonesis_records_t * vector,Uint16 SIZE)
{
    Int16 cc;
    long double result=0;


    for(cc = 0;cc<SIZE;cc++)
        result += pow((*(vector->buffer+cc)-vector->mean_value),2);

    result /= SIZE;
    return sqrt(result);

}

#endif

/*!

    \brief  identify the correlation between values of a given vector and stored reference vector

*/

#if(DATA_TYPE==FRACTIONAL)
static Float32 algorithm_liner_correlation(hif_diagonesis_records_t * vector,Uint16 SIZE)
{
    Uint16 cc;

    fractional *vect1               = (fractional *)vector->buffer;

    fractional  mean_val1           = (fractional)vector->mean_value;

    fractional *vect2               = (fractional *)data_non_volatile.device_data.ref_signal.buffer;

    fractional  mean_val2           = (fractional)data_non_volatile.device_data.ref_signal.mean_value;

    fractional std_div1             = (fractional)algorithm_get_std_deviation(vector->buffer,vector->mean_value,SIZE);

    fractional std_div2             = (fractional)data_non_volatile.device_data.ref_signal.std_deviation;

    Int32  covariance                = 0;



    for(cc = 0;cc<SIZE;cc++){
        covariance +=/*((*(vect1 + cc) - mean_val1)*(*(vect2 + cc) - mean_val2));*/
                     __builtin_mulss(((*(vect1 + cc)) - mean_val1),((*(vect2 + cc)) - mean_val2));
    }


    if((std_div1 == 0)|| (std_div2 == 0))

    /*if a random variable say vect1 has standard deviation ?vect1=0, then
    cov(vect1,vect2)=E[(vect1??vect1)(vect2??vect2)]=0;
    gives the indeterminate form 0/0. in this case, it is conventional to
    define the correlation as zero.
     */
    return 0;
    else
    return ((100*((Float32)covariance))/(SIZE*__builtin_mulss(std_div1,std_div2)));

}
#else
static Float32 algorithm_liner_correlation(hif_diagonesis_records_t * vector,Uint16 SIZE)
{

    Float32 vector_std_deviation    = algorithm_get_std_deviation(vector,SIZE);

    Float32 covariance              = 0;
    Uint16 cc;

    for(cc = 0;cc<SIZE;cc++)
        covariance +=(*(vector->buffer+cc)-vector->mean_value)*
                     (*(data_non_volatile.device_data.ref_signal.buffer+cc) - data_non_volatile.device_data.ref_signal.mean_value);

    if((vector_std_deviation*data_non_volatile.device_data.ref_signal.std_deviation)== 0)

    /*if a random variable say vect1 has standard deviation ?vect1=0, then
    cov(vect1,vect2)=E[(vect1??vect1)(vect2??vect2)]=0;
    gives the indeterminate form 0/0. in this case, it is conventional to
    define the correlation as zero.
     */
    return 0;
    else
    return 100.0*(covariance/(SIZE*(vector_std_deviation*data_non_volatile.device_data.ref_signal.std_deviation)));
}

#endif



/*!
    \brief  The function calculate the relation between the given vectors and standard
            deviation of the resulting vector to determine the similarity between
            the two signals
*/
Bool algorithm_signal_proportionality(Uint16 * vector1,Uint16 * vector2,Uint16 SIZE)
{

    Float32 vector[MAX_EVENT_OBSERVATION_LENGTH];
    long double sum=0;
    Uint8 cc = 0;
  
    for(cc = 0;cc < SIZE;cc++){
        vector[cc] = (Float32)*(vector1 + cc)/(Float32)*(vector2 + cc);
        sum += vector[cc];
    }

    Float32 mean =  sum/SIZE;
    sum = 0;
    for(cc = 0;cc<SIZE;cc++)
        sum += pow((vector[cc]-mean),2);

    Float32 deviation =  sum/SIZE;

    if(deviation < 0.1 /*less than 10% deviation*/)
        return TRUE;
    else
        return FALSE;

}


/*!

    \brief  Processes configuration of modules dependent on algorithm module


    Transfers data from Non-Volatile storage structure and 
    sends the data to light_sensor/uv_sensor/ir_sensor/mfield_sensor modules.
*/



static void algorithm_configure(void)
{

    /*! Keep in sync at startup - internal supervision state and confiugration sent to modules
       Modules will send notifications in case when sensor element error is detected or too many
       false positives marked within predefined period using configured threshold value.
       Modules will not send any notification if configured value sutable for real values read from hw
   */

 
    //Configure mfield_x_sensor module

    //the following expression is drived form the use of biot-savart law
    //please see the project documentation for more detail
    

     Uint16 mfield_amp_max      = (Uint16)(((2)*(Float32)data_non_volatile.device_data.installation.system_info.rated_current*MFIELD_SEN_GAIN)/
                                   ((Float32)data_non_volatile.device_data.installation.system_info.min_distance*ADC_VAL_MAPPING_FACTOR));
                                  
     D_P(fprintf(stdout,"Config : mfield amp max :%d\n",mfield_amp_max);)
     
     
     
     Uint16 mfield_amp_sh_ckt   = (Uint16)(((2)*(Float32)data_non_volatile.device_data.installation.system_info.sh_current*MFIELD_SEN_GAIN)/
                                   ((Float32)data_non_volatile.device_data.installation.system_info.min_distance*ADC_VAL_MAPPING_FACTOR));
                                   
     D_P(fprintf(stdout,"Config : mfield amp fault current :%u\n",mfield_amp_sh_ckt);)

     //dfdt max = amp_max * angular_frequency
     Float32 mfield_dfdt_max     = ((Float32)mfield_amp_max*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq);

             mfield_dfdt_max    *=FAST_TIMER_SAMPLING_RATE;

     D_P(fprintf(stdout,"Config : mfield dfdt max :%f\n",(double)mfield_dfdt_max);)


     Float32 mfield_dfdt_impulse  = (mfield_amp_sh_ckt + mfield_amp_sh_ckt*0.2)*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq;
     D_P(fprintf(stdout,"Config : mfield dfdt impulse :%f\n",(double)mfield_dfdt_impulse);)

             mfield_dfdt_impulse  *=FAST_TIMER_SAMPLING_RATE;
 

     Uint16 treshold_min                  =
            CONFIG__DEFAULT_MIN_DFDT
            /
            (FAST_TIMER_SAMPLING_RATE*PI*data_non_volatile.device_data.installation.system_info.freq);

            treshold_min                 += (treshold_min*0.02);



    //!mfield_x_sensor module configuration
    algorithm_data.mfield_sensor_x_configure.message.configured.amp_treshold.set_value    = mfield_amp_max;
    algorithm_data.mfield_sensor_x_configure.message.configured.amp_treshold.min          = 0;
    algorithm_data.mfield_sensor_x_configure.message.configured.amp_treshold.max          
            = MFIELD_ADC_QUIESCENT_LEVEL - MFIELD_ADC_QUIESCENT_LEVEL/4;
    algorithm_data.mfield_sensor_x_configure.message.configured.dfdt_treshold_max         = mfield_dfdt_max;
    algorithm_data.mfield_sensor_x_configure.message.configured.dfdt_treshold_impulse     = mfield_dfdt_impulse;
    algorithm_data.mfield_sensor_x_configure.message.configured.sys_freq                  = data_non_volatile.device_data.installation.system_info.freq;
    algorithm_data.mfield_sensor_x_configure.message.configured.dfdt_treshold_min         = CONFIG__DEFAULT_MIN_DFDT;

    algorithm_data.mfield_sensor_x_configure.message.configured.min_number_of_observation
                   = data_non_volatile.device_data.installation.thresholds.min_number_of_observation;
    algorithm_data.mfield_sensor_x_configure.message.configured.probablity_treshold_max
                   = data_non_volatile.device_data.installation.thresholds.probablity_max;
    algorithm_data.mfield_sensor_x_configure.message.configured.probablity_treshold_min
                   = data_non_volatile.device_data.installation.thresholds.probablity_min;
    algorithm_data.mfield_sensor_x_configure.message.configured.threshold_adjust_manual
                   = data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual;
    algorithm_data.mfield_sensor_x_configure.message.configured.element_status_sv_en
                   = data_non_volatile.device_data.installation.options.bits.mfield_element_sv_en;
    algorithm_data.mfield_sensor_x_configure.message.disturbance_record = &data_non_volatile.disturbance_record.mfield_x;
  
    algorithm_data.mfield_sensor_x_configure.message.configured.hif_diagonesis_records = NULL;

    x_send_notify(&algorithm_data.mfield_sensor_x_configure.notify);

#ifdef MFIELD_Y
    //!mfield_y_sensor module configuration
    algorithm_data.mfield_sensor_y_configure.message.configured.amp_treshold.set_value    = mfield_amp_max;
    algorithm_data.mfield_sensor_y_configure.message.configured.amp_treshold.min          = 0;
    algorithm_data.mfield_sensor_y_configure.message.configured.amp_treshold.max          
                             = MFIELD_ADC_QUIESCENT_LEVEL - MFIELD_ADC_QUIESCENT_LEVEL/4;
    algorithm_data.mfield_sensor_y_configure.message.configured.dfdt_treshold_max         = mfield_dfdt_max;
    algorithm_data.mfield_sensor_y_configure.message.configured.dfdt_treshold_impulse     = mfield_dfdt_impulse;
    algorithm_data.mfield_sensor_y_configure.message.configured.dfdt_treshold_min         = CONFIG__DEFAULT_MIN_DFDT;
    algorithm_data.mfield_sensor_y_configure.message.configured.sys_freq                  = data_non_volatile.device_data.installation.system_info.freq;
    algorithm_data.mfield_sensor_y_configure.message.configured.min_number_of_observation
                   = data_non_volatile.device_data.installation.thresholds.min_number_of_observation;
    algorithm_data.mfield_sensor_y_configure.message.configured.probablity_treshold_max
                   = data_non_volatile.device_data.installation.thresholds.probablity_max;
    algorithm_data.mfield_sensor_y_configure.message.configured.probablity_treshold_min
                   = data_non_volatile.device_data.installation.thresholds.probablity_min;
    algorithm_data.mfield_sensor_y_configure.message.configured.threshold_adjust_manual
                   = data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual;
    algorithm_data.mfield_sensor_y_configure.message.configured.element_status_sv_en
                   = data_non_volatile.device_data.installation.options.bits.mfield_element_sv_en;
    algorithm_data.mfield_sensor_y_configure.message.disturbance_record = &data_non_volatile.disturbance_record.mfield_y;

    algorithm_data.mfield_sensor_y_configure.message.configured.hif_diagonesis_records = NULL;
    x_send_notify(&algorithm_data.mfield_sensor_y_configure.notify);

#endif

#if(DEVICE == AQ0X_V1)

     Float32 light_amp_max      = ((Float32)data_non_volatile.device_data.installation.thresholds.lux_level_max *LIGHT_SEN_GAIN)/
                                   (ADC_VAL_MAPPING_FACTOR);


     Float32 light_dfdt_max     = light_amp_max*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq;


     Float32 light_dfdt_impulse = (10*light_amp_max)*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq;



    //!light_sensor module configuration
    algorithm_data.light_sensor_configure.message.configured.amp_treshold_max          = light_amp_max;
    algorithm_data.light_sensor_configure.message.configured.dfdt_treshold_max         = light_dfdt_max;
    algorithm_data.light_sensor_configure.message.configured.dfdt_treshold_impulse     = light_dfdt_impulse;
    algorithm_data.light_sensor_configure.message.configured.dfdt_treshold_min         = 0;
    algorithm_data.light_sensor_configure.message.configured.min_number_of_observation
                   = data_non_volatile.device_data.installation.thresholds.min_number_of_observation;
    algorithm_data.light_sensor_configure.message.configured.probablity_treshold_max
                   = data_non_volatile.device_data.installation.thresholds.probablity_max;
    algorithm_data.light_sensor_configure.message.configured.probablity_treshold_min
                   = data_non_volatile.device_data.installation.thresholds.probablity_min;
    algorithm_data.light_sensor_configure.message.configured.threshold_adjust_manual
                   = data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual;


    algorithm_data.light_sensor_configure.message.disturbance_record = &data_non_volatile.disturbance_record.light;
    x_send_notify(&algorithm_data.light_sensor_configure.notify);


#endif

   /*
     Float32 ir_amp_max      = ((Float32)data_non_volatile.device_data.installation.thresholds.ir_level_max*IR_SEN_GAIN)/
                               (ADC_VAL_MAPPING_FACTOR);
   */                            
                               
     Float32 ir_amp_max      = (Float32)data_non_volatile.device_data.installation.thresholds.ir_level_max;

     Float32 ir_dfdt_max     = ir_amp_max*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


     Float32 ir_dfdt_impulse = 10*ir_dfdt_max;
    




    //!ir_sensor module configuration
    algorithm_data.ir_sensor_configure.message.configured.amp_treshold.set_value    = ir_amp_max;
    algorithm_data.ir_sensor_configure.message.configured.amp_treshold.min          = treshold_min;
    algorithm_data.ir_sensor_configure.message.configured.amp_treshold.max          
                                         = ADC_RESOLUTION - ADC_RESOLUTION/4;

    algorithm_data.ir_sensor_configure.message.configured.dfdt_treshold_max         = ir_dfdt_max;
    algorithm_data.ir_sensor_configure.message.configured.dfdt_treshold_impulse     = ir_dfdt_impulse;
    algorithm_data.ir_sensor_configure.message.configured.dfdt_treshold_min         = CONFIG__DEFAULT_MIN_DFDT;
    algorithm_data.ir_sensor_configure.message.configured.sys_freq                  = data_non_volatile.device_data.installation.system_info.freq;
    algorithm_data.ir_sensor_configure.message.configured.min_number_of_observation
                   = data_non_volatile.device_data.installation.thresholds.min_number_of_observation;
    algorithm_data.ir_sensor_configure.message.configured.probablity_treshold_max
                   = data_non_volatile.device_data.installation.thresholds.probablity_max;
    algorithm_data.ir_sensor_configure.message.configured.probablity_treshold_min
                   = data_non_volatile.device_data.installation.thresholds.probablity_min;
    algorithm_data.ir_sensor_configure.message.configured.threshold_adjust_manual
                   = data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual;
    algorithm_data.ir_sensor_configure.message.configured.element_status_sv_en
                   = data_non_volatile.device_data.installation.options.bits.ir_element_sv_en;

    algorithm_data.ir_sensor_configure.message.disturbance_record = &data_non_volatile.disturbance_record.ir;

    algorithm_data.ir_sensor_configure.message.configured.hif_diagonesis_records = &data_non_volatile.disturbance_record.hif_diagonesis_records;
    x_send_notify(&algorithm_data.ir_sensor_configure.notify);



    /*
     Float32 uv_amp_max      = ((Float32)data_non_volatile.device_data.installation.thresholds.uv_level_max *UV_SEN_GAIN)/
                                   (ADC_VAL_MAPPING_FACTOR);
    */
     Float32 uv_amp_max      = (Float32)data_non_volatile.device_data.installation.thresholds.uv_level_max;
 
     Float32 uv_dfdt_max     = uv_amp_max*2*PI*(Float32)data_non_volatile.device_data.installation.system_info.freq*FAST_TIMER_SAMPLING_RATE;


     Float32 uv_dfdt_impulse = (10*uv_dfdt_max);



    //!uv_sensor module configuration
    algorithm_data.uv_sensor_configure.message.configured.amp_treshold.set_value    = uv_amp_max;
    algorithm_data.uv_sensor_configure.message.configured.amp_treshold.min          = treshold_min;
    algorithm_data.uv_sensor_configure.message.configured.amp_treshold.max          
                                          = ADC_RESOLUTION- ADC_RESOLUTION/4;
    algorithm_data.uv_sensor_configure.message.configured.dfdt_treshold_max         = uv_dfdt_max;
    algorithm_data.uv_sensor_configure.message.configured.dfdt_treshold_impulse     = uv_dfdt_impulse;
    algorithm_data.uv_sensor_configure.message.configured.dfdt_treshold_min         = CONFIG__DEFAULT_MIN_DFDT;
    algorithm_data.uv_sensor_configure.message.configured.sys_freq                  = data_non_volatile.device_data.installation.system_info.freq;
    algorithm_data.uv_sensor_configure.message.configured.min_number_of_observation
                   = data_non_volatile.device_data.installation.thresholds.min_number_of_observation;
    algorithm_data.uv_sensor_configure.message.configured.probablity_treshold_max
                   = data_non_volatile.device_data.installation.thresholds.probablity_max;
    algorithm_data.uv_sensor_configure.message.configured.probablity_treshold_min
                   = data_non_volatile.device_data.installation.thresholds.probablity_min;
    algorithm_data.uv_sensor_configure.message.configured.threshold_adjust_manual
                   = data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual;
    algorithm_data.uv_sensor_configure.message.configured.element_status_sv_en
                   = data_non_volatile.device_data.installation.options.bits.uv_element_sv_en;
     algorithm_data.uv_sensor_configure.message.disturbance_record = &data_non_volatile.disturbance_record.uv;
        algorithm_data.uv_sensor_configure.message.configured.hif_diagonesis_records = NULL;
    x_send_notify(&algorithm_data.uv_sensor_configure.notify);




    algorithm_data.diagnosis.samples_per_period       = (Uint16)(SCAN_FREQ/(Float32)(data_non_volatile.device_data.installation.system_info.freq));
    logv.wr = TRUE;


    if(data_non_volatile.device_data.ref_signal.valid != 0x00CC){
        Uint16 cc;
        Uint16 zero_point_start = (algorithm_data.diagnosis.samples_per_period/4);
        Uint16 zero_point_end   = zero_point_start + 25;

        memset(&data_non_volatile.device_data.ref_signal,0,sizeof(data_non_volatile.device_data.ref_signal));

        for(cc = 0; cc < MAX_EVENT_OBSERVATION_LENGTH;cc++){

        if(!((cc > zero_point_end)&&(cc < zero_point_end)))
        data_non_volatile.device_data.ref_signal.buffer[cc]  = abs(cos(2*PI*CONFIG_DEFAULT_FREQ*FAST_TIMER_SAMPLING_RATE*cc)*ADC_RESOLUTION);

        data_non_volatile.device_data.ref_signal.mean_value +=data_non_volatile.device_data.ref_signal.buffer[cc];

        }


          data_non_volatile.device_data.ref_signal.mean_value /=MAX_EVENT_OBSERVATION_LENGTH;

          data_non_volatile.device_data.ref_signal.std_deviation = algorithm_get_std_deviation(data_non_volatile.device_data.ref_signal.buffer
                                                                                               ,data_non_volatile.device_data.ref_signal.mean_value
                                                                                               ,MAX_EVENT_OBSERVATION_LENGTH);

          data_non_volatile.device_data.ref_signal.valid = 0x0000;

    }
    //analog sampling start
    algorithm_sv_scan_timer_init();
    


}



/*!
    \brief Low level analog
*/
void   algorithm_sv_scan_timer_init(void)
{

    // Setup fast timer for analog level polling 

    T1CONbits.TON    = 0;                    // Disable Timer
    T1CON            = 0;                    // Clear the register
    T1CONbits.TCS    = 0;                    // Internal clock Fosc/4
    T1CONbits.TGATE  = 0;                    // Disable Gated Timer mode
    T1CONbits.TCKPS  = 0;                    // 1:1 prescaling

    TMR1             = 0x00;                 // Clear timer register

    PR1 = (FOSC_CPU/2)/SCAN_FREQ;

    IPC0bits.T1IP    = 0x06;                 // Set Timer1 Interrupt Priority Level
    IFS0bits.T1IF    = 0;                    // Clear the Timer1 Interrupt Flag
    IEC0bits.T1IE    = 1;                    // Enable Timer1 Interrupt Service Routine


    T1CONbits.TON    = 1;                    //Start Timer 1

}





/*!

    \brief  Algorithm initialization function 0
    
 
*/

static void algorithm_init(void)
{

    /* Prepare events to be used during runtime */
    
    // Supervision timer
    algorithm_data.supervision_timer.task_id                 = algorithm_id;
    algorithm_data.supervision_timer.message                 = X_NTF_SUPERVISION_TIMER;

   // Wake event
    algorithm_data.wake_event.task_id                        = algorithm_id;
    algorithm_data.wake_event.message                        = X_MSG_WAKE_EVENT;


    // Cfg event
    algorithm_data.config_event.task_id                      = algorithm_id;
    algorithm_data.config_event.message                      = X_MSG_CONFIGURE_EVENT;

   

    //mfield x sensor module configuration message
    algorithm_data.mfield_sensor_x_configure.notify.task_id  = mfield_x_sen_id;
    algorithm_data.mfield_sensor_x_configure.notify.message  = X_NTF_MFIELD_X_SENSOR_CONFIG;
    memset(&algorithm_data.mfield_sensor_x_configure.message,0,sizeof(algorithm_data.mfield_sensor_x_configure.message));

#ifdef MFIELD_Y
    //mfield y sensor module configuration message
    algorithm_data.mfield_sensor_y_configure.notify.task_id  = mfield_y_sen_id;
    algorithm_data.mfield_sensor_y_configure.notify.message  = X_NTF_MFIELD_Y_SENSOR_CONFIG;
    memset(&algorithm_data.mfield_sensor_y_configure.message,0,sizeof(algorithm_data.mfield_sensor_y_configure.message));
#endif


#if(DEVICE == AQ0X_V1)
    //light sensor module configuration message
    algorithm_data.light_sensor_configure.notify.task_id     = light_sen_id;
    algorithm_data.light_sensor_configure.notify.message     = X_NTF_LIGHT_SENSOR_CONFIG;
    memset(&algorithm_data.light_sensor_configure.message,0,sizeof(algorithm_data.light_sensor_configure.message));
#endif

    //ir sensor module configuration message
    algorithm_data.ir_sensor_configure.notify.task_id        = ir_sen_id;
    algorithm_data.ir_sensor_configure.notify.message        = X_NTF_IR_SENSOR_CONFIG;
    memset(&algorithm_data.ir_sensor_configure.message,0,sizeof(algorithm_data.ir_sensor_configure.message));

     //uv sensor module configuration message
    algorithm_data.uv_sensor_configure.notify.task_id        = uv_sen_id;
    algorithm_data.uv_sensor_configure.notify.message        = X_NTF_UV_SENSOR_CONFIG;
    memset(&algorithm_data.uv_sensor_configure.message,0,sizeof(algorithm_data.uv_sensor_configure.message));

    //alarm reset request
    algorithm_data.clear_alarm_ir.task_id                    = ir_sen_id;
    algorithm_data.clear_alarm_ir.message                    = X_NTF_CLR_ALARM_IR_SEN;

    algorithm_data.clear_alarm_uv.task_id                    = uv_sen_id;
    algorithm_data.clear_alarm_uv.message                    = X_NTF_CLR_ALARM_UV_SEN;


    algorithm_data.clear_alarm_mfield_x.task_id              = ir_sen_id;
    algorithm_data.clear_alarm_mfield_x.message              = X_NTF_CLR_ALARM_IR_SEN;


    
    /* Initialize device */
    // Restore pre-poweron state from non-volatile storage    
    if(nov_init() !=0)
    {
        //! nov storage fatal error
        data_non_volatile.device_data.sv_record.system_error_flags.bits.storage = 1;
        data_non_volatile.device_data.installation.is_valid = FALSE;
        nov_set_default(0);
    }
      

    // Request main task to configure the system
    x_send_event(&algorithm_data.config_event);

}




/*!

    \brief  Main processing loop for algorithm module


    Processes events by examining bitfields for various notification reasons.
   
 
*/

void algorithm_wake(Uint8 event)
{

    Bool result = FALSE;

    if((event == EVENT_SV_ALARM)&&(logv.trip_active == FALSE)){

        if(algorithm_data.modules_state_sv.ir_sen.fields.state == HOT){
        switch(data_non_volatile.device_data.installation.trip_criteria.criterion.all)
        {

                 case  TYPE_IR_MF:
                {
                      if(
                          
                           (algorithm_data.modules_state_sv.mfield_x_sen.fields.state == HOT)                         
                            &&
                           (algorithm_data.modules_state_sv.uv_sen.fields.state > STATE_0)
                           )
                                result = TRUE;
                       else


                       if(ir_sen_hif_check() > 2)
                        result = TRUE;

                }break;

                 case  TYPE_IR_MF_UV:
                {
                   if(
                        (algorithm_data.modules_state_sv.mfield_x_sen.fields.state == HOT)
                        &&
                        (algorithm_data.modules_state_sv.uv_sen.fields.state == HOT)
                        )
                            result = TRUE;
                       else if((algorithm_data.modules_state_sv.uv_sen.fields.state == HOT))
                       {

                                       if(algorithm_data.diagnosis.hif.flags.processing == 1){

                                           if(algorithm_data.diagnosis.hif.flags.ready  == TRUE){
                                                if(algorithm_data.diagnosis.hif.flags.result == TRUE)
                                                {
                                                        algorithm_data.diagnosis.hif.flags.processing = 0;
                                                        ir_record_reset(HIF);
                                                        if((algorithm_data.modules_state_sv.uv_sen.fields.state == HOT))
                                                        result = TRUE;

                                                }
                                              else
                                                {
                                                    algorithm_data.diagnosis.hif.flags.processing = 0;

                                                    ir_record_reset(HIF);

                                                    algorithm_data_mark_false_postives();


                                                }
                                          }

                                     }
                                     else
                                     {

                                               if((ir_sen_hif_check() > 0))
                                               {

                                                   algorithm_data.diagnosis.hif.all = 0;
                                                   algorithm_data.diagnosis.hif.flags.processing = 1;
                                                       //linear correlation function requires about 200us to return,
                                                       //therefore,low priority function will be used to execute this procedure
                                                        algorithm_message_arrived_ntf_isr(IRQ_SOURCE_HIF_EVENT);


                                                        //TODO:clear all timers
                                               }

                                    }
                      }else
                          algorithm_data_mark_false_postives();


                }break;


                 default:
                {

                      if(
                        (algorithm_data.modules_state_sv.mfield_x_sen.fields.state == HOT)
                        &&
                        (algorithm_data.modules_state_sv.uv_sen.fields.state == HOT)
                        )
                            result = TRUE;
                       else if((algorithm_data.modules_state_sv.uv_sen.fields.state == HOT))
                       {

                           algorithm_data.diagnosis.hif_checklist.all = ir_sen_hif_check();

                           if((algorithm_data.diagnosis.hif_checklist.all > 0))
                           {

                               IRQ_CTX_T   irq_ctx;
                               IRQ_DISABLE(irq_ctx);
                                 Float32 correlation_result = algorithm_liner_correlation(&data_non_volatile.disturbance_record.hif_diagonesis_records,MAX_EVENT_OBSERVATION_LENGTH);
                               IRQ_ENABLE(irq_ctx);
                               
                               //linear correlation function requires about 236us to return,with 60MIP
                               //therefore,we update global tick by 2 sample time
                               logv.tick +=2;

                                 if(correlation_result >= 80){
                                    //! more than 80% correlation is considered as strong similarity
                                     result = TRUE;
                                 }
                                 else
                                 if
                                 (
                                 (correlation_result > 50)
                                 &&
                                 algorithm_data.diagnosis.hif_checklist.bits.itoi
                                 &&
                                 algorithm_data.diagnosis.hif_checklist.bits.ptop
                                 )
                                {
                                  result = TRUE;
                                }
                                 else
                                algorithm_data_mark_false_postives();

                               data_non_volatile.disturbance_record.hif_diagonesis_records.head = 0;
                               data_non_volatile.disturbance_record.hif_diagonesis_records.mean_value = 0;

                           }

                                    
                      }else if(algorithm_data.modules_state_sv.mfield_x_sen.fields.state == STATE_0){
                            algorithm_data_mark_false_postives();
                      } 
                       

                }break;

        }


        }else if(algorithm_data.modules_state_sv.mfield_x_sen.fields.state == STATE_0){
            if(algorithm_data.modules_state_sv.uv_sen.fields.state == HOT)
            algorithm_data_mark_false_postives();
        }
        
        
        


        if(result == TRUE){
            hwio_execute_gpio_task(HWIO_ALARM_ON);
            logv.trip_active   = TRUE;
             logv.disturbance_record.bits.wr = TRUE;
            /*!
              After a valid trip, disturbance recorder data will be will be saved to
              the memory address allocated for this purpose.  The process will take
              some time depending on the size of the data. Hence, delay counter will
              be armed so as to transfer the data to non volatile memory when ready.
             */
            memset(&algorithm_data.diagnosis.false_alarm,0,sizeof(algorithm_data.diagnosis.false_alarm));
            logv.sv_delay.set  = TRUE;
            logv.sv_delay.cnt  = DR_RECHECK_TIME;
            logv.sv_delay.type = DR_RECHECK;

        }
        
    }else if(event == EVENT_CLEARED){

          hwio_execute_gpio_task(HWIO_ALARM_OFF);
          logv.trip_active   = FALSE;
          
    }else if(event == DR_RECHECK){
                    
            if(
              (algorithm_data.modules_state_sv.ir_sen.fields.post_act_data_ready == TRUE)
             ){

           /*!disturbance record data ready, low priority function will be used,
           to transfer the data to non volatile memory.
            */
          
           data_non_volatile.disturbance_record.new_dr = TRUE;
           logv.disturbance_record.bits.wr = FALSE;
	   algorithm_message_arrived_ntf_isr(IRQ_DR_READY);

           //!reset emulation trip for comunication purpose
           if(srv_hwio_data.emulation_mode.enabled.all != 0)
               srv_hwio_data.emulation_mode.enabled.all = 0;


       }
       else
          ASSERT(0);

            logv.sv_delay.set  = FALSE;
            logv.sv_delay.cnt  = 0;
            logv.sv_delay.type = 0;

       

    }else if(event == 0/*internal event*/){

#if(DEVICE == AQ0X_V1)
    if(data_non_volatile.device_data.sv_record.system_error_flags.bits.power_supply == TRUE)
       nov_store();

     //!TODO: other sv alart event handler
#endif
      
    }

}


/*!

    \brief mark false positive occurrence

*/

 static void algorithm_data_mark_false_postives(void){

   
        if(algorithm_data.modules_state_sv.ir_sen.fields.state == HOT){
            if(algorithm_data.diagnosis.false_alarm.ir.armed == 0){
              algorithm_data.diagnosis.false_alarm.ir.delay = algorithm_data.diagnosis.samples_per_period;
              algorithm_data.diagnosis.false_alarm.ir.armed = 1;
              algorithm_data.diagnosis.false_alarm.ir.cnt = 0;
            }else
            if(algorithm_data.diagnosis.false_alarm.ir.delay == 0){
              algorithm_data.diagnosis.false_alarm.ir.cnt++;
              algorithm_data.diagnosis.false_alarm.ir.delay = algorithm_data.diagnosis.samples_per_period;

              if(algorithm_data.diagnosis.false_alarm.ir.cnt > 2/*3 cycles*/){
               //!persistent false positive for 3 cycles
               x_send_notify(&algorithm_data.clear_alarm_ir);
               algorithm_data.diagnosis.false_alarm.ir.cnt = 0;
               }
            }
        }

        if(algorithm_data.modules_state_sv.uv_sen.fields.state == HOT){
            if(algorithm_data.diagnosis.false_alarm.uv.armed == 0){
              algorithm_data.diagnosis.false_alarm.uv.delay  = algorithm_data.diagnosis.samples_per_period;
               algorithm_data.diagnosis.false_alarm.uv.armed = 1;
               algorithm_data.diagnosis.false_alarm.uv.cnt   = 0;
            }else
            if(algorithm_data.diagnosis.false_alarm.uv.delay == 0){
              algorithm_data.diagnosis.false_alarm.uv.cnt++;
              algorithm_data.diagnosis.false_alarm.uv.delay = algorithm_data.diagnosis.samples_per_period;

              if(algorithm_data.diagnosis.false_alarm.uv.cnt > 2/*3 cycles*/){
              //!persistent false positive for 3 cycles
              x_send_notify(&algorithm_data.clear_alarm_uv);
              algorithm_data.diagnosis.false_alarm.uv.cnt=0;
              }

            }
        }

       
}



       

/*!
    \brief  copy serially received data to given memory address

*/

void msg_to_mem(void* dest,Uint8* msg,Uint16 size,Uint8 base){

 
        Uint16 cc   = 0;
        Uint16 var  = 0;
        if(base == 8){
        Uint8* dst8 = (Uint8*)dest;
        for (var = 0; var < size; ++var) {	       
            *(dst8+var)= serial_make8(msg,cc);
            cc +=2;
        }
        }
        else if(base == 16){
	    Uint16* dst8 = (Uint16*)dest;    
	        
        for (var = 0; var < size; ++var) {	       
            *(dst8+var)= serial_make16(msg,cc);
            cc +=4;
        }
        }

}



/*!
    \brief  Function processing serial requests


    Note: Processes mode selection and direct requests to helper functions

*/

static void algorithm_cmd_handler(void)
{
    Uint16 cmd;
    Uint8 * arg_buff;
    Uint16  arg_cnt;


    if(serial_get_command(&cmd,&arg_buff,&arg_cnt) == 0)
    {

        if(arg_cnt < 4/*message structure minimum length*/)
            //  commands use at least 6 characters
            return;

        memset(algorithm_data.serial_msg.arg,0,sizeof(algorithm_data.serial_msg.arg));

        algorithm_data.serial_msg.attachment.location   = NULL;
        algorithm_data.serial_msg.attachment.offset     = 0;
        algorithm_data.serial_msg.attachment.length     = 0;


        algorithm_data.serial_msg.cmd                   = cmd;
        algorithm_data.serial_msg.arg[0]                = (Uint16)serial_make8((Uint8 *)arg_buff,0);
        algorithm_data.serial_msg.arg[1]                = (Uint16)serial_make8((Uint8 *)arg_buff,2);


#if (DEVICE == AQ0X_V0)
        if((cmd == CMD_DEVICE_ADDRESS)){
           data_non_volatile.device_data.installation.device_address = (Uint16)algorithm_data.serial_msg.arg[1];
           algorithm_data.serial_msg.arg[2] = ACK;
           algorithm_data.serial_msg.arg_cnt = 3;
           
           serial_send_response(algorithm_data.serial_msg);
           serial_get_command_ack();
           
           return;
        }
#endif




        if(algorithm_data.serial_msg.arg[1/*address*/] != data_non_volatile.device_data.installation.device_address){
            serial_get_command_ack();
           return;
        }
        switch(cmd)
        {

          {    
                 case  CMD_INSTALLATION_DITAILS:
                {

                         if(arg_cnt == 28 /*expected argument length*/)
	                     {
                            Uint16 directive_msg =  serial_make16(arg_buff,4);
                            if(directive_msg&0x0001){data_non_volatile.device_data.installation.system_info.rated_current= serial_make16(arg_buff,12);}
                            
                            if(directive_msg&0x0002){data_non_volatile.device_data.installation.system_info.sh_current= serial_make16(arg_buff,16);}
                            
                                           
                            if(directive_msg&0x0004){data_non_volatile.device_data.installation.system_info.freq= serial_make16(arg_buff,20);}
                            
                         
                            if(directive_msg&0x0008){data_non_volatile.device_data.installation.system_info.min_distance= serial_make16(arg_buff,24);}  
                            
                            algorithm_data.serial_msg.arg[2] = ACK;
                        
                            
                          }else{
                            algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                          }
                            algorithm_data.serial_msg.arg_cnt = 3;

                }break;
        }          
                            

                case  CMD_SIGNAL_THRESHOLDS:
                {
                        if(arg_cnt == 20)
	                     {
                            Uint16 directive_msg =  serial_make16(arg_buff,4);
                            
                            if(directive_msg&0x0001)data_non_volatile.device_data.installation.thresholds.ir_level_max = serial_make16(arg_buff,12);
                            
                            if(directive_msg&0x0002)data_non_volatile.device_data.installation.thresholds.uv_level_max = serial_make16(arg_buff,16);
                            
                            algorithm_data.serial_msg.arg[2] = ACK;
                          
                          }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;
                }break;




                case  CMD_EVENT_ANALYSIS:
                {
                          if(arg_cnt == 32)
	                      {
                             Uint16 directive_msg =  serial_make16(arg_buff,4);
                          
                             if(directive_msg&0x0001)data_non_volatile.device_data.installation.thresholds.min_number_of_observation = serial_make16(arg_buff,12);
                          
                             if(directive_msg&0x0002)data_non_volatile.device_data.installation.thresholds.probablity_max            = serial_make16(arg_buff,16);
                          
                             if(directive_msg&0x0004)data_non_volatile.device_data.installation.thresholds.probablity_min            = serial_make16(arg_buff,20);
                          
                             if(directive_msg&0x0008){

                                 data_non_volatile.device_data.installation.trip_criteria.active_modules.all  = (Uint8)serial_make16(arg_buff,24);
	                             //TODO: set active_modules, and criterial 
                               // data_non_volatile.device_data.installation.trip_criteria.all                  = (Uint8)serial_make16(arg_buff,28);
                             
                             }

                             algorithm_data.serial_msg.arg[2] = ACK;
                          
                          }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;

                }break;


          

            
                 case  CMD_RUN_TIME_OPTIONS:
                {
                         if(arg_cnt == 16)
	                     {
                             data_non_volatile.device_data.installation.options.all = serial_make16(arg_buff,12);
                             algorithm_data.serial_msg.arg[2] = ACK;
                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;
                            

                }break;





                 case CMD_COMMIT:
                {
                         if(arg_cnt == 4)
	                     {
                             nov_store();
                           
                        // Request main task to configure the system
                             x_send_event(&algorithm_data.config_event);
                             algorithm_data.serial_msg.arg[2] = ACK;
                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;

                }break;




                case CMD_FACTORY_DEFAULT:
                {
                         if(arg_cnt == 4)
	                     {
                            nov_set_default(NOV_OVERWRITE_ALL);
                             algorithm_data.serial_msg.arg[2] = ACK;
                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;
                }break;



                 case  CMD_LOAD_IR_DATA:
                {
                    //![xxxx:xxCH|offset|length|00000000000000... : crc]
                    Uint16 offset = serial_make16(arg_buff,4);

                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)
                        data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en = TRUE;
                    

                     if(((offset+length) <= EMU_MODE_DATA_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {

                       msg_to_mem((srv_hwio_data.emulation_mode.data[ANA_INDEX_IR] + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2] = ACK;

                         algorithm_data.serial_msg.arg[3] = offset+length;

                         algorithm_data.serial_msg.arg_cnt = 4;
                         
                   

                     }else{

                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                         algorithm_data.serial_msg.arg_cnt = 3;
                     }

                }break;


                case  CMD_LOAD_HIF_REFERENCE_DATA:
                {
                    //![xxxx:xxCH|offset|length|00000000000000... : crc]
                    Uint16 offset = serial_make16(arg_buff,4);

                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)
                        data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en = TRUE;


                     if(((offset+length) <= MAX_EVENT_OBSERVATION_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {

                         msg_to_mem((data_non_volatile.device_data.ref_signal.buffer + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2] = ACK;

                         algorithm_data.serial_msg.arg[3] = offset+length;

                         algorithm_data.serial_msg.arg_cnt = 4;


                         if(algorithm_data.serial_msg.arg[3] == MAX_EVENT_OBSERVATION_LENGTH*sizeof(Uint16)){
                             Uint16 cc;
                             for(cc = 0;cc < MAX_EVENT_OBSERVATION_LENGTH;cc++)
                                data_non_volatile.device_data.ref_signal.mean_value +=data_non_volatile.device_data.ref_signal.buffer[cc];

                             data_non_volatile.device_data.ref_signal.mean_value    /=MAX_EVENT_OBSERVATION_LENGTH;

                             data_non_volatile.device_data.ref_signal.std_deviation = algorithm_get_std_deviation(data_non_volatile.device_data.ref_signal.buffer
                                                                                      ,data_non_volatile.device_data.ref_signal.mean_value
                                                                                      ,MAX_EVENT_OBSERVATION_LENGTH);

                         }



                     }else{

                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                         algorithm_data.serial_msg.arg_cnt = 3;
                     }

                }break;
                


                  case  CMD_LOAD_UV_DATA:
                {
                    //![xxxx:ChAdAk|offset|length|00000000000000 : crc]
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)

                        data_non_volatile.disturbance_record.uv.post_act_data.emu_data_en = TRUE;


                    
                     if(((offset+length) <= EMU_MODE_DATA_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {

                       msg_to_mem((srv_hwio_data.emulation_mode.data[ANA_INDEX_UV] + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2] = ACK;
                         algorithm_data.serial_msg.arg[3] = offset+length;
                         algorithm_data.serial_msg.arg_cnt = 4;
                      }else{
                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt = 3;
                      }

                }break;


#if(DEVICE == AQ0X_V1)
                 case  CMD_EMU_LIGHT_DATA:
                {
                    //![xxxx:CHSN|offset|length|00000000000000... : crc]
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)
                        data_non_volatile.disturbance_record.light.post_act_data.emu_data_en = TRUE;


            
                     if(((offset+length) <= EMU_MODE_DATA_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {

                       msg_to_mem((srv_hwio_data.emulation_mode.data[ANA_INDEX_LIGHT] + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2] = ACK;
                         algorithm_data.serial_msg.arg[3] = offset+length;
                         algorithm_data.serial_msg.arg_cnt = 4;
                     }else{
                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt = 3;
                     }
                }break;

#endif

                 case  CMD_LOAD_MAGNETIC_FIELD_X_DATA:
                {

                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)
                        data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en = TRUE;


               
                     if(((offset+length) <= EMU_MODE_DATA_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {

                       msg_to_mem((srv_hwio_data.emulation_mode.data[ANA_INDEX_MFIELD_X] + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2] = ACK;
                         algorithm_data.serial_msg.arg[3] = offset+length;
                         algorithm_data.serial_msg.arg_cnt = 4;
                     }else{
                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt = 3;
                     }
                }break;


#ifdef MFIELD_Y
                 case  CMD_LOAD_MAGNETIC_FIELD_Y_DATA:
                {

                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0)
                        data_non_volatile.disturbance_record.mfield_y.post_act_data.emu_data_en = TRUE;


  
                     if(((offset+length) <= EMU_MODE_DATA_LENGTH*sizeof(Uint16)) && (length%sizeof(Uint16)==0))
	                  {


                       msg_to_mem((srv_hwio_data.emulation_mode.data[ANA_INDEX_MFIELD_Y] + offset/sizeof(Uint16)),arg_buff+12,length/sizeof(Uint16),16);

                         algorithm_data.serial_msg.arg[2]  = ACK;
                         algorithm_data.serial_msg.arg[3]  = offset+length;
                         algorithm_data.serial_msg.arg_cnt = 4;
                     }else{
                         algorithm_data.serial_msg.arg[2] = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt = 3;
                     }
                }break;

#endif
                 case  CMD_EMU_MODE_SET:
                {
                        if(arg_cnt == 4){
	                        
                        
                        algorithm_data.serial_msg.arg[2]    = ACK;
                        algorithm_data.serial_msg.arg_cnt   = 3;

                         serial_send_response(algorithm_data.serial_msg);
                         serial_get_command_ack();
                         
                        srv_hwio_data.emulation_mode.enabled.bits.ir       = (data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en)?1:0;
                        srv_hwio_data.emulation_mode.enabled.bits.uv       = (data_non_volatile.disturbance_record.uv.post_act_data.emu_data_en)?1:0;
                        srv_hwio_data.emulation_mode.enabled.bits.mfield_x = (data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en)?1:0;
                        #ifdef MFIELD_Y
                        srv_hwio_data.emulation_mode.enabled.bits.mfield_y = (data_non_volatile.disturbance_record.mfield_y.post_act_data.emu_data_en)?1:0;
                        #endif
                        int cc;
                          for(cc = 0; cc < ANALOG_HW_CHANNELS;cc++)
                          srv_hwio_data.emulation_mode.head_ptr[cc] = 0;
                        
                          logv.data_tx_on_progress.all        = 0;
                          
                        return;

                        }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;

                }break;

                 case  CMD_EMU_MODE_RESET:
                {

                        if(arg_cnt == 4){
                        algorithm_data.serial_msg.arg[2] = ACK;
                        algorithm_data.serial_msg.arg_cnt = 3;

                        serial_send_response(algorithm_data.serial_msg);
                        serial_get_command_ack();


                        srv_hwio_data.emulation_mode.enabled.bits.ir       = 0;
                        srv_hwio_data.emulation_mode.enabled.bits.uv       = 0;
                        srv_hwio_data.emulation_mode.enabled.bits.mfield_x = 0;
                        srv_hwio_data.emulation_mode.enabled.bits.mfield_y = 0;

                        int cc;
                          for(cc = 0; cc < ANALOG_HW_CHANNELS;cc++)
                          srv_hwio_data.emulation_mode.head_ptr[cc] = 0;

                        return;
                           
                        }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;
                }break;


                case  CMD_CLEAR_EMU_DATA:
                {

                          if(arg_cnt == 4){
                          srv_hwio_data.emulation_mode.enabled.bits.ir       = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.uv       = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.mfield_x = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.mfield_y = 0;
                          int cc;
                          for(cc = 0; cc < ANALOG_HW_CHANNELS;cc++)
                          srv_hwio_data.emulation_mode.head_ptr[cc] = 0;

                          data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en = FALSE;
                          #ifdef MFIELD_Y
                          data_non_volatile.disturbance_record.mfield_y.post_act_data.emu_data_en = FALSE;
                          #endif
                          data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en       = FALSE;
                          data_non_volatile.disturbance_record.uv.post_act_data.emu_data_en       = FALSE;
                             
                          algorithm_data.serial_msg.arg[2] = ACK;
                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;

                }break;




                 case  CMD_DEVICE_DATA:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);
                   
                   if(length &&((offset+length) <= sizeof(data_non_volatile.device_data))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.device_data;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;


                 case  CMD_IR_CURRENT_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0){
                         /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.ir   = 1;
                    }else if(offset+length >= sizeof(data_non_volatile.disturbance_record.ir.pre_act_data)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.ir_l = 1;
                    }





                   if(length && ((offset+length) <= sizeof(data_non_volatile.disturbance_record.ir.pre_act_data))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.ir.pre_act_data;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;


                 case  CMD_UV_CURRENT_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);


                    if(offset == 0){
                        /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.uv   = 1;
                    }else if((offset+length) >= sizeof(data_non_volatile.disturbance_record.uv.pre_act_data)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.uv_l = 1;
                    }

                   if(length && ((offset+length) <= sizeof(data_non_volatile.disturbance_record.uv.pre_act_data))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.uv.pre_act_data;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }
                }break;




                 case  CMD_MFIELD_X_CURRENT_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);


                    if(offset == 0){
                        /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all             = 0;
                        logv.data_tx_on_progress.bits.mfield_x   = 1;
                    }else if((offset+length) >= sizeof(data_non_volatile.disturbance_record.mfield_x.pre_act_data)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.mfield_x_l = 1;
                    }


            

                   if(length &&((offset+length) <= sizeof(data_non_volatile.disturbance_record.mfield_x.pre_act_data))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.mfield_x.pre_act_data;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;

#ifdef MFIELD_Y
                  case  CMD_MFIELD_Y_CURRENT_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);



                    if(offset == 0){
                        /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all             = 0;
                        logv.data_tx_on_progress.bits.mfield_y   = 1;
                    }else if((offset+length) >= sizeof(data_non_volatile.disturbance_record.mfield_y.pre_act_data)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.mfield_y_l = 1;
                    }


                   if(length &&((offset+length) <= sizeof(data_non_volatile.disturbance_record.mfield_y.pre_act_data))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.mfield_y.pre_act_data;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;

#endif
                 case  CMD_HIF_DIAGNOSIS_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);




                     if(length && ((offset+length) <= sizeof(data_non_volatile.disturbance_record.hif_diagonesis_records))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.hif_diagonesis_records;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;



                 case  CMD_IR_DISTURBANCE_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                     if(offset == 0){
                         /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.ir   = 1;
                    }else if(offset+length >= sizeof(data_non_volatile.disturbance_record.ir)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.ir_l = 1;
                    }

                   if(length &&((offset+length) <= sizeof(data_non_volatile.disturbance_record.ir))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.ir;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;

                 case  CMD_UV_DISTURBANCE_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);


                   if(offset == 0){
                         /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.uv   = 1;
                    }else if(offset+length >= sizeof(data_non_volatile.disturbance_record.uv)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.uv_l = 1;
                    }


                   if(length && ((offset+length) <= sizeof(data_non_volatile.disturbance_record.uv))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.uv;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;



                case  CMD_MFIELD_X_DISTURBANCE_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);


                    if(offset == 0){
                         /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.mfield_x   = 1;
                    }else if(offset+length >= sizeof(data_non_volatile.disturbance_record.mfield_x)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.mfield_x_l = 1;
                    }


                   if(length &&((offset+length) <= sizeof(data_non_volatile.disturbance_record.mfield_x))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.mfield_x;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;

#ifdef MFIELD_Y
                case  CMD_MFIELD_Y_DISTURBANCE_RECORD:
                {
                    Uint16 offset = serial_make16(arg_buff,4);
                    Uint16 length = serial_make16(arg_buff,8);

                    if(offset == 0){
                         /*
                         set data transmission flange so that the current data will not be
                         overwritten for the duration of this and subsequent transfer
                       */
                        logv.data_tx_on_progress.all       = 0;
                        logv.data_tx_on_progress.bits.mfield_y   = 1;
                    }else if(offset+length >= sizeof(data_non_volatile.disturbance_record.mfield_y)){
                        /*
                         set last data transmission flag so that serial module clears
                         overwrite protection flag upon completion of transmission
                       */
                        logv.data_tx_on_progress.bits.mfield_y_l = 1;
                    }



                   if(length &&((offset+length) <= sizeof(data_non_volatile.disturbance_record.mfield_y))){
                       algorithm_data.serial_msg.arg[2]                 = ACK;
                       algorithm_data.serial_msg.arg[3]                 = offset+length;

                       algorithm_data.serial_msg.attachment.location    = (void *)&data_non_volatile.disturbance_record.mfield_y;
                       algorithm_data.serial_msg.attachment.offset      = offset;
                       algorithm_data.serial_msg.attachment.length      = length;
                     }else{
                         algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                     }

                }break;

#endif
                 case   CMD_DEVICE_RESET:
                {

                          if(arg_cnt == 4)
	                 {
                           algorithm_data.serial_msg.arg[2] = ACK;
                           algorithm_data.serial_msg.arg_cnt = 3;
                           serial_send_response(algorithm_data.serial_msg);
                           serial_get_command_ack();
                           
                           __asm__ ("reset");

                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;
                }break;

               

                 case  CMD_CLEAR_EEPROM:
                {
                      //! the action only clears disturbance records and supervision flags
                      if(arg_cnt == 4)
	                 {
		             memset(&data_non_volatile.disturbance_record,0,sizeof(data_non_volatile.disturbance_record));
                             memset(&data_non_volatile.device_data.sv_record,0,sizeof(data_non_volatile.device_data.sv_record));
                        
                             nov_store();

                          srv_hwio_data.emulation_mode.enabled.bits.ir       = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.uv       = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.mfield_x = 0;
                          srv_hwio_data.emulation_mode.enabled.bits.mfield_y = 0;

                          data_non_volatile.disturbance_record.mfield_x.post_act_data.emu_data_en = FALSE;
                          #ifdef MFIELD_Y
                          data_non_volatile.disturbance_record.mfield_y.post_act_data.emu_data_en = FALSE;
                          #endif
                          data_non_volatile.disturbance_record.ir.post_act_data.emu_data_en       = FALSE;
                          data_non_volatile.disturbance_record.uv.post_act_data.emu_data_en       = FALSE;

                          

                          memset(&logv.disturbance_record,0,sizeof(logv.disturbance_record));
                          memset(&logv.sv_delay,0,sizeof(logv.sv_delay));

                          int cc;
                          for(cc = 0; cc < ANALOG_HW_CHANNELS;cc++)
                          srv_hwio_data.emulation_mode.head_ptr[cc] = 0;
                          
                             algorithm_data.serial_msg.arg[2] = ACK;
                         }else
                             algorithm_data.serial_msg.arg[2] = CMD_ERROR;

                             algorithm_data.serial_msg.arg_cnt = 3;






                }break;


                 case  CMD_SUPERVISION_STATUS:
                {
                    //TODO: respond error flags ..

                }break;
                


                default:
	        {
	                 algorithm_data.serial_msg.arg[2]               = CMD_ERROR;
                         algorithm_data.serial_msg.arg_cnt              = 3;
                }break;

        }
        




               serial_send_response(algorithm_data.serial_msg);
               serial_get_command_ack();


     }
 
}







/*!

    \brief  Algorithm task function


    Processes incoming events.

    
 
*/


void algorithm_taskx(x_event *event)
{
    switch(event->message)
    {


        case X_MSG_IRQ_SYNC:
        {
             srv_wdg_kick();
            if(algorithm_data.irq_src.fields.external != 0){
                    if(algorithm_data.irq_src.all == IRQ_SOURCE_SERIAL)
                    algorithm_cmd_handler();

                  algorithm_data.irq_src.fields.external = 0;
            }



            if(algorithm_data.irq_src.fields.internal != 0){
                    if(algorithm_data.irq_src.all == IRQ_SOURCE_HIF_EVENT){
                        Float32 result = algorithm_liner_correlation(&data_non_volatile.disturbance_record.hif_diagonesis_records,MAX_EVENT_OBSERVATION_LENGTH);
                       //if(algorithm_liner_correlation(&data_non_volatile.disturbance_record.hif_diagonesis_records,MAX_EVENT_OBSERVATION_LENGTH) > 80){
                         if(result >= 80){
                            //! more than 80% correlation is considered as strong similarity
                            algorithm_data.diagnosis.hif.flags.result = 1;
                            algorithm_data.diagnosis.hif.flags.ready  = 1;
                         }else{
                            algorithm_data.diagnosis.hif.flags.result = 0;
                            algorithm_data.diagnosis.hif.flags.ready  = 1;
                            memset(&data_non_volatile.disturbance_record.hif_diagonesis_records,0,sizeof(data_non_volatile.disturbance_record.hif_diagonesis_records));
                         }
                    }
                    else
                    if(algorithm_data.irq_src.all == IRQ_DR_READY)
                    {
                        
                        nov_store();
                        logv.disturbance_record.bits.ready = 1;
                    }

                    algorithm_data.irq_src.fields.internal = 0;
         }
        }break;



        case X_MSG_WAKE_EVENT:
        {
              if(algorithm_data.diagnosis.false_alarm.ir.cnt++ > FP_TOLERANCE)
              x_send_notify(&algorithm_data.clear_alarm_ir);

              if(algorithm_data.diagnosis.false_alarm.uv.cnt++ > FP_TOLERANCE)
              x_send_notify(&algorithm_data.clear_alarm_uv);

              if(algorithm_data.diagnosis.false_alarm.mfield_x.cnt++ > FP_TOLERANCE)
              x_send_notify(&algorithm_data.clear_alarm_mfield_x);




        }break;


        case X_MSG_CONFIGURE_EVENT:
        {
            algorithm_configure();
            data_non_volatile.device_data.installation.is_valid = TRUE;

#if(0)
          x_delete_timer(&algorithm_data.supervision_timer);
          x_schedule_timer(&algorithm_data.supervision_timer,X_MS2TICK(SV_TIMER_PERIOD));
#endif
        }break;
    
        default:
        {
            ASSERT(0);
        }break;
    
    };

}



/*!
    \brief  Algorithm module notification function.
    
    \param [in] notify - system distributed notification


    Services initialization notifications.
    Collects notifications from all other modules and internal OS timers (data is copied to internal data structures).
    Such operation triggers sending wake event to main task to process notification data.
    


*/



void algorithm_notifyx(x_notify *notify)
{

    switch(notify->message)
    {
        // System standrd messages 

        case X_NTF_INIT:
        {
            // Module intialization    
            algorithm_init();            
        }break;


        case X_NTF_SUPERVISION_TIMER:
        {
           x_send_event(&algorithm_data.wake_event);
        }break;

        case  X_NTF_ALG_ANALOG_SUPPLY_ALERT:            
        {
            data_non_volatile.device_data.sv_record.system_error_flags.bits.power_supply = 1;
            algorithm_wake(0);
        }break;

#if(DEVICE == AQ0X_V1)
         case  X_NTF_LIGHT_SENSOR_SV_ALART:
        {
            // supervission report indicating persistent error or when change is made on configured data
            x_notify_module_sv_status_report_t *alarm = (x_notify_module_sv_status_report_t *)notify;
            data_non_volatile.device_data.sv_record.error_flags.light = alarm->message.error_flags;
             x_send_event(&algorithm_data.wake_event);
        }break;

#endif
         case  X_NTF_IR_SENSOR_SV_ALART:
        {
            // supervission report indicating persistent error or when change is made on configured data
            x_notify_module_sv_status_report_t *alarm = (x_notify_module_sv_status_report_t *)notify;
            data_non_volatile.device_data.sv_record.error_flags.ir = alarm->message.error_flags;

            if(alarm->message.config.amp_treshold.adjusted == 1)
                data_non_volatile.device_data.installation.thresholds.ir_level_max
                 = alarm->message.config.amp_treshold.set_value;

        }break;

         case  X_NTF_UV_SENSOR_SV_ALART:
        {
            // supervission report indicating persistent error or when change is made on configured data
            x_notify_module_sv_status_report_t * alarm = (x_notify_module_sv_status_report_t *)notify;
            data_non_volatile.device_data.sv_record.error_flags.uv = alarm->message.error_flags;

             if(alarm->message.config.amp_treshold.adjusted == 1)
                data_non_volatile.device_data.installation.thresholds.uv_level_max
                 =alarm->message.config.amp_treshold.set_value;

        }break;

        case  X_NTF_MFIELD_X_SENSOR_SV_ALART:
        {
            // supervission report indicating persistent error
            x_notify_module_sv_status_report_t * alarm = (x_notify_module_sv_status_report_t *)notify;
            data_non_volatile.device_data.sv_record.error_flags.mfield_x = alarm->message.error_flags;
      
        }break;

#ifdef MFIELD_Y
        case  X_NTF_MFIELD_Y_SENSOR_SV_ALART:
        {        
            // supervission report indicating persistent error
            x_notify_module_sv_status_report_t * alarm = (x_notify_module_sv_status_report_t *)notify;
            data_non_volatile.device_data.sv_record.error_flags.mfield_y = alarm->message.error_flags;
            
        }break;
#endif

        default:
        {

            ASSERT(0);
        }
        
    }
}








/*!
    \brief  Causes that system will send notification to the module as this cannot be done directly because
    the function is called from interrupt
*/

void algorithm_message_arrived_ntf_isr(Uint16 id)
{
    if(id==0)
        return;
    if((0x00FF&id)!=0)
    algorithm_data.irq_src.fields.external = (0x00FF&id);
    else if(((0xFF00&id)>>8)!=0)
    algorithm_data.irq_src.fields.internal = ((0xFF00&id)>>8);
    x_task_send_irq_sync(algorithm_id);
}



