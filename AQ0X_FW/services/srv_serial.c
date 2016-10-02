/*! \file   srv_serial.c

    \brief  Serial port driver


*/


#include "..\includes\allheaders.h" 


//! Formula for U1BRG register ( baudrate control)
#define BRGVAL          ((FCY_CPU/BAUDRATE)/16)-1

//! Charcter to hex value conversion
#define HEX2VAL(cc)  (((cc >= '0' ) && (cc <= '9' )) ? (cc - '0') : (cc - 'A' + 10) )

#define CMD_RESPONSE_MSG_HEADER_SIZE     10
#define DATA_TX_MSG_HEADER_SIZE          21
#define TX_MSG_HEADER_FRAME_CHAR         '{'
#define RC_MSG_HEADER_FRAME_CHAR         '['
#define ACK_CHAR                         "06"


pre_act_record_buffer_t pre_data;
#if(DMA_SUPPORT == FALSE)
//! Serial port internal data
typedef struct
{
   //! Receive buffer
    Uint8     rc_buffer[RECEIVE_BUFFER_SIZE];
    //! Recevied message length
    Uint16    rc_data_length;
    //! If set, received message was collected
    Uint8     msg_ready;

    //! If set, then serial sending is blocking ( no data lost)
    //! Otherwise, characters  which do not fit into buffer are ignored
    int    blocking_mode;

    //! Transmit buffer ( circular)
    Uint8     tx_buffer[TRANSMIT_BUFFER_SIZE];
    //! Transmit buffer head
    Uint16    tx_buff_head;
    //! Transmit buffer tail
    Uint16    tx_buff_tail;

    //! Diagnostic counter - transmit message count
    Uint16  tx_cnt;

    //! Diagnostic counter - received message count
    Uint16  rc_cnt;

    Uint16  irq_cnt;
}serial_data_t;

#else

//! Serial port internal data
typedef struct
{
       struct{
        unsigned int tx_a[TRANSMIT_BUFFER_SIZE/2] __attribute__((space(dma)));
        unsigned int tx_b[TRANSMIT_BUFFER_SIZE/2] __attribute__((space(dma)));

       //! Receive buffer
        unsigned int rx_a[RECEIVE_BUFFER_SIZE/2] __attribute__((space(dma)));
        unsigned int rx_b[RECEIVE_BUFFER_SIZE/2] __attribute__((space(dma)));
        }dma_buffer;

}serial_data_t;

#endif



//! srv_serial service internal data
volatile serial_data_t serial_data;




#if(DMA_SUPPORT == TRUE)
/*!
    \brief Low level analog
*/
void   serial_configure_dma0_uart_tx_init(void)
{

    DMA0CON = 0x2001;               // One-Shot, Post-Increment, RAM-to-Peripheral
    DMA0CNT = 100;                  /*Number of dma byte transfer count,
                                     This is the maximum rc buffer size at the relay side */

    DMA0REQ = 0x0C;               // peripheral IRQ to the DMA channel  0001100

    DMA0PAD = (volatile unsigned int) &U1TXREG;
    DMA0STAH:DMA0STAL = __builtin_dmaoffset(serial_data.dma_buffer.tx_a);
   // DMA0STBH:DMA0STBL = __builtin_dmaoffset(serial_data.dma_buffer.tx_b);

    IFS0bits.DMA0IF = 0;            // Clear DMA Interrupt Flag
    IEC0bits.DMA0IE = 1;            // Enable DMA interrupt

    DMA0CONbits.CHEN = 1;           // Enable DMA Channel

}


/*!
    \brief Low level analog
*/
void   serial_configure_dma1_uart_rx_init(void)
{
DMA1CON = 0x0002;                   // Continuous, Ping-Pong, Post-Inc., Periph-RAM
DMA1CNT = 100;                      // 10 DMA requests
DMA1REQ = 0x000B;                   // Select UART1 Receiver
DMA1PAD = (volatile unsigned int) &U1RXREG;
DMA1STAH:DMA1STAL = __builtin_dmaoffset(serial_data.dma_buffer.rx_a);
DMA1STBH:DMA1STBL = __builtin_dmaoffset(serial_data.dma_buffer.rx_b);
IFS0bits.DMA1IF = 0;                // Clear DMA interrupt
IEC0bits.DMA1IE = 1;                // Enable DMA interrupt
DMA1CONbits.CHEN = 1;               // Enable DMA Channel

}


#endif




