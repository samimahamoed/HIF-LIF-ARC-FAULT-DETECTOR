/*! \file   srv_nov.c

    \brief  Low level NOV support
    
    
*/

#include "..\includes\allheaders.h"
#include "..\includes_prv\aq0X\algorithm_prv.h"




//! EEPROM data storage copy 1. 

data_non_volatile_t  	   data_non_volatile;

Uint32                	serial_number;
Uint8                 	nov_wr_inprogress;



static void nov_store_copy(Uint8 copy,int destroy_nov);


/*! Table of CRC values for high–order byte */
static unsigned char table_crc_hi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;

/*! Table of CRC values for low–order byte */
static char table_crc_lo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;



/*!

    \brief  Standard CRC16 routine

    \param [in] puchMsg character stream
    \param [in] usDataLen stream length

    \retval CRC16 value of given stream

    
*/

Uint16 crc16(unsigned char *puchMsg,Uint16 usDataLen)
{
    Uint8 crc_hi = 0xFF ;        /* high byte of CRC initialized */
    Uint8 crc_lo = 0xFF ;        /* low byte of CRC initialized */
    int table_index ;            /* index into CRC lookup table */
    
    while (usDataLen--)
    {
        table_index = crc_hi ^ *puchMsg++ ;            /* calculate the CRC */
        crc_hi = crc_lo ^ table_crc_hi[table_index] ;
        crc_lo = table_crc_lo[table_index] ;
    }
    return (crc_hi << 8 | crc_lo) ;
}



/*!

     \page srv_nov Non volatile storage

    The Nov block is responsible for non volatile storage of device data.


	For non volatile storage requirement of this application an external microchip 
	25LC640A series serial EEPROM is used for storing device configuration data and 
	disturbance records. The device can store 16KB data divided into 64Byte sized 
	pages and can be accessed using SPI interface via provided instructions. The 
	device supports data write operation per byte, per page size or any length data 
	size up to 64byte can be transferred in a single write operation if stored in the 
	same page. Likewise, data can be read per byte or the entire memory can be fetched 
	in a single read operation. Furthermore, the device also provides instructions to 
	access the status registers as well as read write control registers. 
	
	In this application there are  three types of EEPROM data blocks namely: 
		
	Device id: 
			   includes both device serial number and device address. Four copies 
	           of each of these values are stored in the beginning address of the 
			   eeprom. Thus, on every reset the data will be retrieved and will be 
			   considered valid if two or more successive address 
			   contains equal values .
			   
	Configuration data: 
			   contains the device configuration parameters and supervi-sion 
			   records. Here, the data is stored in two identical copies starting from 
			   second copy and then the first copy to make sure that one copy is valid 
			   if the execution is interrupted due to power failure. During startup, first 
			   copy is read and checked for proper checksum (crc 16). If valid, these 
			   parameters are used. If the copy is not valid, the module will attempt to 
			   use the second copy. If the second is also defective then this is fatal error 
			   hence the device will be configured with factory default values.
			   
       Disturbance record: 
	           mainly contains the modules disturbance record data including with
	           the event history recording parameters (e.g. relative timestamp). However,
			   it is possible that emulation mode data can also be preserved across reset 
			   using the same block (see typedef:post_act_record_buffer_t, nov_data.h file). 
			   Here, since this is considerably large amount of data only a single copy is 
			   stored and retrieved when external data transfer request is received.  

*/


/*!
   \brief  Reads NOV storage from EEPROM


    \retval 0 if succesfull NOV retrival was performed

    Note: emulation mode data may be mode preserved across resets on production verssion.

*/

