#ifndef STRUCTS_H
#define STRUCTS_H

/*Estruturas*/

typedef struct{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct {
    char nome[30];
    char email[30];
    int id;
}Pessoa;

typedef struct{
    int id;
    int prioridade;
    char descricao[30];
    int coluna; /*para ver em que seccao e que esta: 1. To Do; 2. Doing; 3. Done*/
    Data data_criacao;
    Data data_prazo;
    Data data_conclusao;
    int id_pessoa;
}Tarefa;

typedef struct no_tarefa *ListTarefa;
typedef struct no_pessoa *ListPessoa;
typedef struct no_apont_tarefa *ListApontTarefa;

typedef struct no_tarefa{
    Tarefa info;
    ListPessoa apont_pessoa;
    ListTarefa next;
}No_tarefa;

typedef struct no_apont_tarefa {
    ListTarefa apont_tarefa;
    ListApontTarefa next;
}No_apont_tarefa;

typedef struct no_pessoa {
    Pessoa info;
    ListApontTarefa apont_tarefas;
    ListPessoa next;
}No_pessoa;


#endif /* STRUCTS_H */