/*!

    \brief  Init function for srv_serial service


*/
static void serial_enable_transmitter(Bool enable)
{

    if(enable){
    ODCBbits.ODCB11    = 0;
    U1STAbits.UTXEN    = 1;     // enable UART TX
    //LATBbits.LATB11    = 0;   //temporary solution to keep
                                //the communication bus voltage low
    }else{
    ODCBbits.ODCB11    = 1;
    U1STAbits.UTXEN    = 0;     // disable UART TX
    }
}



/*!

    \brief  Init function for srv_serial service


*/
void     serial_init(void)
{

    //gpio configuration and peripheral mapping procedure
         __builtin_write_OSCCONL(OSCCON & 0xbf);
             TRISBbits.TRISB10 = 1;
             ODCBbits.ODCB11   = 0;
             TRISBbits.TRISB11 = 0;
      
        
             RPOR4bits.RP43R   = 1; //TX1
             RPINR18bits.U1RXR = 0x2A;
         __builtin_write_OSCCONL(OSCCON | 0x40);


    U1MODE = 0x0000;            //Clear UART1 registers
    U1STA  = 0x0000;

    U1MODEbits.UARTEN = 1;      //Enable UART1 module
    U1MODEbits.PDSEL  = 0;      // 8 bits, no parity
    U1MODEbits.STSEL  = 0;      // 1 stop bit

    U1BRG = BRGVAL;             //Load UART1 Baud Rate Generator

    IFS0bits.U1RXIF   = 0;      //Clear UART1 Receiver Interrupt Flag
    IFS0bits.U1TXIF   = 0;      //Clear UART1 Transmitter Interrupt Flag
    IEC0bits.U1RXIE   = 1;      //Enable UART1 Receiver Interrupt
    IEC0bits.U1TXIE   = 1;      //Enable UART1 Transmitter Interrupt

    U1STAbits.UTXISEL0 = 0;     // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.UTXINV   = 0;     // U1TX Idle state high

    U1STAbits.URXISEL  = 1;     //Setup UART1  receiver to interrupt after character received
    IPC2bits.U1RXIP    = 0x07;
    

    U1MODEbits.UARTEN  = 1;     // Enable UART

    serial_enable_transmitter(FALSE);
    

    serial_data.rc_data_length = 0;
    serial_data.tx_buff_head = serial_data.tx_buff_tail = 0;

    serial_enable_blocking(TRUE);
    serial_get_command_ack();



}



/*!

    \brief  Init function for srv_serial service


*/
void    serial_enable_blocking(int enable)
{
        serial_data.blocking_mode = enable;
}



/*!

    \page srv serial communication


    The module is responsible for external serial communications. Here, two interface
    helper functions are provided for data transmission and reading incoming message
    (see serial_send_response(), serial_get_command(,,)).

    In these application only algorithim module can access these functions.
    Basically the implemented operation principle is that, during receiver
    interrupt the module collects incoming data to receive buffer and sends
    interrupt notification to algorithm module after successful validation
    procedure (see _U1RXInterrupt()). On the other hand, for data transmission
    request the module copies the message to transmit buffer and starts sending
    out data on every transmit interrupt (see _U1TXInterrupt()). Here, sending can
    be configured in blocking mode or non-blocking mode. In the latter case data which
    can?t fit into the buffer will be lost (see serial_send(,)). Whereas, in the first
    case (the default) the serial message will be blocking till the transmission is completed.
    However, this doesn?t affect protection functionality serviced by high priority interrupt.


*/




/*!
     \brief  Function for retriving serial message

     \param [in]     cmd        response command
     \param [in,out] argStrPtr  pointer to be updated with argument buffer
     \param [in,out] argLength  pointer for argument buffer length

*/


