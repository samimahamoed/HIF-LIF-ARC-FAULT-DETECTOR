#ifndef SRV_WATCHDOG_H
#define SRV_WATCHDOG_H

/*! 
    \file   srv_watchdog.h

    \brief  File with support for watchdog( for release version) and watchdog tunning (debug/release)
    
    
*/



#define WATCHDOG_PRODUCTION_MODE     0
#define WATCHDOG_TUNNING_TIMEOUT     3 /* initial value in miliseconds */





#if defined(__DEBUG)|| (WATCHDOG_PRODUCTION_MODE == 0)

void srv_wdg_init_fn(void);
void srv_wdg_kick_fn(void);


#define srv_wdg_init()  srv_wdg_init_fn()
#define srv_wdg_kick()  srv_wdg_kick_fn()


#else

#define srv_wdg_init()  
#define srv_wdg_kick()  { ClrWdt();}


#endif


#endif //SRV_WATCHDOG_H
