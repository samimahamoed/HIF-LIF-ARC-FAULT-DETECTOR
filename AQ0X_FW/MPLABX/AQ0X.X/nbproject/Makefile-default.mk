#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../modules/main.c ../../services/srv_serial.c ../../services/srv_spi.c ../../services/srv_nov.c ../../services/srv_hwio.c ../../services/srv_watchdog.c ../../modules/aq0X/algorithm.c ../../modules/aq0X/analog_sv_scan.c ../../modules/aq0X/ir_sensor.c ../../modules/aq0X/mfield_x_sensor.c ../../modules/aq0X/uv_sensor.c ../../system/sys_traps.c ../../services/srv_delay.s ../../system/sys_eventx.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/2020761063/main.o ${OBJECTDIR}/_ext/202548734/srv_serial.o ${OBJECTDIR}/_ext/202548734/srv_spi.o ${OBJECTDIR}/_ext/202548734/srv_nov.o ${OBJECTDIR}/_ext/202548734/srv_hwio.o ${OBJECTDIR}/_ext/202548734/srv_watchdog.o ${OBJECTDIR}/_ext/1047744896/algorithm.o ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o ${OBJECTDIR}/_ext/1047744896/ir_sensor.o ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o ${OBJECTDIR}/_ext/1047744896/uv_sensor.o ${OBJECTDIR}/_ext/861737009/sys_traps.o ${OBJECTDIR}/_ext/202548734/srv_delay.o ${OBJECTDIR}/_ext/861737009/sys_eventx.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/2020761063/main.o.d ${OBJECTDIR}/_ext/202548734/srv_serial.o.d ${OBJECTDIR}/_ext/202548734/srv_spi.o.d ${OBJECTDIR}/_ext/202548734/srv_nov.o.d ${OBJECTDIR}/_ext/202548734/srv_hwio.o.d ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d ${OBJECTDIR}/_ext/1047744896/algorithm.o.d ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d ${OBJECTDIR}/_ext/861737009/sys_traps.o.d ${OBJECTDIR}/_ext/202548734/srv_delay.o.d ${OBJECTDIR}/_ext/861737009/sys_eventx.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/2020761063/main.o ${OBJECTDIR}/_ext/202548734/srv_serial.o ${OBJECTDIR}/_ext/202548734/srv_spi.o ${OBJECTDIR}/_ext/202548734/srv_nov.o ${OBJECTDIR}/_ext/202548734/srv_hwio.o ${OBJECTDIR}/_ext/202548734/srv_watchdog.o ${OBJECTDIR}/_ext/1047744896/algorithm.o ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o ${OBJECTDIR}/_ext/1047744896/ir_sensor.o ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o ${OBJECTDIR}/_ext/1047744896/uv_sensor.o ${OBJECTDIR}/_ext/861737009/sys_traps.o ${OBJECTDIR}/_ext/202548734/srv_delay.o ${OBJECTDIR}/_ext/861737009/sys_eventx.o

# Source Files
SOURCEFILES=../../modules/main.c ../../services/srv_serial.c ../../services/srv_spi.c ../../services/srv_nov.c ../../services/srv_hwio.c ../../services/srv_watchdog.c ../../modules/aq0X/algorithm.c ../../modules/aq0X/analog_sv_scan.c ../../modules/aq0X/ir_sensor.c ../../modules/aq0X/mfield_x_sensor.c ../../modules/aq0X/uv_sensor.c ../../system/sys_traps.c ../../services/srv_delay.s ../../system/sys_eventx.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP128MC202
MP_LINKER_FILE_OPTION=,--script="..\..\gld\p33EP128MC202.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/202548734/srv_delay.o: ../../services/srv_delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o.ok ${OBJECTDIR}/_ext/202548734/srv_delay.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  ../../services/srv_delay.s -o ${OBJECTDIR}/_ext/202548734/srv_delay.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__DEBUG=1  -g  -MD "${OBJECTDIR}/_ext/202548734/srv_delay.o.d" -I"../.." -I"." -g$(MP_EXTRA_AS_POST)
	
