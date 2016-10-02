/*!
    \file   sys_eventx.c

    \brief  Simple event based operating system


*/


#include "..\includes\allheaders.h" 




/*!
    \page os_design Operating system design

    This is simple event based OS. Rather scheduler then real operating system.

     - Each module has to provide:
       -# Task event function
       -# Task notification function
       -# Variable for module handle.

       Task event function will receive synchronous calls with events distributed by OS.
       Task notification function will receive asynchronous calls from other modules ( routed through
       notification distribution function). Notification functions are not allowed to call other notification functions,
       they are allowed to schedule events only ( if the operation cannot be executed immediately).

     - There is no preemption, everything is voulntary. There is only one common stack.
       There is no need for synchronisation in event tasks ( as only one is run at once). Only notification function
       have to be designed with care as they are called while other module is running.

     - The system provides timer scheduling - on expiration notification will be sent. Such timer can be rearmed to get
       periodical operation.

     - Interrupt functions can schedule special notification to be sent to notification function. Such function must determine
       itself cause of the interrupt ( nothing extra is passed)

     - The system sends initial notification to each module

     - Typical init - see main.c file
     - typical module - see algorithm.c file
    



*/


//! Max amount of tasks
#define X_MAX_MODULE    




//! Task descrtiption data structure
typedef struct
{
    //! Task fevent unction
    x_task_fn      task_fn;
    //! Task notification function
    x_notify_fn  notify_fn;
    //! Task name
    const char      task_name[10];
}x_module_info_t;



//! OS internal data strucutre
typedef struct
{
    //! Modules  info
    x_module_info_t    x_module_info[X_MAX_MODULE];
    //! Total moduilkes count
    Uint8            x_module_count;

    //! Event queue
    x_event             *   event_queue;
    //! Timer queue
    x_notify                timer_queue;


    //! Last processed system tick
    int                        sys_tick;



}x_system_t;


//! OS volatile internal data strucutre
typedef struct
{
    //! Bitcoded module interrupt notifications
    Uint16    x_irq_mask;
    //! System timer tick
    Uint16     irq_tick;

}x_system_volatile_t;


//! OS data
x_system_t            x_system;

//! OS volatile data
x_system_volatile_t    x_systemv;



//! OS timer tick function
#ifdef HI_TECH_C
void interrupt  _T5Interrupt(void) @ T5_VCTR
#else
void __attribute__((__interrupt__,auto_psv)) _T5Interrupt(void)
#endif
{
    x_systemv.irq_tick++;

    IFS1bits.T5IF = 0;
    SET_CPU_IPL(6);
   
}





/*!

    \brief  OS internal init


*/
void  x_init(void)
{
    x_system.x_module_count        = 0;
    x_system.event_queue           = NULL;
    x_system.timer_queue.link      = NULL;
    x_systemv.x_irq_mask           = 0;


    // Setup system - Timer 1
    
    T5CON = 0;                  // Clear the register
    T5CONbits.TCS   = 0;        // Internal clock Fosc/4
    T5CONbits.TCKPS = 1;        //    1:8 prescaling

    PR5 = ((FOSC_CPU/2)/8)/X_TIMER_TICK;
    
    
    IPC7bits.T5IP = 0x06;         // Set Timer5 Interrupt Priority Level
    IFS1bits.T5IF = 0;            //Clear the Timer1 Interrupt Flag
    IEC1bits.T5IE = 1;            //Enable Timer1 Interrupt Service Routine
    T5CONbits.TON = 1;            //Start Timer 1

}




/*!

    \brief  OS task init function

    \param task_fn  Module task event processing function
    \param notify_fn Module task notification processing function
    \param pHandle_id Pointer to module handle variable
    \param task_name Task name (null terminated string)


*/
void  x_init_task(x_task_fn task_fn,x_notify_fn notify_fn,x_task_id * pHandle_id,const char * task_name)
{
    if(x_system.x_module_count < X_MAX_MODULE)
    {
        x_system.x_module_info[x_system.x_module_count].task_fn   = task_fn;
        x_system.x_module_info[x_system.x_module_count].notify_fn = notify_fn;
        strncpy((char*)x_system.x_module_info[x_system.x_module_count].task_name,task_name,sizeof(x_system.x_module_info[x_system.x_module_count].task_name)-1);
        *pHandle_id = x_system.x_module_count++;
    }
    else
    {
        ASSERT(0);
    }
}