#if(1)
Uint8 nov_init(void)
{
    Uint8                result = 1;
    Uint16               crc;
    device_data_t        tmp_data;
    Uint16               state = 0;

    /*!
        \req     REQ-nov_2a:    "CRC checksum protection"
                                CRC16 used.
    */



    memset(&data_non_volatile,0,sizeof(data_non_volatile));

    // Try first copy
    nov_memcpy(NOV_FIRST_DATA_SET_ADDRESS,&data_non_volatile,sizeof(data_non_volatile));

    crc = crc16((unsigned char*)&data_non_volatile,offsetof(data_non_volatile_t,device_data.configured_data_crc));



    if(crc == data_non_volatile.device_data.configured_data_crc)
    {

     

        // Copy valid
        result = 0;
        state |= 0x01;
    }
    else
    {
        //!first NOV copy is invalid
        state |= 0x02;
    }


   //   Try backup copy
      nov_memcpy(NOV_SECOND_DATA_SET_ADDRESS,&tmp_data,sizeof(tmp_data));
      crc = crc16((unsigned char*)&tmp_data,offsetof(device_data_t,configured_data_crc));

    if(crc == tmp_data.configured_data_crc)
    {
        state |= 0x04;
        if(result != 0)
        {
            state |= 0x08;
            // Only second copy valid - restore first copy configuration data
             memcpy(&data_non_volatile.device_data,&tmp_data,sizeof(data_non_volatile.device_data));
             nov_store_copy(0,0);
             result = 0;
        }
    }
    else
    {
        //! second NOV copy is invalid
        state |= 0x10;
        if(result == 0)
        {
            state |= 0x20;
            // First was valid, second corrupted. Restore second.
             nov_store_copy(1,0);
        }
    }

    return result;
}






/*!

    \brief  Function for NOV testing, destroys NOV CRC


    \param [in] copy    NOV copy index ( 0 or 1)


    \retval 0 if the operation was succesfull

            
*/

Uint8 nov_destroy(Uint8 copy)
{

    if(copy == 0)
    {
         nov_store_copy(0,0xDEAD);
    }
    else{
         nov_store_copy(1,0xDEAD);

    }
       
    return 0;
}



/*!

    \brief  Writes selected NOV storage copy to EEPROM

    \param [in] copy           NOV copy index ( 0 or 1)
    \param [in] destroy_nov    If set to 0xDEAD, then the NOV copy will have wrong checksum written


    Note: Updates both copies - one after another    
*/

static void nov_store_copy(Uint8 copy,int destroy_nov)
{
    Uint16         cc = 0;
    Uint16         nov_address;
    device_data_t  tmp_data;
     
    Int16 length;



     /*!
    
        \wdg  Watchdog kicks - Before each NOV storage operations - once, as erase may be 
              relatively ( few miliseconds).
                           
     */
     
     srv_wdg_kick();
    
    
    
     /*!
    
        \req  REQ-NOV_3 : "Allow continuous operation for 200 years"
                      There are no special algorithms for wearout. It is assmued that during normal operation
                      storage will not be used at all ( i.e. used only during installation and trips)
                           
     */
    



    data_non_volatile.device_data.configured_data_crc =
    crc16((unsigned char*)&data_non_volatile.device_data,offsetof(device_data_t,configured_data_crc));

    data_non_volatile.crc = crc16((unsigned char*)&data_non_volatile,offsetof(data_non_volatile_t,crc));



    if(destroy_nov == 0xDEAD)
    {
        tmp_data.configured_data_crc += 1;
    }


    if(copy == 1)
    {        
        //Initialize a variable to represent the Data EEPROM address - Copy 2
       nov_address = NOV_SECOND_DATA_SET_ADDRESS;
       length = sizeof(data_non_volatile.device_data);

    }
    else
    {
       // Initialize a variable to represent the Data EEPROM address - Copy 1
       nov_address = NOV_FIRST_DATA_SET_ADDRESS;
       length = sizeof(data_non_volatile);
    }
           

    

    while((length-NOV_PAGE_SIZE)> 0)
    {
        /*!

           \wdg  Watchdog kicks - Before each NOV storage operations - once, as erase may be 
                 relatively ( few miliseconds).
                              
       */
        
        srv_wdg_kick();

        IRQ_CTX_T         irq_ctx;
        IRQ_DISABLE(irq_ctx);
                            
       // data_size = NOV_PAGE_SIZE - (nov_address & NOV_PAGE_MASK);
        /*Write page size data to serial EEPROM  */
        if(copy == 0)
        nov_page_cpy(nov_address,(((void *)(&data_non_volatile))+cc) ,NOV_PAGE_SIZE);
        else
        nov_page_cpy(nov_address,(((void *)(&data_non_volatile.device_data) + cc)),NOV_PAGE_SIZE);

        IRQ_ENABLE(irq_ctx);

        nov_address += NOV_PAGE_SIZE;
        cc          += NOV_PAGE_SIZE;

        if(length > (NOV_PAGE_SIZE))
        length      -= NOV_PAGE_SIZE;
      
        
        
    }

      if(length > 0){
       if(copy == 0)
        nov_page_cpy(nov_address,(((void *)(&data_non_volatile))+cc),length);
        else
        nov_page_cpy(nov_address,(((void *)(&data_non_volatile.device_data)+cc)),length);
      }


    /*!
               
          \wdg Watchdog kick - after EEPROM operation                                                      
   */
     
    // Watchdog kick for goodbye
    srv_wdg_kick();

}


