#include <stdio.h>
#include <stdlib.h>

#define SEPARADOR_OBSERVADO 0
#define LETRA_OBSERVADA 1

int main(int argc, char* argv[]){

  // verifica se o argumento foi passada de maneira correta
  if(argc != 2) {
    fprintf(stderr, "uso: <nome do arquivo>");
    return 1;
  }

  // variavel de armazenamento dos dados (buffer)
  char *conteudo = NULL;

  // ponteiro do arquivo aberto para leitura
  FILE *ponteiro_arquivo = fopen(argv[1], "r");

  // verifica a existencia do arquivo
  if(ponteiro_arquivo == NULL) {
    fprintf(stderr, "Arquivo inexistente");
    return 1;
  }

  // pega o tamanho do arquivo
  fseek(ponteiro_arquivo, 0L, SEEK_END);
  // armazena o tamanho em bytes
  long tamanho_arquivo = ftell(ponteiro_arquivo);

  // reseta o indicador de posicao do arquivo
  fseek(ponteiro_arquivo, 0L, SEEK_SET);

  // define a memoria exata do buffer
  // mais um para terminar a string com \0
  conteudo = (char*)malloc(sizeof(char) * tamanho_arquivo + 1);

  // verifica se nao houve erros
  // no alocamento de memoria
  if(conteudo == NULL) {
    fprintf(stderr, "erro no alocamento de memoria");
    return 1;
  }

  // copia todo conteudo para o buffer
  // e armazena a ultima posicao
  size_t tamanho_buffer = fread(
    conteudo,
    sizeof(char),
    tamanho_arquivo,
    ponteiro_arquivo
  );

  // termina a string com \0
  conteudo[tamanho_buffer + 1] = '\0';

  // fecha o arquivo
  fclose(ponteiro_arquivo);

  // armazena o numero de linhas
  long quantidade_linhas = 1;

  // armazena o numero de palavras
  long quantidade_palavras = 0;

  // posicao da string para o loop
  long posicao;

  // caractere na posicao para o loop
  char caractere;

  // estado para verificar separadores de palavras
  short estado = SEPARADOR_OBSERVADO;

  // percorre o buffer para contar o numero de palavras e linhas
  for(posicao = 0; conteudo[posicao]; posicao++) {

    caractere = conteudo[posicao];

    if(caractere == ' ' || caractere == '\n' || caractere == '\t') {

      estado = SEPARADOR_OBSERVADO;

      if(caractere == '\n') quantidade_linhas++;

    } else if(estado == SEPARADOR_OBSERVADO) {

      estado = LETRA_OBSERVADA;
      quantidade_palavras++;

    }
  }

  // libera a memoria do buffer
  free(conteudo);

  // printa o resultado
  printf("\nNUMERO DE LINHAS: %d\nNUMERO DE PALAVRAS: %d\n\n",
    quantidade_linhas,
    quantidade_palavras
  );

  return 0;
}