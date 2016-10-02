#ifndef SRV_HWIO_H
#define SRV_HWIO_H
/*! \file   srv_hwio.h

    \brief  Header for low level io/analog port driver
    
    
*/


#include "System.h"
#include "allheaders.h"


//!for ten bit adc module
#define ADC_RESOLUTION                                       (1024L) // 10 bit

//!adc value to real value mapping factor ->  Vref/resolution
#define ADC_VAL_MAPPING_FACTOR                               ((3.3L/*Vref*/)/(ADC_RESOLUTION))





#define ANA_INDEX_UV                   1
#define ANA_INDEX_IR                   2
#define ANA_INDEX_MFIELD_X             3
#define ANA_INDEX_MFIELD_Y             0
#define ANA_INDEX_SUPPLY               4

#if(DEVICE == AQ0X_V1)
#define ANA_INDEX_SV_CURRENT           5
#endif





//! global command to execute gpio output signal
#define         HWIO_SV_LED_ON                              1
#define         HWIO_SV_LED_OFF                             2
#define         HWIO_ALARM_ON                               3
#define         HWIO_ALARM_OFF                              4
#define         HWIO_HMC10XX_PULSE_SET                      5
#define         HWIO_HMC10XX_PULSE_RSET                     6
#define         HWIO_SV_LED_PORT_STATUS                     7
#define         HWIO_ALARM_PORT_STATUS                      8
#define         HWIO_HMC10XX_PULSE_PORT_STATUS              9


#define         DIG_TEST_LED_LAT             		    LATBbits.LATB15
#define         DIG_ALARM_OUTPUT_LAT         		    LATBbits.LATB14
#define         DIG_SUPERVISION_OUTPUT_LAT   		    LATBbits.LATB4
#define         DIG_HMC10XX_SR_PULSE_OUTPUT_LAT             LATAbits.LATA4


#define         DIG_TEST_LED_PORT             		    PORTBbits.RB15
#define         DIG_ALARM_OUTPUT_PORT         		    PORTBbits.RB14
#define         DIG_SUPERVISION_OUTPUT_PORT   		    PORTBbits.RB4
#define         DIG_HMC10XX_SR_PULSE_OUTPUT_PORT            PORTAbits.RA4



void            hwio_configure_init(void);
void            hwio_configure_analog_init(void);
Int8            hwio_execute_gpio_task(Uint8 cmd);
Uint16          hwio_get_analog(Uint16 AnIdx);


#endif //SRV_HWIO_H

