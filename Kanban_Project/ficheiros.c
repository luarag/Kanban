#include <stdio.h>
#include <stdlib.h>
#include "pessoas.h"
#include "tarefas.h"

/*Leitura do ficheiro de pessoas*/

ListPessoa ler_fich_pessoas(char nome_fich[], ListPessoa lista){
    FILE * fp;
    ListPessoa nova_pessoa;
    char input[30];
    int size;
    int i = 1;
    fp = fopen(nome_fich, "r");
    if (fp != NULL) {
        fseek(fp, 0, SEEK_END); /*vai ate ao fim do ficheiro*/
        size = ftell(fp); /*retorna a posição corrente de leitura ou escrita no arquivo (em bytes)*/
        if (size == 0) {
            printf("\nO ficheiro Pessoas.txt esta vazio.\n"); /*o ficheiro das pessoas nao pode começar vazio*/
            exit(1);
        }
        else {
            fseek(fp, 0, SEEK_SET);/*volta ao inicio do ficheiro*/
            while((!feof(fp))) {
                nova_pessoa = (ListPessoa)malloc(sizeof(No_pessoa));
                if(nova_pessoa == NULL) {
                    printf("Erro! Memoria nao disponivel.");
                    exit(2); /*codigo de erro memoria nao disponivel*/
                }
                nova_pessoa->next = NULL;
                nova_pessoa->apont_tarefas = NULL;
                /*le id*/
                fscanf(fp, "%[^\t]s", input);
                if(verifica_se_numero(input) == -1){
                    printf("O id nao e valido. Ficheiro Pessoas.txt incorreto na linha: %d.", i);
                    exit(1);/*erro do ficheiro*/
                }
                else{
                    nova_pessoa->info.id = verifica_se_numero(input);
                }
                fscanf(fp, "\t%[^\t]s", nova_pessoa->info.nome);
                fscanf(fp, "\t%[^\n]s", nova_pessoa->info.email);
                fgetc(fp);
                i++;
                lista = adiciona_pessoa(lista, nova_pessoa);
            }
        }
    }
    else {
        printf("\nO ficheiro Pessoas.txt nao esta disponivel.");
        exit(1); /*codigo de erro ficheiro nao disponivel*/
    }
    fclose(fp);
    return lista;
}

/*Leitura do ficheiro de tarefas*/

