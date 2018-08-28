#include <stdio.h>

int main(int argc, char *argv[]){
	int i = 1;
	printf("Olá ");
	while(i < argc){
		printf("%s ", argv[i]);
		i += 1;
	}
	printf("\n");
	printf("Numero de entradas = %d\n", argc);
	return 0;
}
