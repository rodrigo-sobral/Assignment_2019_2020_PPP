#include "header.h"

/*---------------------FUNCOES AUXILIARES-----------------------*/

/*PRINTS DO MENU PRINCIPAL*/
void menu_principal() {
    system("cls");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t\t\tMENU PRINCIPAL\t\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
    printf("\t|\t1-Registar Um User\t\t\t\t|\n");
    printf("\t|\t2-Alterar Um User\t\t\t\t|\n");
    printf("\t|\t3-Listar Todos os Users\t\t\t\t|\n");
    printf("\t|\t4-Listar Locais e PDIs\t\t\t\t|\n");
    printf("\t|\t5-Adicionar Preferencia\t\t\t\t|\n");
    printf("\t|\t6-Remover Preferencia\t\t\t\t|\n");
    printf("\t|\t7-Construir a Minha Viagem\t\t\t|\n");
    printf("\t|\t0-SAIR\t\t\t\t\t\t|\n");
    printf("\t|-------------------------------------------------------|\n");
}

/*FUNCAO PARA UTILIZADOR DECIDIR SE QUER FAZER MAIS ALGUMA COISA*/
void decisao(node_pessoas listapessoas) {
    int keep;
    char s_keep[DIM_REGISTOS];
    do {
        do {
            printf("\n\tQuer fazer mais alguma coisa?\n\n1-SIM\n0-NAO\n\nOpcao=> ");
            fgets(s_keep, DIM_REGISTOS, stdin);
            keep=verifica_numero(s_keep, 0);
        } while (keep==-2);
        printf("%d", keep);
        if (keep==0) {
            armazena(listapessoas);
            printf("\n\tO PROGRAMA FOI ENCERRADO COM SUCESSO!!\n");
            exit(0);
        }
    } while (keep!=0 && keep!=1);

}

/*FUNCAO MAIUSCULAS NOS NOMES E MORADAS*/
char *maiusculas(char *str) {
    int i;
    str[0]=toupper(str[0]);
    for (i=1; i<DIM_REGISTOS-1; i++) {
        if (str[i]==' ') str[i+1]=toupper(str[i+1]);
    }
    return str;
}


/*VERIFICACOES*/
int verifica_data(dt_nascimento data) {
    if (data.ano>1910 && data.ano<2010) {
        if (data.mes==1||data.ano==3||data.mes==5||data.mes==7||data.mes==8||data.mes==10||data.mes==12) {
            if (data.dia>0&&data.dia<=31) return 1;
            else return 0;
        }
        else if (data.mes==2) {
            if (data.dia>0&&data.dia<28) return 1;
            else return 0;
        }
        else if (data.mes==4||data.mes==6||data.mes==9||data.mes==11) {
            if (data.dia>0&&data.dia<=30) return 1;
            else return 0;
        }
        else return 0;
    }
    else return 0;
}

int verifica_telefone(int telefone) {
    if ((telefone>=910000000 && telefone<940000000)||(telefone>=960000000 && telefone<970000000)) return 1;
    else {
        printf("\nERRO!! NAO INTRODUZIU UM TELEFONE CORRETO!!\n\n");
        return 0;
    }
}

int verifica_numero(char *str1, int m){
    char aux;
    char* str2;
    int opcao;
    if (str1[strlen(str1)-1] == '\n'){
        opcao = (int)strtol(str1, &str2, 10);
        if (*str2 == '\n') return opcao;
        else{
            if (m==0) printf("\nERRO!! NAO INTRODUZIU UMA OPCAO CORRETA!!\n");
            if (m==1) printf("\nERRO!! NAO INTRODUZIU UM ID CORRETO!!\n");
            if (m==2) printf("\nERRO!! NAO INTRODUZIU UM TELEFONE CORRETO!!\n");
            if (m==3) printf("\nERRO!! NAO INTRODUZIU UM DADO CORRETO!!\n");
            return -2;
        }
    }
    else{
        do{
            aux = (char)getchar();
        }while (aux!= '\n');
    }
    return 0;
}

int verifica_string(char *str, int m){
    int i;
    for (i=0 ; i<strlen(str);i++){
        if (str[i] >= '0' && str[i] <= '9') {
            if (m==1) printf("\nERRO!! INTRODUZA O SEU NOME CORRETAMENTE!!\n\n");
            if (m==2) printf("\nERRO!! INTRODUZA A SUA MORADA CORRETAMENTE!!\n\n");
            return 0;
        }
    }
    return 1;
}

/*FUNCAO CONTADORA DE USERS, PDIS OU LOCAIS*/
int cont_users(node_pessoas lista) {
    int id=0;
    node_pessoas cont= lista;

    if (cont==NULL) return 0;
    while (cont) {
        cont=cont->next;
        id++;
    }
    return id;
}

int cont_pdis(node_locais lista) {
    int id=0;
    node_locais cont_locais= lista;
    node_pdis cont_pdis;

    while (cont_locais) {
        cont_pdis= cont_locais->ponto_interesse;
        while (cont_pdis) {
            id++;
            cont_pdis=cont_pdis->next;
        }
        cont_locais=cont_locais->next;
    }
    return id;
}

int cont_pdis_pref(node_pdis lista) {
    node_pdis percorre_lista= lista;
    int contador=0;
    if (lista==NULL) return 0;
    while (percorre_lista) {
        contador++;
        percorre_lista=percorre_lista->next;
    }
    return contador;
}

int cont_locais_pref(node_locais lista) {
    node_locais percorre_lista= lista;
    int contador=0;
    if (lista==NULL) return 0;
    while (percorre_lista) {
        contador++;
        percorre_lista=percorre_lista->next;
    }
    return contador;
}

float cont_pop_pdis(node_locais lista_locs) {
    node_locais perc_locs=lista_locs;
    node_pdis perc_pdis;
    float contador_popularidade=0;
    while (perc_locs) {
        perc_pdis=perc_locs->ponto_interesse;
        while (perc_pdis) {
            contador_popularidade+=perc_pdis->popularidade;
            perc_pdis=perc_pdis->next;
        }
        perc_locs=perc_locs->next;
    }
    return contador_popularidade;
}


/*FUNCOES PARA ALTERAR A POPULARIDADE DE LOCAIS E PDIS*/
node_locais altera_pop_loc(node_locais lista_locs, int id_local, int m) {
  node_locais percorre_locais = lista_locs;
  while(percorre_locais){
    if(percorre_locais->id == id_local){
      if (m==0) percorre_locais->popularidade--;
      if (m==1) percorre_locais->popularidade++;
      break;
    }
    else percorre_locais = percorre_locais->next;
  }
  return lista_locs;
}

node_locais altera_pop_pdi(node_locais lista_locais, int id_pdi, int m) {
  node_locais percorre_locais = lista_locais;
  node_pdis percorre_pdis;
  while(percorre_locais){
    percorre_pdis = percorre_locais->ponto_interesse;
    while(percorre_pdis){
      if(percorre_pdis->id==id_pdi){
        if (m==0) percorre_pdis->popularidade--;
        if (m==1) percorre_pdis->popularidade++;
        break;
      }
      else percorre_pdis = percorre_pdis->next;
    }
    percorre_locais = percorre_locais->next;
  }
  return lista_locais;
}

