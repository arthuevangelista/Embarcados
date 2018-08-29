#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  FILE *tam_arq_texto;
  tam_arq_texto = fopen("nome_arquivo.txt", "r");

  if(!tam_arq_texto)  printf("ERROR 404");

  char *texto;
  unsigned int tamanho;
  tamanho = strlen(fscanf(tam_arq_texto, "%s", texto));

  printf("Tamanho do arquivo: %d\n", tamanho);

  fclose(tam_arq_texto);
  return 0;
}
