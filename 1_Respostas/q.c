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
	int i = 0;
	while(i < argc){
		printf("Argumento: %s / Numero de caracteres: %d", argv[i], Num_Caracs(argv[i]));
		i += 1;
		printf("\n");
	}
	return 0;
}
