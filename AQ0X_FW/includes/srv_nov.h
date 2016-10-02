#ifndef SRV_NOV_H
#define SRV_NOV_H
/*! \file   srv_nov.h

    \brief  Header for low level NOV support
    
    
*/


#define lo(_x)   (unsigned char)(_x&0x00ff)
#define hi(_x)   (unsigned char)((_x>>8)&0x00ff)

//type specifier
#define SERIAL_NUMBER                          0
#define DEVICE_ADDRESS                         1


#define NOV_OVERWRITE_ALL                                      0xED

//! defines the size of the serial EEPROM in bytes
#define NOV_CHIP_SIZE                                          16384
//! defines the size of the serial EEPROM internal page in bytes.
#define NOV_PAGE_SIZE                                          64


//! defines the size of the serial EEPROM in bytes

#define NOV_SERIAL_NUMBER_MEM_ADDRESS                          0

#define NOV_DEVICE_ID_MEM_ADDRESS                              NOV_SERIAL_NUMBER_MEM_ADDRESS + sizeof(Uint32)

#define NOV_FIRST_DATA_SET_ADDRESS                             NOV_PAGE_SIZE

#define NOV_SECOND_DATA_SET_ADDRESS                            ((NOV_CHIP_SIZE - 20*NOV_PAGE_SIZE))  //sets 1280 byte


/* 25AA640A/25LC640A serial eeprom interface commands */

#define NOV_HOLD_CTRL_TRIS                                     TRISBbits.TRISB13
#define NOV_HOLD_CTRL_LAT                                      LATBbits.LATB13
#define NOV_CMD_READ                                          (Uint8)0b00000011
#define NOV_CMD_WRITE                                         (Uint8)0b00000010
#define NOV_CMD_WRDI                                          (Uint8)0b00000100
#define NOV_CMD_WREN                                          (Uint8)0b00000110
#define NOV_CMD_RDSR                                          (Uint8)0b00000101
#define NOV_CMD_WRSR                                          (Uint8)0b00000001


/* the mask size depends on page size */
#if   NOV_PAGE_SIZE == 16
#define NOV_PAGE_MASK         0xf
#elif NOV_PAGE_SIZE == 32
#define NOV_PAGE_MASK         0x1f
#elif NOV_PAGE_SIZE == 64
#define NOV_PAGE_MASK         0x003f
#endif


//! serial eeprom device status register structure
typedef union{
    Uint8 all;
    struct{
    Uint8    WIP:1;
	Uint8    WEL:1;
	Uint8    BP0:1;
	Uint8    BP1:1;
	Uint8    RESERVED:3;
	Uint8    WPEN:1;
    }bits;
}NOV_STATREG;

extern Uint8     nov_wr_inprogress;


void             nov_store_device_id(Uint32 id,Uint8 type);
Uint8            nov_init(void);
void             nov_store(void);
void             nov_set_default(Uint8 overwrite);
Uint32           nov_get_serial_number(void);
Uint8            nov_destroy(Uint8 copy);
void             nov_set_write_enable_latch(Uint8 cmd);
NOV_STATREG      nov_get_status();
void             nov_write_status(Uint8 value);
Uint8            nov_byte_write(Uint8 data,Uint16 address);
Uint8            nov_byte_read(Uint16 address);
Uint8            nov_page_cpy(Uint16 page_address, void * src,Uint16 length);
Uint8            nov_memcpy(Uint16 starting_address,void * dest, Uint16 length);


#endif //SRV_NOV_H