else
${OBJECTDIR}/_ext/202548734/srv_delay.o: ../../services/srv_delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o.ok ${OBJECTDIR}/_ext/202548734/srv_delay.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  ../../services/srv_delay.s -o ${OBJECTDIR}/_ext/202548734/srv_delay.o -omf=elf -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 -g  -MD "${OBJECTDIR}/_ext/202548734/srv_delay.o.d" -I"../.." -I"." -g$(MP_EXTRA_AS_POST)
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/2020761063/main.o: ../../modules/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2020761063 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o.ok ${OBJECTDIR}/_ext/2020761063/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2020761063/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/2020761063/main.o.d" -o ${OBJECTDIR}/_ext/2020761063/main.o ../../modules/main.c    
	
${OBJECTDIR}/_ext/202548734/srv_serial.o: ../../services/srv_serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o.ok ${OBJECTDIR}/_ext/202548734/srv_serial.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_serial.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_serial.o ../../services/srv_serial.c    
	
${OBJECTDIR}/_ext/202548734/srv_spi.o: ../../services/srv_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o.ok ${OBJECTDIR}/_ext/202548734/srv_spi.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_spi.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_spi.o ../../services/srv_spi.c    
	
${OBJECTDIR}/_ext/202548734/srv_nov.o: ../../services/srv_nov.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o.ok ${OBJECTDIR}/_ext/202548734/srv_nov.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_nov.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_nov.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_nov.o ../../services/srv_nov.c    
	
${OBJECTDIR}/_ext/202548734/srv_hwio.o: ../../services/srv_hwio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o.ok ${OBJECTDIR}/_ext/202548734/srv_hwio.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_hwio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_hwio.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_hwio.o ../../services/srv_hwio.c    
	
${OBJECTDIR}/_ext/202548734/srv_watchdog.o: ../../services/srv_watchdog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.ok ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_watchdog.o ../../services/srv_watchdog.c    
	
${OBJECTDIR}/_ext/1047744896/algorithm.o: ../../modules/aq0X/algorithm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o.ok ${OBJECTDIR}/_ext/1047744896/algorithm.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/algorithm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/algorithm.o.d" -o ${OBJECTDIR}/_ext/1047744896/algorithm.o ../../modules/aq0X/algorithm.c    
	
