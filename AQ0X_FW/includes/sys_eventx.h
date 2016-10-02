#ifndef SYS_EVENTX_H
#define SYS_EVENTX_H
/*! 
    \file   sys_eventx.h

    \brief  OS header file
    
    
*/




// Reserved for X system predefined messages

//! System reserved event id - no message
#define X_MSG_NONE            0x0000

//! System reserved event id - IRQ notification
#define X_MSG_IRQ_SYNC        0x0001


//! First event id to be used by user
#define X_MSG_FREE            0x0020
//! Last event id to be used by user
#define X_MSG_LAST            0x1000


//! System init notification
#define X_NTF_INIT            0x1001


//! First notification id to be used by user
#define X_NTF_FREE            0x1003

//! Last notification id to be used by user
#define X_NTF_LAST            0xFFFF




// System macros

//! OS timer tick - 1ms
#define X_TIMER_TICK        1000 /* Hz */

//! Conversion of ms to OS ticks
#define X_MS2TICK(x_)       ((x_)/(1000/X_TIMER_TICK))

//! Conversion of OS ticks to ms
#define X_TICK2MS(x_)       ((x_)*(1000/X_TIMER_TICK))


//! Task identifier type
typedef Uint8 x_task_id;



//! OS event
typedef struct _x_event
{
    struct _x_event * link;  //! Internal, for OS only - event linking
    
    Uint16            message;//! Event message    
    x_task_id         task_id; //! Event target task id
    Uint8             dummy;
}x_event;


typedef struct _x_notify
{
    struct _x_notify * link;    //! Internal, for OS only - notification linking
    Uint16             ticks;   //! Internal, for OS only - used for timer notifications, tick perid
    
    Uint16             message; //! Notification message
    x_task_id          task_id; //! Notification target task id 
    Uint8              active;  //! When set to 1 then already present in internal link lists
}x_notify;

//! Event processing function type ( for modules)
typedef void (* x_task_fn)(x_event *);

//! Notification processing function type ( for modules)
typedef void (* x_notify_fn)(x_notify *);







void    x_init(void);
void    x_init_task(x_task_fn task_fn,x_notify_fn notify_fn,x_task_id * pHandle_id,const char * task_name);

void    x_start(void);        
void    x_loop(void);

Uint16  x_tick(void);



void  x_send_event(x_event * event);
void  x_send_notify(x_notify * notify);


void  x_schedule_timer(x_notify  *  timer,Uint16 ticks);
void  x_delete_timer(x_notify  *  timer);


void  x_task_send_irq_sync(x_task_id task_id);

#endif //SYS_EVENTX_H

