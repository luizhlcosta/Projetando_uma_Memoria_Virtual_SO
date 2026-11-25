#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "memoria.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Erro: quantidade de argumentos invalida.\n");
        printf("Uso: ./vm <arquivo> <fifo|lru> <fifo|lru>\n");
        return 1;
    }

    if (strcmp(argv[2], "fifo") != 0 && strcmp(argv[2], "lru") != 0) {
        printf("Erro: algoritmo de substituicao de paginas invalido.\n");
        return 1;
    }

    if (strcmp(argv[3], "fifo") != 0 && strcmp(argv[3], "lru") != 0) {
        printf("Erro: algoritmo de substituicao da TLB invalido.\n");
        return 1;
    }

    const char *nome_arquivo = argv[1];
    FILE *arquivo_enderecos = fopen(nome_arquivo, "r");

    if (arquivo_enderecos == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo %s.\n", nome_arquivo);
        return 1;
    }

    char linha[64];
    char *fim_conversao;
    long endereco_logico;
    int numero_linha = 0;

    while (fgets(linha, sizeof(linha), arquivo_enderecos) != NULL) {
        numero_linha++;

        if (strchr(linha, '\n') == NULL && strlen(linha) == sizeof(linha) - 1) {
            fprintf(stderr, "Erro: linha %d muito longa.\n", numero_linha);
            fclose(arquivo_enderecos);
            return 1;
        }

        errno = 0;
        endereco_logico = strtol(linha, &fim_conversao, 10);

        if (fim_conversao == linha || errno == ERANGE ||
            endereco_logico < 0 || endereco_logico > INT_MAX) {
            fprintf(stderr, "Erro: endereco invalido na linha %d.\n", numero_linha);
            fclose(arquivo_enderecos);
            return 1;
        }

        while (isspace((unsigned char) *fim_conversao)) {
            fim_conversao++;
        }

        if (*fim_conversao != '\0') {
            fprintf(stderr, "Erro: formato invalido na linha %d.\n", numero_linha);
            fclose(arquivo_enderecos);
            return 1;
        }
    }

    if (ferror(arquivo_enderecos)) {
        fprintf(stderr, "Erro durante a leitura do arquivo.\n");
        fclose(arquivo_enderecos);
        return 1;
    }

    Memoria memoria;
    inicializar_gerenciador(&memoria);

    FILE *arquivo_binario = fopen("BACKING_STORE.bin", "rb");

    if (arquivo_binario == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir BACKING_STORE.bin.\n");
        fclose(arquivo_enderecos);
        return 1;
    }

    rewind(arquivo_enderecos);

    while (fgets(linha, sizeof(linha), arquivo_enderecos) != NULL) {
        endereco_logico = strtol(linha, &fim_conversao, 10);

        int endereco_16bits = (int) (endereco_logico & 0xFFFF);
        int numero_da_pagina = calcular_numero_pagina(endereco_16bits);
        int valor_deslocamento = calcular_valor_deslocamento(endereco_16bits);
        int numero_do_frame = consultar_tabela_paginas(&memoria, numero_da_pagina);

        if (numero_do_frame == -1) {
            int pagina_antiga = -1;
            numero_do_frame = obter_proximo_frame(&memoria);

            if (numero_do_frame == -1) {
                if (strcmp(argv[2], "fifo") == 0) {
                    numero_do_frame = selecionar_frame_fifo(&memoria);
                } else {
                    numero_do_frame = escolher_frame_lru(&memoria);
                }

                pagina_antiga = memoria.vetor_paginas_dos_frames[numero_do_frame];
            }

            if (carregar_pagina(&memoria, arquivo_binario, numero_da_pagina, numero_do_frame) == -1) {
                fprintf(stderr, "Erro ao carregar pagina %d.\n", numero_da_pagina);
                fclose(arquivo_binario);
                fclose(arquivo_enderecos);
                return 1;
            }

            if (pagina_antiga != -1) {
                memoria.tabela_paginas[pagina_antiga] = -1;
            }

            linkar_pagina_ao_frame(&memoria, numero_da_pagina, numero_do_frame);

            printf("Page fault: pagina %d carregada no frame %d.\n",
                   numero_da_pagina, numero_do_frame);
        } else {
            printf("Pagina %d encontrada no frame %d.\n",
                   numero_da_pagina, numero_do_frame);
        }

        int endereco_fisico = numero_do_frame * FRAME_SIZE + valor_deslocamento;

        signed char valor_byte = (signed char) memoria.memoria_fisica[numero_do_frame][valor_deslocamento];

        if (strcmp(argv[2], "lru") == 0) {
            registrar_acesso_lru(&memoria, numero_do_frame);
        }

        printf("Endereco fisico: %d | Valor: %d\n", endereco_fisico, (int) valor_byte);


        printf("Endereco logico: %ld | Pagina: %d | Deslocamento: %d\n",
               endereco_logico, numero_da_pagina, valor_deslocamento);
    }

    if (ferror(arquivo_enderecos)) {
        fprintf(stderr, "Erro durante a leitura dos enderecos.\n");
        fclose(arquivo_binario);
        fclose(arquivo_enderecos);
        return 1;
    }

    fclose(arquivo_binario);
    fclose(arquivo_enderecos);

    return 0;
}
