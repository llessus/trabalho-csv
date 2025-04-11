#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processo.h"

int string_para_int(const char *str, int inicio, int fim) {
    int num = 0;
    for (int i = inicio; i < fim; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
        }
    }
    return num;
}
// Lê os processos de um arquivo CSV e armazena em um vetor de estruturas Processo
// Retorna o número de processos lidos
int ler_processos(const char *nome_arquivo, Processo processos[], int max) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 0;

    char linha[512];
    int i = 0;

    // Ignora o cabeçalho
    fgets(linha, sizeof(linha), f);
// Lê cada linha do arquivo e preenche a estrutura Processo
    while (fgets(linha, sizeof(linha), f) && i < max) {
        int id, ano;
        char numero[32], data[32];
        char classe_str[128], assunto_str[128];

        // Aponta para o início da linha
        char *p = linha;

        // ID
        id = atoi(p);
        p = strchr(p, ',') + 1;

        // Número entre aspas
        char *inicio_numero = strchr(p, '"') + 1;
        char *fim_numero = strchr(inicio_numero, '"');
        int len_num = fim_numero - inicio_numero;
        strncpy(numero, inicio_numero, len_num);
        numero[len_num] = '\0';
        p = strchr(fim_numero + 1, ',') + 1;
        // Data
        char *prox_virgula = strchr(p, ',');
        int len_data = prox_virgula - p;
        strncpy(data, p, len_data);
        data[len_data] = '\0';
        p = prox_virgula + 1;
        // Classe (entre chaves)
        char *inicio_classe = strchr(p, '{') + 1;
        char *fim_classe = strchr(inicio_classe, '}');
        int len_classe = fim_classe - inicio_classe;
        strncpy(classe_str, inicio_classe, len_classe);
        classe_str[len_classe] = '\0';
        p = strchr(fim_classe + 1, '{') + 1;
        // Assunto (entre chaves)
        char *fim_assunto = strchr(p, '}');
        int len_assunto = fim_assunto - p;
        strncpy(assunto_str, p, len_assunto);
        assunto_str[len_assunto] = '\0';
        p = strchr(fim_assunto + 1, ',') + 1;

        // Ano eleição
        ano = atoi(p);

        // Preenche estrutura
        processos[i].id = id;
        strcpy(processos[i].numero, numero);
        strncpy(processos[i].data_ajuizamento, data, 10);
        processos[i].data_ajuizamento[10] = '\0';
        processos[i].ano_eleicao = ano;

        // Processar classes
        processos[i].qtd_classes = 0;
        int start = 0;
        for (int j = 0; j <= len_classe; j++) {
            if (classe_str[j] == ',' || classe_str[j] == '\0') {
        char temp[10];
           strncpy(temp, &classe_str[start], j - start);
           temp[j - start] = '\0';
          processos[i].id_classes[processos[i].qtd_classes++] = atoi(temp);
                start = j + 1;      }  }

        // Processar assuntos
        processos[i].qtd_assuntos = 0;
        start = 0;
        for (int j = 0; j <= len_assunto; j++) {
            if (assunto_str[j] == ',' || assunto_str[j] == '\0') {
      char temp[10];
      strncpy(temp, &assunto_str[start], j - start);
   temp[j - start] = '\0';
   processos[i].id_assuntos[processos[i].qtd_assuntos++] = atoi(temp);
                start = j + 1;
            }
        }  i++;
    }

    fclose(f);
    printf("Total de processos lidos: %d\n", i);
    return i;
}

// Ordena por id (crescente)
void ordenar_por_id(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processos[i].id > processos[j].id) {
      Processo tmp = processos[i];
    processos[i] = processos[j];
      processos[j] = tmp;        }        }
    }
}

int comparar_data(const char *a, const char *b) {
    return strcmp(b, a);
}
/// Ordena por data (decrescente)
/// Formato: YYYY-MM-DD
/// Exemplo: 2023-01-01
void ordenar_por_data(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (comparar_data(processos[i].data_ajuizamento, processos[j].data_ajuizamento) > 0) {
                Processo tmp = processos[i];
                processos[i] = processos[j];
                processos[j] = tmp;
     }
    }
    }
}
/// Contar processos por classe (id_classe)
/// Se o processo tiver mais de 1 classe, conta apenas 1
/// Ex: {123,456} conta 2, mas apenas 1 classe
int contar_por_classe(Processo processos[], int n, int classe) {
    int cont = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            if (processos[i].id_classes[j] == classe) {
                cont++;
                break;
            }  }  }
    return cont;}
// caso tenha mais de 1 assunto, conta apenas 1 ex: {123,456} conta 2, mas apenas 1 assunto
int contar_assuntos_unicos(Processo processos[], int n) {
    int unicos[40000];
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int atual = processos[i].id_assuntos[j];
            int achou = 0;
            for (int k = 0; k < total; k++) {
                if (unicos[k] == atual) {
                    achou = 1;
                    break;  } }
            if (!achou) {
                unicos[total++] = atual; }
        } }
    return total;
}
/// Listar processos com mais de 1 assunto
/// Exibe o id, numero e os ids dos assuntos
/// Se nenhum processo for encontrado, exibe mensagem
void listar_multiplos_assuntos(Processo processos[], int qtd) {
    printf("Processos com multiplos assuntos:\n");
    int encontrados = 0;

    for (int i = 0; i < qtd; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("ID: %d | Numero: %s | Assuntos: ", processos[i].id, processos[i].numero);
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d ", processos[i].id_assuntos[j]);
            }
            printf("\n");
            encontrados++;
        }
    }
// Se nenhum processo foi encontrado, exibe mensagem

    if (encontrados == 0) {
        printf("Nenhum processo com multiplos assuntos foi encontrado.\n");
    } else {
        printf("\nTotal de processos com multiplos assuntos: %d\n", encontrados);
    }
}
/// Calcula os dias em tramitacao de cada processo
void dias_em_tramitacao(Processo processos[], int n) {
    int ano_atual = 2025, mes_atual = 4, dia_atual = 8;
    for (int i = 0; i < n; i++) {
        int ano, mes, dia;
        sscanf(processos[i].data_ajuizamento, "%d-%d-%d", &ano, &mes, &dia);
        int dias = (ano_atual - ano) * 365 + (mes_atual - mes) * 30 + (dia_atual - dia);
        printf("Processo ID %d esta ha %d dias em tramitacao.\n", processos[i].id, dias);
    }
    // so para aparecer no final da execucao
    printf("Total de processos analisados: %d\n", n);}
/// Salva os processos em um arquivo CSV
/// Formato: id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao
/// Exemplo: 1234567890,1234567890,"2023-01-01",123,456,2025
void salvar_em_csv(const char *nome_arquivo, Processo processos[], int n) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return;
    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%s,\"{", processos[i].id, processos[i].numero,
                processos[i].data_ajuizamento);
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            fprintf(f, "%d", processos[i].id_classes[j]);
            if (j < processos[i].qtd_classes - 1) fprintf(f, ",");
        }
        fprintf(f, "}\",\"{");
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(f, "%d", processos[i].id_assuntos[j]);
            if (j < processos[i].qtd_assuntos - 1) fprintf(f, ",");  }
        fprintf(f, "}\",%d\n", processos[i].ano_eleicao);
    }
    fclose(f);
}