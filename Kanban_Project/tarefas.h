#ifndef TAREFAS_H
#define TAREFAS_H
#include "structs.h"
#include "pessoas.h"


/*Leitura e Escrita Ficheiro*/
ListTarefa ler_fich_tarefas(char ficheiro[], ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListPessoa lista_pessoas);
void escreve_fich(ListTarefa lista, char ficheiro[]);
void novo_fich_pessoas(ListPessoa lista);
void novo_fich_tarefas(ListTarefa lista);
void fich_listar_apont_tarefas(ListApontTarefa lista, FILE* fp);
void novo_fich_quadro_kanban(ListApontTarefa lista_todo, ListApontTarefa lista_doing, ListApontTarefa lista_done);
void fich_imprime_apont_tarefa(ListApontTarefa t, FILE *fp);
void novo_fich_quadro_pessoa(ListPessoa pessoa);

/*Funcoes listas*/
ListTarefa adiciona_tarefa(ListTarefa lista, ListTarefa nova_tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done);
void listar_tarefas(ListTarefa lista);
ListTarefa procurar_tarefa(ListTarefa lista, int id);
ListTarefa destroi_lista_tarefa(ListTarefa lista);

ListTarefa inserir_tarefa_todo(ListTarefa lista_tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done);
ListTarefa inserir_ordenado_data_criacao(ListTarefa L2, ListTarefa novo);
ListTarefa ordena_lista_data_criacao(ListTarefa lista);

/*Mover tarefas*/
void mover_tarefa(ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListPessoa lista_pessoas, int max_doing, int max_tarefas_pessoa);
void mover_tarefa_todo(ListTarefa tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done);
void mover_tarefa_doing(ListTarefa tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListPessoa lista_pessoas, int max_tarefas_pessoa);
void mover_tarefa_done(ListTarefa tarefa, ListApontTarefa *lista_doing, ListApontTarefa *lista_done,ListApontTarefa *lista_todo,ListPessoa lista_pessoas, int max_tarefas_pessoa);

int verificar_numero_tarefas(ListTarefa lista);
void alterar_limites(int *max_doing, int *max_tarefas_pessoa);
void alterar_informacao_tarefa(ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done);
int pede_verifica_numero();
int verifica_se_numero(char input[]);

/*Datas*/
void le_data(Data *data);
void data_inexistente(Data *data);
int validar_data(Data *data);
int compara_datas(Data *data_criacao, Data *data_conclusao);
int compara_prazo_tarefas(Data *pessoa,Data *tarefa);
int verifica_prazo_pessoa(ListPessoa pessoa,ListTarefa tarefa);


#endif /* TAREFAS_H */