node_locais altera_hot_pdi(node_locais lista_locais, int id_pdi, int m) {
  node_locais percorre_locais = lista_locais;
  node_pdis percorre_pdis;
  while(percorre_locais){
    percorre_pdis = percorre_locais->ponto_interesse;
    while(percorre_pdis){
      if(percorre_pdis->id==id_pdi){
        if (m==0) percorre_pdis->hot--;
        if (m==1) percorre_pdis->hot++;
        break;
      }
      else percorre_pdis = percorre_pdis->next;
    }
    percorre_locais = percorre_locais->next;
  }
  return lista_locais;
}


/*FUNCOES PARA APAGAR UM NO DA LISTA*/
int apaga_local_preferido(struct local** head_ref, int id_local) {
    struct local* temp = *head_ref, *prev;

    if(temp!= NULL&&temp->id==id_local){
        *head_ref = temp->next;
        free(temp);
        return 1;
    }

    while(temp!=NULL && temp->id != id_local){
        prev = temp;
        temp = temp->next;
    }
    if(temp == NULL) return 0;

    prev->next = temp->next;
    free(temp);
    return 1;
}

int apaga_pdi_preferido(struct pt_int** head_ref, int id_pdi) {
  struct pt_int* temp = *head_ref, *prev;

  if(temp!= NULL&&temp->id==id_pdi){
    *head_ref = temp->next;
    free(temp);
    return 1;
  }

  while(temp!=NULL && temp->id != id_pdi){
    prev = temp;
    temp = temp->next;
  }
  if(temp == NULL) return 0;

  prev->next = temp->next;
  free(temp);
  return 1;
}


/*FUNCOES PARA TROCAR OS LOCAIS E PDIS*/
void troca_nos_locs(node_locais trocado1, node_locais trocado2) {
  int id=trocado1->id, popularidade=trocado1->popularidade;
  char nome[DIM_REGISTOS], pais[DIM_REGISTOS];
  strcpy(nome, trocado1->nome);
  node_pdis ponto_int = trocado1->ponto_interesse;
  strcpy(pais, trocado1->pais);

  trocado1->id = trocado2->id;
  trocado1->popularidade = trocado2->popularidade;
  strcpy(trocado1->nome, trocado2->nome);
  trocado1->ponto_interesse = trocado2->ponto_interesse;
  strcpy(trocado1->pais, trocado2->pais);

  trocado2->id=id;
  trocado2->popularidade=popularidade;
  strcpy(trocado2->nome, nome);
  trocado2->ponto_interesse = ponto_int;
  strcpy(trocado2->pais, pais);
}

void troca_nos_pdis(node_pdis trocado1, node_pdis trocado2) {
  int id=trocado1->id, popularidade=trocado1->popularidade, hot=trocado1->hot;
  tempo hora_entrada=trocado1->hora_entrada, hora_saida=trocado1->hora_saida;
  char nome[DIM_REGISTOS], descricao[DIM_LINHA];
  strcpy(nome, trocado1->nome);
  strcpy(descricao, trocado1->descricao);

  trocado1->hora_entrada=trocado2->hora_entrada;
  trocado1->hora_saida=trocado2->hora_saida;
  trocado1->id = trocado2->id;
  trocado1->popularidade = trocado2->popularidade;
  trocado1->hot= trocado2->hot;
  strcpy(trocado1->nome, trocado2->nome);
  strcpy(trocado1->descricao, trocado2->descricao);

  trocado2->hora_entrada=hora_entrada;
  trocado2->hora_saida=hora_saida;
  trocado2->id=id;
  trocado2->popularidade=popularidade;
  trocado2->hot=hot;
  strcpy(trocado2->nome, nome);
  strcpy(trocado2->descricao, descricao);
}