Uint8   serial_get_command(Uint16 * cmd,Uint8 ** argStrPtr,Uint16 * argLength)
{
    Uint8 result = 1;
    Uint16 crc,calc_crc = 0;
   

    

    if(serial_data.msg_ready != 0)
    {

        // Minimal message - 16 characters
        // [0000:000000:00]
        // 01234567890123
        // 00000000001111
        // Longer messages look like
        // [0000:000000000000000  ... 00000000000:00]


        if(serial_data.rc_data_length >= 14 )
        {

            *cmd = serial_make16((Uint8*)serial_data.rc_buffer,1);

            if( (serial_data.rc_buffer[serial_data.rc_data_length-5]=='C') && (serial_data.rc_buffer[serial_data.rc_data_length-4]=='C'))
            {
                // Manual mode, "CCCC" charaters instead of checksum
                result = 0;
            }
            else
            {

                
                crc  = serial_make16((Uint8*)serial_data.rc_buffer,serial_data.rc_data_length-5);
                
           
                calc_crc = crc16((unsigned char*)serial_data.rc_buffer,serial_data.rc_data_length-6);
              /*
                for(ii = 0; ii < 11;ii++)
                {
                    calc_crc = calc_crc ^serial_data.rc_buffer[ii];
                }
              */
                if(crc == calc_crc)
                {
                    result = 0;
                }
            }

            *argStrPtr =  ((Uint8*)&serial_data.rc_buffer[6]);
            *argLength = serial_data.rc_data_length - 12;

       }

       if(result != 0)
       {
            // Faulty message, internmal ack
            serial_get_command_ack();
       }

    }




    IEC0bits.U1RXIE = 1;


    return result;
}






/*!
    \brief  Function which marks serial buffer as processed, so new command can be taken
*/

void  serial_get_command_ack(void)
{
    IEC0bits.U1RXIE = 0;
    serial_data.rc_data_length = 0;
    serial_data.msg_ready = 0;
    IEC0bits.U1RXIE = 1;
}









/*!
    \brief  Function converting characters to 8 bit value

    \param [in] buf   buffer with characters
    \param [in] idx   index to first character of 8 bit hex number string ( two characters used )

*/

Uint8  serial_make8(Uint8* buf,Uint16 idx)
{
    return ((HEX2VAL(buf[idx+0])<<4)|(HEX2VAL(buf[idx+1]))<<0);
}


/*!
    \brief  Function converting characters to 16 bit value

    \param [in] buf   buffer with characters
    \param [in] idx   index to first character of 16 bit hex number string ( four charaters used)

*/

Uint16  serial_make16(Uint8* buf,Uint16 idx)
{
    return ((HEX2VAL(buf[idx+0])<<12)|(HEX2VAL(buf[idx+1])<<8)|(HEX2VAL(buf[idx+2])<<4)|(HEX2VAL(buf[idx+3])<<0));

}






/*!
    \brief  Function sending command responses on serial line.

    \param [in] cmd response command
    \param [in] arg response argument


*/



/*!
    \brief  Function sending command responses on serial line.

    \param [in] cmd response command
    \param [in] arg response argument


*/

void   serial_send_response(serial_msg_t msg)
{
  
    // {CMD:CHSN00000000....:crc}
    // 0123456789012345
    // 0000000000111111



   //during this time i.e. while processing command, all incoming messages are ignored
   //hence, the receive buffer will be used temporarily while preparing response msg

   char   *message = (char *)(serial_data.rc_buffer);

   Uint16  length = 0;


  Uint16 type =  (((msg.cmd&0x00F0)>>4)== 0xE)?0xE:
                 ((((msg.cmd&0x0F00)>>8)==6)?6:3);

 if(msg.attachment.length > 0)type = 0xE; else type = msg.arg_cnt;

 switch(type)
   {
       case 3/*3 arg msg*/:
       {

       sprintf(message,"{%04X:%02X%02X%04X:",msg.cmd,msg.arg[0],msg.arg[1],msg.arg[2]);

       sprintf(message+15,"%04X}",crc16((Uint8*)message,14));

       length = 20;
       }break;


       case 4/*4 arg msg*/:
       {    sprintf(message,"{%04X:%02X%02X%04X%04X:",msg.cmd,msg.arg[0],msg.arg[1],msg.arg[2],msg.arg[3]);

       sprintf(message+19,"%04X}",crc16((Uint8*)message,18));

       length = 24;

       }break;


       case 0xE /*data transfer*/:
       {

       const char cnv[] = "0123456789ABCDEF";

       memset(message,0,sizeof(message));

       sprintf(message,"{%04X:%02X%02X%04X%04X",msg.cmd,
                                            msg.arg[0],msg.arg[1],
                                            msg.arg[2],msg.arg[3]);

       //if(msg.attachment.offset == 0)
       //  memcpy((void *)((void *)&pre_data + msg.attachment.offset),(void *)(msg.attachment.location + msg.attachment.offset),sizeof(pre_act_record_buffer_t));

       Uint16 var;
       Uint16 cc = 0;
/*
       for (var = 18; cc < msg.attachment.length; ) {

                   message[var]   =
                    cnv[(*((((Uint8 *)&pre_data + msg.attachment.offset))+(cc))>>4)&0x0F];

                   message[var+1]   =
                    cnv[(*((((Uint8 *)&pre_data + msg.attachment.offset))+(cc++))>>0)&0x0F];

                   var+=2;
       }
*/
   
       for (var = 18; cc < msg.attachment.length; ) {

                   message[var]   =
                    cnv[(*((((Uint8 *)msg.attachment.location + msg.attachment.offset))+(cc))>>4)&0x0F];

                   message[var+1]   =
                    cnv[(*((((Uint8 *)msg.attachment.location + msg.attachment.offset))+(cc++))>>0)&0x0F];

                   var+=2;
       }

  
     

       message[var++]=':';

       sprintf(message+var,"%04X}",crc16((Uint8*)message,var-1));

       length = var+5;
       
       }break;


        default:
        {

            ASSERT(0);
        }

 }



    serial_send(message,length);
}




