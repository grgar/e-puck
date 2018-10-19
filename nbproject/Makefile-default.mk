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
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=library/acc_gyro/e_lsm330.c library/I2C/e_I2C_protocol.c library/I2C/e_I2C_master_module.c library/motor_led/advance_one_timer/e_motors.c library/motor_led/advance_one_timer/e_led.c library/motor_led/advance_one_timer/e_agenda.c library/motor_led/e_init_port.c src/main.c src/p1.c src/common.c src/p2.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/library/acc_gyro/e_lsm330.o ${OBJECTDIR}/library/I2C/e_I2C_protocol.o ${OBJECTDIR}/library/I2C/e_I2C_master_module.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o ${OBJECTDIR}/library/motor_led/e_init_port.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/p1.o ${OBJECTDIR}/src/common.o ${OBJECTDIR}/src/p2.o
POSSIBLE_DEPFILES=${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d ${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d ${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d ${OBJECTDIR}/library/motor_led/e_init_port.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/p1.o.d ${OBJECTDIR}/src/common.o.d ${OBJECTDIR}/src/p2.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/library/acc_gyro/e_lsm330.o ${OBJECTDIR}/library/I2C/e_I2C_protocol.o ${OBJECTDIR}/library/I2C/e_I2C_master_module.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o ${OBJECTDIR}/library/motor_led/e_init_port.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/p1.o ${OBJECTDIR}/src/common.o ${OBJECTDIR}/src/p2.o

# Source Files
SOURCEFILES=library/acc_gyro/e_lsm330.c library/I2C/e_I2C_protocol.c library/I2C/e_I2C_master_module.c library/motor_led/advance_one_timer/e_motors.c library/motor_led/advance_one_timer/e_led.c library/motor_led/advance_one_timer/e_agenda.c library/motor_led/e_init_port.c src/main.c src/p1.c src/common.c src/p2.c


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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F6014A
MP_LINKER_FILE_OPTION=,--script=p30F6014A.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/library/acc_gyro/e_lsm330.o: library/acc_gyro/e_lsm330.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/acc_gyro" 
	@${RM} ${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d 
	@${RM} ${OBJECTDIR}/library/acc_gyro/e_lsm330.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/acc_gyro/e_lsm330.c  -o ${OBJECTDIR}/library/acc_gyro/e_lsm330.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/I2C/e_I2C_protocol.o: library/I2C/e_I2C_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/I2C" 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/I2C/e_I2C_protocol.c  -o ${OBJECTDIR}/library/I2C/e_I2C_protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/I2C/e_I2C_master_module.o: library/I2C/e_I2C_master_module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/I2C" 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_master_module.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/I2C/e_I2C_master_module.c  -o ${OBJECTDIR}/library/I2C/e_I2C_master_module.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o: library/motor_led/advance_one_timer/e_motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_motors.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o: library/motor_led/advance_one_timer/e_led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_led.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o: library/motor_led/advance_one_timer/e_agenda.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_agenda.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/e_init_port.o: library/motor_led/e_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led" 
	@${RM} ${OBJECTDIR}/library/motor_led/e_init_port.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/e_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/e_init_port.c  -o ${OBJECTDIR}/library/motor_led/e_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/e_init_port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/e_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/p1.o: src/p1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/p1.o.d 
	@${RM} ${OBJECTDIR}/src/p1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/p1.c  -o ${OBJECTDIR}/src/p1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/p1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/p1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/common.o: src/common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/common.o.d 
	@${RM} ${OBJECTDIR}/src/common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/common.c  -o ${OBJECTDIR}/src/common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/common.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/p2.o: src/p2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/p2.o.d 
	@${RM} ${OBJECTDIR}/src/p2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/p2.c  -o ${OBJECTDIR}/src/p2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/p2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/p2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/library/acc_gyro/e_lsm330.o: library/acc_gyro/e_lsm330.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/acc_gyro" 
	@${RM} ${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d 
	@${RM} ${OBJECTDIR}/library/acc_gyro/e_lsm330.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/acc_gyro/e_lsm330.c  -o ${OBJECTDIR}/library/acc_gyro/e_lsm330.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/acc_gyro/e_lsm330.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/I2C/e_I2C_protocol.o: library/I2C/e_I2C_protocol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/I2C" 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_protocol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/I2C/e_I2C_protocol.c  -o ${OBJECTDIR}/library/I2C/e_I2C_protocol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/I2C/e_I2C_protocol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/I2C/e_I2C_master_module.o: library/I2C/e_I2C_master_module.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/I2C" 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d 
	@${RM} ${OBJECTDIR}/library/I2C/e_I2C_master_module.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/I2C/e_I2C_master_module.c  -o ${OBJECTDIR}/library/I2C/e_I2C_master_module.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/I2C/e_I2C_master_module.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o: library/motor_led/advance_one_timer/e_motors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_motors.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_motors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o: library/motor_led/advance_one_timer/e_led.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_led.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_led.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o: library/motor_led/advance_one_timer/e_agenda.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led/advance_one_timer" 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/advance_one_timer/e_agenda.c  -o ${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/advance_one_timer/e_agenda.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/library/motor_led/e_init_port.o: library/motor_led/e_init_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/library/motor_led" 
	@${RM} ${OBJECTDIR}/library/motor_led/e_init_port.o.d 
	@${RM} ${OBJECTDIR}/library/motor_led/e_init_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  library/motor_led/e_init_port.c  -o ${OBJECTDIR}/library/motor_led/e_init_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/library/motor_led/e_init_port.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/library/motor_led/e_init_port.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/p1.o: src/p1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/p1.o.d 
	@${RM} ${OBJECTDIR}/src/p1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/p1.c  -o ${OBJECTDIR}/src/p1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/p1.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/p1.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/common.o: src/common.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/common.o.d 
	@${RM} ${OBJECTDIR}/src/common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/common.c  -o ${OBJECTDIR}/src/common.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/common.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/common.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/p2.o: src/p2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/p2.o.d 
	@${RM} ${OBJECTDIR}/src/p2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/p2.c  -o ${OBJECTDIR}/src/p2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/p2.o.d"        -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -mlarge-data -O0 -I"library/uart" -I"library" -I"src" -msmart-io=1 -Wall -msfr-warn=off  
	@${FIXDEPS} "${OBJECTDIR}/src/p2.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--heap=128,--stack=500,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="C:/Program Files/Microchip/MPLAB C30/lib",--library-path="C:/Program Files (x86)/Microchip/MPLAB C30/support/dsPIC30F/gld",--no-force-link,--smart-io,-Map="${DISTDIR}/demoGCtronic.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=128,--stack=500,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="C:/Program Files/Microchip/MPLAB C30/lib",--library-path="C:/Program Files (x86)/Microchip/MPLAB C30/support/dsPIC30F/gld",--no-force-link,--smart-io,-Map="${DISTDIR}/demoGCtronic.X.${IMAGE_TYPE}.map",--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/e-puck.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
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

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
