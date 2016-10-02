/*! \file   Readme.h

    \brief  Documentation readme file
 */




/*!


 \mainpage AQ0X project readme.

 \section IDE
 The project is created with MPLAB 8.92 ,
 However, The new MPLAB X can also be used to open the project 
 
 \section files File organisation
    The project files have been placed in following directories
    - mplab\\doxygen         - Doxygen related configuration files and doxygen output
    - mplab\\includes        - Public  include files from all functional/service/system modules
    - mplab\\includes_prv    - Private include files from all functional/service/system modules. These files are private for modules.                              
    - mplab\\services        - Low-level stateless driver modules ( called services in this project)
    - mplab\\system          - OS files and Microchip processor specific files 
    - mplab\\modules         - OS dependent and product specific system modules
                          

 \section os Operating system

    The OS used in this project is simple event based scheduler. 
    See here for details: @ref os_design


   
 \section measurements Code timing

    
    SW 18.05.2014
        The main load comes from 100us interrupt which scans sensors.
        CPU load 
         - 80% of CPU time from the fast timer interrupt
         - 30% of CPU time is used of operating system routine with the exception 2.5s during startup

 \section  genpri Generic principles
   
   - The OS is started in ( main.c ) file together with some OS independent services
   - The OS calls ( by sending system notification) to all modules ( see e.g. algorithm_notifyx() and algorithm_init() , all 
     modules are desgined like that )
   
   - The modules prepare system objects ( event/notification objects for using during runtime) and schedule timers for its
     periodical operations ( e.g. analog_init() )
   - The Algortihm module ( algorithm.c ) launches NOV read and sends configuration notifications to other modules 
   - The modules execute supervison timers, in case of any alert send notifications to Algorithm module.
   - The Algorithm module gets all notifications and sends event to itself to process them one by one in its main task loop.
     See algorithm_notifyx() and algorithm_wake()
    


  


 \section  wdglist Watchdog related processing
      
    See @ref wdg

 \section  nov Non volatile storage
      
    See @ref srv_nov
	


 
   
  \section  hwio Software validation 

   The firmware supports hardware reading redirection to allow testing firmware functionality.
   Basically, user can replace real hw reading with values supplied by serial commands


    \section  requirements List of requirements supported directly by the code
      
   See @ref req

   
*/
   
   



   


 



