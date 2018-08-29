#include <stdio.h>
#include <stdlib.h>
#include "num_caracs.h"

int Num_Caracs(char *string){
	int i = 0;
	while(string[i] != '\0'){
		i++;
	}
	return i;
}

int main(int argc, char *argv[]){
	int i = 0; int total = 0; int numero;
	while(i < argc){
		numero = Num_Caracs(argv[i]);
		printf("Argumento: %s / Numero de caracteres: %d\n", argv[i], numero);
		i += 1;
		total += numero;
	}
	printf("Total de caracteres de entrada: %d\n", total);
	return 0;
}
