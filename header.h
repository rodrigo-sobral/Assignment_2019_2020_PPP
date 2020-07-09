/*----------------BIBLIOTECAS-------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
/*------------------CONSTANTES------------------*/

#define DIM_REGISTOS 100
#define DIM_LINHA 1000

/*---------------------------ESTRUTURAS--------------------------*/

/*ESTRUTURA DAS HORAS*/
typedef struct {
    int horas;
    int minutos;
} tempo;

/*ESTRUTURA DOS PDIS*/
typedef struct pt_int *node_pdis;
typedef struct pt_int{
    char nome[DIM_REGISTOS];
    char descricao[DIM_LINHA];
    int id, popularidade, hot;
    tempo hora_entrada, hora_saida;
    /*-------------------*/
    node_pdis next;
} pdi;

/*ESTRUTURA DOS LOCAIS*/
typedef struct local *node_locais;
typedef struct local {
    char nome[DIM_REGISTOS-30], pais[DIM_REGISTOS-30];
    int id, popularidade;
    node_pdis ponto_interesse;
    /*--------------*/
    node_locais next;
} cidade;

/*ESTRUTURA DA DATA*/
typedef struct {
    int dia, mes, ano;
}dt_nascimento;

/*ESTRUTURA DOS USERS*/
typedef struct humano *node_pessoas;
typedef struct humano{
    char nome[DIM_REGISTOS];
    char morada[DIM_REGISTOS];
    int id, telefone;
    dt_nascimento data_nasc;
    /*Preferencias*/
    node_locais locais;
    node_pdis pdis;
    node_pdis hot;
    /*------------*/
    node_pessoas next;
}pessoa;


/*---------------------LISTAGEM DAS FUNCOES UTILIZADAS----------------*/
/*FUNCOES AUXILIARES*/
void menu_principal();
void decisao(node_pessoas listapessoas);
char *maiusculas(char *str);

int verifica_data(dt_nascimento data);
int verifica_telefone(int telefone);
int verifica_string(char *str, int m);
int verifica_numero(char *str1, int m);

int cont_users(node_pessoas no_contado);
int cont_pdis(node_locais lista);
int cont_pdis_pref(node_pdis lista);
int cont_locais_pref(node_locais lista);
float cont_pop_pdis(node_locais lista_locs);

node_locais altera_pop_loc(node_locais lista_locais, int id_local, int m);
node_locais altera_pop_pdi(node_locais lista_locais, int id_pdi, int m);
node_locais altera_hot_pdi(node_locais lista_locais, int id_pdi, int m);

int apaga_local_preferido(struct local** head_ref, int id_local);
int apaga_pdi_preferido(struct pt_int** head_ref, int id_pdi);

void troca_nos_locs(node_locais trocado1, node_locais trocado2);
void troca_nos_pdis(node_pdis trocado1, node_pdis trocado2);

node_pdis ordenar_pdis_pop(node_pdis start);
node_pdis ordenar_pdis_id(node_pdis start);
node_pdis ordenar_pdis_alfa(node_pdis start);
node_locais ordenar_locs_pop(node_locais start);
node_locais ordenar_locs_id(node_locais start);
node_locais ordenar_locs_alfa(node_locais start);

node_locais encontra_local_escolhido(node_locais list_locs, int id_loc);
node_pdis encontra_pdi_escolhido(node_locais list_locs, int id_pdi);
node_pessoas procura_user(node_pessoas list_users);

node_pessoas adiciona_lista_pessoas(node_pessoas listapessoas, node_locais locais_preferidos, node_pdis pdis_preferidos, node_pdis ponto_hot, char nome[], char morada[], int telefone, dt_nascimento nascimento, int id);
node_locais adiciona_lista_locais(node_locais listalocais, node_pdis listapdis, char nome[], char pais[], int pop, int id);
node_pdis adiciona_lista_pdis(node_pdis lista, char nome[], char descricao[], tempo entrada, tempo saida, int id, int pop, int hot);

node_locais escolher_pdi(node_pessoas pdi_preferido, node_locais listalocais);
node_locais escolher_hot(node_pessoas ponto_hot, node_locais listalocais);

node_pessoas remove_pdi(node_pessoas listapessoas, node_locais listalocs);
node_pessoas remove_hot(node_pessoas listapessoas, node_locais lista_locais);

node_pessoas adiciona_preferencia_local(node_pessoas local_preferido, node_locais listalocais);
node_pessoas adiciona_preferencia_pdi(node_pessoas pdi_preferido, node_locais listalocais);

node_pessoas remove_preferencia_pdi(node_pessoas listausers, node_locais lista_locs);
node_pessoas remove_preferencia_local(node_pessoas listausers, node_locais lista_locs);

node_pessoas le_fich_users(node_pessoas no_lido, node_locais list_locs);
node_locais le_fich_cidades (node_locais cidades);


/*FUNCOES PRINCIPAIS*/
int armazena (node_pessoas no_armazenado);
node_pessoas regista(node_pessoas no_registado, int id);
int altera(node_pessoas no_alterado, int M_id);
void imprime_lista_pessoas(node_pessoas no_escolhido);
void imprime_lista_locais_e_pdis(node_locais cid_impressa, int m);
node_locais ordenamento(node_locais list_locs);
node_pessoas opcao_preferencia(node_pessoas user_opcional, node_locais local_opcional, int m);
void fazer_viagem(node_pessoas lista_users, node_locais lista_locs, int M_id);
int main();

