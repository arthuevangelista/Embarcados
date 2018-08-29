Embarcados - 1.2018

1_  

\#include <stdio.h>  

int main(){  
	printf("Olá mundo!\n");  
	return 0;  
}  

2_  

\#include <stdio.h>  

int main(){  
	char nome[100];  
	printf("Digite o seu nome: ");  
	scanf("%s", nome);  
	printf("Olá %s", nome);  
	return 0;  
}  

3_  

a) Ola Arthur  
b) Ola "Arthur  
c) Digite o seu nome: Olá Arthur  
d) Digite o seu nome: Olá Arthur  
e) Digite o seu nome: Olá Arthur  
f) Digite o seu nome: Olá Arthur    

4_  

\#include <stdio.h>

int main(int argc, char \*argv[]){  
	int i = 1;  
	printf("Olá ");  
	while(i < argc){  
		printf("%s ", argv[i]);  
		i += 1;  
	}  
	printf("\n");  
	return 0;  
}  

5_  

a) Olá Eu Mesmo  
b) Olá Eu Mesmo  
c) Olá  
d) Olá  
e) Olá  
f) Olá  

6_  

\#include <stdio.h>  

int main(int argc, char \*argv[]){  
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

7_  

\#include <stdio.h>  

int main(int argc, char \*argv[]){    
	int i = 1;  
	printf("Argumentos: ");  
	while(i < argc){  
		printf("%s ", argv[i]);  
		i += 1;  
	}  
	printf("\n");  
	return 0;  
}  

8_  

Função num_caracs.h:  
		"  
		\#ifndef NUM_CARACS_H  
		\#define NUM_CARACS_H  

		int Num_Caracs(char \*string);  

		\#endif  
		"  

Função num_caracs.c:  
	"  
	\#include <stdio.h>  
	\#include <stdlib.h>  
	\#include "num_caracs.h"  

	int Num_Caracs(char \*string){  
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
	"  

9_  

	\#include <stdio.h>  
	\#include <stdlib.h>  
	\#include "num_caracs.h"  

	int Num_Caracs(char \*string){  
		int i = 0;  
		while(string[i] != '\0'){  
			i++;  
		}  
		return i;  
	}  

	int main(int argc, char \*argv[]){  
		int i = 0;  
		printf("Olá ");  
		while(i < argc){  
			printf("Argumento: %s / Numero de caracteres: %d", argv[i], Num_Caracs(argv[i]));  
			i += 1;  
		}  
		printf("\n");  
		return 0;  
	}  

10_  
