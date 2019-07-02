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
OBJS = testeGPS.o implementacaoGPS.o buzzer.o

# Usado para o link
LOCALLIBDIR = -L/usr/lib/arm-linux-gnueabihf

PROG = aout

CODE = testeGPS.cpp

all: $(PROG)

$(PROG): $(OBJS)
	@echo "Compilando aplicativos de teste de GPS ..."
	@$(CXX) -g $(CFLAGS) $(OBJS) -o $@ $(LOCALLIBDIR) $(LDLIBS)

testeGPS.o: implementacaoGPS.h
implementacaoGPS.o: implementacaoGPS.h
buzzer.o: buzzer.h

clean:
	@echo "Deletando binários ..."
	@rm -f *~ *.o $(PROG) core a.out
