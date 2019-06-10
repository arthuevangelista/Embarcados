# Uso:
# make        ; compila o aplicativo
# make clean  ; deleta o binário

.PHONY: install clean

CXX = g++
CFLAGS = -Wall
INCPATH = -I.
LFLAGS = -lm -lgps

# Usado apenas para o link ???
LIBS = -L/usr/lib/arm-linux-gnueabihf

APP1 = aout
CODEUM = testeGPS.cpp
APP2 = bout
CODEDOIS = testeGPS2.cpp

install:
	@echo "Compilando aplicativos de teste de GPS ..."
	@$(CXX) -c -g $(CODEUM) -o $(APP1) $(CFLAGS) $(LFLAGS)
	@$(CXX) -c -g $(CODEDOIS) -o $(APP2) $(CFLAGS) $(LFLAGS)

clean:
	@echo "Deletando binários ..."
	@rm -f $(APP1)
	@rm -f $(APP2)
