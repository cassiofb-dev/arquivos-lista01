#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(1)

struct Registro {
  int id_inscricao;
  char cpf[4];
  char fim_linha;
};

int main(int argc, char* argv[]) {

  // verifica se o argumento foi passada de maneira correta
  if(argc != 3) {
    fprintf(stderr, "uso: <nome do arquivo de entrada> <nome do arquivo de saida>");
    return 1;
  }

  // ponteiro do arquivo aberto para leitura
  FILE *ponteiro_arquivo_leitura = fopen(argv[1], "r");

  // verifica a existencia do arquivo
  if(ponteiro_arquivo_leitura == NULL) {
    fprintf(stderr, "Arquivo de entrada inexistente");
    return 1;
  }

  // pega o tamanho do arquivo
  fseek(ponteiro_arquivo_leitura, 0L, SEEK_END);
  // armazena o tamanho em bytes
  long tamanho_arquivo = ftell(ponteiro_arquivo_leitura);

  // armazena a quantidade de registros
  long numero_registros = tamanho_arquivo / sizeof(struct Registro);

  // verifica rapidamente se o arquivo esta corrompido
  if(tamanho_arquivo % sizeof(struct Registro) ){
    fprintf(
      stderr,
      "Arquivo corrompido\nTamanho do arquivo: %d\nTamanho do registro: %d\nNumero de registros: %d",
      tamanho_arquivo,
      sizeof(struct Registro),
      numero_registros
    );
    return 1;
  }

  // reseta o indicador de posicao do arquivo
  fseek(ponteiro_arquivo_leitura, 0L, SEEK_SET);

  // abre arquivo para escrita
  FILE *ponteiro_arquivo_escrita = fopen(argv[2], "w");

  //verifica se o arquivo foi aberto com sucesso
  if(ponteiro_arquivo_escrita == NULL) {
    fprintf(stderr, "Erro ao criar arquivo de escrita.");
    return 1;
  }

  // instancia de Registro
  struct Registro cadastro;
  // instancia de Registro;
  struct Registro cadastro_valido;

  // armazena a quantidade de registros lidos
  long registros_lidos = 0;

  fread(
    &cadastro,
    sizeof(struct Registro),
    1,
    ponteiro_arquivo_leitura
  );

  cadastro_valido = cadastro;

  // percorre os registros (no minimo 2)
  while(registros_lidos < numero_registros) {
    printf(
      "cpf: %.4s | id: %d: valido cpf: %.4s id: %d",
      cadastro.cpf,
      cadastro.id_inscricao,
      cadastro_valido.cpf,
      cadastro_valido.id_inscricao
    );
    fread(
      &cadastro,
      sizeof(struct Registro),
      1,
      ponteiro_arquivo_leitura
    );
    registros_lidos ++;

    if(strncmp(cadastro.cpf, cadastro_valido.cpf, 4) == 0) {
      if(cadastro.id_inscricao > cadastro_valido.id_inscricao) {
        cadastro_valido = cadastro;
      }
      if(registros_lidos == numero_registros) {
        fwrite(
          &cadastro_valido,
          sizeof(struct Registro),
          1,
          ponteiro_arquivo_escrita
        );
        printf(" <-- ESCRITO!");
      }
    } else {
      fwrite(
        &cadastro_valido,
        sizeof(struct Registro),
        1,
        ponteiro_arquivo_escrita
      );
      printf(" <-- ESCRITO!");
      cadastro_valido = cadastro;
    }
    printf("\n");
  }
}