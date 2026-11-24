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
} Memoria;

int calcular_numero_pagina(int endereco_logico);
int calcular_valor_deslocamento(int endereco_logico);

void inicializar_tabela_paginas(int tabela_paginas[]);
void inicializar_gerenciador(Memoria *memoria);

int consultar_tabela_paginas(const Memoria *memoria, int numero_pagina);
void linkar_pagina_ao_frame(Memoria *memoria, int numero_da_pagina, int numero_do_frame);
int obter_proximo_frame(Memoria *memoria);

#endif