/*!

    \brief  Writes NOV storage to EEPROM


    Note: Updates both copies - one after another    
*/

void nov_store(void)
{

     nov_store_copy(1,0);
     nov_store_copy(0,0);
}


/*!

     \brief  Stores in EEPROM serial number

*/

void   nov_store_device_id(Uint32 id ,Uint8 type)
{
    Uint16       nov_address = NOV_DEVICE_ID_MEM_ADDRESS;
    void*        data = NULL;
    Uint8        size = 0;


    if(type == SERIAL_NUMBER){
    nov_address = NOV_SERIAL_NUMBER_MEM_ADDRESS;

    union
    {
        Uint32 tmp32[4];
        Uint8  tmp8[4*sizeof(Uint32)];
    }tmp;


    tmp.tmp32[0] = id;
    tmp.tmp32[1] = id;
    tmp.tmp32[2] = id;
    tmp.tmp32[3] = id;
    
    data = &tmp;
    size = sizeof(tmp);

    }else if(type == DEVICE_ADDRESS){
       Uint8  tmp[4];


        tmp[0] = id;
        tmp[1] = id;
        tmp[2] = id;
        tmp[3] = id;

        data = &tmp;
        size = sizeof(tmp);

    }
 
    nov_page_cpy(nov_address,data,size);

    
}



/*!

    \brief  Reads from EEPROM device serial number

*/


Uint32 nov_get_serial_number(void)
{


    union
    {
        Uint32 tmp32[4];
        Uint8  tmp8[4*sizeof(Uint32)];
    }tmp;



    nov_memcpy(NOV_SERIAL_NUMBER_MEM_ADDRESS,&tmp,sizeof(tmp));


    if(tmp.tmp32[0] == tmp.tmp32[1])
    {
        if(tmp.tmp32[0] != 0xFFFFFFFF || tmp.tmp32[0] != 0)
        {
            return  tmp.tmp32[0];
        }
    }

    if(tmp.tmp32[2] == tmp.tmp32[3])
    {
        if(tmp.tmp32[2] != 0xFFFFFFFF || tmp.tmp32[2] != 0)
        {
            return  tmp.tmp32[2];
        }
    }

    // backup plan
    return 0;
        
}


/*!

    \brief  Reads from EEPROM device serial number

*/


Int8 nov_get_device_address(void)
{

    Uint8  tmp[4];

    nov_memcpy(NOV_DEVICE_ID_MEM_ADDRESS,&tmp,sizeof(tmp));


    if(tmp[0] == tmp[1])
    {
        if(tmp[0] != 0xFF || tmp[0] != 0)
        {
            return  tmp[0];
        }
    }

    if(tmp[2] == tmp[3])
    {
        if(tmp[2] != 0xFF || tmp[2] != 0)
        {
            return  tmp[2];
        }
    }

    return -1;

}







/*!
    \brief set or reset write enable latch,
           the proceedure is drived from the information provided in
           microchip 25XX640A series serial eeprom dataseet.
    \Note  the function must be called prior to any attempt to write data to
           the 25XX640A with write enable command. and it's a recommended practice
           to reset this latch after the wright operation completed.


*/

void nov_set_write_enable_latch(Uint8 cmd)
{
    Uint8 cc;
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(cmd);
    SPI_SS_CTRL_LAT = 1;

}

/*!
    \brief   reads status register from EEPROM.

*/