/*FUNCOES PARA ORDENAR OS LOCAIS E PDIS*/
node_pdis ordenar_pdis_pop(node_pdis start) {
    int troca;
    node_pdis atual, anterior = NULL;
    if (start == NULL) return NULL;
    do {
        troca = 0;
        atual = start;
        while (atual->next != anterior) {
            if (atual->popularidade < atual->next->popularidade) {
                troca_nos_pdis(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}

node_pdis ordenar_pdis_id(node_pdis start) {
    int troca;
    node_pdis atual, anterior = NULL;

    if (start == NULL) return NULL;

    do {
        troca = 0;
        atual = start;
        while (atual->next != anterior) {
            if (atual->id > atual->next->id) {
                troca_nos_pdis(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}

node_pdis ordenar_pdis_alfa(node_pdis start) {
    int troca;
    node_pdis atual, anterior = NULL;

    if (start == NULL)
        return NULL;
    do {
        troca = 0;
        atual = start;
        while (atual->next != anterior) {
            if (strcmp(atual->nome,atual->next->nome)>0) {
                troca_nos_pdis(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}

node_locais ordenar_locs_pop(node_locais start) {
    int troca;
    node_locais atual, anterior = NULL;
    if (start == NULL) return start;
    do {
        troca = 0;
        atual = start;
        while (atual->next != anterior) {
            ordenar_pdis_pop(atual->ponto_interesse);
            if (atual->popularidade < atual->next->popularidade) {
                troca_nos_locs(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}

node_locais ordenar_locs_id(node_locais start) {
    int troca;
    node_locais atual, anterior = NULL;
    if (start == NULL) return start;
    do {
        troca = 0;
        atual = start;
        while (atual->next != anterior) {
            ordenar_pdis_id(atual->ponto_interesse);
            if (atual->id > atual->next->id) {
                troca_nos_locs(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}

node_locais ordenar_locs_alfa(node_locais start) {
    int troca;
    node_locais atual, anterior = NULL, temp=start;

    if (start == NULL) return start;
    do {
        troca = 0;
        atual = temp;
        while (atual->next != anterior) {
            ordenar_pdis_alfa(atual->ponto_interesse);
            if (strcmp(atual->nome,atual->next->nome)>0) {
                troca_nos_locs(atual, atual->next);
                troca = 1;
            }
            atual= atual->next;
        }
        anterior=atual;
    } while (troca);
    return start;
}


/*FUNCOES PARA ENCONTRAR UM NO NUMA LISTA*/
node_locais encontra_local_escolhido(node_locais list_locs, int id_loc) {
    node_locais percorre_locais=list_locs;
    node_locais loc_encontrado=NULL;
    while (percorre_locais) {
        if (percorre_locais->id==id_loc) {
            loc_encontrado=adiciona_lista_locais(loc_encontrado, percorre_locais->ponto_interesse, percorre_locais->nome, percorre_locais->pais, percorre_locais->popularidade, percorre_locais->id);
            break;
        }
        else percorre_locais=percorre_locais->next;
    }
    return loc_encontrado;
}

node_pdis encontra_pdi_escolhido(node_locais list_locs, int id_pdi) {
    node_locais percorre_locs=list_locs;
    node_pdis percorre_pdis=NULL;
    node_pdis pdi_encontrado=NULL;
    while (percorre_locs) {
        percorre_pdis=percorre_locs->ponto_interesse;
        while (percorre_pdis) {
            if (percorre_pdis->id==id_pdi) {
                pdi_encontrado=adiciona_lista_pdis(pdi_encontrado, percorre_pdis->nome, percorre_pdis->descricao, percorre_pdis->hora_entrada, percorre_pdis->hora_saida, percorre_pdis->id, percorre_pdis->popularidade, percorre_pdis->hot);
                break;
            }
            else percorre_pdis=percorre_pdis->next;
        }
        percorre_locs=percorre_locs->next;
    }
    return pdi_encontrado;
}

node_pessoas procura_user(node_pessoas list_users) {
    node_pessoas user_escolhido=list_users;
    int user, n_users=cont_users(list_users)-1;
    char s_user[DIM_REGISTOS];

    volta:
    imprime_lista_pessoas(list_users);
    do {
        printf("Que USER pretende escolher?\n\nUSER=> ");
        fgets(s_user, DIM_REGISTOS, stdin);
        user=verifica_numero(s_user, 1);
    } while (user==-2);

    if (user>n_users || user<0) {
        system("cls");
        printf("ERRO!! O USER que inseriu nao existe!!\n\n");
        goto volta;
    }

    while (user_escolhido->id!=user) user_escolhido=user_escolhido->next;
    return user_escolhido;
}


/*FUNCAO PARA INSERIR AS PESSOAS, OS LOCAIS E OS PDIS POR ORDEM ALFABETICA*/
node_pessoas adiciona_lista_pessoas(node_pessoas listapessoas, node_locais locais_preferidos, node_pdis pdis_preferidos, node_pdis ponto_hot, char nome[], char morada[], int telefone, dt_nascimento nascimento, int id) {
    node_pessoas no_adicionado, percorre_lista;
    no_adicionado = (node_pessoas) malloc(sizeof(struct humano));

    strcpy(no_adicionado->nome, nome);
    strcpy(no_adicionado->morada, morada);
    no_adicionado->data_nasc = nascimento;
    no_adicionado->telefone = telefone;
    no_adicionado->id = id;
    no_adicionado->locais = locais_preferidos;
    no_adicionado->pdis= pdis_preferidos;
    no_adicionado->hot= ponto_hot;
    no_adicionado->next = NULL;

    if (listapessoas==NULL) {
        listapessoas=no_adicionado;
    }
    else {
        percorre_lista=listapessoas;
        while(percorre_lista->next) percorre_lista= percorre_lista->next;
        percorre_lista->next= no_adicionado;
    }
    return listapessoas;
}

node_locais adiciona_lista_locais(node_locais listalocs, node_pdis listapdis, char *nome, char *pais, int pop, int id){
  node_locais ultimo_no, no_adicionado;

  no_adicionado = (node_locais) malloc(sizeof(cidade));
  no_adicionado->popularidade = pop;
  no_adicionado->id = id;
  strcpy(no_adicionado->nome, nome);
  strcpy(no_adicionado->pais, pais);
  no_adicionado->ponto_interesse = listapdis;
  no_adicionado->next = NULL;

  if(listalocs == NULL) listalocs = no_adicionado;
  else{
    ultimo_no = listalocs;
    while(ultimo_no->next) ultimo_no = ultimo_no->next;
    ultimo_no->next=no_adicionado;
  }
  return listalocs;
}

node_pdis adiciona_lista_pdis(node_pdis lista_pdis, char nome[], char descricao[], tempo entrada, tempo saida, int id, int pop, int hot) {
    node_pdis ultimo_no, no_adicionado;
    no_adicionado = (node_pdis) malloc(sizeof(struct pt_int));

    no_adicionado->popularidade = pop;
    no_adicionado->id = id;
    strcpy(no_adicionado->nome, nome);
    strcpy(no_adicionado->descricao, descricao);
    no_adicionado->hora_entrada = entrada;
    no_adicionado->hora_saida = saida;
    no_adicionado->hot = hot;
    no_adicionado->next = NULL;

    if(lista_pdis == NULL) lista_pdis = no_adicionado;
    else{
        ultimo_no = lista_pdis;
        while(ultimo_no->next) {
            ultimo_no = ultimo_no ->next;
        }
        ultimo_no->next = no_adicionado;
    }
    return lista_pdis;
}


/*FUNCOES PARA ADICIONAR OU REMOVER UM PDI PREFERIDO E UM PONTO HOT*/
node_locais escolher_pdi(node_pessoas listapessoas, node_locais listalocais) {
    volta:
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(listapessoas);
    node_pdis percorre_pdis_preferidos=user_escolhedor->pdis;
    node_pdis pdi_escolhido;
    int preferencia;
    char s_preferencia[DIM_REGISTOS];

    imprime_lista_locais_e_pdis(listalocais, 1);
    do {
        printf("\n\nInsira o ID do seu PDI preferido: [-1 para sair]\nPDI=> ");
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia=verifica_numero(s_preferencia, 1);
    } while (preferencia==-2);

    if (preferencia==-1) return listalocais;

    /*Verifica se o PDI existe*/
    if (preferencia>cont_pdis(listalocais)-1) {
        system("cls");
        printf("ERRO!! O PDI que inseriu nao existe!!\n\n");
        goto volta;
    }
    pdi_escolhido=encontra_pdi_escolhido(listalocais, preferencia);

    while (percorre_pdis_preferidos) {
        if (percorre_pdis_preferidos->id==pdi_escolhido->id) {
            printf("\nERRO!! %s ja inseriu uma preferencia em %s\n\n", user_escolhedor->nome, pdi_escolhido->nome);
            goto volta;
        }
        else percorre_pdis_preferidos=percorre_pdis_preferidos->next;
    }
    user_escolhedor->pdis=adiciona_lista_pdis(user_escolhedor->pdis, pdi_escolhido->nome, pdi_escolhido->descricao, pdi_escolhido->hora_entrada, pdi_escolhido->hora_saida, pdi_escolhido->id, pdi_escolhido->popularidade, pdi_escolhido->hot);
    altera_pop_pdi(listalocais, preferencia, 1);
    printf("\nPDI ADICIONADO COM SUCESSO\n\n");
    sleep(2);
    return listalocais;
}

node_locais escolher_hot(node_pessoas listapessoas, node_locais listalocais) {
    volta:
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(listapessoas);
    node_pdis pdi_escolhido;
    int preferencia;
    char s_preferencia[DIM_REGISTOS];

    printf("\nPDIS DISPONIVEIS:\n\n");
    imprime_lista_locais_e_pdis(listalocais, 1);

    do {
        printf("\n\nInsira o ID do seu PDI HOT: [-1 para sair]\nHOT=> ");
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia=verifica_numero(s_preferencia, 1);
    } while(preferencia==-2);

    if (preferencia==-1) return listalocais;

    /*Verifica se o PDI existe*/
    if (preferencia>cont_pdis(listalocais)-1) {
        system("cls");
        printf("ERRO!! O Local que inseriu nao existe!!\n\n");
        goto volta;
    }

    pdi_escolhido=encontra_pdi_escolhido(listalocais, preferencia);

    /*Verifica se ja foi inserido um HOT*/
    if (user_escolhedor->hot!=NULL) {
        printf("\nERRO!! %s ja inseriu um ponto HOT em %s e nao pode inserir mais!!\n\n", user_escolhedor->nome, user_escolhedor->hot->nome);
        goto volta;
    }
    else {
        altera_hot_pdi(listalocais, preferencia, 1);
        user_escolhedor->hot=adiciona_lista_pdis(user_escolhedor->hot, pdi_escolhido->nome, pdi_escolhido->descricao, pdi_escolhido->hora_entrada, pdi_escolhido->hora_saida, pdi_escolhido->id, pdi_escolhido->popularidade, pdi_escolhido->hot);
        printf("\nPONTO HOT ADICIONADO COM SUCESSO\n\n");
        sleep(2);
    }
    return listalocais;
}

node_pessoas remove_pdi(node_pessoas listapessoas, node_locais listalocs) {
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(listapessoas);
    node_pdis percorre_pdis_preferidos=user_escolhedor->pdis;
    int preferencia;
    char s_preferencia[DIM_REGISTOS];

    printf("\nQual dos PDIS pretende remover? [-1 para sair]\n");
    while(percorre_pdis_preferidos) {
        printf("\n[%d]%s", percorre_pdis_preferidos->id, percorre_pdis_preferidos->nome);
        percorre_pdis_preferidos=percorre_pdis_preferidos->next;
    }

    do {
        printf("\n\nOpcao=> ");
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia=verifica_numero(s_preferencia, 1);
    } while(preferencia==-2);

    if (preferencia==-1) return listapessoas;

    if (apaga_pdi_preferido(&user_escolhedor->pdis, preferencia)!=0) {
        altera_pop_pdi(listalocs, preferencia, 0);
        printf("\n\nPDI REMOVIDO COM SUCESSO\n\n");
        sleep(2);
    }
    else printf("O PDI que inseriu nao se encontra nos PDIS preferidos de %s", user_escolhedor->nome);
    return listapessoas;
}

node_pessoas remove_hot(node_pessoas listapessoas, node_locais lista_locais) {
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(listapessoas);
    int preferencia, id_removido=0;
    char s_preferencia[DIM_REGISTOS];

    if (user_escolhedor->hot==NULL) {
        printf("ERRO!! %s nao tem nenhum Ponto HOT escolhido!!", user_escolhedor->nome);
        return listapessoas;
    }

    do {
        printf("\nTem a certeza que pretende remover o Ponto HOT:\t[%d]%s\n[1- Sim, 0-Nao]\nOpcao=> ", user_escolhedor->hot->id, user_escolhedor->hot->nome);
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia= verifica_string(s_preferencia, 0);
    } while(preferencia==-2);

    if (preferencia==0) return listapessoas;
    else {
        id_removido=user_escolhedor->hot->id;
        altera_pop_pdi(lista_locais, id_removido, 0);
        user_escolhedor->hot=NULL;
        printf("\n\nPDI HOT REMOVIDO COM SUCESSO\n\n");
        sleep(2);
    }
    return listapessoas;
}


/*FUNCAO PARA INSERIR OU REMOVER AS PREFERENCIAS DOS LOCAIS E PDIS*/
node_pessoas adiciona_preferencia_local(node_pessoas user, node_locais listalocais) {
    volta:
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(user);
    node_locais percorre_locais_preferidos= user_escolhedor->locais;
    node_locais loc_escolhido=NULL;
    int preferencia;
    char s_preferencia[DIM_REGISTOS];

    printf("\nLOCAIS DISPONIVEIS:\n\n");
    imprime_lista_locais_e_pdis(listalocais, 0);
    do {
        printf("\n\nInsira o ID do seu Local preferido? [-1 para sair]\nLocal=> ");
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia= verifica_numero(s_preferencia, 1);
    } while (preferencia==-2);

    if (preferencia==-1) return user;

    /*Verifica se o Local existe*/
    if (preferencia>cont_locais_pref(listalocais)-1) {
        system("cls");
        printf("ERRO!! O Local que inseriu nao existe!!\n\n");
        goto volta;
    }

    loc_escolhido=encontra_local_escolhido(listalocais, preferencia);

    /*Se tem mais de 3 Locais Preferidos*/
    if  (cont_locais_pref(user_escolhedor->locais)>=3) {
        printf("\nERRO!! Ja inseriu o maximo de Locais permitido!!\n\n");
        goto volta;
    }
    else {
        while (percorre_locais_preferidos) {
            if (percorre_locais_preferidos->id==preferencia) {
                printf("\nERRO!! %s ja inseriu uma preferencia em %s\n\n", user_escolhedor->nome, percorre_locais_preferidos->nome);
                goto volta;
            }
            else percorre_locais_preferidos=percorre_locais_preferidos->next;
        }
        user_escolhedor->locais=adiciona_lista_locais(user_escolhedor->locais, loc_escolhido->ponto_interesse, loc_escolhido->nome, loc_escolhido->pais, loc_escolhido->popularidade, loc_escolhido->id);
        altera_pop_loc(listalocais, preferencia, 1);
        printf("\nLOCAL ADICIONADO COM SUCESSO!\n\n");
        sleep(2);
    }
    return user;
}

node_pessoas adiciona_preferencia_pdi(node_pessoas listausers, node_locais escolha_pdi) {
    int choice;
    char s_choice[DIM_REGISTOS];
    do {
        do {
            system("cls");
            printf("O que pretende?\n");
            printf("\n1- Adicionar apenas uma preferencia\n");
            printf("2- Adicionar um Ponto HOT\n");
            printf("0- VOLTAR\n\nOpcao=> ");
            fgets(s_choice, DIM_REGISTOS, stdin);
            choice=verifica_numero(s_choice, 0);
        } while(choice==-2);

        if (choice==0) opcao_preferencia(listausers, escolha_pdi, 0);
        if (choice==1) escolher_pdi(listausers, escolha_pdi);
        if (choice==2) escolher_hot(listausers, escolha_pdi);
    } while (choice!=(0 && 1 && 2));

    return listausers;
}

node_pessoas remove_preferencia_local(node_pessoas listausers, node_locais lista_locs) {
    printf("\nUSERS DISPONIVEIS\n");
    node_pessoas user_escolhedor= procura_user(listausers);
    node_locais percorre_locais_preferidos= user_escolhedor->locais;
    int preferencia;
    char s_preferencia[DIM_REGISTOS];

    printf("Qual dos locais pretende remover? [-1 para sair]\n");
    while(percorre_locais_preferidos) {
        printf("[%d]%s\n", percorre_locais_preferidos->id, percorre_locais_preferidos->nome);
        percorre_locais_preferidos=percorre_locais_preferidos->next;
    }
    do {
        printf("\nOpcao=> ");
        fgets(s_preferencia, DIM_REGISTOS, stdin);
        preferencia=verifica_numero(s_preferencia, 1);
    } while (preferencia==-2);

    if (preferencia==-1) return listausers;
    if (apaga_local_preferido(&user_escolhedor->locais, preferencia)!=0) {
        altera_pop_loc(lista_locs, preferencia, 0);
        printf("\nLOCAL REMOVIDO COM SUCESSO!\n\n");
        sleep(2);
    }
    else printf("O Local que inseriu nao se encontra nos PDIS preferidos de %s", user_escolhedor->nome);
    return listausers;
}

node_pessoas remove_preferencia_pdi(node_pessoas listausers, node_locais lista_locs) {
    int choice;
    char s_choice[DIM_REGISTOS];
    do {
        do {
            system("cls");
            printf("O que pretende?\n");
            printf("\n1- Remover apenas uma preferencia\n");
            printf("2- Remover um Ponto HOT\n");
            printf("0- VOLTAR\n\nOpcao=> ");
            fgets(s_choice, DIM_REGISTOS, stdin);
            choice=verifica_numero(s_choice, 0);
        } while(choice==-2);

        if (choice==0) opcao_preferencia(listausers, lista_locs, 0);
        if (choice==1) remove_pdi(listausers, lista_locs);
        if (choice==2) remove_hot(listausers, lista_locs);
    } while (choice!=(0 && 1 && 2));

    return listausers;
}


/*FUNCOES PARA LER OS FICHS DOS USERS, CIDADES E PDIS*/
node_pessoas le_fich_users(node_pessoas users_lidos, node_locais lista_locs) {
    FILE *fp=fopen("users.txt", "r");
    char linha[DIM_LINHA];
    int i;
    char nome[DIM_REGISTOS], morada[DIM_REGISTOS];
    int id, telefone, n_locs, n_pdis, n_hots;
    dt_nascimento nascimento;
    int id_local, id_pdi, id_hot;
    node_pessoas user_a_ler = users_lidos;
    node_locais locs_preferidos;
    node_pdis pdis_preferidos;

    node_pdis ponto_hot;
    node_locais lista_locs_preferidos;
    node_pdis lista_pdis_preferidos;

    if (fp==NULL) return NULL;
    else {
        fgets(linha, DIM_LINHA, fp);
        while (!feof(fp)) {
            ponto_hot=NULL;
            locs_preferidos=NULL;
            pdis_preferidos=NULL;
            lista_locs_preferidos=NULL;
            lista_pdis_preferidos=NULL;

            fgets(linha, DIM_LINHA, fp);
            id = atoi(strtok(linha, "|"));
            strcpy(nome, strtok(NULL, "|"));
            strcpy(morada, strtok(NULL, "|"));
            telefone = atoi(strtok(NULL, "|"));
            nascimento.dia = atoi(strtok(NULL, "/"));
            nascimento.mes = atoi(strtok(NULL, "/"));
            nascimento.ano = atoi(strtok(NULL, "|"));
            n_locs= atoi(strtok(NULL, "|"));
            n_pdis= atoi(strtok(NULL, "|"));
            n_hots= atoi(strtok(NULL, "\n"));

            if (n_locs!=0) {
                fgets(linha, DIM_LINHA, fp);
                for (i=0; i<n_locs; i++) {
                    if (i==0) id_local= atoi(strtok(linha, "|"));
                    else id_local= atoi(strtok(NULL, "|"));
                    locs_preferidos=encontra_local_escolhido(lista_locs, id_local);
                    altera_pop_loc(lista_locs, id_local, 1);
                    lista_locs_preferidos=adiciona_lista_locais(lista_locs_preferidos, NULL, locs_preferidos->nome, locs_preferidos->pais, locs_preferidos->popularidade, locs_preferidos->id);
                }
            }

            if (n_pdis!=0) {
                fgets(linha, DIM_LINHA, fp);
                for (i=0; i<n_pdis; i++) {
                    if (i==0) id_pdi= atoi(strtok(linha, "|"));
                    else id_pdi= atoi(strtok(NULL, "|"));
                    altera_pop_pdi(lista_locs, id_pdi, 1);
                    pdis_preferidos=encontra_pdi_escolhido(lista_locs, id_pdi);
                    lista_pdis_preferidos=adiciona_lista_pdis(lista_pdis_preferidos, pdis_preferidos->nome, pdis_preferidos->descricao, pdis_preferidos->hora_entrada, pdis_preferidos->hora_saida, pdis_preferidos->id, pdis_preferidos->popularidade, 0);
                }
            }

            if (n_hots!=0) {
                fgets(linha, DIM_LINHA, fp);
                id_hot= atoi(linha);
                altera_hot_pdi(lista_locs, id_hot, 1);
                ponto_hot=encontra_pdi_escolhido(lista_locs, id_hot);
            }
            user_a_ler = adiciona_lista_pessoas(user_a_ler, lista_locs_preferidos, lista_pdis_preferidos, ponto_hot, nome, morada, telefone, nascimento, id);
        }
    }
    fclose(fp);
    return user_a_ler;
}

node_locais le_fich_cidades (node_locais cidades) {
    node_locais aux=cidades;
    FILE *fp=fopen("cidades_e_pdis.txt", "r");
    int id=0, id_pdi=0, id_local=0, n_pdis, i;
    char nome_cid[DIM_REGISTOS], nome_pdi[DIM_REGISTOS], pais[DIM_REGISTOS], descricao[DIM_REGISTOS*10], linha[DIM_LINHA];
    node_pdis listaPDI[DIM_REGISTOS];
    tempo entrada, saida;

    for (i=0; i<DIM_REGISTOS; i++) listaPDI[i]=NULL;
    if(!fp) return 0;
    else{
        while (!feof(fp)) {

            /*Leituras*/
            fgets(linha, DIM_LINHA, fp);

            strcpy(nome_cid,strtok(linha, "|"));
            strcpy(pais, strtok(NULL,"|"));
            n_pdis = atoi(strtok(NULL, "\n"));

            for(i = 0; i < n_pdis; i++) {
                fgets(linha, DIM_LINHA, fp);
                strcpy(nome_pdi, strtok(linha,"|"));
                strcpy(descricao, strtok(NULL,"|"));
                entrada.horas= atoi(strtok(NULL,":"));
                entrada.minutos= atoi(strtok(NULL,":"));
                saida.horas= atoi(strtok(NULL,":"));
                saida.minutos= atoi(strtok(NULL,"\n"));
                listaPDI[id]= adiciona_lista_pdis(listaPDI[id], nome_pdi, descricao, entrada, saida, id_pdi, 0, 0);
                id_pdi++;
            }
            aux = adiciona_lista_locais(aux, listaPDI[id], nome_cid, pais, 0, id_local);
            fgets(linha, DIM_LINHA, fp);
            id++;
            id_local++;
        }
    }
    fclose(fp);
    cidades=aux;
    return cidades;
}



/*---------------------FUNCOES PRINCIPAIS-----------------------*/


/*0- FUNCAO PARA ARMAZENAR AS PESSOAS NO FICHEIRO*/
int armazena(node_pessoas lista_users) {
    node_pessoas percorre_users= lista_users;
    node_locais percorre_locs_pref=NULL;
    node_pdis percorre_pdis_pref=NULL;
    int n_locs_pref=0, n_pdis_pref=0, n_hot=0;
    FILE *fp= fopen("users.txt", "w");
    if (percorre_users==NULL) return 0;

    while (percorre_users) {
        n_locs_pref=cont_locais_pref(percorre_users->locais);
        n_pdis_pref=cont_pdis_pref(percorre_users->pdis);
        percorre_locs_pref=percorre_users->locais;
        percorre_pdis_pref=percorre_users->pdis;
        if (percorre_users->hot!=NULL) n_hot=1;
        else n_hot=0;

        fprintf(fp, "\n%d|%s|%s|%d|%d/%d/%d|%d|%d|%d", percorre_users->id, percorre_users->nome, percorre_users->morada, percorre_users->telefone, percorre_users->data_nasc.dia, percorre_users->data_nasc.mes, percorre_users->data_nasc.ano, n_locs_pref, n_pdis_pref, n_hot);

        if (percorre_locs_pref!=NULL) {
            fprintf(fp, "\n");
            while (percorre_locs_pref) {
                fprintf(fp, "%d|", percorre_locs_pref->id);
                percorre_locs_pref=percorre_locs_pref->next;
            }
        }

        if (percorre_pdis_pref!=NULL) {
            fprintf(fp, "\n");
            while(percorre_pdis_pref) {
                fprintf(fp, "%d|", percorre_pdis_pref->id);
                percorre_pdis_pref=percorre_pdis_pref->next;
            }
        }

        if (percorre_users->hot!=NULL) fprintf(fp, "\n%d", percorre_users->hot->id);
        percorre_users = percorre_users->next;
    }
    fclose(fp);
    return 1;
}

/*1- FUNCAO PARA O REGISTO DE USERS*/
node_pessoas regista(node_pessoas no_registado, int id) {
    char nome[DIM_REGISTOS], morada[DIM_REGISTOS];
    int telefone;
    char s_telefone[DIM_REGISTOS];
    dt_nascimento data;

    system("cls");
    printf("\tREGISTO DE USER\n\n");

    /*INPUTS*/
    do {
        printf("Insira o seu nome:\t");
        scanf(" %[^\n]", nome);
    } while (verifica_string(nome, 1)==0);
    maiusculas(nome);

    do {
        printf("Insira a sua morada:\t");
        scanf(" %[^\n]", morada);
    } while (verifica_string(morada, 2)==0);
    maiusculas(morada);

    getchar();
    do {
        do {
            printf("Insira o seu numero de telefone:\t");
            fgets(s_telefone, DIM_REGISTOS, stdin);
            telefone=verifica_numero(s_telefone, 2);
        } while(telefone==-2);
    } while (verifica_telefone(telefone)!=1);

    do {
        printf("Insira a sua data de nascimento [dd/mm/aaaa]:\t");
        scanf("%d/%d/%d", &data.dia, &data.mes, &data.ano);
    } while (verifica_data(data)==0);

    printf("\nUSER REGISTADO COM SUCESSO!!\n\n");
    no_registado= adiciona_lista_pessoas(no_registado, NULL, NULL, NULL, nome, morada, telefone, data, id);
    return no_registado;
}

/*2- FUNCAO PARA A ALTERACAO DE USERS*/
int altera(node_pessoas no_alterado, int M_id) {
    node_pessoas aux= no_alterado;
    int id_selecionado, dado_alterado;
    char s_telefone[DIM_REGISTOS], s_id_selecionado[DIM_REGISTOS], s_dado_alterado[DIM_REGISTOS];

    printf("\t\t\tALTERACAO DE USER\n\n");
    printf("ID\t\tNome\t\tMorada\t\tTelefone\tData de Nascimento\n");
    imprime_lista_pessoas(no_alterado);
    do {
        printf("\nQue USER pretende alterar? [-1 para sair]\t");
        fgets(s_id_selecionado, DIM_REGISTOS, stdin);
        id_selecionado=verifica_numero(s_id_selecionado, 1);
    } while(id_selecionado==-2);

    if (id_selecionado>M_id) altera(no_alterado, M_id);
    if (id_selecionado==-1) return -1;

    while (aux->id!=id_selecionado) aux= aux->next;

    do {
        printf("\n\nQue dado deseja alterar?\n[0->Nome, 1->Morada, 2->Telefone, 3->Data de Nascimento]\nOpcao=> ");
        fgets(s_dado_alterado, DIM_REGISTOS, stdin);
        dado_alterado=verifica_numero(s_dado_alterado, 3);
    } while(dado_alterado==-2);

    if (dado_alterado==0) {
        do {
            printf("\nInsira um novo nome:\t");
            scanf(" %[^\n]", aux->nome);
        } while (verifica_string(aux->nome, 1)==0);

        maiusculas(aux->nome);
        printf("\nNOME ALTERADO COM SUCESSO!!\n\n");
        return 1;
    }

    if (dado_alterado==1) {
        do {
            printf("\nInsira uma nova morada:\t");
            scanf(" %[^\n]", aux->morada);
        } while (verifica_string(aux->morada, 2)==0);

        maiusculas(aux->morada);
        printf("\nMORADA ALTERADA COM SUCESSO!!\n\n");
        return 1;
    }

    if (dado_alterado==2) {
        do {
            do {
                printf("\nInsira um novo telefone:\t");
                fgets(s_telefone, DIM_REGISTOS, stdin);
                aux->telefone=verifica_numero(s_telefone, 2);
            } while (aux->telefone==-2);
        } while (verifica_telefone(aux->telefone)!=1);
        printf("\nTELEFONE ALTERADO COM SUCESSO!!\n\n");
        return 1;
    }
    if (dado_alterado==3) {
        do {
            printf("Insira uma nova data de nascimento [dd/mm/aaaa]:\t");
            scanf("%d/%d/%d", &aux->data_nasc.dia, &aux->data_nasc.mes, &aux->data_nasc.ano);
        } while (verifica_data(aux->data_nasc)==0);
        printf("\nDATA DE NASCIMENTO ALTERADA COM SUCESSO!!\n\n");
        return 1;
    }
    else altera(no_alterado, M_id);
    return 1;
}

/*3- IMPRIME OS USERS USERS*/
void imprime_lista_pessoas(node_pessoas no_escolhido) {
    node_pessoas aux_pessoas= no_escolhido;
    node_locais aux_locais=NULL;
    node_pdis aux_pdis=NULL;
    while (aux_pessoas) {
        /*Imprime os users*/
        printf("\nUSER[%d]:   %s | %s | %d | %d/%d/%d\n", aux_pessoas->id, aux_pessoas->nome, aux_pessoas->morada, aux_pessoas->telefone, aux_pessoas->data_nasc.dia, aux_pessoas->data_nasc.mes, aux_pessoas->data_nasc.ano);

        /*Imprime os locais preferidos o user*/
        aux_locais= aux_pessoas->locais;
        if (aux_pessoas->locais!=NULL) {
            printf("Locais Preferidos de %s:\t", aux_pessoas->nome);
            while(aux_locais) {
                printf("[%d]%s | ", aux_locais->id, aux_locais->nome);
                aux_locais=aux_locais->next;
            }
            printf("\n");
        }

        /*Imprime os pdis preferidos o user*/
        aux_pdis=aux_pessoas->pdis;
        if (aux_pessoas->pdis!=NULL) {
            printf("PDIs Preferidos de %s:\t", aux_pessoas->nome);
            while(aux_pdis) {
                printf("[%d]%s | ", aux_pdis->id, aux_pdis->nome);
                aux_pdis=aux_pdis->next;
            }
            printf("\n");
        }

        /*Imprime os pdis Hots do user*/
        if (aux_pessoas->hot!=NULL) printf("PDI HOT de %s:\t[%d]%s\n", aux_pessoas->nome, aux_pessoas->hot->id, aux_pessoas->hot->nome);
        aux_pessoas=aux_pessoas->next;
    }
    printf("\n");
}

void imprime_lista_locais_e_pdis(node_locais cid_impressa, int m) {
    node_locais percorre_local = cid_impressa;
    node_pdis percorre_pdi=NULL;
    printf("\n\n");
    if (m==0) {
        while(percorre_local){
            printf("LOCAL[%d]:  Cidade: %s  |  Pais: %s  |  Popularidade: %d\n", percorre_local->id, percorre_local->nome, percorre_local->pais, percorre_local->popularidade);
            percorre_local = percorre_local -> next;
        }
    }

    if (m==1) {
        while (percorre_local) {
            percorre_pdi= percorre_local->ponto_interesse;
            printf("LOCAL[%d]:  Cidade: %s  |  Pais: %s  |  Popularidade: %d\n", percorre_local->id, percorre_local->nome, percorre_local->pais, percorre_local->popularidade);
            while(percorre_pdi){
                if ((percorre_pdi->hora_entrada.horas==percorre_pdi->hora_saida.horas) && (percorre_pdi->hora_entrada.minutos==percorre_pdi->hora_saida.minutos))
                    printf("PDI[%d]: %s\nPopularidade: %d\nHorario: Aberto durante todo o dia\n\n",percorre_pdi->id, percorre_pdi->nome, percorre_pdi->popularidade);
                else printf("PDI[%d]: %s\nPopularidade: %d\nHOTS: %d\nHorario: %d:%d - %d:%d\n\n",percorre_pdi->id, percorre_pdi->nome, percorre_pdi->popularidade, percorre_pdi->hot, percorre_pdi->hora_entrada.horas, percorre_pdi->hora_entrada.minutos, percorre_pdi->hora_saida.horas, percorre_pdi->hora_saida.minutos);
                percorre_pdi=percorre_pdi->next;
            }
        percorre_local=percorre_local->next;
        }
    }

    if (m==2) {
        while (percorre_local) {
            percorre_pdi= percorre_local->ponto_interesse;
            printf("=> LOCAL[%d]:  Cidade: %s  |  Pais: %s  |  Popularidade: %d\n\n", percorre_local->id, percorre_local->nome, percorre_local->pais, percorre_local->popularidade);
            printf("=> PONTOS DE INTERESSE\n\n");
            while(percorre_pdi){
                if ((percorre_pdi->hora_entrada.horas==percorre_pdi->hora_saida.horas) && (percorre_pdi->hora_entrada.minutos==percorre_pdi->hora_saida.minutos))
                    printf("PDI[%d]: %s\nDescricao: %s\nPopularidade: %d\nNumero de HOTS: %d\nHorario: Aberto durante todo o dia\n\n",percorre_pdi->id, percorre_pdi->nome, percorre_pdi->descricao, percorre_pdi->popularidade, percorre_pdi->hot);
                else printf("PDI[%d]: %s\nDescricao: %s\nPopularidade: %d\nNumero de HOTS: %d\nHorario: %d:%d - %d:%d\n\n",percorre_pdi->id, percorre_pdi->nome, percorre_pdi->descricao, percorre_pdi->popularidade, percorre_pdi->hot, percorre_pdi->hora_entrada.horas, percorre_pdi->hora_entrada.minutos, percorre_pdi->hora_saida.horas, percorre_pdi->hora_saida.minutos);
                percorre_pdi=percorre_pdi->next;
            }
        percorre_local=percorre_local->next;
        }
        printf("\n");
    }
}

/*4- FUNCAO PARA ORDENAR E LISTAR OS LOCAIS E PDIS POR ORDEM DE POPULARIDADE*/
node_locais ordenamento(node_locais list_locs) {
    int opcao;
    char s_opcao[DIM_REGISTOS];
    do {
        do {
            system("cls");
            printf("\tORDENAR POR:\n\n");
            printf("1- ALFABETO\n");
            printf("2- POPULARIDADE\n");
            printf("3- ID\n");
            printf("0- VOLTAR\n\nOpcao=> ");
            fgets(s_opcao, DIM_REGISTOS, stdin);
            opcao=verifica_numero(s_opcao, 0);
        } while(opcao==-2);

        if (opcao==0) main();
        if (opcao==1) {
            ordenar_locs_alfa(list_locs);
            imprime_lista_locais_e_pdis(list_locs, 2);
        }
        if (opcao==2) {
            ordenar_locs_pop(list_locs);
            imprime_lista_locais_e_pdis(list_locs, 2);
        }
        if (opcao==3) {
            ordenar_locs_id(list_locs);
            imprime_lista_locais_e_pdis(list_locs, 2);
        }
    } while (opcao!=0 && opcao!=1 && opcao!=2 && opcao!=3);
    return list_locs;
}

/*5/6- FUNCAO PARA INSERIR AS PREFERENCIAS*/
node_pessoas opcao_preferencia(node_pessoas user_opcional, node_locais local_opcional, int m) {
    int opcao;
    char s_opcao[DIM_REGISTOS];
    node_pessoas lista_users=user_opcional;
    if (m==1) {
        do {
            do {
                system("cls");
                printf("\nADICAO DE PREFERENCIA\n\n");
                printf("1- LOCAL\n");
                printf("2- PDI\n");
                printf("0- VOLTAR\n\nOpcao=> ");
                fgets(s_opcao, DIM_REGISTOS, stdin);
                opcao=verifica_numero(s_opcao, 0);
            } while(opcao==-2);

            if (opcao==0) return user_opcional;
            if (opcao==1) {
                if (user_opcional==NULL) printf("\nERRO! Ainda nao inseriu nenhum USER!!\n\n");
                else lista_users=adiciona_preferencia_local(user_opcional, local_opcional);
            }
            if (opcao==2) {
                if (user_opcional==NULL) printf("\nERRO! Ainda nao inseriu nenhum USER!!\n\n");
                else lista_users=adiciona_preferencia_pdi(user_opcional, local_opcional);
            }
        } while (opcao!=0 && opcao!= 1 && opcao!=2);
    }
    if (m==0) {
        do {
            do {
                system("cls");
                printf("\nREMOCAO DE PREFERENCIA\n\n");
                printf("1- LOCAL\n");
                printf("2- PDI\n");
                printf("0- VOLTAR\n\nOpcao=> ");
                fgets(s_opcao, DIM_REGISTOS, stdin);
                opcao=verifica_numero(s_opcao, 0);
            } while(opcao==-2);

            if (opcao==0) return user_opcional;
            if (opcao==1) {
                if (user_opcional==NULL) printf("\nERRO! Ainda nao inseriu nenhum USER!!\n\n");
                else lista_users=remove_preferencia_local(user_opcional, local_opcional);
            }
            if (opcao==2) {
                if (user_opcional==NULL) printf("\nERRO! Ainda nao inseriu nenhum USER!!\n\n");
                else lista_users=remove_preferencia_pdi(user_opcional, local_opcional);
            }
        } while (opcao!=0 && opcao!= 1 && opcao!=2);
    }
    return lista_users;
}

/*FUNCAO PARA FAZER VIAGEM*/
void fazer_viagem(node_pessoas lista_users, node_locais lista_locs, int n_users) {
    node_pessoas usuario_viajante=procura_user(lista_users);
    node_locais temp=usuario_viajante->locais, locais_preferidos=NULL, aux;
    node_pdis perc_pdis_loc_pref, pdis_printados;
    int i;
    float taxa_locais=0, taxa_pdis=0, taxa_hots=0, escolheram_local_pref=0, escolheram_hot=0, escolheram_pdi_pref=0;

    if (usuario_viajante==NULL) return;
    /*VAI BUSCAR AS POPULARIDADES DOS LOCAIS PREFERIDOS DO USER*/
    while (temp) {
        aux=encontra_local_escolhido(lista_locs, temp->id);
        locais_preferidos=adiciona_lista_locais(locais_preferidos, aux->ponto_interesse, aux->nome, aux->pais, aux->popularidade, aux->id);
        temp=temp->next;
    }

    /*VERIFICA SE O USER TEM MENOS DE 3 LOCAIS PREFERIDOS*/
    if (cont_locais_pref(usuario_viajante->locais)!=3) {
        printf("\nERRO!! Ainda nao inseriu 3 Locais preferidos!!\n\n");
        sleep(2);
    }
    else {
        printf("\n\n\t\tVAMOS VIAJAR %s!!\n\nOs teus Locais preferidos:\n\n", usuario_viajante->nome);

        while (locais_preferidos) {
            pdis_printados=NULL;
            perc_pdis_loc_pref=locais_preferidos->ponto_interesse;
            ordenar_pdis_pop(perc_pdis_loc_pref);
            escolheram_local_pref+= (locais_preferidos->popularidade)-1;

            while (perc_pdis_loc_pref) {
                /*SE HOUVER UM PONTO HOT, E O PRIMEIRO A SER GUARDADO*/
                if ((perc_pdis_loc_pref->hot) > 0) {
                    escolheram_hot+= (perc_pdis_loc_pref->hot);
                    escolheram_pdi_pref+= (perc_pdis_loc_pref->popularidade);
                    pdis_printados=adiciona_lista_pdis(pdis_printados, perc_pdis_loc_pref->nome, perc_pdis_loc_pref->descricao, perc_pdis_loc_pref->hora_entrada, perc_pdis_loc_pref->hora_saida,  perc_pdis_loc_pref->id, perc_pdis_loc_pref->popularidade, perc_pdis_loc_pref->hot);
                }
                perc_pdis_loc_pref=perc_pdis_loc_pref->next;
            }

            /*VOLTA PARA O INICIO DA LISTA*/
            perc_pdis_loc_pref=locais_preferidos->ponto_interesse;

            while (cont_pdis_pref(pdis_printados) < 3) {
                /*NAO PRINTA NOVAMENTE O PONTO HOT*/
                if ((perc_pdis_loc_pref->hot) > 0) perc_pdis_loc_pref=perc_pdis_loc_pref->next;

                escolheram_pdi_pref+= (perc_pdis_loc_pref->popularidade);
                pdis_printados=adiciona_lista_pdis(pdis_printados, perc_pdis_loc_pref->nome, perc_pdis_loc_pref->descricao, perc_pdis_loc_pref->hora_entrada, perc_pdis_loc_pref->hora_saida,  perc_pdis_loc_pref->id, perc_pdis_loc_pref->popularidade, perc_pdis_loc_pref->hot);
                perc_pdis_loc_pref=perc_pdis_loc_pref->next;
            }

            /*PRINTS FINAIS DOS LOCAIS E PDIS DA VIAGEM*/
            printf("PDIS Mais Populares de %s:  ", strupr(locais_preferidos->nome));
            for (i=0; i<3; i++) {
                if (i!=2) printf("%s -> ", pdis_printados->nome);
                else printf("%s\n", pdis_printados->nome);
                pdis_printados=pdis_printados->next;
            }
            locais_preferidos=locais_preferidos->next;
        }

        /*CALCULO E PRINT DAS TAXAS DE PREFERENCIAS*/
        if (n_users-1==0) {
            taxa_locais=0;
            taxa_hots=0;
        }
        else {
            taxa_locais= (escolheram_local_pref/(n_users-1)) *100;
            taxa_hots= (escolheram_hot/(n_users-1))*100;
        }
        if (cont_pop_pdis(lista_locs)==0) taxa_pdis=0;
        else taxa_pdis= (escolheram_pdi_pref/cont_pop_pdis(lista_locs))*100;
        printf("\n%f%% dos utilizadores escolheram pelo menos 1 local igual ao teu!!", taxa_locais);
        printf("\n%f%% dos utilizadores tem um Ponto HOT na tua viagem!!", taxa_hots);
        printf("\n%f%% das preferencias de todos os usuarios encontram-se na tua viagem!!\n\n", taxa_pdis);
    }
}


/*-------------------------EXECUCAO-------------------------*/

int main() {
    int opcao=0, M_id;
    char s_opcao[DIM_REGISTOS];
    node_pessoas lista_pessoas=NULL;
    node_locais lista_locais=NULL;
    lista_locais= le_fich_cidades(lista_locais);
    lista_pessoas= le_fich_users(lista_pessoas, lista_locais);
    /*CONTA O NUMERO DE USERS QUE JA ESTAO REGISTADOS*/
    M_id= cont_users(lista_pessoas);

    menu:
    do {
        menu_principal();
        printf("\n\tOpcao=> ");
        fgets(s_opcao, DIM_REGISTOS, stdin);
        opcao=verifica_numero(s_opcao, 0);
    } while (opcao==-1);

    /*SAIR*/
    if (opcao==0) {
        armazena(lista_pessoas);
        printf("\n\tO PROGRAMA FOI ENCERRADO COM SUCESSO\n");
        exit(0);
    }
    /*REGISTAR*/
    if (opcao==1) {
        lista_pessoas = regista(lista_pessoas, M_id);
        decisao(lista_pessoas);
    }
    /*ALTERAR*/
    if (opcao==2) {
        if (lista_pessoas==NULL) printf("\n\tERRO!! Ainda nao foram registados USERS!!\n\n");
        else {
           if (altera(lista_pessoas, M_id)==-1) goto menu;
        }
        decisao(lista_pessoas);
    }
    /*LISTAR USERS*/
    if (opcao==3) {
       if (lista_pessoas==NULL) printf("\n\tERRO!! Ainda nao foram registados USERS!!\n\n");
       else {
           printf("\n\n\tUSERS REGISTADOS\n");
           imprime_lista_pessoas(lista_pessoas);
       }
       decisao(lista_pessoas);
    }
    /*LISTAR LOCAIS E PDIS*/
    if (opcao==4) {
        ordenamento(lista_locais);
        decisao(lista_pessoas);
    }
    /*ADICIONAR PREFERENCIAS*/
    if (opcao==5) {
        if (lista_pessoas!=NULL) lista_pessoas= opcao_preferencia(lista_pessoas, lista_locais, 1);
    }
    /*REMOVER PREFERENCIAS*/
    if (opcao==6) {
        if (lista_pessoas!=NULL) lista_pessoas= opcao_preferencia(lista_pessoas, lista_locais, 0);
    }
    /*FAZER VIAGEM*/
    if (opcao==7) {
        if (lista_pessoas==NULL) printf("\n\tERRO!! Ainda nao foram registados USERS!!\n\n");
        else fazer_viagem(lista_pessoas, lista_locais, M_id);
        decisao(lista_pessoas);
   }
    goto menu;
    return 0;
}


