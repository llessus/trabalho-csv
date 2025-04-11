#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 20000
#define MAX_NUMERO 30
#define MAX_DATA 20
#define MAX_ASSUNTOS 10
#define MAX_CLASSES 10 

typedef struct {
    int id;
    char numero[MAX_NUMERO];
    char data_ajuizamento[MAX_DATA];
    int id_classes[MAX_CLASSES];
    int qtd_classes;
    int id_assuntos[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;

// Funções para manipular o tad Processo
int ler_processos(const char *nome_arquivo, Processo processos[], int max);
void ordenar_por_id(Processo processos[], int n);
void ordenar_por_data(Processo processos[], int n);
int contar_por_classe(Processo processos[], int n, int classe);
int contar_assuntos_unicos(Processo processos[], int n);
void listar_multiplos_assuntos(Processo processos[], int n);
void dias_em_tramitacao(Processo processos[], int n);
void salvar_em_csv(const char *nome_arquivo, Processo processos[], int n);

#endif