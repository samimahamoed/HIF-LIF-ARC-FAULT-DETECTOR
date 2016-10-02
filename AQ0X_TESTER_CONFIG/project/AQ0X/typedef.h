#ifndef TYPEDEF_H
#define TYPEDEF_H


#include <QString>
#include <QObject>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QCheckBox>


#include "define.h"



typedef quint8               Uint8;
typedef quint16              Uint16;
typedef quint32              Uint32;
typedef quint64              Uint64;

typedef float                Float32;
typedef qint8                Int8;
typedef qint16               Int16;
typedef qint32               Int32;
typedef qint64               Int64;
typedef bool                 Bool;




//! Input state structure
typedef union
{
    Uint8 inputs_all;
    struct
    {
        Uint8 bin1_fixed:1;
        Uint8 bin1_pulse:1;
        Uint8 bin2:1;
    }inputs_bits;

}inputs_u;



//! sensor supervission replay  msg structure
typedef union
{
    Uint8 btn_all;
    struct
    {
        Uint8 home:1;
        Uint8 serial_log:1;
        Uint8 tester:1;
        Uint8 db:1;
        Uint8 config:1;
        Uint8 scaner:1;
        Uint8 emu:1;
        Uint8 aqsim:1;

    }bits;


}btn_u;


//! sensor supervission replay  msg structure
typedef union
{
    Uint8 all;
    struct
    {
        Uint8 serial_num:1;
        Uint8 device_address:1;
        Uint8 timestamp:1;
        Uint8 self_test_dis:1;
        Uint8 reserved_1:1;
        Uint8 reserved_2:1;
        Uint8 reserved_3:1;
        Uint8 reserved_4:1;
    }bits;


}configured_u;



//! Led control structure
typedef union
{
    Uint32 leds_all;

    //! leds, order same as on the front device panel
    //! for AQ-110 mode more leds on second board
    struct
    {
       Uint16 power:1;
       Uint16 error:1;
       Uint16 trip:1;
       Uint16 cbfp:1;

       Uint16 sen_1:1;
       Uint16 sen_2:1;
       Uint16 sen_3:1;
       Uint16 sen_4:1;

       Uint16 sen_5_fiber:1;
       Uint16 bin1_fixed_or_pulse:1;

       Uint16 bin2:1;
       Uint16 bout:1;

//#if (AQ_TRAFO == DEV_TRAFOCARD)

        Uint16 red1:1;
        Uint16 red2:1;

        Uint16 orange1:1;
        Uint16 orange2:1;
        Uint16 orange3:1;
        Uint16 orange4:1;
        Uint16 orange5:1;
        Uint16 orange6:1;
//#endif


    }leds_bits;

}leds_u;

//! sensors mask
typedef union
{
    Uint16 sensors_all;
    struct
    {
        Uint8 sen_1:1;
        Uint8 sen_2:1;
        Uint8 sen_3:1;
    }ch_1;

    struct
    {
        Uint8 sen_1:1;
        Uint8 sen_2:1;
        Uint8 sen_3:1;
    }ch_2;

      struct
    {
        Uint8 sen_1:1;
        Uint8 sen_2:1;
        Uint8 sen_3:1;
    }ch_3;

      struct
    {
        Uint8 sen_1:1;
        Uint8 sen_2:1;
        Uint8 sen_3:1;
    }ch_4;
}sm_sen_info_u;


//! smart sensors mask
typedef union
{
    Uint8 ch_all;
    struct
    {
        Uint8 ch_1:1;
        Uint8 ch_2:1;
        Uint8 ch_3:1;
        Uint8 ch_4:1;
    }bits;

}channels_u;


//! Dipswitch state structure
//! For AQ-110 option double amount of dip witches
typedef union
{
    Uint16 dsw_all;
    struct
    {

        Uint16 sw11    :1;
        Uint16 sw12    :1;
        Uint16 sw13    :1;
        Uint16 sw14    :1;
        Uint16 sw15    :1;
        Uint16 sw16    :1;
        Uint16 sw17    :1;
        Uint16 sw18    :1;

//#if (AQ_TRAFO == DEV_TRAFOCARD)


        Uint16 sw21    :1;
        Uint16 sw22    :1;
        Uint16 sw23    :1;
        Uint16 sw24    :1;
        Uint16 sw25    :1;
        Uint16 sw26    :1;
        Uint16 sw27    :1;
        Uint16 sw28    :1;
//#endif



    }dsw_bits;

}dsw_u;


//! Sensor state structure
typedef union
{
    Uint8 fibers_all;
    struct
    {
        Uint8 sen_5:1;
    }fibers_bits;

}fibers_u;




//! sensor msg structure
typedef union
{
    Uint8 msg_all;
    struct
    {
        Uint8 sender_address:2;
        Uint8 receiver_address:2;
        Uint8 cmd :4;
    }msg_fields;


}msg_u;

