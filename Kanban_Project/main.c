#include <stdio.h>
#include <stdlib.h>
#include "pessoas.h"
#include "tarefas.h"
#include "apont_tarefas.h"
#include <conio.h>


int main()
{
    char fich_pessoas[] = "Pessoas.txt";
    char fich_tarefas[] = "Tarefas.txt";
    int max_doing, max_tarefas_pessoa;
    int id_tarefa, id_pessoa, num, pessoa_valida;
    ListTarefa tarefa; ListPessoa pessoa;

    /*Criar listas ligadas: pessoas, tarefas e apontadores para tarefas (todo, doing e done)*/
    ListPessoa lista_pessoas = NULL;
    ListTarefa lista_tarefas = NULL;
    ListApontTarefa lista_todo = NULL;
    ListApontTarefa lista_doing = NULL;
    ListApontTarefa lista_done = NULL;

    system("COLOR B");
    printf("Insira o numero maximo de tarefas em doing: ");
    max_doing = pede_verifica_numero();
    while(max_doing == -1){
        printf("Insira um numero valido: ");
        max_doing = pede_verifica_numero();
    }
    printf("Insira o numero maximo de tarefas que podem ser atribuidas a uma pessoa: ");
    max_tarefas_pessoa = pede_verifica_numero();
    while(max_tarefas_pessoa == -1){
        printf("Insira um numero valido: ");
        max_tarefas_pessoa = pede_verifica_numero();
    }

    lista_pessoas = ler_fich_pessoas(fich_pessoas, lista_pessoas);
    lista_tarefas = ler_fich_tarefas(fich_tarefas, lista_tarefas, &lista_todo, &lista_doing, &lista_done, lista_pessoas);
    criar_ligacao_pessoa_tarefa(lista_tarefas, lista_pessoas);

    while(1) {

        printf("\n\n|===============================================================|\n");
        printf("|                           MENU                                |\n");
        printf("|===============================================================|\n\n");
        printf("BEM-VINDO AO QUADRO KANBAN!\nSelecione uma opcao:\n\n");
        printf("1:  Listar todas as pessoas\n");
        printf("2:  Listar todas as tarefas por data de criacao\n");
        printf("3:  Inserir nova tarefa em \"To Do\"\n");
        printf("4:  Mover tarefas\n");
        printf("5:  Alterar pessoa responsavel por uma tarefa\n");
        printf("6:  Alterar informacao de uma tarefa\n");
        printf("7:  Alterar limites de tarefas\n");
        printf("8:  Visualizar o quadro de uma pessoa\n");
        printf("9:  Visualizar o quadro Kanban\n");
        printf("10: Gravar e Sair\n");

        num = pede_verifica_numero();
        while(num == -1){
            printf("Insira uma opcao valida (1-10): ");
            num = pede_verifica_numero();
        }

        switch(num){
            case 1:
                printf("\nLISTA DE PESSOAS:\n\n");
                listar_pessoas(lista_pessoas);
                printf("Deseja guardar esta informacao num ficheiro?\n1:Sim\n2:Nao\n");
                num = pede_verifica_numero();
                while(num == -1 || num < 1 || num > 2){
                    printf("\nInsira uma opcao valida (1-2): ");
                    num = pede_verifica_numero();
                }
                if (num == 1)
                    novo_fich_pessoas(lista_pessoas);
                break;
            case 2:
                /*tarefas ordenadas por data de criacao*/
                printf("\nLISTA DE TAREFAS:\n\n");
                lista_tarefas = ordena_lista_data_criacao(lista_tarefas);
                listar_tarefas(lista_tarefas);
                printf("\nDeseja guardar esta informacao num ficheiro?\n1:Sim\n2:Nao\n");
                num = pede_verifica_numero();
                while(num == -1 || num < 1 || num > 2){
                    printf("\nInsira uma opcao valida (1-2): ");
                    num = pede_verifica_numero();
                }
                if (num == 1)
                    novo_fich_tarefas(lista_tarefas);
                break;
            case 3:
                lista_tarefas = inserir_tarefa_todo(lista_tarefas, &lista_todo, &lista_doing, &lista_done);
                escreve_fich(lista_tarefas, fich_tarefas);
                break;
            case 4:
                mover_tarefa(lista_tarefas, &lista_todo, &lista_doing, &lista_done, lista_pessoas, max_doing, max_tarefas_pessoa);
                escreve_fich(lista_tarefas, fich_tarefas);
                break;
            case 5:
                printf("\nLista de tarefas:\n\n");
                imprimir_quadro(&lista_todo, &lista_doing, &lista_done);
                printf("\nEscolha o id da tarefa que esteja em doing ou done e que pretende alterar: ");
                id_tarefa = pede_verifica_numero();
                tarefa = procurar_tarefa(lista_tarefas, id_tarefa);
                while(id_tarefa == -1 || tarefa == NULL || tarefa->info.coluna==1){
                    printf("\nInsira um numero valido.\n");
                    printf("Escolha o id da tarefa que deseja alterar: ");
                    id_tarefa = pede_verifica_numero();
                    tarefa = procurar_tarefa(lista_tarefas, id_tarefa);
                }
                pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
                while(!pessoa_valida){
                    pessoa_valida = alterar_pessoa_responsavel(lista_pessoas, tarefa, max_tarefas_pessoa);
                }
                escreve_fich(lista_tarefas, fich_tarefas);
                break;
            case 6:
                alterar_informacao_tarefa(lista_tarefas, &lista_todo, &lista_doing, &lista_done);
                escreve_fich(lista_tarefas, fich_tarefas);
                break;
            case 7:
                alterar_limites(&max_doing, &max_tarefas_pessoa);
                break;
            case 8:
                printf("\nDe que pessoa quer ver as tarefas?\n\n");
                listar_pessoas(lista_pessoas);
                id_pessoa = pede_verifica_numero();
                pessoa = procurar_pessoa(lista_pessoas, id_pessoa);
                while(id_pessoa == -1 || pessoa == NULL){
                    printf("\nInsira um id valido.\n");
                    printf("Escolha o id da pessoa de quem quer ver as tarefas: ");
                    id_pessoa = pede_verifica_numero();
                    pessoa = procurar_pessoa(lista_pessoas, id_pessoa);
                }
                imprimir_quadro_pessoa(pessoa);
                printf("Deseja guardar esta informacao num ficheiro?\n1:Sim\n2:Nao\n");
                num = pede_verifica_numero();
                while(num == -1 || num < 1 || num > 2){
                    printf("\nInsira uma opcao valida (1-2): ");
                    num = pede_verifica_numero();
                }
                if (num == 1)
                    novo_fich_quadro_pessoa(pessoa);
                break;
            case 9:
                imprimir_titulo_quadro();
                imprimir_quadro(&lista_todo, &lista_doing, &lista_done);
                printf("Deseja guardar esta informacao num ficheiro?\n1:Sim\n2:Nao\n");
                num = pede_verifica_numero();
                while(num == -1 || num < 1 || num > 2){
                    printf("\nInsira uma opcao valida (1-2): ");
                    num = pede_verifica_numero();
                }
                if (num == 1)
                    novo_fich_quadro_kanban(lista_todo, lista_doing, lista_done);
                break;
            case 10:
                escreve_fich(lista_tarefas, fich_tarefas);
                lista_todo = destroi_lista_aponttarefa(lista_todo);
                lista_doing = destroi_lista_aponttarefa(lista_doing);
                lista_done = destroi_lista_aponttarefa(lista_done);
                lista_tarefas = destroi_lista_tarefa(lista_tarefas);
                lista_pessoas = destroi_lista_pessoa(lista_pessoas);
                exit(0); /*codigo de fecho de programa sem qualquer problema*/
                break;
            default: printf("Escolha uma opcao valida.\n");
        }
    }
    return 0;
}
