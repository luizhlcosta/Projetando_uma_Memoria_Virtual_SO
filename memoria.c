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

int consultar_tabela_paginas(const int tabela_paginas[], int numero_pagina) {
    if (numero_pagina < 0 || numero_pagina >= NUM_PAGES) {
        return -1;
    }

    return tabela_paginas[numero_pagina];
}

void linkar_pagina_ao_frame(int tabela_paginas[], int numero_da_pagina, int numero_do_frame) {
    if (numero_da_pagina < 0 || numero_da_pagina >= NUM_PAGES) {
        return;
    }

    if (numero_do_frame < 0 || numero_do_frame >= NUM_FRAMES) {
        return;
    }

    tabela_paginas[numero_da_pagina] = numero_do_frame;
}


int obter_proximo_frame(int *proximo_frame) {
    if (*proximo_frame < 0 || *proximo_frame >= NUM_FRAMES) {
        return -1;
    }

    int frame_vazio = *proximo_frame;
    (*proximo_frame)++;

    return frame_vazio;
}
