#include "memoria.h"

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
}

int obter_proximo_frame(Memoria *memoria) {
    if (memoria->proximo_frame < 0 || memoria->proximo_frame >= NUM_FRAMES) {
        return -1;
    }

    int frame_vazio = memoria->proximo_frame;
    memoria->proximo_frame++;

    return frame_vazio;
}

void inicializar_gerenciador(Memoria *memoria) {
    inicializar_tabela_paginas(memoria->tabela_paginas);

    for (int frame_index = 0; frame_index < NUM_FRAMES; frame_index++) {
        for (int byte_index = 0; byte_index < FRAME_SIZE; byte_index++) {
            memoria->memoria_fisica[frame_index][byte_index] = 0;
        }
    }

    memoria->proximo_frame = 0;
}
