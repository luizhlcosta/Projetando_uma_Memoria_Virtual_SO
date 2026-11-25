#include <stdio.h>

#ifndef MEMORIA_H
#define MEMORIA_H

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define NUM_FRAMES 128
#define NUM_PAGES 256

typedef struct {
    int tabela_paginas[NUM_PAGES];
    unsigned char memoria_fisica[NUM_FRAMES][FRAME_SIZE];
    int proximo_frame;
    int vetor_paginas_dos_frames[NUM_FRAMES];
    int proximo_frame_fifo;
    unsigned long long contador_acessos;
    unsigned long long ultimo_acesso[NUM_FRAMES];
} Memoria;

int calcular_numero_pagina(int endereco_logico);
int calcular_valor_deslocamento(int endereco_logico);

void inicializar_tabela_paginas(int tabela_paginas[]);
void inicializar_gerenciador(Memoria *memoria);

int consultar_tabela_paginas(const Memoria *memoria, int numero_pagina);
void linkar_pagina_ao_frame(Memoria *memoria, int numero_da_pagina, int numero_do_frame);
int obter_proximo_frame(Memoria *memoria);
int selecionar_frame_fifo(Memoria *memoria);
void registrar_acesso_lru(Memoria *memoria, int numero_do_frame);
int escolher_frame_lru(const Memoria *memoria);

int carregar_pagina(Memoria *memoria, FILE *arquivo_binario, int numero_da_pagina, int numero_do_frame);

#endif
