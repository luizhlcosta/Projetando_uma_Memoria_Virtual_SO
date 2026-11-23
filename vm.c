#include <stdio.h>
#include <string.h>

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

    printf("Argumentos validos.\n");

    return 0;
}
