/*! \file   System.h

    \brief  Generic project defines ( data types, basic hardware setting)
    
    
*/



#ifndef SYSTEM_H
#define SYSTEM_H


//Defines for System Clock Timing -
// Configure Oscillator to operate the device at 60MIPS
// Fosc  = Fin*M/(N1*N2)=Fin*(PLLDIV+2)/((PLLPRE+2)*2(PLLPOST+1)), Fcy=Fosc/2
// Fvco  = Fin*M/N1 = Fin*(PLLDIV+2)/(PLLPRE+2)
// Fosc  = 7.37M*65/(2*3) = 80Mhz for 7.37MHz FRC input clock

#define PLL_PRESCALER_N1      2

#define PLL_POSTSCALER_N2     3

#define PLL_FEEDBACK_M        65


//!On-chip FRC oscillator used frequency
#define XTFREQ_CPU           (7370000)


//!On-chip PLL setting
#define PLLMODE_CPU          (16)
//!Instruction Cycle Frequency
//#define FOSC_CPU             (XTFREQ_CPU*PLL_FEEDBACK_M/(PLL_PRESCALER_N1*PLL_POSTSCALER_N2))
#define FOSC_CPU             120000000
#define FCY_CPU              (FOSC_CPU/2)
//#define _XTAL_FREQ           (7370000 * 16 /4)


typedef unsigned char        Uint8;
typedef unsigned int         Uint16;
typedef unsigned long        Uint32;
typedef unsigned long long   Uint64;

typedef float                Float32;
typedef signed char          Int8;
typedef signed int           Int16;
typedef signed long          Int32;
typedef signed long long     Int64;
typedef unsigned char        Bool;
typedef const char           CChar;




#ifdef __DEBUG
#define ASSERT(c_)    if(!(c_))while(1){};
#else
#define ASSERT(c_)

#endif



#endif
