#ifndef APONT_TAREFAS_H
#define APONT_TAREFAS_H
#include "structs.h"


void adiciona_apont_tarefa(ListApontTarefa *lista, ListTarefa tarefa);
void listar_apont_tarefas(ListApontTarefa lista);
void imprime_apont_tarefa(ListApontTarefa t);
ListApontTarefa eliminar_apont_tarefa(ListApontTarefa lista, int id);
ListApontTarefa destroi_lista_aponttarefa(ListApontTarefa lista);

int verificar_maximo_tarefas(ListApontTarefa lista, int num_max_tarefas);
int verificar_maximo_tarefas_pessoa(ListApontTarefa lista, int num_max_tarefas);

ListApontTarefa ordena_lista(ListApontTarefa lista);
ListApontTarefa inserir_ordenado(ListApontTarefa L2, ListApontTarefa novo);

void imprimir_quadro_pessoa(ListPessoa pessoa);
void imprimir_titulo_quadro();
void imprimir_quadro(ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done);


#endif /* APONT_TAREFAS_H */
