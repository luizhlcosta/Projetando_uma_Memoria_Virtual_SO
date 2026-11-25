#include <stdio.h>

#ifndef MEMORIA_H
#define MEMORIA_H

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define NUM_FRAMES 128
#define NUM_PAGES 256
#define TAMANHO_TLB 16

typedef struct {
    int numero_pagina;
    int numero_frame;
} ItemTLB;

typedef struct {
    ItemTLB itens[TAMANHO_TLB];

    unsigned long long contador_insercoes;
    unsigned long long ordem_insercao[TAMANHO_TLB];

    unsigned long long contador_acessos;
    unsigned long long ultimo_acesso[TAMANHO_TLB];
} TLB;

typedef struct {
    const TLB *tlb;
    int numero_pagina;
    int item_index;
    int encontrou;
} DadosConsultaTLB;

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

void inicializar_tlb(TLB *tlb);
void *consultar_item_tlb(void *argumento);
int consultar_tlb_sequencial(const TLB *tlb, int numero_pagina);
int consultar_tlb(const TLB *tlb, int numero_pagina);
void invalidar_tlb(TLB *tlb, int numero_pagina);
int inserir_tlb(TLB *tlb, int numero_pagina, int numero_frame);
void registrar_acesso_tlb(TLB *tlb, int item_index);
int escolher_item_fifo(const TLB *tlb);
int escolher_item_lru(const TLB *tlb);

#endif
