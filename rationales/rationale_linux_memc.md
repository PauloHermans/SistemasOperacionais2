# Local do benchmark

Foi calculado ambos os tempos de cada bateria (já incluso no código do mmery cost), quanto a quantidade de *faults*, tanto minors como majors, durante sua execução.

# Minor Fault v. Major Fault

No Linux, uma *minor fault* é uma falta onde, por mais da página não estar mapeada ao processo, se encontra na memória. Um exemplo é o sistema de lazy allocation do mmap do Linux, onde, quando é alocado memória ao processo, invéz de também ser alocado memória física, por padrão é paginado uma página compartilhada nula, onde usa-se semântica de *Copy-On-Write* para alocada dinamicamente, quando o processo tentar escrever nela.

Um *major fault* é uma falha tradicional, onde uma página do processo se encontra em disco invéz de na memória principal.

# Método

Os benchmarks foram realizados em um computador de 8GiB RAM DDR3, processador Core i3-4330 (4) @ 3.5GHz, em um sistema nativo Debian 12 Bookworm, Linux 6.6.13, GCC 12.2.0, glibc 2.36. Em todos os benchmarks foram utilizados dez iterações, salvo o teste mais pesado, que usou somente uma. Houve tanto uma rodada de testes, tanto em ambiente leve quanto em ambiente pesado.

Os testes foram executados cinco vezes cada, com exceção do mais pesado, que foi executado somente uma vez.

# Resultados - Ambiente Leve

O computador foi dedicado ao programa (salvo programas de fundo do sistema operacional).

Foram realizados testes de 32MiB, 128MiB, 512MiB, 1024MiB, 2048MiB, 4096MiB e 8128MiB.

É possivel observar que a taxa de minor faults aumenta com o consumo de memória. Porem disso, as minor faults não aumentam na mesma proporção que a memoria.

Os aumentos de memória, começando dos 32MiB, forma de 4 vezes, 4 vezes, 2 vezes, 2 vezes, 2 vezes e 2 vezes. Enquanto isso, os aumentos de minor faults foram de ~1.08 vezes, ~1.32 vezes, ~1.32 vezes , ~1.49 vezes, ~115 (!!!) vezes assumindo o pior caso (veja dados), e 1.21 vezes.

Fora outliers, em geral, a cada 2x mais memória, 1.35x mais minor faults, com um alto pulo quando o sistema começa a atingir os 100% de memoria física (que foi o caso do 4096MiB).

O unico teste que houve major faults foi o de 8128MiB, já que todo seu conteudo não cabia namemória.

Enquanto ao timings, a alocação e dealocação de memória que não é usada acaba sendo uma operação gratuita quanto ao tempo de execução, devido ao mecanismo de lazy allocation, já mencionado.

Tanto a escrita como a leitura são demoradas, com a leitura sendo mais demorada do que a escrita (acreditamos que o motivo seja pelo fato que os benchs de escrita foram implementados usando o `memset()`, enquanto os de leitura usaram um `for()` comum). Há uma inversão no ultimo teste, onde a leitura se torna mais barata do que a escrita, muito provalvelmente devido ao fato do lazy allocator ter que gastar muito mais tempo em busca de memória num cenário onde a memória física está escassa.

A dealocação de memória que foi utilizada é mais demorada do que a dealocação de memoria não utilizada, já que se tratam de frames físicos reais que precisam ser devolvidos ao sistema.

# Dados - Ambiente Leve