ListTarefa ler_fich_tarefas(char ficheiro[], ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListPessoa lista_pessoas){
    FILE *fp;
    char input[30];
    ListTarefa nova_tarefa;
    int size;
    int i = 1;
    fp = fopen(ficheiro,"r");
    if (fp !=NULL) {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        if (size == 0) {
            printf("\nO ficheiro Tarefas.txt esta vazio.\n"); /*o ficheiro pode começar vazio*/
        }
        else {
            fseek(fp, 0, SEEK_SET);/*volta ao inicio do ficheiro*/
            while(!feof(fp)){
                nova_tarefa = (ListTarefa)malloc(sizeof(No_tarefa));
                if(nova_tarefa == NULL) {
                    printf("Erro! Memoria nao disponivel.\n");
                    exit(2); /*codigo de erro memoria nao disponivel*/
                }
                nova_tarefa->next = NULL;
                nova_tarefa->apont_pessoa = NULL;
                /*le id*/
                fscanf(fp, " %[^\t]s", input);
                if(verifica_se_numero(input) == -1){
                    printf("O id nao e valido. Ficheiro Tarefas.txt incorreto na linha: %d.", i);
                    exit(1);/*erro do ficheiro*/
                }
                else{
                    nova_tarefa->info.id = verifica_se_numero(input);
                }
                 /*le coluna*/
                fscanf(fp, "\t%[^\t]s", input);
                if(verifica_se_numero(input) == -1 || verifica_se_numero(input)<1 || verifica_se_numero(input)>3){
                    printf("A coluna nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                    exit(1);
                }
                else{
                     nova_tarefa->info.coluna = verifica_se_numero(input);
                }
                 /*le prioridade*/
                fscanf(fp, "\t%[^\t]s", input);
                if(verifica_se_numero(input) == -1 || verifica_se_numero(input)<1 || verifica_se_numero(input)>10){
                    printf("A prioridade nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                    exit(1);
                }
                else{
                     nova_tarefa->info.prioridade = verifica_se_numero(input);
                }
                /*le id_pessoa*/
                fscanf(fp, "\t%[^\t]s", input);
                if(verifica_se_numero(input) == -1) {
                    printf("A pessoa nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.", i);
                    exit(1);
                }
                else{
                    if((nova_tarefa->info.coluna != 1 && procurar_pessoa(lista_pessoas, verifica_se_numero(input))!= NULL) || (nova_tarefa->info.coluna == 1 && verifica_se_numero(input) == 0 )){
                         nova_tarefa->info.id_pessoa = verifica_se_numero(input);
                    }
                    else{
                        printf("A pessoa nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                        exit(1);
                    }
                }
                /*le descricao*/
                fscanf(fp, "\t%[^\t]s",  nova_tarefa->info.descricao);
                /*le data de criacao*/
                fscanf(fp, " %d", &(nova_tarefa->info.data_criacao.ano));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_criacao.mes));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_criacao.dia));
                if (validar_data(&(nova_tarefa->info.data_criacao))==0){
                    printf("data de criacao errada: %d/%d/%d", nova_tarefa->info.data_criacao.ano, nova_tarefa->info.data_criacao.mes, nova_tarefa->info.data_criacao.dia);
                    printf("A data de criacao nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                    exit(1);
                }
                /*le data de prazo*/
                fscanf(fp, " %d", &(nova_tarefa->info.data_prazo.ano));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_prazo.mes));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_prazo.dia));
                if(validar_data(&nova_tarefa->info.data_prazo) == 0){
                    if(nova_tarefa->info.coluna == 1 && ((nova_tarefa->info.data_prazo.ano)!=0 && nova_tarefa->info.data_prazo.mes != 0 && nova_tarefa->info.data_prazo.dia != 0)){
                        printf("O prazo de conclusao nao e valido esta na coluna 1 e nao sao iguais a 0. Ficheiro Tarefas.txt incorreto na linha: %d",i);
                        exit(1);
                    }
                    else if (nova_tarefa->info.coluna != 1){
                        printf("O prazo de conclusao nao e valido. Ficheiro Tarefas.txt incorreto na linha: %d",i);
                        exit(1);
                    }
                }
                else if(compara_datas(&nova_tarefa->info.data_criacao, &nova_tarefa->info.data_prazo)== 0){
                    printf("O prazo de conclusao nao e posterior a data de criacao. Ficheiro Tarefas.txt incorreto na linha: %d",i);
                    exit(1);
                }
                /*le data conclusao*/
                fscanf(fp, " %d", &(nova_tarefa->info.data_conclusao.ano));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_conclusao.mes));
                fscanf(fp, "/%d", &(nova_tarefa->info.data_conclusao.dia));
                if(nova_tarefa->info.coluna == 3){
                   if((validar_data(&nova_tarefa->info.data_conclusao) == 0) || (compara_datas(&nova_tarefa->info.data_criacao, &nova_tarefa->info.data_conclusao)== 0) ){
                        printf("A data de conclusao esta na coluna 3 e ou a data nao e valida ou a comparacao nao e valida. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                        exit(1);
                   }
                }
                if(nova_tarefa->info.coluna != 3 && nova_tarefa->info.data_conclusao.ano != 0 && nova_tarefa->info.data_conclusao.mes != 0 && nova_tarefa->info.data_conclusao.dia != 0 ){
                    printf("A data de conclusao nao esta na coluna 3 e nao e igual a 0. Ficheiro Tarefas.txt incorreto na linha: %d.",i);
                    exit(1);
                }
                i++;
                lista = adiciona_tarefa(lista, nova_tarefa, lista_todo, lista_doing, lista_done);
            }
            fclose(fp);
        }
    }
    else {
        printf("\nO ficheiro Tarefas.txt nao esta disponivel.");
        exit(1);
    }
    return lista;
}

void escreve_fich(ListTarefa lista, char ficheiro[]){
    FILE *fp;
    fp = fopen(ficheiro, "w");
    if (fp != NULL) {
        while (lista != NULL) {
            fprintf(fp,"%d\t%d\t%d\t%d\t%s", lista->info.id, lista->info.coluna, lista->info.prioridade, lista->info.id_pessoa, lista->info.descricao);
            fprintf(fp, "\t%d/%d/%d", lista->info.data_criacao.ano, lista->info.data_criacao.mes, lista->info.data_criacao.dia);
            fprintf(fp, "\t%d/%d/%d", lista->info.data_prazo.ano, lista->info.data_prazo.mes, lista->info.data_prazo.dia);
            fprintf(fp, "\t%d/%d/%d", lista->info.data_conclusao.ano, lista->info.data_conclusao.mes, lista->info.data_conclusao.dia);
            if (lista->next != NULL)
                fprintf(fp, "\n");
            lista = lista->next;
        }
        fclose(fp);
    }
    else {
        printf("O ficheiro nao esta disponivel.");
        exit(1);
    }
}

/*Funções que guardam em ficheiro as listagens a pedido do utilizador*/