#if(DMA_SUPPORT == FALSE)

/*!
    \brief  Function waits until all characters in transmit buffer have been sent

*/


static void  serial_wait_sent(void)
{
    Uint16 tmp;


    /*!
        \wdg  Watchdog kick - Sending serial messages may be blocking for a while
                (in case the buffer is full and the sending is configured for blocking mode).
                Worst case is buffer full - then each character takes about 200us ( with 57600 baudrate).
                Note:
                - this is default configuration ( by default characters should not lost)
                - this does not interfer with tripping ( which is serviced by high priority interrupt),


                We kick watchdog:
                - Before sending
                - After each character being sent.
                - After all characters were sent.

    */

    srv_wdg_kick();

    tmp = serial_data.tx_buff_tail ;
    while(serial_data.tx_buff_tail != serial_data.tx_buff_head)
    {
        if(tmp != serial_data.tx_buff_tail /*character is transmitted*/)
        {
           tmp = serial_data.tx_buff_tail;
           srv_wdg_kick();
        }
    }
    srv_wdg_kick();
}


/*!
    \brief  Function sending buffers over serial port

    \param [in] buffer character buffer to be sent
    \param [in] length buffer length

*/

void  serial_send(void * msg,Uint16 length)
{
  
    Uint16 TrNewHead;
    Uint8  dummy;

    CChar * buffer = (CChar *)msg;


    IEC0bits.U1TXIE    = 0;

    serial_enable_transmitter(TRUE);

    Delay5us(100);
    
    while((U1STAbits.URXDA != 0))dummy = U1RXREG;
    


    if(length > 0)
    {
        if(serial_data.tx_buff_tail == serial_data.tx_buff_head)
        {
            // Launch transmission ( if not already in progress )
            while(U1STAbits.UTXBF!=0)continue; // wait unitl FIFO is available
            U1TXREG = *buffer;
            buffer++;
            length--;
        }
    }

    while (length > 0)
    {
        TrNewHead = (serial_data.tx_buff_head + 1)%TRANSMIT_BUFFER_SIZE;

        if(TrNewHead != serial_data.tx_buff_tail)
        {
            serial_data.tx_buffer[serial_data.tx_buff_head] = *buffer;
            buffer++;
            length--;
            serial_data.tx_buff_head = TrNewHead;
        }
        else
        {
            if(serial_data.blocking_mode == 0)
            {
                // Cut the message - does not fit into buffer :-(
                length = 0;
            }
            else
            {
                IEC0bits.U1TXIE   = 1;
                serial_wait_sent();
                IEC0bits.U1TXIE   = 0;

                serial_enable_transmitter(FALSE);

                while((U1STAbits.URXDA != 0))dummy = U1RXREG;
                IEC0bits.U1RXIE   = 1;
                length = 0;
            }
        }

    }

    IEC0bits.U1TXIE   = 1;

}





/*!
    \brief  Serial port receive interrupt

*/

