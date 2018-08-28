Embarcados - 1.2018

1_  

#include <stdio.h>  

int main(){  
	printf("Olá mundo!\n");  
	return 0;  
}  

2_  

#include <stdio.h>  

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

#include <stdio.h>

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

#include <stdio.h>  

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
