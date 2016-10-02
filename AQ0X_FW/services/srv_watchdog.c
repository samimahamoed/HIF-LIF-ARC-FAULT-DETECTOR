#include "..\includes\allheaders.h" 

/*!
    File : main.c
     \page wdg Watchdog initialization.
               
       
	            The Watchdog Timer consists of a configurable prescaler and postscaler clocked with
                    Low-Power RC (LPRC) 32kHz oscillator. The watchdog time-out period is
                    selected by configuring the prescaler and postscaler dividers.

                    In release mode it is configured to _FWDT(FWDTEN_ON & WINDIS_OFF & WDTPOST_PS8 & WDTPRE_PR32);-
                    to obtain 8ms overflow



                    TWTO = (N1)x(N2)x(TLPRC)

                    Where,
                    N1 = Prescaler divider ratio
                    N2 = Postscaler divider ratio
                    TLPRC = LPRC clock period (32 kHz)

                    TWTO = 32*8/(32*10E3) = 8ms

               
                                 
*/


//!Turns off the Watch-Dog Timer in debug mode.
//!Turns on the Watch-Dog Timer in release mode.
//!Watchdog set to 10ms (2ms x WDTPSA x WDTPSA )

/*

 

 */

#if defined(__DEBUG) || (WATCHDOG_PRODUCTION_MODE == 0)
#ifdef __C30_VERSION__
_FWDT(FWDTEN_OFF & WINDIS_OFF);
#endif
#else
#ifdef __C30_VERSION__
_FWDT(FWDTEN_ON & WINDIS_OFF & WDTPOST_PS8 & WDTPRE_PR32);
#endif
#endif


#if defined(__DEBUG) || (WATCHDOG_PRODUCTION_MODE == 0)


Uint32 watchdog_timeout = WATCHDOG_TUNNING_TIMEOUT;



/*!
    \brief  Watchdog tunning timeout function

*/
#ifdef HI_TECH_C
void interrupt _T4Interrupt(void) @ T4_VCTR
#else
void __attribute__((__interrupt__,auto_psv )) _T4Interrupt(void)
#endif
{

    // Ack interrupt
    IFS1bits.T4IF = 0;


    // Stop timer
    T4CONbits.TON = 0;
    TMR4          = 0;  
    
    // Increase watchdog timeout
    if(watchdog_timeout< 1000)
    {
        watchdog_timeout++;
    }

    // Reinitialize tunning timer
    srv_wdg_init_fn();

    
}


/*!
    \brief   Watchdog tunning init function

*/ 

void srv_wdg_init_fn(void)
{

    // Setup tunning timer
    
    T4CON = 0;                    // Clear the register
    T4CONbits.TCS    = 0;         // Internal clock Fosc/4   
    T4CONbits.TCKPS = 1;          //    1:8 prescaling    



    PR4 = (((FOSC_CPU/2L)/8)*watchdog_timeout)/(1000L); 
    
    IPC6bits.T4IP = 1;            // Set Timer4 Interrupt Priority Level
    IFS1bits.T4IF = 0;            //Clear the Timer4 Interrupt Flag
    IEC1bits.T4IE = 1;            //Enable Timer4I nterrupt Service Routine
    T4CONbits.TON = 1;            //Start Timer 4




    
}


/*!
    \brief   Watchdog tunning kick function

*/

void srv_wdg_kick_fn(void)
{
    T4CONbits.TON = 0;
    TMR4 = 0;
    T4CONbits.TON = 1;
}
#else

void srv_wdg_kick_fn(void)
{
   ASSERT(0);
}

#endif //  (WATCHDOG_PRODUCTION_MODE == 0)


