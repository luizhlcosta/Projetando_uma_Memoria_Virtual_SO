# Gerenciamento de Memória Virtual

Projeto desenvolvido em linguagem C para a disciplina de Infraestrutura de Software, com o objetivo de simular o funcionamento de um gerenciador de memória virtual.

O programa recebe endereços lógicos de um arquivo, realiza a tradução para endereços físicos e consulta os valores armazenados em uma memória simulada.

## 1. Funcionamento do programa

O gerenciador utiliza:

- **256 páginas virtuais**, com 256 bytes cada.
- **128 frames de memória física**, com 256 bytes cada.
- **Tabela de páginas** para relacionar páginas virtuais aos frames físicos.
- **TLB com 16 entradas** para armazenar traduções recentes.
- **Pthreads** para realizar a consulta paralela das entradas da TLB.

Quando um endereço lógico é recebido, o programa calcula o número da página e o deslocamento.

Primeiro, consulta a TLB. Caso a página não seja encontrada, consulta a tabela de páginas.

Se a página não estiver carregada na memória física, ocorre um *page fault*. Nesse caso, o programa busca os dados correspondentes no arquivo `BACKING_STORE.bin`.

Quando a memória física está cheia, uma página precisa ser substituída para permitir o carregamento da nova página.

## 2. Algoritmos de substituição

O programa permite escolher dois algoritmos, tanto para a memória física quanto para a TLB:

- **FIFO (First In, First Out):** substitui o elemento que foi inserido há mais tempo.
- **LRU (Least Recently Used):** substitui o elemento que não é utilizado há mais tempo.

Os algoritmos são escolhidos por argumentos na execução do programa.

## 3. Organização dos arquivos

| Arquivo | Descrição |
|---|---|
| `vm.c` | Função principal, leitura dos endereços, tradução e estatísticas |
| `memoria.c` | Implementação da memória, da TLB e dos algoritmos de substituição |
| `memoria.h` | Estruturas, constantes e protótipos das funções |
| `Makefile` | Compilação do programa |
| `README.md` | Documentação do projeto |
| `addresses.txt` | Arquivo de entrada com os endereços lógicos |
| `BACKING_STORE.bin` | Arquivo binário utilizado para carregar as páginas |
| `correct.txt` | Saída de referência para comparação dos resultados |
| `evidencias.log` | Registro das compilações e dos testes realizados |
| `lhlc.pdf` | Relatório da atividade |
| `testes/` | Arquivos e programas auxiliares utilizados durante o desenvolvimento |

Os arquivos principais, os arquivos de referência e o `evidencias.log` ficam na raiz do repositório.

A pasta `testes/` contém arquivos auxiliares utilizados durante o desenvolvimento, como testes individuais de funções e saídas temporárias.

Os arquivos fornecidos pelo professor são utilizados para a execução e a conferência dos resultados, mas não precisam ser incluídos na entrega.

## 4. Compilação

O programa utiliza GCC e a biblioteca Pthreads.

Para compilar, execute:

```bash
make
```

O comando gera o executável `vm`.

Para remover os arquivos gerados pela compilação:

```bash
make clean
```

Os comandos devem ser executados a partir da raiz do repositório.

## 5. Execução

A execução utiliza três argumentos:

```bash
./vm <arquivo> <algoritmo_paginas> <algoritmo_tlb>
```

O primeiro argumento indica o caminho do arquivo com os endereços lógicos.

O segundo define o algoritmo de substituição das páginas da memória física.

O terceiro define o algoritmo de substituição das entradas da TLB.

Exemplos:

```bash
./vm addresses.txt fifo fifo
./vm addresses.txt fifo lru
./vm addresses.txt lru fifo
./vm addresses.txt lru lru
```

## 6. Saída do programa

Para cada endereço traduzido, o programa apresenta:

- Endereço lógico.
- Posição utilizada na TLB.
- Endereço físico.
- Valor armazenado no endereço.

Ao final, apresenta as estatísticas:

- Número de endereços traduzidos.
- Quantidade de page faults.
- Taxa de page faults.
- Quantidade de TLB hits.
- Taxa de TLB hits.

As taxas são apresentadas como valores entre 0 e 1.

## 7. Testes realizados

Foram realizados testes de compilação e execução com as quatro combinações de FIFO e LRU.

Utilizando os 1.000 endereços do arquivo `addresses.txt`, foram obtidos:

| Páginas | TLB | Page faults | TLB hits |
|---|---|---:|---:|
| FIFO | FIFO | 538 | 54 |
| FIFO | LRU | 538 | 55 |
| LRU | FIFO | 539 | 54 |
| LRU | LRU | 539 | 55 |

Também foram realizados testes de tradução de endereços, conferência dos valores retornados e verificação do tratamento de entradas inválidas.

Na configuração LRU para páginas e FIFO para a TLB, a saída foi comparada com o arquivo `correct.txt` por meio do comando:

```bash
diff -u correct.txt <(./vm addresses.txt lru fifo)
```

A comparação direta pode apresentar diferenças nas quebras de linha, pois o arquivo de referência utiliza o formato do Windows e o programa executado no Linux utiliza o formato Unix.

Após normalizar as quebras de linha, os resultados das traduções e das estatísticas coincidiram com a referência.

Os registros dos testes realizados durante o desenvolvimento estão documentados em `evidencias.log`.

Para registrar novas evidências, inicie uma sessão a partir da raiz:

```bash
script -a evidencias.log
```

Após executar os comandos desejados, utilize `exit` para encerrar o registro.

## 8. Considerações finais

O projeto permitiu aplicar conceitos de memória virtual, tabelas de páginas, page faults, TLB, algoritmos de substituição e programação com threads.

A implementação foi desenvolvida utilizando estruturas e funções simples da linguagem C, buscando manter o código organizado e facilitar a compreensão do funcionamento de cada parte do gerenciador.