/*!

    \brief  OS function which starts event scheduling


*/
void  x_start(void)
{
    int                 jj;
    x_notify            ntf;


    ntf.task_id = -1;
    ntf.message = X_NTF_INIT;


    for(jj = 0; jj < x_system.x_module_count;jj++)
    {

        /*!

             \wdg Watchdog kick - before each module start
       */

        srv_wdg_kick();

        ntf.task_id = jj;
        D_I(fprintf(stdout,"Initalizing module:%s\n",x_system.x_module_info[jj].task_name);)
        x_system.x_module_info[jj].notify_fn(&ntf);
    }

}


/*!

    \brief  OS function which should be called in idle loop. This function processes events and notifications.


*/

void  x_loop(void)
{
    Uint16            tmp;
    Uint16            delta;
    int               ii;
    x_event           irq_event;
    x_notify          * timer_prev;
    x_notify          * timer_run;
    x_event           * event;
    x_event           * event_run;
    IRQ_CTX_T         irq_ctx;

    // Service interrupt sync requests
    irq_event.task_id = -1;
    irq_event.message = X_MSG_IRQ_SYNC;

    do
    {

        IRQ_DISABLE(irq_ctx);

        tmp = x_systemv.x_irq_mask;
        x_systemv.x_irq_mask = 0;



        IRQ_ENABLE(irq_ctx);

        if(tmp != 0)
        {
            for(ii = 0; ii < x_system.x_module_count;ii++)
            {
                if(tmp & ( 1<<ii))
                {
                   /*!

                      \wdg Watchdog kick - before executing notification function
                                           with interrupt notification


                 */

                    // Kick watchdog
                    srv_wdg_kick();
                    // Execute notification function
                    x_system.x_module_info[ii].task_fn(&irq_event);
                }
            }
        }
    }while(tmp != 0);


    // Service timers
    if(x_systemv.irq_tick != x_system.sys_tick)
    {
        delta =  x_systemv.irq_tick - x_system.sys_tick;
        x_system.sys_tick = x_systemv.irq_tick;

        timer_prev = &x_system.timer_queue;
        while(timer_prev != NULL)
        {
            //Scan through all timers
            timer_run  = timer_prev->link;
            if(timer_run != NULL)
            {
                if(timer_run->ticks > delta)
                {
                    timer_run->ticks -= delta;
                }
                else
                {
                    timer_run->ticks = 0;
                }

                if(timer_run->ticks == 0)
                {
                    // Remove timer
                    timer_prev->link = timer_run->link;
                    // Launch timer
                    timer_run->link   = NULL;
                    timer_run->active = 0;
                    D_T(fprintf(stdout,"Timer Notification time:%u Task name:%s msg:%x\n",X_TICK2MS(x_systemv.irq_tick),x_system.x_module_info[timer_run->task_id].task_name,timer_run->message);)
                    /*!
                        \wdg Watchdog kick - before executing notification function
                                        with timer notification

                 */
                    srv_wdg_kick();
                    x_system.x_module_info[timer_run->task_id].notify_fn(timer_run);
                 }
            }

            if(timer_prev->link == NULL)
            {
                break;
            }
            else
            {
                timer_prev = timer_prev->link;
            }
        }
    }


    // Service events
    event = x_system.event_queue;
    while(event != NULL)
    {
        // Send event
        event_run = event;

        // First remove event from queue
        x_system.event_queue = event->link;
        event = event->link;

        // Clean event link ( which marks as executed)
        event_run->link = NULL;
        D_E(fprintf(stdout,"sending event time:%u task:%s msg:%x\n",X_TICK2MS(x_systemv.irq_tick),x_system.x_module_info[event_run->task_id].task_name,event_run->message);)
        /*!
          \wdg Watchdog kick - before executing task event function
                      with event from system queue
       */
        srv_wdg_kick();
        x_system.x_module_info[event_run->task_id].task_fn(event_run);
    }


    /*!
      \wdg Watchdog kick - needed for idle cases

   */
    srv_wdg_kick();

}




