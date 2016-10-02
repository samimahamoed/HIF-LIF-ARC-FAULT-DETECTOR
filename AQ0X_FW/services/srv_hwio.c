/*! \file   srv_hwio_101.c

    \brief  Low level driver for CPU operations - IO ports and  analog ports
    
    
*/

#include "..\includes\allheaders.h"
//#include "algorithm_prv.h"
#include "..\includes_prv\aq0X\srv_hwio_prv.h"


srv_hwio_data_t                         srv_hwio_data;

/*!

    \page hw Hardware driver module

    The module is responsible for the device analog and digital input output operations.
    Here, two interface helper functions are provided for analog level reading and digital
    input output operations. Thus, other modules with the privilege to ac-cess hardware
    functionality uses these functions to for such purpose (see hwio_get_analog() and hwio_execute_gpio_task()).

    The module also supports hardware reading redirection to allow testing firmware functionality. Thus, user can
    replace real hardware reading with values supplied by serial commands so as to validate the software operation.
    In the case of analog signal emulation the module can accept up to 400 byte (200 samples) data per input and
    sequentially supply the values in circular manner instead of real hardware reading, that is under the condition
    in which if the sensory module monitoring the signal is running in emulation mode (see hwio_get_analog()).

 *

*/

/*!

    \brief Low level gpio/analog configuration
    
*/


void hwio_configure_init()
{
    memset((void*)&srv_hwio_data,0,sizeof(srv_hwio_data));

    //hwio_configure_analog_init();

    // Configuring port outputs and startup value
    // All other port signals are inputs by default


    //self supervision lead
      DIG_TEST_LED_TRIS                 = 0;
      DIG_TEST_LED_LAT                  = 0;

    // Alarm output
      DIG_ALARM_OUTPUT_TRIS             = 0;
      DIG_ALARM_OUTPUT_LAT              = 0;

    //relay supervision signal output
      DIG_SUPERVISION_OUTPUT_TRIS       = 0;
      DIG_SUPERVISION_OUTPUT_LAT        = 0;


   //HMC10XX set/reset strap operation pulse control
      DIG_HMC10XX_PULSE_OUTPUT_TRIS     = 0;
      DIG_HMC10XX_SR_PULSE_OUTPUT_LAT   = 1;

  


    //other used ports are configured by responsible module

     srv_hwio_data.emulation_mode.data[ANA_INDEX_IR] =
             data_non_volatile.disturbance_record.ir.post_act_data.buffer.data.h;
     srv_hwio_data.emulation_mode.data[ANA_INDEX_UV] =
             data_non_volatile.disturbance_record.uv.post_act_data.buffer.data.h;
     srv_hwio_data.emulation_mode.data[ANA_INDEX_MFIELD_X] =
             data_non_volatile.disturbance_record.mfield_x.post_act_data.buffer.data.h;
#ifdef MFIELD_Y
     srv_hwio_data.emulation_mode.data[ANA_INDEX_MFIELD_Y] =
             data_non_volatile.disturbance_record.mfield_y.post_act_data.buffer.data.h;
#endif
#if(DEVICE == AQ0X_V1)
     srv_hwio_data.emulation_mode.data[ANA_INDEX_LIGHT] = 
             data_non_volatile.disturbance_record.light.post_act_data.buffer.data.h;
#endif

       // Init A/D
    hwio_configure_analog_init();
    Delay5us(4);
}




