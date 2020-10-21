#include <stdio.h>
#include <stdlib.h>

struct Registro {
  int id_inscricao;
  char curso[20];
  char cpf[15];
  char dataNacimento[11];
  char sexo;
  char email[40];
  char opcaoQuadro;
};

int main(int argc, char* argv[]) {

  // verifica se o argumento foi passada de maneira correta
  if(argc != 3) {
    fprintf(stderr, "uso: <nome do arquivo de entrada 1> <nome do arquivo de entrada 2>");
    return 1;
  }

  // ponteiro do arquivo aberto para leitura
  FILE *ponteiro_arquivo_leitura_um = fopen(argv[1], "r");

  // verifica a existencia do arquivo
  if(ponteiro_arquivo_leitura_um == NULL) {
    fprintf(stderr, "Arquivo de entrada um inexistente");
    return 1;
  }

  // pega o tamanho do arquivo
  fseek(ponteiro_arquivo_leitura_um, 0L, SEEK_END);
  // armazena o tamanho em bytes
  long tamanho_arquivo = ftell(ponteiro_arquivo_leitura_um);

  // verifica rapidamente se o arquivo esta corrompido
  if(tamanho_arquivo % sizeof(struct Registro) ){
    fprintf(
      stderr,
      "Arquivo corrompido\nTamanho do arquivo: %d\nTamanho do registro: %d",
      tamanho_arquivo,
      sizeof(struct Registro)
    );
    return 1;
  }

  // reseta o indicador de posicao do arquivo
  fseek(ponteiro_arquivo_leitura_um, 0L, SEEK_SET);

  // abre arquivo para escrita
  FILE *ponteiro_arquivo_leitura_dois = fopen(argv[2], "w");

  //verifica se o arquivo foi aberto com sucesso
  if(ponteiro_arquivo_leitura_dois == NULL) {
    fprintf(stderr, "Arquivo de entrada dois inexistente");
    return 1;
  }

  // instancia de Registro para arquivo um
  struct Registro cadastro_um;
  // instancia de Registro para arquivo dois;
  struct Registro cadastro_dois;

  // verifica se arquivo um foi lido com sucesso
  int codigo_leitura_um = fread(
    &cadastro_um,
    sizeof(struct Registro),
    1,
    ponteiro_arquivo_leitura_um
  );

  // verifica se arquivo dois foi lido com sucesso
  int codigo_leitura_dois = fread(
    &cadastro_dois,
    sizeof(struct Registro),
    1,
    ponteiro_arquivo_leitura_dois
  );

  while(codigo_leitura_um && codigo_leitura_dois){

    if(cadastro_um.cpf == cadastro_dois.cpf) {
      // se o cpf for igual printa
      printf("\n%s", cadastro_um.cpf);
    }
    
    if(cadastro_um.cpf < cadastro_dois.cpf) {
      // se cpf do arquivo um for menor
      codigo_leitura_um = fread(
        &cadastro_um,
        sizeof(struct Registro),
        1,
        ponteiro_arquivo_leitura_um
      );
    } else {
      // cpf do arquivo dois menor
      codigo_leitura_dois = fread(
        &cadastro_dois,
        sizeof(struct Registro),
        1,
        ponteiro_arquivo_leitura_dois
      );
    }
  };
}