#ifdef HI_TECH_C
void interrupt _U1RXInterrupt(void) @ U1RX_VCTR
#else
void __attribute__((__interrupt__,auto_psv)) _U1RXInterrupt(void)
#endif
{
     

     
    Uint8 cc;
    
    
   srv_wdg_kick();
   


   
  while (U1STAbits.URXDA != 0)
    {
		
		if( (U1STAbits.FERR)||(U1STAbits.PERR))
		{
			cc = U1RXREG;
			continue;
		}

     
        cc = U1RXREG;
    
      

        if(serial_data.msg_ready != 0)
		{
			// Ignore messages until the old one is processed
			continue;
		}

        if(serial_data.rc_data_length >= sizeof(serial_data.rc_buffer))
        {
            serial_data.rc_data_length = 0;
        }

   
        if(cc == '[' )
        {
            serial_data.rc_data_length = 0;
            serial_data.rc_buffer[serial_data.rc_data_length++] = cc;
        }
        else if (cc == ']')
        {
            serial_data.rc_buffer[serial_data.rc_data_length++] = cc;

            // Full message arrived
            serial_data.msg_ready = 1;
            serial_data.irq_cnt++;
        }
        else
        {        
            if ( ((cc >= '0')&&(cc <= '9')) || ((cc >= 'A')&&(cc <= 'F')) || ( cc == ':') )
            {
                serial_data.rc_buffer[serial_data.rc_data_length++] = cc;
            }
            else
            {
                serial_data.rc_data_length = 0;
            }                          
        }
    
    }
    
	// Clear errors
	if(U1STAbits.OERR)
	{
		U1STAbits.OERR = 0;
	}
		 







    if((serial_data.msg_ready == 1)&&(serial_data.rc_buffer[0]=='['))
	     algorithm_message_arrived_ntf_isr(IRQ_SOURCE_SERIAL);
           
  
 
    
    IFS0bits.U1RXIF = 0;  

    SET_CPU_IPL(7);
}



/*!
    \brief  Serial port transmit interrupt

*/
#ifdef HI_TECH_C
void interrupt  _U1TXInterrupt(void) @ U1TX_VCTR
#else
void __attribute__((__interrupt__,auto_psv)) _U1TXInterrupt(void)
#endif
{
   Uint8 dummy;
    while ((serial_data.tx_buff_tail != serial_data.tx_buff_head) && (U1STAbits.UTXBF==0))
    {
        U1TXREG = serial_data.tx_buffer[serial_data.tx_buff_tail];
        serial_data.tx_buff_tail = (serial_data.tx_buff_tail +1)%TRANSMIT_BUFFER_SIZE;
        serial_data.tx_cnt++;
        while(U1STAbits.TRMT == 0);
        while(U1STAbits.URXDA !=0)dummy = U1RXREG;

        if(serial_data.tx_buff_tail == serial_data.tx_buff_head){

          if((logv.data_tx_on_progress.all&0xF0) != 0)
              /*the last packet of data transfer response just completed */
              logv.data_tx_on_progress.all = 0;

          while(U1STAbits.URXDA !=0)dummy = U1RXREG;
          if(U1STAbits.OERR)U1STAbits.OERR = 0;
          serial_enable_transmitter(FALSE);
          IEC0bits.U1RXIE = 1;



        }
    }
    IFS0bits.U1TXIF = 0;

    SET_CPU_IPL(5);
}
#endif


#if(DMA_SUPPORT == TRUE)

void __attribute__((__interrupt__,no_auto_psv)) _U2EInterrupt(void)
{
 // An error has occured on the last
 // reception. Check the last recieved
 // word.

 _U2EIF = 0;
 int lastWord;

 if(DMAPPSbits.PPST1 == 0)
 {
 // Get the last word received from ping pong buffer A.
 lastWord = *(unsigned int *)((unsigned int)serial_data.dma_buffer.rx_a + DMA1STAH:DMA1STAL);
 }
 else
 {
 // Get the last word received from ping pong buffer B.
 lastWord = *(unsigned int *)((unsigned int)(serial_data.dma_buffer.rx_b) + DMA1STBH:DMA1STBL);
 }

 if((lastWord & 0x800) != 0)
 {
 //Check for Parity Error
  //wait for re transmission ,do nothing for now
 }
 // Check for framing error
 if ((lastWord & 0x400) != 0)
 {
 // framing error
   //wait for re transmission ,do nothing for now
 }
}

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
	IFS0bits.DMA0IF = 0;			// Clear the DMA0 Interrupt Flag;
        //flag set

}


void __attribute__((__interrupt__)) _DMA1Interrupt(void)
{
 static unsigned int BufferCount = 0; // Keep record of which buffer
// contains RX Data
 if(BufferCount == 0)
 {


 }
 else
 {

 }

 BufferCount ^= 1;
 IFS0bits.DMA1IF = 0; // Clear the DMA1 Interrupt Flag
}



#endif