/*!
    \brief Low level analog 
*/
void   hwio_configure_analog_init(void)
{


         /* Set port configuration */
        ANSELA = ANSELB = 0x0000;        //
        ANA_SEL_MFIELD_X_ANSEL      = 1; 
        ANA_SEL_MFIELD_Y_ANSEL      = 1; 
        ANA_SEL_IR_ANSEL            = 1; 
        ANA_SEL_UV_ANSEL            = 1;
        ANA_SEL_SUPPLY_SV_A_ANSEL   = 1;
        
        
        #if(DEVICE == AQ0X_V1)
        ANA_SEL_SUPPLY_SV_B_ANSEL   = 1;
        #endif


        /* Initialize and enable ADC module */
        AD1CON1bits.ADON  = 0;     //ADC is off


        AD1CON1bits.ADDMABM = 0;   //DMA not used
        AD1CON1bits.AD12B   = 0;   //10-bit mode
        AD1CON1bits.FORM    = 0;   //Data Output Format integer



        AD1CON1bits.SSRCG   = 0;   //Sample Clock Source Group bit
        AD1CON1bits.SSRC    = 7;   //Internal counter ends sampling and starts conversion (auto-convert)
        //AD1CON1bits.SSRC    = 0; //Clearing the Sample bit (SAMP) ends sampling and starts conversion (Manual mode)
        AD1CON1bits.SIMSAM  = 1;   //Samples CH0, CH1, CH2, CH3 simultaneously (when CHPS<1:0> = 1x)
        AD1CON1bits.ASAM    = 1;   //ADC Sample Auto-Start bit, 0 =>Sampling begins when SAMP bit is set
        AD1CON1bits.SAMP    = 0;   //ADC Sample and Hold amplifiers are holding




        AD1CON2bits.VCFG    = 0;    //Vref Vdd = 3.3 & Vss = 0
        AD1CON2bits.CSCNA   = 0;    //Do not scan inputs

        AD1CON2bits.CHPS    = 3;    //Channel Selection, Multi-Channel (CH0-CH3)

        AD1CON2bits.SMPI    = 0;    // every sample/conversion
        AD1CON2bits.BUFM    = 0;
        AD1CON2bits.ALTS    = 0;    //Always uses channel input selects for Sample A



        AD1CON3bits.ADRC      = 0;    //Clock Derived from System Clock
        AD1CON3bits.SAMC      = 0;    //Auto Sample Time bits, 0TAD
        AD1CON3bits.ADCS      = 5;    // ADC Conversion Clock TAD=Tcy*(ADCS+1)= (1/60M)*6 = 100ns
        //112 for 90us Ts             // ADC Conversion Time for 10-bit Tc=12*Tad = 1.2us


                                        /*
                                        TSMP = SAMC<4:0> * TAD
                                        TCONV = 12 * TAD
                                        *
                                        *
                                        TSIM = TSMP + (M · TCONV)
                                        Where:
                                        TSIM = Total time to sample and convert multiple channels with simultaneous sampling.
                                        TSMP = Sampling Time = SAMC<4:0>*TAD = 0
                                        TCONV = Conversion Time = 12*TAD = (1/60M)*6 = 100ns 
                                        M = Number of channels selected by the CHPS<1:0> bits = 4

                                        =>Total time to sample and convert multiple channels with simultaneous sampling.
                                        TSIM = TSMP + (M · TCONV) = 4*Tc = 48*TAD =  4.8us

                                        */


        AD1CON4  = 0;                  //DMA disabled


        AD1CSSH               = 0;
        AD1CSSL               = 0;    //input scan select register

        AD1CHS0bits.CH0SA     = 3;    // MUXA +ve input selection (AIN3) for CH0
        AD1CHS0bits.CH0NA     = 0;    // MUXA -ve input selection (Vref-) for CH0

        AD1CHS123bits.CH123SA = 0;   /* Select AN0 for CH1 +ve input
                                        Select AN1 for CH2 +ve input
                                        Select AN2 for CH3 +ve input*/
        AD1CHS123bits.CH123NA = 0;   // Select Vref- for CH1/CH2/CH3 -ve inputs


          /* Assign MUXB inputs */
        AD1CHS0bits.CH0SB       = 0;      // Select AN0 for CH0 +ve input
        AD1CHS0bits.CH0NB       = 0;      // Select VREF- for CH0 -ve input
        AD1CHS123bits.CH123SB   = 0;      // Select AN3 for CH1 +ve input
                                          // Select AN4 for CH2 +ve input
                                          // Select AN5 for CH3 +ve input
        AD1CHS123bits.CH123NB   = 0;        // Select VREF- for CH1/CH2/CH3 -ve inputs

        IFS0bits.AD1IF = 0;	      // Clear the A/D interrupt flag bit
        IEC0bits.AD1IE = 0;           // Do Not Enable A/D interrupt
        AD1CON1bits.ADON = 1;	      // Turn on the A/D converter

        AD1CON1bits.SAMP = 1;

        Delay5us(4);

}





/*!

    \brief Retrives A/D hardware buffer value

    \param [in] AnIdx A/D buffer index

    \retval A/D buffer value

    Note: If emulation enabled, returns emulated value instead of real hw value

*/



Uint16 hwio_get_analog(Uint16 ana_idx)
{
    if(ana_idx >=ANALOG_HW_CHANNELS)
    {
        return 0;
    }


    if((srv_hwio_data.emulation_mode.enabled.all & (1<<ana_idx)) == 0)
    {
        switch(ana_idx)
        {
            case  0: return ADC1BUF0;
            case  1: return ADC1BUF1;
            case  2: return ADC1BUF2;
            case  3: return ADC1BUF3;
            case  4: return ADC1BUF4;
            case  5: return ADC1BUF5;

            default: return (0);
        }
    }
    else
    {

       
        if(srv_hwio_data.emulation_mode.data[ana_idx] != 0)
        {
            Uint16 head_ptr = srv_hwio_data.emulation_mode.head_ptr[ana_idx];
            srv_hwio_data.emulation_mode.head_ptr[ana_idx] = (srv_hwio_data.emulation_mode.head_ptr[ana_idx] + 1)%EMU_MODE_DATA_LENGTH;
            
            
            return *(srv_hwio_data.emulation_mode.data[ana_idx] + head_ptr);
        }
       
    }

    return 0;
}


/*!

    \brief gpio port control helper function

*/



Int8   hwio_execute_gpio_task(Uint8 cmd){

    Int8 result = -1;
        switch(cmd)
        {
                 case  HWIO_SV_LED_ON:
                {
                    DIG_TEST_LED_LAT = 1;
                    result = 0;
                }break;

                 case  HWIO_SV_LED_OFF:
                {
                    DIG_TEST_LED_LAT = 0;
                    result = 0;
                }break;

                 case  HWIO_SV_LED_PORT_STATUS:
                {
                    result = DIG_TEST_LED_PORT;
                }break;

                 case  HWIO_ALARM_ON:
                {
                    DIG_ALARM_OUTPUT_LAT = 1;
                    result = 0;
                }break;

                 case  HWIO_ALARM_OFF:
                {
                    DIG_ALARM_OUTPUT_LAT = 0;
                    result = 0;
                }break;

                 case  HWIO_ALARM_PORT_STATUS:
                {
                    result = DIG_ALARM_OUTPUT_PORT;

                }break;

                 case  HWIO_HMC10XX_PULSE_SET:
                {
                    DIG_HMC10XX_SR_PULSE_OUTPUT_LAT = 1;
               
                    result = 0;
                }break;

                 case  HWIO_HMC10XX_PULSE_RSET:
                {
                    DIG_HMC10XX_SR_PULSE_OUTPUT_LAT = 0;
                    result = 0;
                }break;


                 case  HWIO_HMC10XX_PULSE_PORT_STATUS:
                {
                    result = DIG_HMC10XX_SR_PULSE_OUTPUT_PORT;
                    result = 0;
                }break;
                
                default:
                {
                   ASSERT(0);

                }break;

        }

        return result;
}




