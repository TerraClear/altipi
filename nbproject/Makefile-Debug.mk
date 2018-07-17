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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/46522b86/xkerror.o \
	${OBJECTDIR}/_ext/46522b86/xkserial.o \
	${OBJECTDIR}/_ext/46522b86/xkthread.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/xkthread_serialrx.o


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

${OBJECTDIR}/_ext/46522b86/xkerror.o: ../tcpserial/xkerror.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/46522b86
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/46522b86/xkerror.o ../tcpserial/xkerror.cpp

${OBJECTDIR}/_ext/46522b86/xkserial.o: ../tcpserial/xkserial.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/46522b86
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/46522b86/xkserial.o ../tcpserial/xkserial.cpp

${OBJECTDIR}/_ext/46522b86/xkserial.hpp.gch: ../tcpserial/xkserial.hpp
	${MKDIR} -p ${OBJECTDIR}/_ext/46522b86
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o "$@" ../tcpserial/xkserial.hpp

${OBJECTDIR}/_ext/46522b86/xkthread.o: ../tcpserial/xkthread.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/46522b86
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/46522b86/xkthread.o ../tcpserial/xkthread.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/xkthread_serialrx.o: xkthread_serialrx.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libserialport` -std=c++11  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/xkthread_serialrx.o xkthread_serialrx.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
