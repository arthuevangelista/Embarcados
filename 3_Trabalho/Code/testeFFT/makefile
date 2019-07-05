# Uso:
# make        ; compila o aplicativo
# make clean  ; deleta o binário

.PHONY: install clean

CXX = g++
CFLAGS = -Wall

# Caminho do include
INCPATH = -I.

# Flags necessarias para o GPS
LDLIBS = -lm -lgps -lpthread -lwiringPi

# Listagem dos Objetos
OBJS = fft-test.o fft.o

# Usado para o link
LOCALLIBDIR = -L/usr/lib/arm-linux-gnueabihf

PROG = aout

CODE = fft-test.cpp

all: $(PROG)

$(PROG): $(OBJS)
	@echo "Compilando aplicativos de teste da FFT ..."
	@$(CXX) -g $(CFLAGS) $(OBJS) -o $@ $(LOCALLIBDIR) $(LDLIBS)

fft-test.o: fft.h
fft.o: fft.h

clean:
	@echo "Deletando binários ..."
	@rm -f *~ *.o $(PROG) core a.out
