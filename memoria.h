#ifndef MEMORIA_H
#define MEMORIA_H

int calcular_numero_pagina(int endereco_logico);
int calcular_valor_deslocamento(int endereco_logico);
void inicializar_tabela_paginas(int tabela_paginas[]);
int consultar_tabela_paginas(const int tabela_paginas[], int numero_paginas);

#endif
