/*! \file   Main.c

    \brief  Low-level startup , calls OS initalization

*/

#include "..\includes\allheaders.h"


//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers, BOR characteristics etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.


//!Oscilator setting
//!Run this project using an internal FRC
#ifdef __C30_VERSION__
// Select Internal FRC at POR
_FOSCSEL(FNOSC_FRC & IESO_OFF);
// Enable Clock Switching and Configure Primary Oscillator in XT mode
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
#endif




//!Disable Code Protection
#ifdef __C30_VERSION__
_FGS(GCP_OFF);
#endif



//!Select PGD/PGC as debugging channel
//!Communicate on PGEC2 and PGED2
#ifdef __C30_VERSION__
_FICD(ICS_PGD2 & JTAGEN_OFF);
#endif


//!Enable MCLR reset pin and turn off the
//!power-up timers.
#if !defined (__DEBUG)
#ifdef __C30_VERSION__
//_FPOR(MCLR_EN & PBOR_ON & BORV_42 & PWRT_4 );
#endif
#else
#ifdef __C30_VERSION__

#endif
#endif



//Watchdog Window Select bits:

//!Disable memory security
/*
#ifdef __C30_VERSION__
_FSS( NO_SEC_RAM & NO_SEC_EEPROM & NO_SEC_CODE );
_FBS( NO_BOOT_RAM & NO_BOOT_EEPROM & NO_BOOT_CODE );
#endif
*/



sqtp_t                  sqtp_info;


/*!

    \brief  System startup function

    Configures operating system and basic system independent services


*/

int main (void)
{
/*!
 Configure Oscillator to operate the device at 60MIPS
 Fpll_out= Fin*M/(N1*N2)=Fin*(PLLDIV+2)/((PLLPRE+2)+(PLLPOST+1)), Fcy=Fosc/2
 Fvco    = Fin*M/N1 = Fin*(PLLDIV+2)/(PLLPRE+2)
 Fpll_out = 7.37M*65/(2*3) = 80Mhz for 7.37MHz FRC input clock
 */


// Configure PLL prescaler, PLL postscaler, PLL divisor
PLLFBD=63;                                           // M=65
CLKDIVbits.PLLPOST=0;                                // N2=2
CLKDIVbits.PLLPRE =0;                                // N1=3 //TODO: other option set to N1 = 2
// Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
__builtin_write_OSCCONH(0x01);
__builtin_write_OSCCONL(OSCCON | 0x01);
// Wait for Clock switch to occur
while (OSCCONbits.COSC!= 0b001);
// Wait for PLL to lock
while (OSCCONbits.LOCK!= 1);




#if !defined(__DEBUG)
   // _SWDTEN = 1;
#endif
    /*!
 
        \wdg Init watchdog. Initial kick.
    */
    
    srv_wdg_init();
    srv_wdg_kick();
        

  
    // Init hwio
    hwio_configure_init();
    


    {
    hwio_execute_gpio_task(HWIO_SV_LED_ON);
    if(RCONbits.WDTO)
        while(1);
     #if (DEVICE == AQ0X_V1)
    Uint16 sv_current_level = hwio_get_analog(ANA_INDEX_SV_CURRENT);
    Uint8  cc = 1;
    Uint16 single_sensor_output =  SV_CURREN_OUTPUT_LEVEL;



   
		    while(1){
		        /*!Device will stay in this loop until it recognize the communication
		        address, the lead is used  as an indication incase of fatal error  */
		         if(cc++ < RANGE_MAX_8BIT){
		             if(sv_current_level <= single_sensor_output){
		                 data_non_volatile.device_data.installation.device_address = cc;
		                 break;
		             }else
		                single_sensor_output +=SV_CURREN_OUTPUT_LEVEL;
		         }
		      srv_wdg_kick();
		
		    }
    #endif

    hwio_execute_gpio_task(HWIO_SV_LED_OFF);
    }

       
 
    // Init serial port communication
    serial_init();
   

    spi_init();


  
    sqtp_info.serial_number = 0;
    // Initialize SQPT production info
    sqtp_info.serial_number = nov_get_serial_number();
    //TODO:what to do if empty
 
    // Init operating system internal structures
    x_init();        
    
    

    // Init modules
    x_init_task(algorithm_taskx,algorithm_notifyx,&algorithm_id,"algorithm");
    x_init_task(ir_sen_taskx,ir_sen_notifyx,&ir_sen_id,"ir");
    x_init_task(uv_sen_taskx,uv_sen_notifyx,&uv_sen_id,"uv");
    #if(DEVICE == AQ0X_V1)
    x_init_task(light_sen_taskx,light_sen_notifyx,&light_sen_id,"light");
    #endif
    x_init_task(mfield_x_sen_taskx,mfield_x_sen_notifyx,&mfield_x_sen_id,"mfield_x");
     
#ifdef MFIELD_Y
    x_init_task(mfield_y_sen_taskx,mfield_y_sen_notifyx,&mfield_y_sen_id,"mfield_y");
#endif
   
   
    // Send startup notifications to modules
    x_start();        

     
    /*!
              
         \wdg Watchdog kick - before OS starts
                     
                          
    */
    
    srv_wdg_kick();


    while (1)             
    {
         
        // Execute operating system loop
        // The operating system takes care over watchdog bites
        x_loop();

    }
    return 0;             
}


