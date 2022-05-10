#include <stdio.h>
#include <stdlib.h>
#include "tarefas.h"
#include "pessoas.h"
#include "apont_tarefas.h"


void mover_tarefa(ListTarefa lista, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListPessoa lista_pessoas, int max_doing, int max_tarefas_pessoa){
    ListTarefa tarefa;
    int id_tarefa, num;
    printf("\n\n|---------------------------------------------------------------|\n");
    printf("|\t\t\tMover Tarefas\t\t\t\t|\n");
    printf("|---------------------------------------------------------------|\n");
    imprimir_quadro(lista_todo, lista_doing, lista_done);
    printf("Escolha o numero da tarefa que deseja mover: ");

    id_tarefa = pede_verifica_numero();
    tarefa = procurar_tarefa(lista, id_tarefa);
    while(id_tarefa == -1 || tarefa == NULL){
        printf("\nInsira um numero valido.\n");
        printf("Escolha o numero da tarefa que deseja alterar: ");
        id_tarefa = pede_verifica_numero();
        tarefa = procurar_tarefa(lista, id_tarefa);
    }

    printf("\nPara que coluna quer mover?\n1:TO DO\n2:DOING\n3:DONE\n4:Nenhuma\n");
    num = pede_verifica_numero();
    while(num == -1 || num <1 || num > 4){
        printf("Insira uma opcao valida (1-4): ");
        num = pede_verifica_numero();
    }

    switch(num) {
        case 1:
            if(tarefa->info.coluna == 1)
                printf("Nao e possivel mover a tarefa.\nA tarefa ja se encontra na coluna \"To Do\".\n");
            else
                mover_tarefa_todo(tarefa, lista_todo, lista_doing, lista_done);
            break;
        case 2:
            if(tarefa->info.coluna == 2)
                printf("Nao e possivel mover a tarefa.\nA tarefa ja se encontra na coluna \"Doing\".\n");
            else {
                if (verificar_maximo_tarefas(*lista_doing, max_doing))
                    mover_tarefa_doing(tarefa, lista_todo, lista_doing, lista_done, lista_pessoas, max_tarefas_pessoa);
                else
                    printf("\nNao e possivel mover mais tarefas para doing.\nFoi atingido o limite maximo de %d.\n", max_doing);
            }
            break;
        case 3:
            if(tarefa->info.coluna == 3)
                printf("Nao e possivel mover a tarefa.\nA tarefa ja se encontra na coluna \"Done\".\n");
            else
                mover_tarefa_done(tarefa, lista_doing, lista_done, lista_todo,lista_pessoas, max_tarefas_pessoa);
            break;
        case 4:
            break;
    }
}

void mover_tarefa_todo(ListTarefa tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing,ListApontTarefa *lista_done){
    ListPessoa pessoa; int id_tarefa = tarefa->info.id; int num;
    pessoa = tarefa->apont_pessoa;
    pessoa->apont_tarefas = eliminar_apont_tarefa(pessoa->apont_tarefas, id_tarefa);

    tarefa->info.id_pessoa = 0;
    tarefa->apont_pessoa = NULL;

    printf("\nA tarefa ja contem uma data de prazo: %d/%d/%d\nComo deseja proceder?\n1:Manter a data de prazo\n2:Alterar a data de prazo\n3:Retirar a data de prazo\n", tarefa->info.data_prazo.ano, tarefa->info.data_prazo.mes, tarefa->info.data_prazo.dia);
    num = pede_verifica_numero();
    while(num == -1 || num <1 || num > 3){
        printf("Insira uma opcao valida (1-3): ");
        num = pede_verifica_numero();
    }
    if (num == 2) { /*Alterar o prazo*/
        printf("\nIndique um prazo de conclusao para a tarefa:");
        le_data(&tarefa->info.data_prazo);
        while (!validar_data(&tarefa->info.data_prazo)){
            printf("\nA data inserida nao e valida.\nInsira uma data de prazo de conclusao que exista.");
            le_data(&tarefa->info.data_prazo);
        }
        while (!compara_datas(&tarefa->info.data_criacao, &tarefa->info.data_prazo)){
            printf("\nData de criacao: %d/%d/%d\n", tarefa->info.data_criacao.ano, tarefa->info.data_criacao.mes, tarefa->info.data_criacao.dia);
            printf("Insira uma data de prazo de conclusao posterior ou igual a data de criacao da tarefa.");
            le_data(&tarefa->info.data_prazo);
        }
    }
    else if(num == 3) /*retirar o prazo e definir mais tarde*/
        data_inexistente(&(tarefa->info.data_prazo));


    if (tarefa->info.coluna == 2){
        *lista_doing = eliminar_apont_tarefa(*lista_doing, id_tarefa);
    }
    if (tarefa->info.coluna == 3){
        data_inexistente(&(tarefa->info.data_conclusao));
        *lista_done = eliminar_apont_tarefa(*lista_done, id_tarefa);
    }

    tarefa->info.coluna = 1;
    adiciona_apont_tarefa(lista_todo, tarefa);
}

