#ifndef MEMORIA_H
#define MEMORIA_H

#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define NUM_PAGES 256
#define NUM_FRAMES 128

int calcular_numero_pagina(int endereco_logico);
int calcular_valor_deslocamento(int endereco_logico);
void inicializar_tabela_paginas(int tabela_paginas[]);
int consultar_tabela_paginas(const int tabela_paginas[], int numero_paginas);
void linkar_pagina_ao_frame(int tabela_paginas[], int numero_da_pagina, int numero_do_frame);
#endif