${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o: ../../modules/aq0X/analog_sv_scan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.ok ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d" -o ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o ../../modules/aq0X/analog_sv_scan.c    
	
${OBJECTDIR}/_ext/1047744896/ir_sensor.o: ../../modules/aq0X/ir_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/ir_sensor.o ../../modules/aq0X/ir_sensor.c    
	
${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o: ../../modules/aq0X/mfield_x_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o ../../modules/aq0X/mfield_x_sensor.c    
	
${OBJECTDIR}/_ext/1047744896/uv_sensor.o: ../../modules/aq0X/uv_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/uv_sensor.o ../../modules/aq0X/uv_sensor.c    
	
${OBJECTDIR}/_ext/861737009/sys_traps.o: ../../system/sys_traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/861737009 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o.ok ${OBJECTDIR}/_ext/861737009/sys_traps.o.err 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/861737009/sys_traps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/861737009/sys_traps.o.d" -o ${OBJECTDIR}/_ext/861737009/sys_traps.o ../../system/sys_traps.c    
	
${OBJECTDIR}/_ext/861737009/sys_eventx.o: ../../system/sys_eventx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/861737009 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o.d 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o.ok ${OBJECTDIR}/_ext/861737009/sys_eventx.o.err 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/861737009/sys_eventx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/861737009/sys_eventx.o.d" -o ${OBJECTDIR}/_ext/861737009/sys_eventx.o ../../system/sys_eventx.c    
	
else
${OBJECTDIR}/_ext/2020761063/main.o: ../../modules/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/2020761063 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o.ok ${OBJECTDIR}/_ext/2020761063/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/2020761063/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/2020761063/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/2020761063/main.o.d" -o ${OBJECTDIR}/_ext/2020761063/main.o ../../modules/main.c    
	
${OBJECTDIR}/_ext/202548734/srv_serial.o: ../../services/srv_serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o.ok ${OBJECTDIR}/_ext/202548734/srv_serial.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_serial.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_serial.o ../../services/srv_serial.c    
	
${OBJECTDIR}/_ext/202548734/srv_spi.o: ../../services/srv_spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o.ok ${OBJECTDIR}/_ext/202548734/srv_spi.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_spi.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_spi.o ../../services/srv_spi.c    
	
${OBJECTDIR}/_ext/202548734/srv_nov.o: ../../services/srv_nov.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o.ok ${OBJECTDIR}/_ext/202548734/srv_nov.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_nov.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_nov.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_nov.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_nov.o ../../services/srv_nov.c    
	
${OBJECTDIR}/_ext/202548734/srv_hwio.o: ../../services/srv_hwio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o.ok ${OBJECTDIR}/_ext/202548734/srv_hwio.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_hwio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_hwio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_hwio.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_hwio.o ../../services/srv_hwio.c    
	
${OBJECTDIR}/_ext/202548734/srv_watchdog.o: ../../services/srv_watchdog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/202548734 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.ok ${OBJECTDIR}/_ext/202548734/srv_watchdog.o.err 
	@${RM} ${OBJECTDIR}/_ext/202548734/srv_watchdog.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/202548734/srv_watchdog.o.d" -o ${OBJECTDIR}/_ext/202548734/srv_watchdog.o ../../services/srv_watchdog.c    
	
${OBJECTDIR}/_ext/1047744896/algorithm.o: ../../modules/aq0X/algorithm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o.ok ${OBJECTDIR}/_ext/1047744896/algorithm.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/algorithm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/algorithm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/algorithm.o.d" -o ${OBJECTDIR}/_ext/1047744896/algorithm.o ../../modules/aq0X/algorithm.c    
	
${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o: ../../modules/aq0X/analog_sv_scan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.ok ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o.d" -o ${OBJECTDIR}/_ext/1047744896/analog_sv_scan.o ../../modules/aq0X/analog_sv_scan.c    
	
${OBJECTDIR}/_ext/1047744896/ir_sensor.o: ../../modules/aq0X/ir_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/ir_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/ir_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/ir_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/ir_sensor.o ../../modules/aq0X/ir_sensor.c    
	
${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o: ../../modules/aq0X/mfield_x_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/mfield_x_sensor.o ../../modules/aq0X/mfield_x_sensor.c    
	
${OBJECTDIR}/_ext/1047744896/uv_sensor.o: ../../modules/aq0X/uv_sensor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1047744896 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.ok ${OBJECTDIR}/_ext/1047744896/uv_sensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/1047744896/uv_sensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/1047744896/uv_sensor.o.d" -o ${OBJECTDIR}/_ext/1047744896/uv_sensor.o ../../modules/aq0X/uv_sensor.c    
	
${OBJECTDIR}/_ext/861737009/sys_traps.o: ../../system/sys_traps.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/861737009 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o.d 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o.ok ${OBJECTDIR}/_ext/861737009/sys_traps.o.err 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_traps.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/861737009/sys_traps.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/861737009/sys_traps.o.d" -o ${OBJECTDIR}/_ext/861737009/sys_traps.o ../../system/sys_traps.c    
	
${OBJECTDIR}/_ext/861737009/sys_eventx.o: ../../system/sys_eventx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/861737009 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o.d 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o.ok ${OBJECTDIR}/_ext/861737009/sys_eventx.o.err 
	@${RM} ${OBJECTDIR}/_ext/861737009/sys_eventx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/861737009/sys_eventx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -Wall -I"../../../../includes" -I"../../../includes" -I"../../includes" -I"../../includes_prv" -I"../../includes/aq0X" -I"../../includes_prv/aq0X" -I"." -MMD -MF "${OBJECTDIR}/_ext/861737009/sys_eventx.o.d" -o ${OBJECTDIR}/_ext/861737009/sys_eventx.o ../../system/sys_eventx.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../gld/p33EP128MC202.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG  -o dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-L"../../gld",-L".",-Map="${DISTDIR}/AQ0X.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../gld/p33EP128MC202.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-L"../../gld",-L".",-Map="${DISTDIR}/AQ0X.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/AQ0X.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
