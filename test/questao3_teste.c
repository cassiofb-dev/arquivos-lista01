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

  // abre segundo arquivo para leitura
  FILE *ponteiro_arquivo_leitura_dois = fopen(argv[2], "r");

  //verifica se o arquivo foi aberto com sucesso
  if(ponteiro_arquivo_leitura_dois == NULL) {
    fprintf(stderr, "Arquivo de entrada dois inexistente");
    return 1;
  }

  // abre arquivo para escrita
  FILE *ponteiro_arquivo_escrita = fopen("cpf_printado.dat", "w");

  //verifica se o arquivo foi aberto com sucesso
  if(ponteiro_arquivo_escrita == NULL) {
    fprintf(stderr, "Nao foi possivel abrir arquivo para escrita");
    return 1;
  }

  // instancia de Registro para arquivo um
  struct Registro cadastro_um;
  // instancia de Registro para arquivo dois;
  struct Registro cadastro_dois;
  // instancia de Registro para guardar o ultimo cadastro printado
  struct Registro cadastro_printado;

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

    if(
      strncmp(cadastro_um.cpf, cadastro_dois.cpf, 4) == 0 &&
      strncmp(cadastro_um.cpf, cadastro_printado.cpf, 4) != 0
    ) {
      // se o cpf for igual printa e nao printado
      printf("%.4s\n", cadastro_um.cpf);
      fwrite(
        &cadastro_um,
        sizeof(struct Registro),
        1,
        ponteiro_arquivo_escrita
      );
      cadastro_printado = cadastro_um;
    }
    
    if(strncmp(cadastro_um.cpf, cadastro_dois.cpf, 4) < 0) {
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