/*!

    \brief  OS event scheduling function

    \param [in] event

    The user is expexted to fill: message,task_id.

    The event will be placed in FIFO queue and send (during OS loop exectuion) to target task event function

*/

void  x_send_event(x_event * event)
{
    x_event *    event_next;


    if(x_system.event_queue == NULL)
    {
        x_system.event_queue = event;
        event->link = NULL;
    }
    else
    {
        event_next = x_system.event_queue;

        // Go through all linked events
        while(event_next != NULL)
        {
            if(event_next == event)
            {
                D_E(fprintf(stdout,"Scheduling event :%s msg:%x\n",x_system.x_module_info[event->task_id].task_name,event->message);)
                D_P(fprintf(stdout,"Event already in queue");)
                
                return;
            }

            if(event_next->link  == NULL)
            {
                // No more events - link new one as last
                D_E(fprintf(stdout,"Scheduling new event:%s msg:%x\n",x_system.x_module_info[event->task_id].task_name,event->message);    )
                event_next->link = event;
                event->link = NULL;
                break;
            }
            else
            {
                event_next = event_next->link;
            }
        }
    }


}

/*!

    \brief  OS notification processing request

    \param [in] notify Pointer to OS notification object

    The user is expexted to fill message and task_id fields


*/

void  x_send_notify(x_notify * notify)
{
    D_N(fprintf(stdout,"Notification time:%u task:%s msg:%x\n",X_TICK2MS(x_systemv.irq_tick),x_system.x_module_info[notify->task_id].task_name,notify->message);)
    x_system.x_module_info[notify->task_id].notify_fn(notify);
}


/*!

    \brief  OS timer configuration request

    \param timer Pointer to timer object
    \param ticks Timer timeout in OS ticks

    The user is expexted to fill: message,task_id and ticks fields.
    If the timer is running, the timer period will be updated.



*/

void  x_schedule_timer(x_notify  *  timer,Uint16 ticks)
{

    if( ticks == 0)ticks = 1;
    timer->ticks = ticks;

    if( timer->active == 0)
    {
        // If not linked ( already scheduled) - link it
        timer->link = x_system.timer_queue.link;
        x_system.timer_queue.link = timer;
        timer->active = 1;
    }
}

/*!

    \brief  OS timer delete request

    \param timer Pointer to timer object

    If timer is running, it is taken away from active timer queue.


*/

void  x_delete_timer(x_notify  *  timer)
{
    x_notify  * timer_curr;

    if(timer->active == 1)
    {
        timer_curr = &x_system.timer_queue;

        while(timer_curr->link!= NULL)
        {
            if(timer_curr->link == timer)
            {
                // Timer found
                timer_curr->link = (timer_curr->link)->link;
                timer->link = NULL;
                timer->active = 0;
                return;
            }
            timer_curr = timer_curr->link;
        }

        // Does not exist ??
        ASSERT(0);
    }


}




/*!

    \brief  OS interrupt notification timer

    \param task_id  Task id to which timer notification should be sent

    The receiver will get X_MSG_IRQ_SYNC notification
*/
void  x_task_send_irq_sync(x_task_id task_id)
{
    IRQ_CTX_T   irq_ctx;


    if(task_id < X_MAX_MODULE)
    {
        IRQ_DISABLE(irq_ctx);

        x_systemv.x_irq_mask |= (1<< task_id);
        IRQ_ENABLE(irq_ctx);
    }
    else
    {
        ASSERT(0);
    }
}


/*!

    \brief  OS time function

    \retval Returns current system tick counter


*/

Uint16  x_tick(void)
{
    return x_systemv.irq_tick;
}