//! sensor msg structure
typedef union
{
    Uint8 all;
    struct
    {
        Uint8 serial_log:1;
        Uint8 emu:1;
        Uint8 test:1;
        Uint8 scan:1;
        Uint8 aqsim:1;
        Uint8 config:1;
        Uint8 db:1;
        Uint8 none:1;
    }modes;

    struct
    {
        Uint8 wait_for_response:1;
    }flags;


}operation;



//! sensor msg structure
typedef union
{
    Uint16 all;
    struct
    {
        Uint8 val0:1;
        Uint8 val1:1;
        Uint8 val2:1;
        Uint8 val3:1;
        Uint8 val4:1;
        Uint8 val5:1;
        Uint8 val6:1;
        Uint8 val7:1;
        Uint8 val8:1;
        Uint8 val9:1;
        Uint8 val10:1;
        Uint8 val11:1;
        Uint8 val12:1;
        Uint8 val13:1;
        Uint8 val14:1;
        Uint8 val15:1;

    }bits;


}bit_fileds_u;




//! internal use objects structure
typedef union
{

    Uint8 ob_type;

    struct
      {
          QObject * Ob0;
          QObject * Ob1;
          QObject * Ob2;
          QObject * Ob3;
          QObject * Ob4;
          QObject * Ob5;
          QObject * Ob6;
          QObject * Ob7;
      }ob;


    struct
    {
        Uint16  Ob0;
        Uint16  Ob1;
        Uint16  Ob2;
        Uint16  Ob3;
        Uint16  Ob4;
        Uint16  Ob5;
        Uint16  Ob6;
        Uint16  Ob7;
    }val;

    struct
    {
        wchar_t  Ob0[32];
        wchar_t  Ob1[32];
        wchar_t  Ob2[32];
        wchar_t  Ob3[32];
        wchar_t  Ob4[32];
        wchar_t  Ob5[32];
        wchar_t  Ob6[32];
        wchar_t  Ob7[32];
    }val_str;


     bit_fileds_u bits_fileds;
	
}gui_group;


//! sensor supervission replay  msg structure
typedef union
{
    Uint8 flags_all;
    struct
    {
        //relay supervision replay
        Uint8 LIGHT_ACT:1;
        Uint8 PRESSURE_ACT:1;
        Uint8 EVENT_DATA_NEW:1;
        Uint8 SELF_TEST_ERROR:1;

        //for internal use
        Uint8 SELF_TEST_EN:1;
        Uint8 ADDRESS_CONFIGURED:1;
        Uint8 NEW_CMD:1;
        Uint8 SILENCE:1;
    }bits;

}status_flags_u;


//! sensor supervission
typedef union
{
    Uint8 error_all;
    struct
    {
        //relay supervision replay
        Uint8 eeprom_error:1;
        Uint8 light_element_error:1;
        Uint8 timestamp_invalid:1;
        Uint8 configuration_error:1;

        //for internal use
        Uint8 rc_error:1;
        Uint8 reset:1;
        Uint8 reserved1:1;
        Uint8 reserved2:1;
    }bits;


}error_u;





//! sensor supervission
typedef union
{
    Uint8 all;
    struct
    {
        //relay supervision replay
        Uint8 bit0:1;
        Uint8 bit1:1;
        Uint8 bit2:1;
        Uint8 bit3:1;

        Uint8 bit4:1;
        Uint8 bit5:1;
        Uint8 bit6:1;
        Uint8 bit7:1;

    }bits;


}flags_u;

typedef struct mult_sen_t
{
    int         address;
    int         cmd;
    struct
    {
    QLabel       *label;
    QPushButton  *btn;
    QProgressBar *pBar;
    QHBoxLayout  *hLayout;
    bool btnCreated;
    }gui;
    QString data_dir_name;
    QString data_file_name;

    bool varified;
}mult_sen_t;


typedef struct emu_sample_edit
{
    unsigned int         val;

    struct
    {

    QSlider      *slider;
    QCheckBox    *chbox;
    QLabel       *label;
    QVBoxLayout  *vLayout;
    bool btnCreated;
    }gui;

    bool varified;
}emu_data_edit_point_t;


#define ARG_MAX_LENGTH                  8
#define ARG_INDEX_CHANNEL               0
#define ARG_INDEX_SELECTED_SENSOR       1


//! serial command record

typedef struct
{

  Uint16        cmd;
  Uint16        arg[10/*max arg length */];
  Uint16        arg_cnt;
  
  
  struct{
  Uint16        offset;
  struct{

   Uint16       copied;
   Uint16       expected;
  }total_size;

  void         *storage_location;
  }data_download;
  bool          multi_address;
  Uint16        vPos;
  

  Uint16        device_response_cnt;
  Uint16        last_response_cnt;
  bool          processed;
  
  const char   *listener_slot;
  char          cmd_type[50/*max length */];


  Uint16        attachment_length;
  void*         attachment;

}serial_msg_t;





#endif // TYPEDEF_H
