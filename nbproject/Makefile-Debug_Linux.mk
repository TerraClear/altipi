#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug_Linux
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/e1dda48/basicserial.o \
	${OBJECTDIR}/_ext/e1dda48/error_base.o \
	${OBJECTDIR}/_ext/e1dda48/thread_base.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/thread_serialrx.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/tests/testRunner.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,'..\tcpserial' -lwiringPi `pkg-config --libs libserialport`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/altipi

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/altipi: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/altipi ${OBJECTFILES} ${LDLIBSOPTIONS} -pthread -lwiringPi -lm -lrt -lcrypt

${OBJECTDIR}/_ext/e1dda48/basicserial.o: ../libterraclear/src/basicserial.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/basicserial.o ../libterraclear/src/basicserial.cpp

${OBJECTDIR}/_ext/e1dda48/error_base.o: ../libterraclear/src/error_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/error_base.o ../libterraclear/src/error_base.cpp

${OBJECTDIR}/_ext/e1dda48/thread_base.o: ../libterraclear/src/thread_base.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/thread_base.o ../libterraclear/src/thread_base.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/thread_serialrx.o: thread_serialrx.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/thread_serialrx.o thread_serialrx.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/tests/testRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   


${TESTDIR}/tests/testRunner.o: tests/testRunner.cpp 
	${MKDIR} -p ${TESTDIR}/tests
	${RM} "$@.d"
	$(COMPILE.cc) -g -I. `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${TESTDIR}/tests/testRunner.o tests/testRunner.cpp


${OBJECTDIR}/_ext/e1dda48/basicserial_nomain.o: ${OBJECTDIR}/_ext/e1dda48/basicserial.o ../libterraclear/src/basicserial.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e1dda48/basicserial.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/basicserial_nomain.o ../libterraclear/src/basicserial.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e1dda48/basicserial.o ${OBJECTDIR}/_ext/e1dda48/basicserial_nomain.o;\
	fi

${OBJECTDIR}/_ext/e1dda48/error_base_nomain.o: ${OBJECTDIR}/_ext/e1dda48/error_base.o ../libterraclear/src/error_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e1dda48/error_base.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/error_base_nomain.o ../libterraclear/src/error_base.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e1dda48/error_base.o ${OBJECTDIR}/_ext/e1dda48/error_base_nomain.o;\
	fi

${OBJECTDIR}/_ext/e1dda48/thread_base_nomain.o: ${OBJECTDIR}/_ext/e1dda48/thread_base.o ../libterraclear/src/thread_base.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/e1dda48
	@NMOUTPUT=`${NM} ${OBJECTDIR}/_ext/e1dda48/thread_base.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e1dda48/thread_base_nomain.o ../libterraclear/src/thread_base.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/_ext/e1dda48/thread_base.o ${OBJECTDIR}/_ext/e1dda48/thread_base_nomain.o;\
	fi

${OBJECTDIR}/main_nomain.o: ${OBJECTDIR}/main.o main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main_nomain.o main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/main.o ${OBJECTDIR}/main_nomain.o;\
	fi

${OBJECTDIR}/thread_serialrx_nomain.o: ${OBJECTDIR}/thread_serialrx.o thread_serialrx.cpp 
	${MKDIR} -p ${OBJECTDIR}
	@NMOUTPUT=`${NM} ${OBJECTDIR}/thread_serialrx.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/thread_serialrx_nomain.o thread_serialrx.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/thread_serialrx.o ${OBJECTDIR}/thread_serialrx_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
