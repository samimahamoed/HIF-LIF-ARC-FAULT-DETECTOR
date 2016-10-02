/*! \file   srv_hwio_prv_101.h

    \brief  Header for private low level io/analog port driver
       
*/


#ifndef SRV_HWIO_PRV_H
#define SRV_HWIO_PRV_H

#define ANALOG_HW_CHANNELS             6


#define ANA_SEL_UV_ANSEL                    ANSELAbits.ANSA0    //AN0
#define ANA_SEL_IR_ANSEL		    ANSELAbits.ANSA1    //AN1
#define ANA_SEL_MFIELD_X_ANSEL         	    ANSELBbits.ANSB0    //AN2
#define ANA_SEL_MFIELD_Y_ANSEL         	    ANSELBbits.ANSB1    //AN3
#define ANA_SEL_SUPPLY_SV_A_ANSEL           ANSELBbits.ANSB2    //AN4
#define ANA_SEL_SUPPLY_SV_B_ANSEL           ANSELBbits.ANSB3    //AN5


#define ANA_SEL_UV_TRIS               	    TRISAbits.TRISA0    //AN0
#define ANA_SEL_IR_TRIS		            TRISAbits.TRISA1    //AN1
#define ANA_SEL_MFIELD_X_TRIS         	    TRISBbits.TRISB0    //AN2
#define ANA_SEL_MFIELD_Y_TRIS               TRISBbits.TRISB1    //AN3
#define ANA_SEL_SUPPLY_SV_A_TRIS     	    TRISBbits.TRISB2    //AN4
#define ANA_SEL_SUPPLY_SV_B_TRIS     	    TRISBbits.TRISB3    //AN5



#define DIG_TEST_LED_TRIS                   TRISBbits.TRISB15
#define DIG_ALARM_OUTPUT_TRIS               TRISBbits.TRISB14
#define DIG_SUPERVISION_OUTPUT_TRIS         TRISBbits.TRISB4
#define DIG_HMC10XX_PULSE_OUTPUT_TRIS       TRISAbits.TRISA4




//! Structure which contains emulation mode operation details

//! Structure which contains all data used by srv_hwio module
typedef struct
{
          struct
        {
            union{
                Uint8 all;
                 struct
                {
                     Uint8 mfield_y:1;
	             Uint8 uv:1;
	             Uint8 ir:1;
                     Uint8 mfield_x:1;
                     
                     #if(DEVICE == AQ0X_V1)
                     Uint8 light:1;
                     #endif
                     
                }bits;
            }enabled;

            Uint16*    data[ANALOG_HW_CHANNELS];
            Uint16     head_ptr[ANALOG_HW_CHANNELS];
            Uint16     size;

        }emulation_mode;
}srv_hwio_data_t;



extern  srv_hwio_data_t                         srv_hwio_data;

#endif


