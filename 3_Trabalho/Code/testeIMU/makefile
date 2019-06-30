TESTEIMUPATH  = RTIMULib

CC    			= gcc
CXX   			= g++
DEFINES       	= -lpthread -lwiringPi
CFLAGS			= -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      	= -pipe -O2 -Wall -W $(DEFINES)
INCPATH       	= -I. -I$(TESTEIMUPATH)
LINK  			= g++
LFLAGS			= -Wl,-O1
LIBS  			= -L/usr/lib/arm-linux-gnueabihf $(DEFINES)
COPY  			= cp -f
COPY_FILE     	= $(COPY)
COPY_DIR      	= $(COPY) -r
STRIP 			= strip
INSTALL_FILE  	= install -m 644 -p
INSTALL_DIR   	= $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      	= rm -f
SYMLINK       	= ln -f -s
DEL_DIR       	= rmdir
MOVE  			= mv -f
CHK_DIR_EXISTS	= test -d
MKDIR			= mkdir -p

# Output directory

OBJECTS_DIR   = objects/

# Files

DEPS    = $(TESTEIMUPATH)/RTMath.h \
    $(TESTEIMUPATH)/RTIMULib.h \
    $(TESTEIMUPATH)/RTIMULibDefs.h \
    $(TESTEIMUPATH)/RTIMUHal.h \
    $(TESTEIMUPATH)/RTFusion.h \
    $(TESTEIMUPATH)/RTFusionKalman4.h \
    $(TESTEIMUPATH)/RTFusionRTQF.h \
    $(TESTEIMUPATH)/RTIMUSettings.h \
    $(TESTEIMUPATH)/RTIMUAccelCal.h \
    $(TESTEIMUPATH)/RTIMUMagCal.h \
    $(TESTEIMUPATH)/RTIMUCalDefs.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMU.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUNull.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUMPU9150.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUMPU9250.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUGD20HM303D.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUGD20M303DLHC.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUGD20HM303DLHC.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMULSM9DS0.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMULSM9DS1.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUBMX055.h \
    $(TESTEIMUPATH)/IMUDrivers/RTIMUBNO055.h \
    $(TESTEIMUPATH)/IMUDrivers/RTPressure.h \
    $(TESTEIMUPATH)/IMUDrivers/RTPressureBMP180.h \
    $(TESTEIMUPATH)/IMUDrivers/RTPressureLPS25H.h \
    $(TESTEIMUPATH)/IMUDrivers/RTPressureMS5611.h \
    $(TESTEIMUPATH)/IMUDrivers/RTPressureMS5637.h \
		$(TESTEIMUPATH)/implementacaoIMU.h \
		$(TESTEIMUPATH)/buzzer.h

OBJECTS = objects/testeIMU.o \
    objects/RTMath.o \
    objects/RTIMUHal.o \
    objects/RTFusion.o \
    objects/RTFusionKalman4.o \
    objects/RTFusionRTQF.o \
    objects/RTIMUSettings.o \
    objects/RTIMUAccelCal.o \
    objects/RTIMUMagCal.o \
    objects/RTIMU.o \
    objects/RTIMUNull.o \
    objects/RTIMUMPU9150.o \
    objects/RTIMUMPU9250.o \
    objects/RTIMUGD20HM303D.o \
    objects/RTIMUGD20M303DLHC.o \
    objects/RTIMUGD20HM303DLHC.o \
    objects/RTIMULSM9DS0.o \
    objects/RTIMULSM9DS1.o \
    objects/RTIMUBMX055.o \
    objects/RTIMUBNO055.o \
    objects/RTPressure.o \
    objects/RTPressureBMP180.o \
    objects/RTPressureLPS25H.o \
    objects/RTPressureMS5611.o \
    objects/RTPressureMS5637.o \
		objects/implementacaoIMU.o \
		objects/buzzer.o

MAKE_TARGET	= testeIMU

STDIR		= Output/
TARGET		= Output/$(MAKE_TARGET)

# Build rules

$(TARGET): $(OBJECTS)
	@$(CHK_DIR_EXISTS) Output/ || $(MKDIR) Output/
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core

# Compile

$(OBJECTS_DIR)%.o : $(TESTEIMUPATH)/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

$(OBJECTS_DIR)%.o : $(TESTEIMUPATH)/IMUDrivers/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

$(OBJECTS_DIR)testeIMU.o : testeIMU.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ testeIMU.cpp $(CFLAGS) $(INCPATH)

# Install

install_target: FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/
	-$(INSTALL_PROGRAM) "Output/$(MAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"
	-$(STRIP) "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:
