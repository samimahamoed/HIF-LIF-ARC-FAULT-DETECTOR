#ifndef SRV_SERIAL_H
#define SRV_SERIAL_H
/*! \file   srv_serial.h

    \brief  Header for serial port driver
    
    
*/






//! Serial port baudrate UART1
#define BAUDRATE                 9600

#define MAX_PACKET_LENGTH        240
                           
//! Receive buffer length
#define RECEIVE_BUFFER_SIZE      MAX_PACKET_LENGTH

//! Transmit buffer length
#define TRANSMIT_BUFFER_SIZE     MAX_PACKET_LENGTH


typedef struct 
{
      Uint16 cmd;
      Uint16 arg[10/*max arg length */];
      Uint8  arg_cnt;

      struct{
      void         *location;
      Uint16        offset;
      Uint16        length;
      }attachment;

      Bool manula_mode_crc_en;

}serial_msg_t;


/*
#define MAKE16(buf_,idx_) ((HEX2VAL(buf_[idx_+0])<<12)|(HEX2VAL(buf_[idx_+1])<<8)|(HEX2VAL(buf_[idx_+2])<<4)|(HEX2VAL(buf_[idx_+3])<<0))
#define MAKE8(buf_,idx_)  ((HEX2VAL(buf_[idx_+0])<<4)|(HEX2VAL(buf_[idx_+1]))<<0)
*/

//! Charcter to hex value conversion
#define HEX2VAL(cc)  (((cc >= '0' ) && (cc <= '9' )) ? (cc - '0') : (cc - 'A' + 10) )


void    			serial_init(void);
void    			serial_enable_blocking(int enable);
Uint8   			serial_get_command(Uint16 * cmd,Uint8 ** argStrPtr,Uint16 * argLength);
void    			serial_get_command_ack(void);
void   	 			serial_send_command(Uint16  cmd,Uint16  arg);
void                            serial_send_data(Uint16 cmd,Uint16  arg,void* data,Uint16 offset,Uint16 length,Bool add_crc);
void    			serial_send(void * msg,Uint16 length);
void                            serial_send_response(serial_msg_t msg);


Uint8   			serial_make8(Uint8* buf,Uint16 idx);
Uint16  			serial_make16(Uint8* buf,Uint16 idx);







void __attribute__((__interrupt__,auto_psv)) _U1RXInterrupt(void);

/*void __attribute__((__interrupt__)) _INT1Interrupt(void);
void __attribute__((__interrupt__)) _INT2Interrupt(void);


void __attribute__((__interrupt__)) _IC1Interrupt(void);
void __attribute__((__interrupt__)) _IC2Interrupt(void);*/


#endif //SRV_SERIAL_H



