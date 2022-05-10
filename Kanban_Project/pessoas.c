#include <stdio.h>
#include <stdlib.h>
#include "pessoas.h"
#include "tarefas.h"
#include "apont_tarefas.h"



ListPessoa adiciona_pessoa(ListPessoa lista, ListPessoa nova_pessoa) {
    ListPessoa aux = lista;
    if (lista == NULL) {
        lista = nova_pessoa;
        return lista;
    }
    while(aux->next != NULL) {
        aux = aux->next;
    }
    aux->next = nova_pessoa;
    return lista;
}

void listar_pessoas(ListPessoa lista) {
    if (lista == NULL)
        printf("Nao ha nenhuma pessoa inserida.");
    while(lista != NULL) {
        printf("Nome: %s\n", lista->info.nome);
        printf("Email: %s\n", lista->info.email);
        printf("Id: %d\n\n", lista->info.id);
        lista = lista->next;
    }
}


ListPessoa procurar_pessoa(ListPessoa lista, int id) {
    ListPessoa atual = lista;
    while((atual != NULL) && (atual->info.id != id)) {
        atual = atual->next;
    }
    return atual;
}

int alterar_pessoa_responsavel(ListPessoa lista_pessoas, ListTarefa tarefa, int max_tarefas_pessoa){
    int id_pessoa_original, id_pessoa;
    ListPessoa pessoa, pessoa_original;
    printf("\nLista de Pessoas:\n");
    listar_pessoas(lista_pessoas);
    printf("Escolha o id da pessoa a quem deseja atribuir a tarefa: ");

    id_pessoa = pede_verifica_numero();
    pessoa = procurar_pessoa(lista_pessoas, id_pessoa);
    while(id_pessoa == -1 || pessoa == NULL){
        printf("\nInsira um id valido.\n");
        printf("Escolha o id da pessoa a quem deseja atribuir a tarefa: ");
        id_pessoa = pede_verifica_numero();
        pessoa = procurar_pessoa(lista_pessoas, id_pessoa);
    }
    /*verificar se ainda nao foi atingido o maximo de tarefas em doing da pessoa*/
    /*verficar se a pessoa nao contem nenhuma tarefa em doing com prazo de conclusao inferior a 7 dias do prazo da tarefa*/
    if(verificar_maximo_tarefas_pessoa(pessoa->apont_tarefas, max_tarefas_pessoa)) {
        if (verifica_prazo_pessoa(pessoa, tarefa) == 0){
            printf("A pessoa ja tem outra tarefa que contem um prazo de conclusao inferior a 7 dias da tarefa selecionada.\n");
            return 0;
        }
        /*adicionar pessoa responsavel após mover tarefa de TO DO para DOING ou para DONE*/
        if (tarefa->info.coluna == 1){
            tarefa->info.id_pessoa = id_pessoa;
            tarefa->apont_pessoa = pessoa;
            adiciona_apont_tarefa(&(pessoa->apont_tarefas), tarefa);
            return 1;
        }
        /*alterar pessoa anteriormente responsavel em DOING ou DONE*/
        else{
            id_pessoa_original = tarefa->info.id_pessoa;
            pessoa_original = procurar_pessoa(lista_pessoas, id_pessoa_original);
            pessoa_original->apont_tarefas = eliminar_apont_tarefa(pessoa_original->apont_tarefas, tarefa->info.id);
            tarefa->info.id_pessoa = id_pessoa;
            tarefa->apont_pessoa = pessoa;
            adiciona_apont_tarefa(&(pessoa->apont_tarefas), tarefa);
            return 1;
        }
    }
    printf("\nNao e possivel atribuir mais tarefas a pessoa escolhida.\nFoi atingido o limite maximo de %d.\n", max_tarefas_pessoa);
    return 0;
}

/*Criar listas de apontadores de tarefas de cada pessoa ao iniciar o programa*/
void criar_ligacao_pessoa_tarefa(ListTarefa lista_tarefas, ListPessoa lista_pessoas){
    ListTarefa aux;
    ListPessoa pessoa;
    int id_pessoa;
    aux = lista_tarefas;
    while(aux != NULL){
        if(aux->info.id_pessoa != 0){
            id_pessoa = aux->info.id_pessoa;
            pessoa = procurar_pessoa(lista_pessoas, id_pessoa);
            aux->apont_pessoa = pessoa;
            adiciona_apont_tarefa(&(pessoa->apont_tarefas), aux);
        }
        aux = aux->next;
    }
}

ListPessoa destroi_lista_pessoa(ListPessoa lista){
    ListPessoa aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
    return NULL;
}
