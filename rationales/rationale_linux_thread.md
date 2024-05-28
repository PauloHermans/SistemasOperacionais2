# Local do Benchmark

Foi calculado a quantidade de faltas (min & maj) durante a execução de um programa que, em uma thread, escreve valores aleatorios no buffer, e outra lê.

A maior diferença entre este teste e o MemoryCost é o fato de a memoria ser consumida de forma lenta (diferente do memory cost,
 que consome a memoria de forma quase instantânea).

# Minor Fault v. Major Fault

[Minor Fault v. Major Fault](/rationales/rationale_linux_memc.md)

# Método

Os benchmarks foram realizados em um computador de 8GiB RAM DDR3, processador Core i3-4330 (4) @ 3.5GHz, em um sistema nativo Debian 12 Bookworm, Linux 6.6.13, GCC 12.2.0, glibc 2.36.

Os teste de 512MiB foi executado cinco vezes, o de 4GiB e 8GiB uma vez só.

Para o ambiente pesado, foi aberto uma aba no Firefox tocando música, e foi usado o MemoryCost de fundo consumindo 4GiB de RAM para por pressão no sistema.

# Resultados & Dados

Em geral, as minor faults foram iguais as de todos os outros testes.

Para as major faults, o ambiente pesado causou mais faltas em ambos os testes, com destaque ao teste de 4GiB, com 371 faltas.

| Tamanho : It  | MINFL   | MAJFL |
| ------------- | ------- | ----- |
| 512  : 4      | ~898.8  | 0     |
| 512  : 4 H    | ~899.8  | ~0.2  |
| 8128 : 1      | 3075064 | 33    |
| 4096 : 1 H    | 1734613 | 371   |
