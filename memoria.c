#include "memoria.h"

int calcular_numero_pagina(int endereco_logico) {
    return endereco_logico / 256;
}

int calcular_valor_deslocamento(int endereco_logico) {
    return endereco_logico % 256;
}
