#ifndef COMMON_DEFINE_AQ0X_H
#define COMMON_DEFINE_AQ0X_H

#define CONFIG_DEFAULT_FREQ                                     50
#define CONFIG_DEFAULT_RATED_CURRENT                            2000
#define CONFIG_DEFAULT_SHORT_CKT_CURRENT                        45000
#define CONFIG_DEFAULT_MIN_INSTALLATION_DISTANCE                1

#define CONFIG_DEFAULT_IR_LEVEL_MAX                             300
#define CONFIG_DEFAULT_UV_LEVEL_MAX                             300
#define CONFIG_DEFAULT_LUX_LEVEL_MAX                            300
#define CONFIG_DEFAULT_PROBABLITY_MAX                           90
#define CONFIG_DEFAULT_PROBABLITY_MIN                           10
#define CONFIG_DEFAULT_NO_OF_OBSERVATION                        4

#define CONFIG__DEFAULT_MIN_DFDT                                3     //12mV




//! Sensor/input scan frequency - 8kHz, 125us period
#define SCAN_FREQ                                       10000L

#define FAST_TIMER_SAMPLING_RATE                        100E-6        //timer rate in second

#define FAST_TIMER_SAMPLING_RATE_XU                     100           //timer rate in us







//common commands
#define CMD_CONFIG                                              0xE000
#define CMD_DEVICE_ADDRESS                                      0xE101
#define CMD_SIGNAL_THRESHOLDS                                   0xE203
#define CMD_INSTALLATION_DITAILS                                0xE302
#define CMD_EVENT_ANALYSIS                                      0xE404
#define CMD_RUN_TIME_OPTIONS                                    0xE505
#define CMD_FACTORY_DEFAULT                                     0xE106
#define CMD_COMMIT                                              0xE107

#define CMD_EMULATION_SETTING                                   0xE008
#define CMD_LOAD_IR_DATA                                        0xE609
#define CMD_LOAD_UV_DATA                                        0xE60A
#define CMD_LOAD_MAGNETIC_FIELD_X_DATA                          0xE60B
#define CMD_LOAD_MAGNETIC_FIELD_Y_DATA                          0xE60C
#define CMD_LOAD_HIF_REFERENCE_DATA                             0xE60D

#define CMD_EMU_MODE_SET                                        0xE10D
#define CMD_EMU_MODE_RESET                                      0xE10E
#define CMD_CLEAR_EMU_DATA                                      0xE10F
#define CMD_MISCELLANEOUS                                       0xE01A
#define CMD_DEVICE_RESET                                        0xE11B
#define CMD_CLEAR_EEPROM                                        0xE11C
#define CMD_SUPERVISION_STATUS                                  0xE11D

#define CMD_GET_DATA                                            0xE0E0
#define CMD_DEVICE_DATA                                         0xE1E1
#define CMD_IR_DISTURBANCE_RECORD                               0xE1E2
#define CMD_UV_DISTURBANCE_RECORD                               0xE1E3
#define CMD_MFIELD_X_DISTURBANCE_RECORD                         0xE1E4
#define CMD_MFIELD_Y_DISTURBANCE_RECORD                         0xE1E5
#define CMD_IR_CURRENT_RECORD                                   0xE1E6
#define CMD_UV_CURRENT_RECORD                                   0xE1E7
#define CMD_MFIELD_X_CURRENT_RECORD                             0xE1E8
#define CMD_MFIELD_Y_CURRENT_RECORD                             0xE1E9
#define CMD_HIF_DIAGNOSIS_RECORD                                0xE1EA

//!for ten bit adc module
#define ADC_RESOLUTION                                          (1024L) // 10 bit

//!adc value to real value mapping factor ->  Vref/resolution
#define ADC_VAL_MAPPING_FACTOR                                  ((3.3L/*Vref*/)/(ADC_RESOLUTION))

#define MAX_ATTACHMENT_LENGTH                                   100



#endif // COMMON_DEFINE_AQ0X_H
