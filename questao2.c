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
      "Arquivo corrompido\nTamanho do arquivo: %d\nTamanho do registro: %d",
      tamanho_arquivo,
      sizeof(struct Registro)
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
  struct Registro cadastro_anterior;

  // armazena a quantidade de registros lidos
  long registros_lidos = 0;

  // le o primeiro registro
  fread(
    &cadastro,
    sizeof(struct Registro),
    1,
    ponteiro_arquivo_leitura
  );

  registros_lidos++;

  // percorre os registros (no minimo 2)
  while(registros_lidos < numero_registros) {
    cadastro_anterior = cadastro;

    fread(
      &cadastro,
      sizeof(struct Registro),
      1,
      ponteiro_arquivo_leitura
    );

    registros_lidos++;

    if(strncmp(cadastro.cpf, cadastro_anterior.cpf, 15) == 0) {
      // verifica se os cpfs sao iguais

      if(cadastro.id_inscricao < cadastro_anterior.id_inscricao) {
        // escolhe o maior id
        cadastro = cadastro_anterior;
      }

      if(registros_lidos == numero_registros) {
        // caso todos os ultimos forem iguais
        fwrite(
          &cadastro,
          sizeof(struct Registro),
          1,
          ponteiro_arquivo_escrita
        );
      }
    } else {
      // se for diferente pode escrever
      fwrite(
        &cadastro,
        sizeof(struct Registro),
        1,
        ponteiro_arquivo_escrita
      );
    }
  }
}