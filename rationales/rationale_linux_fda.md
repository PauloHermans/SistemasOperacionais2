# Local do benchmark

Foi calculado a quantidade de fauls, tanto minor quanto major, durante a execução.

# Minor Fault v. Major Fault

[Minor Fault v. Major Fault](/rationales/rationale_linux_memc.md)

# Método

Os benchmarks foram realizados em um computador de 8GiB RAM DDR3, processador Core i3-4330 (4) @ 3.5GHz, em um sistema nativo Debian 12 Bookworm, Linux 6.6.13, GCC 12.2.0, glibc 2.36.

Os testes foram executados cinco vezes cada.

# Resultados & Dados

Em geral, o tamanho da imagem e a quantidade de iterações ambas aumentam a quantidade de minor faults. O algoritmo do fda em sí parece resiliente a pressão de memória e tal não aumentou muito a quantidade de major faults. O Lena 128, em destaque, parece ser mais vulnerável à major faults.

Enquanto aumentar a quantidade de iterações automenta a quantidade de minor faults, a proporção em sí de `minfl / it` diminui.

| Teste      | MINFL     | MAJFL | `it/minfl` |
| ---------- | --------- | ----- | ---------- |
| Lena 16    | ~757      |  0    | 48.9375    |
| Lena 128   | ~4349.4   | ~0.2  | 34.3437    |
| Lena 1024  | ~33021.2  |  0    | 32.2472    |
| Tux  16    | ~3292     |  0    | 205.750    |
| Tux  128   | ~23338    |  0    | 182.328    |
| Tux  1024  | ~183725   |  0    | 179.418    |
| Car  16    | ~279292.6 |  0    | 17455.7    |
| Lena H 128 | ~4352.8   | ~0.6  | 34.0006    |
| Car H  16  |  283061   |  1    | 17691.3    |
