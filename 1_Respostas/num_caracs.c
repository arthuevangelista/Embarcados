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

int main(){
	int num_char;
	char string[100];
	printf("Digite uma string: ");
	scanf("%s", string);
	num_char = Num_Caracs(string);
	printf("Número de caracteres: %d\n", num_char);
	return 0;
}
