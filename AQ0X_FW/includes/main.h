#ifndef MAIN_H
#define MAIN_H
/*! \file   Main.h

    \brief  Few useful generic macros. Contains defines for assembler declared functions
    
    
*/

#include "System.h"




//! Interrupt save context

#define IRQ_CTX_T  int

//! Global IRQ disable
#define IRQ_DISABLE(save_sr) \
    SET_AND_SAVE_CPU_IPL (save_sr, 7);


//! Global IRQ enable
#define IRQ_ENABLE(save_sr)   \
    RESTORE_CPU_IPL (save_sr);


typedef struct
{
    Uint32  serial_number;
    Uint16  device_address;
}sqtp_t;


extern sqtp_t  sqtp_info;
extern const char  version_string[64];



//! Assembler function - 5us granularity loop delay 
void Delay5us(int Count);

//! Assembler function - 5ms granularity loop delay 
void Delay5ms(int Count);


#endif //MAIN_H





