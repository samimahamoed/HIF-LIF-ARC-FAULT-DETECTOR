/*! \file   Io_101.c

    \brief  Module responsible for fast scanning (in 100us interrupts) sensors and other inputs    
    
*/



#include "..\..\includes\allheaders.h" 
#include "..\..\includes_prv\aq0X\algorithm_prv.h"




/*!

    \brief  Fast timer interrupt to scan sensor element analog values.
            Processe also delay blocks from concerned modules

    
*/


 
void __attribute__((__interrupt__,auto_psv)) _T1Interrupt(void)
{
        
  IRQ_CTX_T   irq_ctx;
  IRQ_DISABLE(irq_ctx);	



    if(nov_wr_inprogress==TRUE)
        //If nov operation is on progress suspend for a moment
        NOV_HOLD_CTRL_LAT = 0;

  // Increment tick - used as fast timestamp
    logv.tick++;

     /*!
            req  REQ-8a:  "Read the sensor inputs "
                          Sensors and inputs are read and filtered.
             
    */


#if(TEST_MODE)
    //For timing measurements only - prolog
    DIG_TEST_LED_LAT =1;
#endif
    if(data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.ir)
         algorithm_data.modules_state_sv.ir_sen = ir_sen_get_status(hwio_get_analog(ANA_INDEX_IR),0);

    if(data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.uv)
          algorithm_data.modules_state_sv.uv_sen = uv_sen_get_status(hwio_get_analog(ANA_INDEX_UV),0);

    if(data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_x)
          algorithm_data.modules_state_sv.mfield_x_sen = mfield_x_sen_get_status(hwio_get_analog(ANA_INDEX_MFIELD_X),0);

  #if(DEVICE == AQ0X_V1)
    if(data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_y)
         algorithm_data.modules_state_sv.mfield_y_sen = mfield_y_sen_get_status(hwio_get_analog(ANA_INDEX_MFIELD_Y),0);
  #endif



     if(
        (algorithm_data.modules_state_sv.ir_sen.fields.state == HOT)
       || (algorithm_data.modules_state_sv.uv_sen.fields.state == HOT)
       )
       algorithm_wake(EVENT_SV_ALARM);
     else if(logv.trip_active == TRUE)
          algorithm_wake(EVENT_CLEARED);
    


     if(logv.sv_delay.cnt > 0)
         if((--logv.sv_delay.cnt) == 0)
             algorithm_wake(logv.sv_delay.type);


     if(algorithm_data.diagnosis.false_alarm.ir.delay > 0)
        algorithm_data.diagnosis.false_alarm.ir.delay--;

     if(algorithm_data.diagnosis.false_alarm.uv.delay > 0)
        algorithm_data.diagnosis.false_alarm.uv.delay--;

    
     


#if(TEST_MODE)
    //For timing measurements only - prolog
    DIG_TEST_LED_LAT =0;     
#endif



     
  
    if((nov_wr_inprogress==1) && (NOV_HOLD_CTRL_LAT == 0))
        //Restart suspended nov operation
    NOV_HOLD_CTRL_LAT = 1;

    IFS0bits.T1IF = 0;    // Clear Timer1 Interrupt Flag
    
   IRQ_ENABLE(irq_ctx);
   SET_CPU_IPL(6);       // Fix for core interrupt in-interupt issue


}


