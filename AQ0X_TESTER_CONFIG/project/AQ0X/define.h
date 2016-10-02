#ifndef DEFINE_H
#define DEFINE_H




//! Charcter to hex value conversion
#define HEX2VAL(cc)  (((cc >= '0' ) && (cc <= '9' )) ? (cc - '0') : (cc - 'A' + 10) )



//!gui setting constants
#define DEFAULT                         0
#define NO_CONNECTION           		1          	/*!<*/
#define CONNECTIED_AQ101                2
#define CONNECTIED_AQ110                3
#define CONNECTIED_AQ101S               4

#define CONNECTED                       "* { background-color: #1E90FF; }"
#define ERROR                           "* { background-color: #FF0000; }"
#define NORMAL                          "* { background-color: #C0C0C0; }"
#define ACTIVE                          "* { background-color: #00CC00; }"
#define STATUS_OK                       "* { background-color: #FFA500; }"


#define NO_COLOR                        "* {background-color:;}"
#define COLOR_SELECTION                 "* {background-color: #1E90FF; }"


#define PBAR_FAILURE   "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #FF0350,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 #FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}"
#define PBAR_SUCCESS   "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 );border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}"




//!emulation data editor
#define WAVEFORM_SINUSOID                                       0
#define WAVEFORM_DC                                             1
#define WAVEFORM_0_TO_3_3                                       2
#define WAVEFORM_PLUS_MINUS_1_65                                3

#define SET_TYPE_SIGNAL_AMP                                     0
#define SET_TYPE_SIGNAL_FREQ                                    1
#define SET_TYPE_SIGNAL_PHASE                                   2
#define SET_TYPE_DC_VALUE                                       3
#define SET_TYPE_AC_AMPLITUDE                                   4
#define SET_TYPE_PHASE_SHIFT                                    5
#define SET_TYPE_DEFAULT                                        6
#define SET_TYPE_ADJUST                                         7




//!message box type index
#define MSG_ERROR                                   1
#define MSG_INFO                                    2


#include "common_define_aq0x.h"

//! Maximum number of re-transmission attempt before reporting error
#define MAX_RE_TX_ATTEMPT                         3


// Relay replays commads
#define ARG_ERROR								    0xEEEE


#define ARG_SEN_COMM_ERROR_NO_RESPONSE				0xEEED


#define ARG_RECEPTION_ERROR 						0xEEEC


#define ARG_SEN_CMD_ERROR		 				    0xEEEB


#define ARG_SEN_RC_ERROR		 				    0xEEEA


#define	ARG_PROCESSING_CMD_WAIT_MSG                 0xD000


#define ARG_FATAL_ERROR_DATA_DOWNLOAD		        0xDEEF



//-----------------------------------------------
//!Config commands


#define    ACK                                                  0x0006
#define    RC_ERROR                                             0x0008
#define    CMD_ERROR                                            0xA008



#endif // DEFINE_H