void novo_fich_pessoas(ListPessoa lista){
    FILE *fp; char ficheiro[30];
    printf("\nInsira o nome do ficheiro onde quer guardar os dados seguido de .txt: ");
    scanf("%[^\n]s", ficheiro);
    fp = fopen(ficheiro, "w");
    if (fp != NULL) {
        fprintf(fp, "LISTA DE PESSOAS\n\n");
        if (lista == NULL)
            fprintf(fp, "Nao ha nenhuma pessoa inserida.");
        while (lista != NULL) {
            fprintf(fp,"Nome: %s\n", lista->info.nome);
            fprintf(fp, "Email: %s\n", lista->info.email);
            fprintf(fp, "Id: %d\n", lista->info.id);
            if (lista->next != NULL)
                fprintf(fp, "\n");
            lista = lista->next;
        }
        fclose(fp);
    }
    else {
        printf("O ficheiro nao esta disponivel.");
        exit(1);
    }
}

void novo_fich_tarefas(ListTarefa lista){
    FILE *fp; char ficheiro[30];
    printf("Insira o nome do ficheiro onde quer guardar os dados seguido de .txt: ");
    scanf("%[^\n]s", ficheiro);
    fp = fopen(ficheiro, "w");
    if (fp != NULL) {
        fprintf(fp, "LISTA DE TAREFAS\n\n");
        if (lista == NULL)
            fprintf(fp, "Nao ha nenhuma tarefa inserida.");
        while (lista != NULL) {
            fprintf(fp, "\nTAREFA %d\n", lista->info.id);
            fprintf(fp, "Descricao: %s\n", lista->info.descricao);
            if (lista->info.coluna == 1)
                fprintf(fp, "Coluna: To Do ");
            else if (lista->info.coluna == 2)
                fprintf(fp, "Coluna: Doing ");
            else
                fprintf(fp, "Coluna: Done ");
            fprintf(fp, "Prioridade: %d\n",lista->info.prioridade);
            fprintf(fp, "Data criacao: %d/%d/%d\n",lista->info.data_criacao.ano,lista->info.data_criacao.mes,lista->info.data_criacao.dia);
            if (lista->info.data_prazo.ano != 0)
                fprintf(fp, "Prazo conclusao: %d/%d/%d\n",lista->info.data_prazo.ano,lista->info.data_prazo.mes,lista->info.data_prazo.dia);
            if (lista->info.data_conclusao.ano != 0)
                fprintf(fp, "Data conclusao: %d/%d/%d\n", lista->info.data_conclusao.ano, lista->info.data_conclusao.mes, lista->info.data_conclusao.dia);
            if (lista->apont_pessoa != NULL){
                fprintf(fp, "Pessoa responsavel: %s\n", lista->apont_pessoa->info.nome);
            }
            if (lista->next != NULL)
                fprintf(fp, "\n");
            lista = lista->next;
        }
        fclose(fp);
    }
    else {
        printf("O ficheiro nao esta disponivel.");
        exit(1);
    }
}

void fich_listar_apont_tarefas(ListApontTarefa lista, FILE* fp){
    if (lista == NULL)
        fprintf(fp, "Nao ha nenhuma tarefa inserida.\n\n");
    else{
        while(lista != NULL){
            fprintf(fp, "TAREFA %d\n", lista->apont_tarefa->info.id);
            fprintf(fp, "Descricao: %s\n", lista->apont_tarefa->info.descricao);
            fprintf(fp, "Prioridade: %d\n", lista->apont_tarefa->info.prioridade);
            fprintf(fp, "Data criacao: %d/%d/%d\n", lista->apont_tarefa->info.data_criacao.ano, lista->apont_tarefa->info.data_criacao.mes,lista->apont_tarefa->info.data_criacao.dia);
            if (lista->apont_tarefa->info.data_prazo.ano != 0)
                fprintf(fp, "Prazo conclusao: %d/%d/%d\n", lista->apont_tarefa->info.data_prazo.ano, lista->apont_tarefa->info.data_prazo.mes,lista->apont_tarefa->info.data_prazo.dia);
            if (lista->apont_tarefa->info.data_conclusao.ano != 0)
                fprintf(fp, "Data conclusao: %d/%d/%d\n", lista->apont_tarefa->info.data_conclusao.ano, lista->apont_tarefa->info.data_conclusao.mes, lista->apont_tarefa->info.data_conclusao.dia);
            if(lista->apont_tarefa->apont_pessoa != NULL){
                fprintf(fp, "Pessoa responsavel: %s\n", lista->apont_tarefa->apont_pessoa->info.nome);
            }
            fprintf(fp, "\n");
            lista = lista->next;
        }
    }
}


