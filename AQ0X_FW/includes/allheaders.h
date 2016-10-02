/*! \file   Allheaders.h

    \brief  All project header files together so modules can include just this one.
    
    
*/


#ifndef ALLHEADERS_H
#define ALLHEADERS_H



#if  defined(__DEBUG) & !defined(__MPLAB_DEBUGGER_ICD3) 
//! OS debug - print init   phase
#define DBG_SHOW_INIT    1

//! OS debug - print timer notifications
#define DBG_SHOW_TIMERS  1

//! OS debug - print event notifications
#define DBG_SHOW_EVENTS  1

//! OS debug - print standard notifications
#define DBG_SHOW_NOTIFY  1


//! debug - print test point message 
#define DBG_TEST_POINT  1

#else

//! OS debug - print init   phase
#define DBG_SHOW_INIT    0

//! OS debug - print timer notifications
#define DBG_SHOW_TIMERS  0

//! OS debug - print event notifications
#define DBG_SHOW_EVENTS  0

//! OS debug - print standard notifications
#define DBG_SHOW_NOTIFY  0

//! debug - print test point message 
#define DBG_TEST_POINT  0
#endif


#if (DBG_SHOW_INIT == 1)
#define D_I(x_) x_
#else
#define D_I(x_)
#endif

#if (DBG_SHOW_TIMERS == 1)
#define D_T(x_) x_
#else
#define D_T(x_)
#endif


#if (DBG_SHOW_EVENTS == 1)
#define D_E(x_) x_
#else
#define D_E(x_)
#endif


#if (DBG_SHOW_NOTIFY == 1)
#define D_N(x_) x_
#else
#define D_N(x_)
#endif


#if (DBG_TEST_POINT == 1)
#define D_P(x_) x_
#else
#define D_P(x_)
#endif


#define TRUE    1

#define FALSE   0

//code enable macros
#define  TEST_MODE  0

//!Device version 0 , include 3 magnetic sensors,1 infrared sensor, 1 ultraviolet sensor, and 1 light sensor
#define  AQ0X_V0      0
//!Device version 1 , include 2 magnetic sensors,1 infrared sensor, 1 ultraviolet sensor 
#define  AQ0X_V1      1

#define  DEVICE      AQ0X_V0

#include "string.h"

#include "stdlib.h"
#include "stddef.h"
#include "stdarg.h"
#include "stdio.h"


#include <math.h>

#include "libpic30.h"
#include "p33EP128MC202.h"


#include "System.h"
#include "srv_watchdog.h"
#include "sys_eventx.h"
#include "main.h"

#include "nov_data.h"
#include "srv_serial.h"
#include "srv_nov.h"
#include "srv_spi.h"

#include "srv_hwio.h"
/*





*/
/*

#include "srv_testlog.h"
#include "srv_progmem.h"
*/


#include "aq0X/algorithm.h"

#include "aq0X/uv_sensor.h"


#include "aq0X/light_sensor.h"


#include "aq0X/mfield_sensor.h" 
#include "aq0X/ir_sensor.h"





/*
#include "gprintf.h"
*/



















#endif


