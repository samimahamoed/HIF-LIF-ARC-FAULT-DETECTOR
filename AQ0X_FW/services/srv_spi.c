/*! \file   srv_spi.c

    \brief  spi port driver


*/



#include "..\includes\allheaders.h"





//! spi port internal data
typedef struct
{

}spi_data_t;



/*!

    \brief  Init function for srv_spi service


*/
void     spi_init(void)
{


	// setup the SPI1 port
	
            SPI_SCK_TRIS        = 0;
            SPI_SDO_TRIS        = 0;
            SPI_SDI_TRIS        = 1;
            SPI_SS_TRIS         = 0;
            SPI_SS_CTRL_LAT     = 1;
            SPI_SDO_ANSEL       = 0;
            
           NOV_HOLD_CTRL_TRIS   = 0;
           NOV_HOLD_CTRL_LAT    = 1;
           nov_wr_inprogress    = 0;


      //disable SPI module
       SPI1STATbits.SPIEN       = 0;
       SPI1STATbits.SPISIDL     = 0;
    

// Configure SPI1CON register
    
        IFS0bits.SPI1IF         = 0;    // Clear the Interrupt flag
        IEC0bits.SPI1IE         = 0;    // Disable the interrupt

        SPI1CON1bits.MSTEN      = 1;   //Enable Master mode

        SPI1CON1bits.MODE16     = 0;  //Communication is byte-wide
        SPI1CON1bits.CKP        = 0;   //Idle state for clock is a high level
        SPI1CON1bits.CKE        = 1;   //Data out on Active to Idle Edge

        SPI1CON1bits.SMP        = 0;  /*Master mode => Input data is sampled at
                                        the middle of data output time */

        //TODO: Fsck = Fp/(Primary Prescaler * Secondary Prescaler)
        SPI1CON1bits.SPRE       = 6;     //Set Primary Pre-scalar for 4:1 ratio
        SPI1CON1bits.PPRE       = 2;     //Set Secondary Pre-scalar for 2:1 ratio

        SPI1CON1bits.DISSDO     = 0;     //Internal SPI clock is enabled
        SPI1CON1bits.DISSCK     = 0;     //SDOx pin is controlled by the module

        SPI1CON1bits.SSEN       = 0;     //SSx pin is not used by the module.
                                         //Pin is controlled by port function

        SPI1CON2                = 0;     //Framed SPI modes not used


        SPI1STATbits.SPIROV     = 0;
        //  Enable SPI module
        SPI1STATbits.SPIEN       = 1;


}






/*!
    \brief   send one byte of data and receive one back at the same time

*/

Uint8 spi_put_data(Uint8 data)
{
    SPI1BUF = data;			         // write to buffer for TX
    while(!SPI1STATbits.SPIRBF);	         // wait for transfer to complete
    return SPI1BUF;    				 // read the received value

}