NOV_STATREG nov_get_status()
{
    Uint8 cc;

    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_RDSR);
    cc = spi_put_data(0);
    SPI_SS_CTRL_LAT = 1;

    return (NOV_STATREG)cc;

}


/*!
    \brief   write EEPROM status register.

*/

void nov_write_status(Uint8 value)
{
    Uint8 cc;
    nov_set_write_enable_latch(NOV_CMD_WREN);
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_WRSR);
    cc = spi_put_data(value);
    SPI_SS_CTRL_LAT = 1;

    // wait for completion of previous write operation
    while(nov_get_status().bits.WIP);

    nov_set_write_enable_latch(NOV_CMD_WRDI);

}


/*!
    \brief   writes a single byte to the specified eeprom address.

*/

Uint8 nov_byte_write(Uint8 data,Uint16 address)
{
    Uint8 cc;
    nov_set_write_enable_latch(NOV_CMD_WREN);
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_WRITE);
    cc = spi_put_data((address>>8)&0x00ff);
    cc = spi_put_data(address&0x00ff);
    cc = spi_put_data(data);
    SPI_SS_CTRL_LAT = 1;

    // wait for completion of previous write operation
    while(nov_get_status().bits.WIP);

    nov_set_write_enable_latch(NOV_CMD_WRDI);

    return 0;
}


/*!
    \brief   reads single from the specified eeprom address.

*/

Uint8 nov_byte_read(Uint16 address)
{
    Uint8 cc;
    nov_set_write_enable_latch(NOV_CMD_WREN);
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_READ);
    cc = spi_put_data((address>>8)&0x00ff);
    cc = spi_put_data(address&0x00ff);
    cc = spi_put_data(0);
    SPI_SS_CTRL_LAT = 1;

    return cc;
}



/*!
    \brief  Function transfer data from RAM to EEPROM over spi port.

    \param [in] pointer to RAM memory address, i.e. to read from
    \param [in] length of the data (should be less than page size)
    \param [in] eeprom page address

    \Note    data can only be written until the end of the eeprom page.
             the page is either 64 bytes max (refer to datasheet).



*/

Uint8  nov_page_cpy(Uint16 page_address, void * src,Uint16 length)
{
	volatile Uint16 ii =1 ;
	

	while(page_address > ii*NOV_PAGE_SIZE)ii++;

	
	if((ii*NOV_PAGE_SIZE - page_address -1) < length)ASSERT(0);  /*page write operations attempts to cross page boundary.*/
	      
    if(length > ii*NOV_PAGE_SIZE)return 1;

    Uint8* src8 = (Uint8*)src;

    nov_wr_inprogress = 1;

    // Launch transmission
    Uint8 cc;
    nov_set_write_enable_latch(NOV_CMD_WREN);
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_WRITE);
    cc = spi_put_data((page_address>>8)&0x00ff);
    cc = spi_put_data(page_address&0x00ff);
    while (length > 0)
    {
           cc = spi_put_data(*(src8++));  
           length--;
    }
   
    SPI_SS_CTRL_LAT = 1;





    // wait for completion of previous write operation
    while(nov_get_status().bits.WIP);
    


    nov_set_write_enable_latch(NOV_CMD_WRDI);

    nov_wr_inprogress = 0;
    IFS0bits.SPI1IF = 0;   //no need but good to clear
    return 0;
}


/*!
    \brief  Standard library memcpy() replacement function to copy data from
            serial EEPROM to the specified RAM memory address
            via spi interface.

    \param [in] starting address of eeprom memory , i.e. to read from
    \param [in] pointer to the destination object which is in RAM
    \param [in] length of the data to copy


*/

Uint8  nov_memcpy(Uint16 starting_address,void * dest, Uint16 length)
{
    if((NOV_CHIP_SIZE-starting_address) < length)
        return 1;

    Uint8* dst8 = (Uint8*)dest;
    Uint8  cc;
    // Launch transmission
    SPI_SS_CTRL_LAT = 0;
    cc = spi_put_data(NOV_CMD_READ);
    cc = spi_put_data((starting_address>>8)&0x00ff);
    cc = spi_put_data(starting_address&0x00ff);

    while (length > 0)
    {
           *(dst8++) = spi_put_data(0);
           length--;
    }

    SPI_SS_CTRL_LAT =1;

    IFS0bits.SPI1IF = 0;   //no need but it's good to clear it
    return 0;
}