| Memória : Iterações | MINFL | MAJFL | Allocate T | Delete T | Write T | Read T | Delete w/ Write T |
| ------------------- | ----- | ----- | ---------- | -------- | ------- | ------ | ----------------- |
| 0032MiB : 10 | ~11261 | 0 | ~0.1ms | ~0.1ms | ~13.9ms | ~185.7ms | ~1.6ms  |
| 0128MiB : 10 | ~12270 | 0 | ~0.1ms | ~0.1ms | ~57.7ms | ~742.5ms | ~4ms    |
| 0512MiB : 10 | ~16301 | 0 | ~0.1ms | ~0.1ms | ~230.0ms | ~2.975s | ~12.5ms |
| 1024MiB : 10 | ~21677 | 0 | ~0.1ms | ~0.1ms | ~460.6ms | ~5.94s | ~23.8ms |
| 2048MiB : 10 | ~32430 | 0 | ~0.1ms | ~0.1ms | ~919.6ms | ~11.88s | ~48.1ms |
| 4096MiB : 10 | Veja Abaixo | ~0.1ms | ~0.1ms | ~1.9s | ~23.7s | ~53.9ms |
| 8128MiB : 01 | 4531056 | 209 | >=0ms | >=0ms | ~327.7s | ~273.8s | ~259.5ms |

O teste de 4096MiB não apresentou um MINFL constante perante multiplas execuções, com o valor decrescendo à cada execução; segue a tabela:

| Nº | MINFL |
| -- | ----- |
| 1 | 3742843 |
| 2 | 2756101 |
| 3 | 2596669 |
| 4 | 1893533 |
| 5 | 1880759 |

# Resultados - Ambiente Pesado

O computador foi dividindo entre a tarefa em sí, uma segunda tarefa rodando constantemente o teste de 3072MiB (`./memc 3072 1000`) e uma aba do Firefox tocando música.

Foram realizados testes de 32MiB, 128MiB, 512MiB, 1024MiB, 2048MiB e 3072MiB.

Similarmente ao ambiente leve, é possivel ver uma vaga linearidade entre a quantidade de memória e MINFL, no fator de ~1.35x.

O sistema comeã a ficar pesado mais cedo, com o teste de 2048MiB já sofrendo major faults, as minor faults tornando-se não constântes já no teste de 512MiB.

A reprodução de audio do Firefox travou diversas vezes duranto a execução dos testes, com microstutters sendo presentes já no teste de 512MiB, e o playback sendo interrompido no teste de 3072MiB.

Os timings são parecidos com o do ambiente leve, porém com as seguintes diferenças:

1. O sistema, mais agora mais pesado, mostra que a alocação de memória é levemente mais pesada do que desalocação quando se trata de memória não usada;
2. O custo sobe até o teste de 512MiB, após isto, o custo de gerenciamento de memória desce (sistema está dando prioridade ao processo mais pesado?);
3. No último teste, a disparidade entre a escrita e a leitura é **muito** maior, já que a pouco memória fisica disponivel está sendo competida entre vários processos.


# Dados - Ambiente Pesado
| Memória : Iterações | MINFL | MAJFL | Allocate T | Delete T | Write T | Read T | Delete w/ Write T |
| ------------------- | ----- | ----- | ---------- | -------- | ------- | ------ | ----------------- |
| 0032MiB : 10 | ~11261 | ~0.2ms | ~0.1ms | ~31.4ms | ~192.5ms | ~3ms |
| 0128MiB : 10 | ~12270 | 0 | ~0.2ms | ~0.1ms | ~125ms | ~763.4ms | ~7.5ms |
| 0512MiB : 10 | Veja Abaixo | 0 | ~0.2ms | ~0.1ms | ~500ms | ~3.08s | ~45ms |
| 1024MiB : 10 | 21678 | 0 | ~0.2ms | ~0.1ms | ~764ms | ~6.1s | ~38ms |
| 2048MiB : 10 | Veja Abaixo | Veja Abaixo | ~0.1ms | ~0.1ms | ~1.7s | ~12s | ~33ms |
| 3072MiB : 01 | 1158707 | 36 | ~0.1ms | >=0ms | ~78s | ~1.9s | ~102.8ms |

Os teste de 512MiB e 2048MiB não tiveram um MINFL constante; 2048MiB teve MAJFL:

| Nº | MINFL 512 | MINFL 2048 | MAJFL 2048 |
| -- | --------- | ---------- | ---------- |
| 1 | 244209 | 2287722 | 6 |
| 2 | 223767 | 1820418 | 8 |
| 3 | 34201 | 1753478 | 0 |
| 4 | 16302 | 1599667 | 0 |
| 5 | 16303 | 1560322 | 0 |