void novo_fich_quadro_kanban(ListApontTarefa lista_todo, ListApontTarefa lista_doing, ListApontTarefa lista_done){
    FILE *fp; char ficheiro[30];
    printf("\nInsira o nome do ficheiro onde quer guardar os dados seguido de .txt: ");
    scanf("%[^\n]s", ficheiro);
    fp = fopen(ficheiro, "w");
    if (fp != NULL) {
        fprintf(fp, " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
        fprintf(fp, "*                       QUADRO KANBAN                         *\n");
        fprintf(fp, " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");

        if (lista_todo == NULL && lista_doing == NULL && lista_done == NULL)
            fprintf(fp, "Nao ha nenhuma tarefa inserida.\n");
        else{
            fprintf(fp, "\n|--------------------------|\n");
            fprintf(fp, "|          TO DO           |\n");
            fprintf(fp, "|--------------------------|\n\n");
            fich_listar_apont_tarefas(lista_todo, fp);
            fprintf(fp, "\n|--------------------------|\n");
            fprintf(fp, "|          DOING           |\n");
            fprintf(fp, "|--------------------------|\n\n");
            fich_listar_apont_tarefas(lista_doing, fp);
            fprintf(fp, "\n|--------------------------|\n");
            fprintf(fp, "|          DONE            |\n");
            fprintf(fp, "|--------------------------|\n\n");
            fich_listar_apont_tarefas(lista_done, fp);
            printf("\n");
        }
        fclose(fp);
    }
    else {
        printf("O ficheiro nao esta disponivel.");
        exit(1);
    }
}


void fich_imprime_apont_tarefa(ListApontTarefa t, FILE *fp) {
    if(t == NULL)
        fprintf(fp, "A tarefa nao existe.\n");
    else {
        fprintf(fp, "TAREFA %d\n", t->apont_tarefa->info.id);
        fprintf(fp, "Descricao: %s\n", t->apont_tarefa->info.descricao);
        fprintf(fp, "Prioridade: %d\n", t->apont_tarefa->info.prioridade);
        fprintf(fp, "Data criacao: %d/%d/%d\n", t->apont_tarefa->info.data_criacao.ano, t->apont_tarefa->info.data_criacao.mes, t->apont_tarefa->info.data_criacao.dia);
        fprintf(fp, "Prazo conclusao: %d/%d/%d\n", t->apont_tarefa->info.data_prazo.ano, t->apont_tarefa->info.data_prazo.mes, t->apont_tarefa->info.data_prazo.dia);
        if (t->apont_tarefa->info.data_conclusao.ano != 0)
            fprintf(fp, "Data conclusao: %d/%d/%d\n", t->apont_tarefa->info.data_conclusao.ano, t->apont_tarefa->info.data_conclusao.mes, t->apont_tarefa->info.data_conclusao.dia);
        fprintf(fp, "\n");
    }
}


void novo_fich_quadro_pessoa(ListPessoa pessoa) {
    FILE *fp; char ficheiro[30];
    ListApontTarefa aux;
    int contador = 0;
    printf("\nInsira o nome do ficheiro onde quer guardar os dados seguido de .txt: ");
    scanf("%[^\n]s", ficheiro);
    fp = fopen(ficheiro, "w");
    if (fp != NULL) {
        fprintf(fp, "Tarefas de %s:\n", pessoa->info.nome);
        aux = pessoa->apont_tarefas;
        if (aux == NULL)
            fprintf(fp, "Nao ha tarefas a mostrar.\n\n");
        else {
            fprintf(fp, "\n|--------------------------|\n");
            fprintf(fp, "|          DOING           |\n");
            fprintf(fp, "|--------------------------|\n\n");
            while(aux != NULL) {
                if (aux->apont_tarefa->info.coluna == 2) {
                    fich_imprime_apont_tarefa(aux, fp);
                    contador += 1;
                }
                aux = aux->next;
            }
            if (contador == 0)
                fprintf(fp, "Nao ha nenhuma tarefa a mostrar.\n");
            aux = pessoa->apont_tarefas;
            contador = 0;
            fprintf(fp, "\n|--------------------------|\n");
            fprintf(fp, "|          DONE            |\n");
            fprintf(fp, "|--------------------------|\n\n");
            while(aux != NULL) {
                if (aux->apont_tarefa->info.coluna == 3) {
                    fich_imprime_apont_tarefa(aux, fp);
                    contador += 1;
                }
                aux = aux->next;
            }
            if (contador == 0)
                fprintf(fp, "Nao ha nenhuma tarefa a mostrar.\n");
            fprintf(fp, "\n");
        }
        fclose(fp);
    }
    else {
        printf("O ficheiro nao esta disponivel.");
        exit(1);
    }
}
