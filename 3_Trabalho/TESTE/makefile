# Uso:
# make        # compila o aplicativo
# make clean  # deleta o binário

.PHONY: install clean

CC = gcc
CXX = g++
CFLAGS = -g -c
LIBS = -lm -lgps

APP1 = a.out
CODEUM = testeGPS.cpp
APP2 = b.out
CODEDOIS = testeGPS2.cpp

install:
	@echo "Compilando aplicativos de teste de GPS ..."
	@$(CXX) $(CFLAGS) $(CODEUM) -o $(APP1) $(LIBS)
	@$(CXX) $(CFLAGS) $(CODEDOIS) -o $(APP2) $(LIBS)

clean:
	@echo "Deletando binários ..."
	@rm -f $(APP1)
	@rm -f $(APP2)
