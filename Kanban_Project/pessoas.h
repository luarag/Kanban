#ifndef PESSOAS_H
#define PESSOAS_H
#include "structs.h"


/*Leitura Ficheiro*/
ListPessoa ler_fich_pessoas(char nome_fich[], ListPessoa lista);

/*Funcoes listas*/
ListPessoa adiciona_pessoa(ListPessoa lista, ListPessoa nova_pessoa);
void listar_pessoas(ListPessoa lista);
ListPessoa procurar_pessoa(ListPessoa lista, int id);
ListPessoa destroi_lista_pessoa(ListPessoa lista);

int alterar_pessoa_responsavel(ListPessoa lista_pessoas, ListTarefa tarefa, int max_tarefas_pessoa);
void criar_ligacao_pessoa_tarefa(ListTarefa lista_tarefas, ListPessoa lista_pessoas);


#endif /* PESSOAS_H */
