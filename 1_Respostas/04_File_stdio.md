Embarcados - 1.2018

1_  

  \#include <stdio.h>
  \#include <stdlib.h>

  int main(){
    FILE \*ola;
    ola = fopen("ola_mundo.txt", "w+");
    if(!ola)  printf("ERRO 404");
    fputs("Olá mundo!\n", ola);
    fclose(ola);
    return 0;
  }

2_  

  #include <stdio.h>  
  #include <stdlib.h>  
  #include <string.h>  

  int main(){  
    FILE \*ola;  
    char nome[50];  
    int idade;  

    printf("Digite o seu nome: ");  
    scanf("%s", nome);  
    printf("Digite a sua idade: ");  
    scanf("%d", &idade);  
    char tempNome[50];  
    \*tempNome = \*nome;  
    strcat(tempNome,".txt");  
    ola = fopen(tempNome, "w+");  

    if(!ola)  printf("ERRO 404");  

    fprintf(ola, "Nome: %s\n", nome);  
    fprintf(ola, "Idade: %d\n", idade);  

    fclose(ola);  
    return 0;  
  }    

3_  

  \#include <stdio.h>  
  \#include <stdlib.h>  
  \#include <string.h>  

  int main(int argc, char \*argv[]){  
    FILE \*ola;  
    char nome[50];  
    int idade;  

    strcpy(nome, argv[1]);  
    idade = atoi(argv[2]);  
    char tempNome[50];  
    strcpy(tempNome, nome);  
    strcat(tempNome,".txt");  
    ola = fopen(tempNome, "w+");  

    if(!ola)  printf("ERRO 404");  

    fprintf(ola, "Nome: %s\n", nome);  
    fprintf(ola, "Idade: %d\n", idade);  

    fclose(ola);  
    return 0;  
  }  

4_  

BIB_ARQS_H:
"
  #ifndef BIB_ARQS_H
  #define  BIB_ARQS_H

  int tam_arq_texto(char \*nome_arquivo);

  \#endif
"
BIB_ARQS_C:
"

"
