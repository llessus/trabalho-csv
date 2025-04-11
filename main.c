#include <stdio.h>
#include <stdlib.h>
#include "processo.h"
#include <locale.h>
#include <conio.h>

#define MAX_PROCESSOS 20000

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    Processo *processos;
    processos = malloc(sizeof(Processo) * MAX_PROCESSOS);
    if (!processos) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    int total = ler_processos("processo_043_202409032338.csv", processos, MAX_PROCESSOS);


    //printf("Total de processos lidos: %d\n", total);
    
    

    
    int opcao, classe;

    
    do {
        printf("\nMenu:\n");
        printf("1 - Ordenar por ID\n");
        printf("2 - Ordenar por Data\n");
        printf("3 - Contar por Classe\n");
        printf("4 - Contar Assuntos Unicos\n");
        printf("5 - Listar Multiplos Assuntos\n");
        printf("6 - Dias em Tramitacao\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total);
                salvar_em_csv("saida_ordenada_id.csv", processos, total);
                printf("Processos ordenados por ID e salvos em saida_ordenada_id.csv\n");
                break;
            case 2:
                ordenar_por_data(processos, total);
                salvar_em_csv("saida_ordenada_data.csv", processos, total);
                printf("Processos ordenados por Data (decrescente) e salvos em saida_ordenada_data.csv\n");
                break;
            case 3:
                printf("Digite o id_classe a ser contado (processos podem ter multiplos): ");
                scanf("%d", &classe);
                printf("Total de processos com id_classe %d: %d\n", classe, contar_por_classe(processos, total, classe));
                break;
            case 4:
                printf("Total de assuntos unicos: %d\n", contar_assuntos_unicos(processos, total));
                break;
            case 5:
                listar_multiplos_assuntos(processos, total);
                break;
            case 6:
                dias_em_tramitacao(processos, total);
                break;
            case 0:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }} while (opcao != 0);

    free(processos); // liberar espaco depois que terminar de usar
    printf("precione qualquer tecla para continuar...\n");
    getchar(); // Limpa o buffer de entrada
    getch();
    return 0;
}