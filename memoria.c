#include "memoria.h"

void inicializar_tlb(TLB *tlb) {
    for (int item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        tlb->itens[item_index].numero_pagina = -1;
        tlb->itens[item_index].numero_frame = -1;
        tlb->ordem_insercao[item_index] = 0;
        tlb->ultimo_acesso[item_index] = 0;
    }

    tlb->contador_insercoes = 0;
    tlb->contador_acessos = 0;
}

int consultar_tlb(const TLB *tlb, int numero_pagina) {
    for (int item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        if (tlb->itens[item_index].numero_pagina != -1 &&
            tlb->itens[item_index].numero_pagina == numero_pagina) {
            return item_index;
        }
    }

    return -1;
}

void invalidar_tlb(TLB *tlb, int numero_pagina) {
    for (int item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        if (tlb->itens[item_index].numero_pagina == numero_pagina) {
            tlb->itens[item_index].numero_pagina = -1;
            tlb->itens[item_index].numero_frame = -1;
            tlb->ordem_insercao[item_index] = 0;
            tlb->ultimo_acesso[item_index] = 0;
            return;
        }
    }
}

int inserir_tlb(TLB *tlb, int numero_pagina, int numero_frame) {
    if (numero_pagina < 0 || numero_pagina >= NUM_PAGES) {
        return -1;
    }

    if (numero_frame < 0 || numero_frame >= NUM_FRAMES) {
        return -1;
    }

    int item_index = consultar_tlb(tlb, numero_pagina);

    if (item_index != -1) {
        return item_index;
    }

    for (item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        if (tlb->itens[item_index].numero_pagina == -1) {
            tlb->itens[item_index].numero_pagina = numero_pagina;
            tlb->itens[item_index].numero_frame = numero_frame;
            tlb->contador_insercoes++;
            tlb->ordem_insercao[item_index] = tlb->contador_insercoes;
            return item_index;
        }
    }

    return -1;
}

void registrar_acesso_tlb(TLB *tlb, int item_index) {
    if (item_index < 0 || item_index >= TAMANHO_TLB) {
        return;
    }

    if (tlb->itens[item_index].numero_pagina == -1) {
        return;
    }

    tlb->contador_acessos++;
    tlb->ultimo_acesso[item_index] = tlb->contador_acessos;
}

int escolher_item_fifo(const TLB *tlb) {
    int item_escolhido = -1;

    for (int item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        if (tlb->itens[item_index].numero_pagina != -1) {
            if (item_escolhido == -1 ||
                tlb->ordem_insercao[item_index] < tlb->ordem_insercao[item_escolhido]) {
                item_escolhido = item_index;
            }
        }
    }

    return item_escolhido;
}

int escolher_item_lru(const TLB *tlb) {
    int item_escolhido = -1;

    for (int item_index = 0; item_index < TAMANHO_TLB; item_index++) {
        if (tlb->itens[item_index].numero_pagina != -1) {
            if (item_escolhido == -1 ||
                tlb->ultimo_acesso[item_index] < tlb->ultimo_acesso[item_escolhido]) {
                item_escolhido = item_index;
            }
        }
    }

    return item_escolhido;
}

int calcular_numero_pagina(int endereco_logico) {
    return endereco_logico / PAGE_SIZE;
}

int calcular_valor_deslocamento(int endereco_logico) {
    return endereco_logico % PAGE_SIZE;
}

void inicializar_tabela_paginas(int tabela_paginas[]) {
    for (int index = 0; index < NUM_PAGES; index++) {
        tabela_paginas[index] = -1;
    }
}

int consultar_tabela_paginas(const Memoria *memoria, int numero_pagina) {
    if (numero_pagina < 0 || numero_pagina >= NUM_PAGES) {
        return -1;
    }

    return memoria->tabela_paginas[numero_pagina];
}

void linkar_pagina_ao_frame(Memoria *memoria, int numero_da_pagina, int numero_do_frame) {
    if (numero_da_pagina < 0 || numero_da_pagina >= NUM_PAGES) {
        return;
    }

    if (numero_do_frame < 0 || numero_do_frame >= NUM_FRAMES) {
        return;
    }

    memoria->tabela_paginas[numero_da_pagina] = numero_do_frame;
    memoria->vetor_paginas_dos_frames[numero_do_frame] = numero_da_pagina;
}

int obter_proximo_frame(Memoria *memoria) {
    if (memoria->proximo_frame < 0 || memoria->proximo_frame >= NUM_FRAMES) {
        return -1;
    }

    int frame_vazio = memoria->proximo_frame;
    memoria->proximo_frame++;

    return frame_vazio;
}

int selecionar_frame_fifo(Memoria *memoria) {
    int frame_selecionado = memoria->proximo_frame_fifo;
    memoria->proximo_frame_fifo = (memoria->proximo_frame_fifo + 1) % NUM_FRAMES;

    return frame_selecionado;
}

void registrar_acesso_lru(Memoria *memoria, int numero_do_frame) {
    if (numero_do_frame < 0 || numero_do_frame >= NUM_FRAMES) {
        return;
    }

    memoria->contador_acessos++;
    memoria->ultimo_acesso[numero_do_frame] = memoria->contador_acessos;
}

int escolher_frame_lru(const Memoria *memoria) {
    int frame_escolhido = 0;

    for (int frame_index = 1; frame_index < NUM_FRAMES; frame_index++) {
        if (memoria->ultimo_acesso[frame_index] < memoria->ultimo_acesso[frame_escolhido]) {
            frame_escolhido = frame_index;
        }
    }

    return frame_escolhido;
}

void inicializar_gerenciador(Memoria *memoria) {
    inicializar_tabela_paginas(memoria->tabela_paginas);

    for (int frame_index = 0; frame_index < NUM_FRAMES; frame_index++) {
        memoria->vetor_paginas_dos_frames[frame_index] = -1;
        memoria->ultimo_acesso[frame_index] = 0;
        for (int byte_index = 0; byte_index < FRAME_SIZE; byte_index++) {
            memoria->memoria_fisica[frame_index][byte_index] = 0;
        }
    }

    memoria->proximo_frame = 0;
    memoria->proximo_frame_fifo = 0;
    memoria->contador_acessos = 0;
}


int carregar_pagina(Memoria *memoria, FILE *arquivo_binario, int numero_da_pagina, int numero_do_frame) {
    if (memoria == NULL || arquivo_binario == NULL) {
        return -1;
    }

    if (numero_da_pagina < 0 || numero_da_pagina >= NUM_PAGES) {
        return -1;
    }

    if (numero_do_frame < 0 || numero_do_frame >= NUM_FRAMES) {
        return -1;
    }

    long posicao_inicial = (long) numero_da_pagina * PAGE_SIZE;

    if (fseek(arquivo_binario, posicao_inicial, SEEK_SET) != 0) {
        return -1;
    }

    size_t bytes_lidos = fread(memoria->memoria_fisica[numero_do_frame], 1, PAGE_SIZE, arquivo_binario);

    if (bytes_lidos != PAGE_SIZE) {
        return -1;
    }

    return 0;
}

