#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apont_tarefas.h"
#include "tarefas.h"
#include "pessoas.h"


void adiciona_apont_tarefa(ListApontTarefa *lista, ListTarefa tarefa) {
    ListApontTarefa novo_apont_tarefa;
    ListApontTarefa aux = *lista;
    novo_apont_tarefa = (ListApontTarefa)malloc(sizeof(No_apont_tarefa));
    if(novo_apont_tarefa == NULL) {
        printf("Erro! Memoria nao disponivel.\n");
        exit(2); /*codigo de erro memoria nao disponivel*/
    }
    novo_apont_tarefa->apont_tarefa = tarefa;
    novo_apont_tarefa->next = NULL;
    if (*lista == NULL) {
        *lista = novo_apont_tarefa;
    }
    else {
        while(aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = novo_apont_tarefa;
    }
}

ListApontTarefa eliminar_apont_tarefa(ListApontTarefa lista, int id) {
    ListApontTarefa anterior = NULL;
    ListApontTarefa atual = lista;
    while((atual != NULL) && (atual->apont_tarefa->info.id != id)) {
        anterior = atual;
        atual = atual->next;
    }
    if (atual!= NULL) {
        if (anterior!=NULL)
            anterior->next = atual->next;
        else
            lista=atual->next;
        free(atual);
    }
    return lista;
}

ListApontTarefa inserir_ordenado(ListApontTarefa L2, ListApontTarefa novo){
    ListApontTarefa ant, act;
    act = L2;
    ant = NULL;
    if (act != NULL && act->apont_tarefa->info.coluna == 1) {
        while(act!=NULL && act->apont_tarefa->info.prioridade > novo->apont_tarefa->info.prioridade) {
            ant = act;
            act = act->next;
        }
        while(act!=NULL  && act->apont_tarefa->info.prioridade == novo->apont_tarefa->info.prioridade && (compara_datas(&(act->apont_tarefa->info.data_criacao),&(novo->apont_tarefa->info.data_criacao))==1)){
                ant = act;
                act = act->next;
        }
    }
    else if (act != NULL && act->apont_tarefa->info.coluna == 2) {
        while(act!=NULL && strcmp(act->apont_tarefa->info.descricao, novo->apont_tarefa->info.descricao) < 0) {
            ant = act;
            act = act->next;
        }
    }
    else {
         while(act!=NULL && (compara_datas(&(act->apont_tarefa->info.data_conclusao), &(novo->apont_tarefa->info.data_conclusao)))==0){
            ant = act;
            act = act->next;
        }
    }
    if (ant != NULL)
        ant->next = novo;
    else
        L2 = novo;
        novo->next = act;
    return L2;
}


ListApontTarefa ordena_lista(ListApontTarefa lista) {
    ListApontTarefa act = lista;
    ListApontTarefa L2 = NULL;
    while(act!=NULL) {
        lista = act->next;
        L2 = inserir_ordenado(L2, act);
        act = lista;
    }
    return L2;
}


void listar_apont_tarefas(ListApontTarefa lista){
    if (lista == NULL)
        printf("Nao ha nenhuma tarefa inserida.\n\n");
    else{
        while(lista != NULL){
            printf("TAREFA %d\n", lista->apont_tarefa->info.id);
            printf("Descricao: %s\n", lista->apont_tarefa->info.descricao);
            printf("Prioridade: %d\n", lista->apont_tarefa->info.prioridade);
            printf("Data criacao: %d/%d/%d\n", lista->apont_tarefa->info.data_criacao.ano, lista->apont_tarefa->info.data_criacao.mes,lista->apont_tarefa->info.data_criacao.dia);
            if (lista->apont_tarefa->info.data_prazo.ano != 0)
                printf("Prazo conclusao: %d/%d/%d\n", lista->apont_tarefa->info.data_prazo.ano, lista->apont_tarefa->info.data_prazo.mes,lista->apont_tarefa->info.data_prazo.dia);
            if (lista->apont_tarefa->info.data_conclusao.ano != 0)
                printf("Data conclusao: %d/%d/%d\n", lista->apont_tarefa->info.data_conclusao.ano, lista->apont_tarefa->info.data_conclusao.mes, lista->apont_tarefa->info.data_conclusao.dia);
            if(lista->apont_tarefa->apont_pessoa != NULL){
                printf("Pessoa responsavel: %s\n", lista->apont_tarefa->apont_pessoa->info.nome);
            }
            printf("\n");
            lista = lista->next;
        }
    }
}

int verificar_maximo_tarefas(ListApontTarefa lista, int num_max_tarefas){
    int cont = 0;
    while(lista != NULL){
        cont++;
        lista = lista->next;
    }
    if (cont < num_max_tarefas)
        return 1;
    else
        return 0;
}

/*verificar numero de tarefas da coluna doing que cada pessoa tem*/
int verificar_maximo_tarefas_pessoa(ListApontTarefa lista, int num_max_tarefas){
    int cont = 0;
    while(lista != NULL){
        if (lista->apont_tarefa->info.coluna == 2)
            cont++;
        lista = lista->next;
    }
    if (cont < num_max_tarefas)
        return 1;
    else
        return 0;
}

void imprime_apont_tarefa(ListApontTarefa t) {
    if(t == NULL)
        printf("A tarefa nao existe.\n");
    else {
        printf("TAREFA %d\n", t->apont_tarefa->info.id);
        printf("Descricao: %s\n", t->apont_tarefa->info.descricao);
        printf("Prioridade: %d\n", t->apont_tarefa->info.prioridade);
        printf("Data criacao: %d/%d/%d\n", t->apont_tarefa->info.data_criacao.ano, t->apont_tarefa->info.data_criacao.mes, t->apont_tarefa->info.data_criacao.dia);
        printf("Prazo conclusao: %d/%d/%d\n", t->apont_tarefa->info.data_prazo.ano, t->apont_tarefa->info.data_prazo.mes, t->apont_tarefa->info.data_prazo.dia);
        if (t->apont_tarefa->info.data_conclusao.ano != 0)
            printf("Data conclusao: %d/%d/%d\n", t->apont_tarefa->info.data_conclusao.ano, t->apont_tarefa->info.data_conclusao.mes, t->apont_tarefa->info.data_conclusao.dia);
        printf("\n");
    }
}

void imprimir_quadro_pessoa(ListPessoa pessoa){
    ListApontTarefa aux;
    int contador = 0;
    printf("Tarefas de %s:\n", pessoa->info.nome);
    aux = pessoa->apont_tarefas;
    if (aux == NULL)
        printf("Nao ha tarefas a mostrar.\n\n");
    else {
        printf("\n|--------------------------|\n");
        printf("|          DOING           |\n");
        printf("|--------------------------|\n\n");
        while(aux != NULL) {
            if (aux->apont_tarefa->info.coluna == 2) {
                imprime_apont_tarefa(aux);
                contador += 1;
            }
            aux = aux->next;
        }
        if (contador == 0)
            printf("Nao ha nenhuma tarefa a mostrar.\n");
        aux = pessoa->apont_tarefas;
        contador = 0;
        printf("\n|--------------------------|\n");
        printf("|          DONE            |\n");
        printf("|--------------------------|\n\n");
        while(aux != NULL) {
            if (aux->apont_tarefa->info.coluna == 3) {
                imprime_apont_tarefa(aux);
                contador += 1;
            }
            aux = aux->next;
        }
        if (contador == 0)
            printf("Nao ha nenhuma tarefa a mostrar.\n");
        printf("\n");
    }
}


void imprimir_titulo_quadro() {
    printf("\n\n * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
    printf("*                       QUADRO KANBAN                         *\n");
    printf(" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
}


void imprimir_quadro(ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done) {
    if (*lista_todo == NULL && *lista_doing == NULL && *lista_done == NULL)
        printf("Nao ha nenhuma tarefa inserida.\n");
    else{
        printf("\n|--------------------------|\n");
        printf("|          TO DO           |\n");
        printf("|--------------------------|\n\n");
        *lista_todo = ordena_lista(*lista_todo);
        listar_apont_tarefas(*lista_todo);
        printf("\n|--------------------------|\n");
        printf("|          DOING           |\n");
        printf("|--------------------------|\n\n");
        *lista_doing = ordena_lista(*lista_doing);
        listar_apont_tarefas(*lista_doing);
        printf("\n|--------------------------|\n");
        printf("|          DONE            |\n");
        printf("|--------------------------|\n\n");
        *lista_done = ordena_lista(*lista_done);
        listar_apont_tarefas(*lista_done);
        printf("\n");
    }
}

ListApontTarefa destroi_lista_aponttarefa(ListApontTarefa lista){
    ListApontTarefa aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
    return NULL;
}
