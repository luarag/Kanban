#include <stdio.h>
#include <stdlib.h>
#include "pessoas.h"
#include "tarefas.h"

void data_inexistente(Data *data){
    data->ano = 0;
    data->mes = 0;
    data->dia = 0;
}

void le_data(Data *data){
    printf("\nInsira ano: ");
    scanf("%d", &(data->ano));
    printf("Insira mes: ");
    scanf("%d", &(data->mes));
    printf("Insira dia: ");
    scanf("%d", &(data->dia));
}


/*verifica se data de criacao e anterior(1), igual (2) ou posterior(0) a data de conclusao*/
int compara_datas(Data *data_criacao, Data *data_conclusao){
    if(data_criacao->ano < data_conclusao->ano)
        return 1;
    else if(data_criacao->ano == data_conclusao->ano){
        if(data_criacao->mes < data_conclusao->mes)
            return 1;
        else if(data_criacao->mes == data_conclusao->mes){
            if (data_criacao->dia == data_conclusao->dia)
                return 2;
            else if (data_criacao->dia < data_conclusao->dia)
                return 1;
        }
    }
    return 0;
}


int validar_data(Data *data){
    if (data->ano > 0){
        if (data->mes > 0 && data->mes <= 12){
            if (data->mes <= 7){
                if(data->mes == 2){
                    if((data->ano%400 == 0 || (data->ano%4 == 0 && data->ano%100 != 0)) && data->dia > 0 && data->dia <= 29)
                        return 1;
                    else if (data->dia > 0 && data->dia <= 28)
                        return 1;
                }
                else if(data->mes%2 == 0 && data->dia > 0 && data->dia <= 30)
                    return 1;
                else if(data->dia > 0 && data->dia <= 31)
                    return 1;
            }
            if(data->mes >= 8){
                if(data->mes%2 == 0 && data->dia > 0 && data->dia <= 31)
                    return 1;
                if(data->mes%2 != 0 && data->dia > 0 && data->dia <= 30)
                    return 1;
            }
        }
    }
    return 0;
}

int verifica_prazo_pessoa(ListPessoa pessoa, ListTarefa tarefa){
    ListApontTarefa aux;
    int res;
    aux = pessoa->apont_tarefas;
    while(aux!=NULL){
        if (aux->apont_tarefa->info.coluna == 2){
            res = compara_prazo_tarefas(&(aux->apont_tarefa->info.data_prazo), &(tarefa->info.data_prazo));
            if(res == 0){
                return 0;
            }
        }
        aux = aux->next;
    }
    return 1;
}

int compara_prazo_tarefas(Data *pessoa,Data *tarefa){
    int res;
    if(pessoa->ano == tarefa->ano){
        if (pessoa->mes == tarefa->mes){
            res = pessoa->dia - tarefa->dia;
            if(abs(res)<8)
                return 0;
        }
        if (pessoa->mes ==((tarefa->mes)+1)){
            res = 31-(pessoa->dia);
            printf("31-%d: %d",pessoa->dia,res);
            if(tarefa->mes == 2){
                if((pessoa->ano%400 == 0 || (pessoa->ano%4 == 0 && pessoa->ano%100 != 0)) && ((29-tarefa->dia) + pessoa->dia)<8)
                    return 0;
                else if((28-tarefa->dia) + (pessoa->dia)<8)
                    return 0;
            }

            else if((tarefa->mes == 3 || tarefa->mes == 5 || tarefa->mes == 7 || tarefa->mes == 8 || tarefa->mes == 10) && ((31-tarefa->dia)+pessoa->dia)<8)
                return 0;
            else if(((30-tarefa->dia) + pessoa->dia)<8)
                return 0;
        }
        if (tarefa->mes ==((pessoa->mes)+1)){
            if(pessoa->mes == 2){
                if((pessoa->ano%400 == 0 || (pessoa->ano%4 == 0 && pessoa->ano%100 != 0)) && ((29-pessoa->dia) + tarefa->dia)<8)
                     return 0;
                else if(((28 - pessoa->dia) + tarefa->dia)<8)
                    return 0;
            }
            else if((pessoa->mes == 3 || pessoa->mes == 5 || pessoa->mes == 7 || pessoa->mes == 8 || pessoa->mes == 10) && ((31-pessoa->dia)+tarefa->dia)<8)
                return 0;
            else if(((30-(pessoa->dia))+ (tarefa->dia))<8)
                return 0;
        }
    }
    if((tarefa->ano == (pessoa->ano+1) && (pessoa->mes == 12 && tarefa->mes ==1 &&((31-pessoa->dia)+(tarefa->dia))<8)) || ((pessoa->ano == (tarefa->ano+1)) && (pessoa->mes == 1 && tarefa->mes ==12) && ((31-tarefa->dia)+(pessoa->dia))<8))
        return 0;

    return 1;
}