/*!

    \brief  Updates internal structures with default factory settings

    \param set_default_calibration  If set to non-zero value, then also analog calibration is overwritten


    Note: 
*/

void  nov_set_default(Uint8 overwrite)
{

        data_non_volatile.device_data.installation.system_info.freq                     = CONFIG_DEFAULT_FREQ;
        data_non_volatile.device_data.installation.system_info.rated_current            = CONFIG_DEFAULT_RATED_CURRENT;
        data_non_volatile.device_data.installation.system_info.sh_current               = CONFIG_DEFAULT_SHORT_CKT_CURRENT;
        data_non_volatile.device_data.installation.system_info.min_distance             = CONFIG_DEFAULT_MIN_INSTALLATION_DISTANCE;

        data_non_volatile.device_data.installation.options.bits.ir_element_sv_en        = TRUE;
        data_non_volatile.device_data.installation.options.bits.uv_element_sv_en        = TRUE;
        data_non_volatile.device_data.installation.options.bits.mfield_element_sv_en    = TRUE;

        data_non_volatile.device_data.installation.options.bits.threshold_adjust_manual = TRUE;


        data_non_volatile.device_data.installation.thresholds.ir_level_max              = CONFIG_DEFAULT_IR_LEVEL_MAX;
        data_non_volatile.device_data.installation.thresholds.uv_level_max              = CONFIG_DEFAULT_UV_LEVEL_MAX;
        
        data_non_volatile.device_data.installation.thresholds.lux_level_max             = CONFIG_DEFAULT_LUX_LEVEL_MAX;
        data_non_volatile.device_data.installation.thresholds.probablity_max            = CONFIG_DEFAULT_PROBABLITY_MAX;
        data_non_volatile.device_data.installation.thresholds.probablity_min            = CONFIG_DEFAULT_PROBABLITY_MIN;
        data_non_volatile.device_data.installation.thresholds.min_number_of_observation = CONFIG_DEFAULT_NO_OF_OBSERVATION;



        data_non_volatile.device_data.installation.trip_criteria.criterion.all          = DEFAULT_TRIP_CRITERION;


        
#if(DEVICE == AQ0X_V0)
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.ir           = TRUE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.uv           = TRUE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_x     = TRUE;
        #ifdef MFIELD_Y
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_y     = TRUE;
        #endif

#else
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.ir           = TRUE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.uv           = TRUE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.light        = FALSE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_x     = TRUE;
        data_non_volatile.device_data.installation.trip_criteria.active_modules.bits.mfield_y     = TRUE;
#endif



    {
        Uint16 cc;
        Uint16 zero_point_start = (algorithm_data.diagnosis.samples_per_period/4);
        Uint16 zero_point_end   = zero_point_start + 25;

        memset(&data_non_volatile.device_data.ref_signal,0,sizeof(data_non_volatile.device_data.ref_signal));

        for(cc = 0; cc < MAX_EVENT_OBSERVATION_LENGTH;cc++){

        if(!((cc > zero_point_end)&&(cc < zero_point_end)))
        data_non_volatile.device_data.ref_signal.buffer[cc]  = abs(cos(2*PI*CONFIG_DEFAULT_FREQ*FAST_TIMER_SAMPLING_RATE*cc)*ADC_RESOLUTION);

        data_non_volatile.device_data.ref_signal.mean_value +=data_non_volatile.device_data.ref_signal.buffer[cc];

        }


          data_non_volatile.device_data.ref_signal.mean_value /=MAX_EVENT_OBSERVATION_LENGTH;

          data_non_volatile.device_data.ref_signal.std_deviation = algorithm_get_std_deviation(data_non_volatile.device_data.ref_signal.buffer
                                                                                               ,data_non_volatile.device_data.ref_signal.mean_value
                                                                                               ,MAX_EVENT_OBSERVATION_LENGTH);

          data_non_volatile.device_data.ref_signal.valid = 0x0000;

    }
        if(overwrite == NOV_OVERWRITE_ALL)
        {
            //TODO: Calibration data



        }
}



#endif

