#include "memoria.h"

int calcular_numero_pagina(int endereco_logico) {
    return endereco_logico / 256;
}

int calcular_valor_deslocamento(int endereco_logico) {
    return endereco_logico % 256;
}

void inicializar_tabela_paginas(int tabela_paginas[]) {
    for (int index = 0; index < 256; index++) {
        tabela_paginas[index] = -1;
    }
}

int consultar_tabela_paginas(const int tabela_paginas[], int numero_pagina) {
    if (numero_pagina < 0 || numero_pagina >= 256) {
        return -1;
    }

    return tabela_paginas[numero_pagina];
}