void mover_tarefa_doing(ListTarefa tarefa, ListApontTarefa *lista_todo, ListApontTarefa *lista_doing,ListApontTarefa *lista_done, ListPessoa lista_pessoas, int max_tarefas_pessoa){
    int id_tarefa, pessoa_valida, num;
    id_tarefa = tarefa->info.id;
    if (tarefa->info.data_prazo.ano != 0) {
        printf("\nA tarefa ja contem uma data de prazo: %d/%d/%d\nDeseja altera-la?\n1:Sim\n2:Nao\n", tarefa->info.data_prazo.ano, tarefa->info.data_prazo.mes, tarefa->info.data_prazo.dia);
        num = pede_verifica_numero();
        while(num == -1 || num <1 || num > 2){
            printf("Insira uma opcao valida (1-2): ");
            num = pede_verifica_numero();
        }
    }
    if (num == 1 || tarefa->info.data_prazo.ano == 0) {
        printf("\nIndique um prazo de conclusao para a tarefa:");
        le_data(&tarefa->info.data_prazo);
        while (!validar_data(&tarefa->info.data_prazo)){
            printf("\nA data inserida nao e valida.\nInsira uma data de prazo de conclusao que exista.");
            le_data(&tarefa->info.data_prazo);
        }
        while (!compara_datas(&tarefa->info.data_criacao, &tarefa->info.data_prazo)){
            printf("\nData de criacao: %d/%d/%d\n", tarefa->info.data_criacao.ano, tarefa->info.data_criacao.mes, tarefa->info.data_criacao.dia);
            printf("Insira uma data de prazo de conclusao posterior ou igual a data de criacao da tarefa.");
            le_data(&tarefa->info.data_prazo);
        }
    }
    /*TO DO -> DOING*/
    if(tarefa->info.coluna == 1){
        pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
        while(!pessoa_valida){
            printf("Selecione outra pessoa.\n");
            pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
        }
        *lista_todo = eliminar_apont_tarefa(*lista_todo, id_tarefa);
    }
    /*DONE -> DOING*/
    if(tarefa->info.coluna == 3){
        printf("\nDeseja alterar a pessoa anteriormente responsavel por esta tarefa?:\n1:Sim\n2:Nao\n");
        num = pede_verifica_numero();
        while(num == -1 || num <1 || num > 2){
            printf("Insira uma opcao valida (1-2): ");
            num = pede_verifica_numero();
        }
        if(num == 1){
            pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
            while(!pessoa_valida){
                printf("\nNao e possivel atribuir mais tarefas a pessoa escolhida.\nFoi atingido o limite maximo de %d.\n", max_tarefas_pessoa);
                pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
            }
        }
        data_inexistente(&(tarefa->info.data_conclusao));
        *lista_done = eliminar_apont_tarefa(*lista_done, id_tarefa);
    }
    tarefa->info.coluna = 2;
    adiciona_apont_tarefa(lista_doing, tarefa);
}


void mover_tarefa_done(ListTarefa tarefa, ListApontTarefa *lista_doing, ListApontTarefa *lista_done, ListApontTarefa *lista_todo,ListPessoa lista_pessoas, int max_tarefas_pessoa){
    int id_tarefa, ver, data_valida, pessoa_valida;
    id_tarefa = tarefa->info.id;
    printf("\nInsira uma data de conclusao para a tarefa:\n");
    le_data(&tarefa->info.data_conclusao);
    data_valida = validar_data(&tarefa->info.data_conclusao);
    while (data_valida == 0){
        printf("\nA data inserida nao e valida.\nInsira uma data de conclusao que exista.");
        le_data(&tarefa->info.data_conclusao);
        data_valida = validar_data(&tarefa->info.data_conclusao);
    }
    ver = compara_datas(&tarefa->info.data_criacao, &tarefa->info.data_conclusao);
    while (ver == 0){
        printf("\nData de criacao: %d/%d/%d\n", tarefa->info.data_criacao.ano, tarefa->info.data_criacao.mes, tarefa->info.data_criacao.dia);
        printf("Insira uma data de conclusao posterior a data de criacao da tarefa.");
        le_data(&tarefa->info.data_conclusao);
        ver = compara_datas(&tarefa->info.data_criacao, &tarefa->info.data_conclusao);
    }
    /*TO DO -> DONE*/
    if(tarefa->info.coluna == 1){
        pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
        while(!pessoa_valida){
            printf("\nNao e possivel atribuir mais tarefas a pessoa escolhida.\nFoi atingido o limite maximo de %d.\n", max_tarefas_pessoa);
            pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
        }
        *lista_todo = eliminar_apont_tarefa(*lista_todo, id_tarefa);
    }
    /*DOING -> DONE*/
    if(tarefa->info.coluna == 2){
        *lista_doing = eliminar_apont_tarefa(*lista_doing, id_tarefa);
    }
    tarefa->info.coluna = 3;
    adiciona_apont_tarefa(lista_done, tarefa);
}
