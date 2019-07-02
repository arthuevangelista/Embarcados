# ====================================================
#  SISTEMA DE AQUISIÇÃO DE DADOS
# ====================================================
# Universidade de Brasília
# campus Gama
#
# Autor: Arthur Evangelista
# Matrícula: 14/0016686
#
# Este makefile foi fortemente baseado no utilizado na
# RTIMULib para compilar os exemplos.
# ====================================================
# Para executar o makefile digite no terminal:
# 	- make -j4
# 	- sudo make install
# Para iniciar o programa:
#		- sudo finalPROG
# ====================================================

# ====================================================
# Define de algumas macros
# ====================================================

# Caminho das libs
IMUPATH  = libs/RTIMULib
GPSPATH = libs/GPSLib
BUZZERPATH = libs/BUZZERLib

# Definicoes do compilador e etc
CC    = gcc
CXX   = g++
DEFINES      = -lpthread -lwiringPi -lm -lgps
CFLAGS			= -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
INCPATH      = -I. -I$(IMUPATH) -I$(GPSPATH) -I$(BUZZERPATH)
LINK  = g++
LFLAGS			= -Wl,-O1
LIBS  = -L/usr/lib/arm-linux-gnueabihf $(DEFINES)
COPY  = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE  = mv -f
CHK_DIR_EXISTS	= test -d
MKDIR			= mkdir -p

# Diretorio para output
OBJECTS_DIR   = objects/

# Arquivos
DEPS    = $(IMUPATH)/RTMath.h \
    $(IMUPATH)/RTIMULib.h \
    $(IMUPATH)/RTIMULibDefs.h \
    $(IMUPATH)/RTIMUHal.h \
    $(IMUPATH)/RTFusion.h \
    $(IMUPATH)/RTFusionKalman4.h \
    $(IMUPATH)/RTFusionRTQF.h \
    $(IMUPATH)/RTIMUSettings.h \
    $(IMUPATH)/RTIMUAccelCal.h \
    $(IMUPATH)/RTIMUMagCal.h \
    $(IMUPATH)/RTIMUCalDefs.h \
    $(IMUPATH)/IMUDrivers/RTIMU.h \
    $(IMUPATH)/IMUDrivers/RTIMUNull.h \
    $(IMUPATH)/IMUDrivers/RTIMUMPU9150.h \
    $(IMUPATH)/IMUDrivers/RTIMUMPU9250.h \
    $(IMUPATH)/IMUDrivers/RTIMUGD20HM303D.h \
    $(IMUPATH)/IMUDrivers/RTIMUGD20M303DLHC.h \
    $(IMUPATH)/IMUDrivers/RTIMUGD20HM303DLHC.h \
    $(IMUPATH)/IMUDrivers/RTIMULSM9DS0.h \
    $(IMUPATH)/IMUDrivers/RTIMULSM9DS1.h \
    $(IMUPATH)/IMUDrivers/RTIMUBMX055.h \
    $(IMUPATH)/IMUDrivers/RTIMUBNO055.h \
    $(IMUPATH)/IMUDrivers/RTPressure.h \
    $(IMUPATH)/IMUDrivers/RTPressureBMP180.h \
    $(IMUPATH)/IMUDrivers/RTPressureLPS25H.h \
    $(IMUPATH)/IMUDrivers/RTPressureMS5611.h \
    $(IMUPATH)/IMUDrivers/RTPressureMS5637.h \
		$(IMUPATH)/implementacaoIMU.h \
		$(GPSPATH)/implementacaoGPS.h \
		$(BUZZERPATH)/buzzer.h

OBJECTS = objects/main.o \
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
		objects/implementacaoGPS.o \
		objects/buzzer.o

MAKE_TARGET	= finalPROG

STDIR		= Output/
TARGET		= Output/$(MAKE_TARGET)
# ====================================================

# Regras
$(TARGET): $(OBJECTS)
	@$(CHK_DIR_EXISTS) Output/ || $(MKDIR) Output/
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core

# ====================================================
# Compilar
# ====================================================

# RTIMULib
$(OBJECTS_DIR)%.o : $(IMUPATH)/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

# Drivers dos IMUs
$(OBJECTS_DIR)%.o : $(IMUPATH)/IMUDrivers/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

# Lib do GPS
$(OBJECTS_DIR)%.o : $(GPSPATH)/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

# Lib do buzzer
$(OBJECTS_DIR)%.o : $(BUZZERPATH)/%.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCPATH)

# Principal
$(OBJECTS_DIR)main.o : main.cpp $(DEPS)
	@$(CHK_DIR_EXISTS) objects/ || $(MKDIR) objects/
	$(CXX) -c -o $@ main.cpp $(CFLAGS) $(INCPATH)
# ====================================================

# ====================================================
# Instalar
# ====================================================
install_target: FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/local/bin/ || $(MKDIR) $(INSTALL_ROOT)/usr/local/bin/
	-$(INSTALL_PROGRAM) "Output/$(MAKE_TARGET)" "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"
	-$(STRIP) "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/local/bin/$(MAKE_TARGET)"


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:
