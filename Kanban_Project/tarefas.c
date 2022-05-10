#include <stdio.h>
#include <stdlib.h>
#include "tarefas.h"
#include "apont_tarefas.h"
#include <ctype.h>
#include <string.h>



ListTarefa adiciona_tarefa(ListTarefa lista, ListTarefa nova_tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done) {
    ListTarefa aux = lista;
    if (lista == NULL) {
        lista = nova_tarefa;
        /*adicionar a lista de apontadores respetiva*/
        if (nova_tarefa->info.coluna == 1)
            adiciona_apont_tarefa(lista_todo, nova_tarefa);
        else if (nova_tarefa->info.coluna == 2)
            adiciona_apont_tarefa(lista_doing, nova_tarefa);
        else
            adiciona_apont_tarefa(lista_done, nova_tarefa);
        return lista;
    }
    while(aux->next != NULL) {
        aux = aux->next;
    }
    aux->next = nova_tarefa;
    /*adicionar a lista de apontadores respetiva*/
    if (nova_tarefa->info.coluna == 1)
        adiciona_apont_tarefa(lista_todo, nova_tarefa);
    else if (nova_tarefa->info.coluna == 2)
        adiciona_apont_tarefa(lista_doing, nova_tarefa);
    else
        adiciona_apont_tarefa(lista_done, nova_tarefa);
    return lista;
}


ListTarefa inserir_tarefa_todo(ListTarefa lista_tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done){
    ListTarefa nova_tarefa;
    int id, num;
    nova_tarefa = (ListTarefa)malloc(sizeof(No_tarefa));
    if(nova_tarefa == NULL) {
        printf("Erro! Memoria nao disponivel.\n");
        exit(2); /*codigo de erro memoria nao disponivel*/
    }
    nova_tarefa->next = NULL;
    nova_tarefa->apont_pessoa = NULL;
    id = verificar_numero_tarefas(lista_tarefa);

    nova_tarefa->info.id = id+1;
    nova_tarefa->info.coluna = 1;
    nova_tarefa->info.id_pessoa = 0;
    printf("Insira as informacoes da nova tarefa\n");
    printf("Descricao: ");
    scanf(" %[^\n]s", nova_tarefa->info.descricao);
    printf("Insira uma Prioridade para a tarefa de 1-10:");
    num = pede_verifica_numero();
    while(num == -1 || num <1 || num >10){
        printf("Insira uma prioridade valida (1-10): ");
        num = pede_verifica_numero();
    }
    nova_tarefa->info.prioridade = num;

    printf("Data de criacao:");
    le_data(&nova_tarefa->info.data_criacao);
    while (!validar_data(&nova_tarefa->info.data_criacao)){
        printf("Insira uma data de criacao que exista.");
        le_data(&nova_tarefa->info.data_criacao);
    }
    printf("Deseja indicar um prazo de conclusao para a tarefa?\n1:Sim\n2:Nao\n");
    num = pede_verifica_numero();
    while(num == -1 || num <1 || num >2){
        printf("Insira uma opcao valida (1-2): ");
        num = pede_verifica_numero();
    }

    if (num == 1) {
        printf("Prazo de conclusao:");
        le_data(&nova_tarefa->info.data_prazo);
        while (!validar_data(&nova_tarefa->info.data_prazo)){
            printf("Insira uma data de prazo de conclusao que exista.");
            le_data(&nova_tarefa->info.data_prazo);
        }
        while (!compara_datas(&nova_tarefa->info.data_criacao, &nova_tarefa->info.data_prazo)){
            printf("Insira uma data de prazo posterior ou igual a data de criacao da tarefa. Data criacao: %d/%d/%d", nova_tarefa->info.data_criacao.ano, nova_tarefa->info.data_criacao.mes, nova_tarefa->info.data_criacao.dia);
            le_data(&nova_tarefa->info.data_prazo);
        }
    }
    else {
        data_inexistente(&nova_tarefa->info.data_prazo);
    }
    data_inexistente(&nova_tarefa->info.data_conclusao);
    lista_tarefa = adiciona_tarefa(lista_tarefa, nova_tarefa, lista_todo, lista_doing, lista_done);

    return lista_tarefa;
}


void listar_tarefas(ListTarefa lista){
    if (lista == NULL)
        printf("Nao ha nenhuma tarefa inserida.");
    else{
        while(lista != NULL){
            printf("\nTAREFA %d\n", lista->info.id);
            printf("Descricao: %s\n",lista->info.descricao);
            if (lista->info.coluna == 1)
                printf("Coluna: To Do ");
            else if (lista->info.coluna == 2)
                printf("Coluna: Doing ");
            else
                printf("Coluna: Done ");
            printf("Prioridade: %d\n",lista->info.prioridade);
            printf("Data criacao: %d/%d/%d\n",lista->info.data_criacao.ano,lista->info.data_criacao.mes,lista->info.data_criacao.dia);
            if (lista->info.data_prazo.ano != 0)
                printf("Prazo conclusao: %d/%d/%d\n",lista->info.data_prazo.ano,lista->info.data_prazo.mes,lista->info.data_prazo.dia);
            if (lista->info.data_conclusao.ano != 0)
                printf("Data conclusao: %d/%d/%d\n", lista->info.data_conclusao.ano, lista->info.data_conclusao.mes, lista->info.data_conclusao.dia);
            if (lista->apont_pessoa != NULL){
                printf("Pessoa responsavel: %s\n", lista->apont_pessoa->info.nome);
            }
            lista = lista->next;
        }
    }
}

void alterar_informacao_tarefa(ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done){
    int id_tarefa; int num;
    ListTarefa tarefa;
    imprimir_quadro(lista_todo, lista_doing, lista_done);
    printf("Escolha o numero da tarefa que deseja alterar: ");

    id_tarefa = pede_verifica_numero();
    tarefa = procurar_tarefa(lista, id_tarefa);
    while(id_tarefa == -1 || tarefa == NULL){
        printf("Insira um numero valido.\n");
        printf("Escolha o numero da tarefa que deseja alterar: ");
        id_tarefa = pede_verifica_numero();
        tarefa = procurar_tarefa(lista, id_tarefa);
    }
    printf("\nQue informacao deseja alterar?\n");
    printf("1: Descricao\n2: Prioridade\n3: Prazo conclusao\n");
    num = pede_verifica_numero();
    while(num == -1 || num <1 || num >3){
        printf("Insira uma opcao valida (1-3): ");
        num = pede_verifica_numero();
    }
    if (num == 1){
        printf("\nInsira a nova descricao: ");
        scanf("%s[^\n]", tarefa->info.descricao);
    }
    else if (num == 2){
        printf("\nInsira a nova prioridade: ");
        num = pede_verifica_numero();
        while(num == -1 || num <1 || num >10){
            printf("Insira uma prioridade valida (1-10): ");
            num = pede_verifica_numero();
        }
        tarefa->info.prioridade = num;
    }
    else if (num == 3) {
        printf("\nInsira um prazo de conclusao:");
        le_data(&tarefa->info.data_prazo);
        while (!validar_data(&tarefa->info.data_prazo)){
            printf("Insira uma data de prazo de conclusao que exista.");
            le_data(&tarefa->info.data_prazo);
        }
        while (!compara_datas(&tarefa->info.data_criacao, &tarefa->info.data_prazo)){
            printf("\nData de criacao: %d/%d/%d\n", tarefa->info.data_criacao.ano, tarefa->info.data_criacao.mes, tarefa->info.data_criacao.dia);
            printf("Insira uma data de prazo posterior ou igual a data de criacao da tarefa.");
            le_data(&tarefa->info.data_prazo);
        }
    }
}

ListTarefa procurar_tarefa(ListTarefa lista, int id) {
    ListTarefa atual = lista;
    while((atual != NULL) && (atual->info.id != id)) {
        atual = atual->next;
    }
    return atual;
}

int verifica_se_numero(char input[]){
    int ln, i;
    ln = strlen(input) - 1;
    if(input[ln] == '\n')
        input[ln] = '\0';
    for(i = 0; i < ln; i++){
        if(!isdigit(input[i]))
            return -1;
    }
    return atoi(input);
}

int pede_verifica_numero(){
    int ln, i;
    char input[30];

    fflush(stdin);
    fgets(input, sizeof(input), stdin);
    ln = strlen(input) - 1;
    if(input[ln] == '\n')
        input[ln] = '\0';
    for(i = 0; i < ln; i++){
        if(!isdigit(input[i]))
            return -1;
    }
    return atoi(input);
}


void alterar_limites(int *max_doing, int *max_tarefas_pessoa) {
    int novo_num, num;

    printf("\nQue limite deseja alterar?\n");
    printf("1: Limite de tarefas na coluna \"doing\"\n");
    printf("2: Limite de tarefas por pessoa\n");

    num = pede_verifica_numero();
    while(num == -1 || num <1 || num >2){
        printf("Insira uma opcao valida (1-2): ");
        num = pede_verifica_numero();
    }
    if (num == 1){
        printf("\nInsira novo limite para o numero de tarefas na coluna \"doing\": ");
        novo_num = pede_verifica_numero();
        while(novo_num == -1){
            printf("\nInsira um numero valido: ");
            novo_num = pede_verifica_numero();
        }
        *max_doing = novo_num;
    }
    else {
        printf("\nInsira novo limite para o numero de tarefas por pessoa: ");
        novo_num = pede_verifica_numero();
        while(novo_num == -1){
            printf("\nInsira um numero valido: ");
            novo_num = pede_verifica_numero();
        }
        *max_tarefas_pessoa = novo_num;
    }
}

/*contar numero de tarefas existente para obter ultimo id*/
int verificar_numero_tarefas(ListTarefa lista){
    int cont = 0;
    while(lista != NULL){
        cont++;
        lista = lista->next;
    }
    return cont;
}

ListTarefa inserir_ordenado_data_criacao(ListTarefa L2, ListTarefa novo){
    ListTarefa ant, act;
    act = L2;
    ant = NULL;
    while(act!=NULL && (compara_datas(&(act->info.data_criacao), &(novo->info.data_criacao)))==1) {
        ant = act;
        act = act->next;
    }
    if (ant != NULL)
        ant->next = novo;
    else
        L2 = novo;
        novo->next = act;
    return L2;
}


ListTarefa ordena_lista_data_criacao(ListTarefa lista) {
    ListTarefa act = lista;
    ListTarefa L2 = NULL;
    while(act!=NULL) {
        lista = act->next;
        L2 = inserir_ordenado_data_criacao(L2, act);
        act = lista;
    }
    return L2;
}

ListTarefa destroi_lista_tarefa(ListTarefa lista){
    ListTarefa aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
    return NULL;
}
