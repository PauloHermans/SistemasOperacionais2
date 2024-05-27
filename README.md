# SistemasOperacionais2

Trabalho apresentado como requisito avaliativo para a disciplina de Sistemas Operacionais

Trabalho M2 - Análise de Page Fault. Alunos: Larissa de Souza, Paulo Hermans e Vinícius Schütz Piva . Data: 28/05/2024.

Link para acesso ao relatório: https://docs.google.com/document/d/1rm3XDncF9BXFDeVu1JYQ2QIrlLKXhIuP/edit?usp=sharing&ouid=107006664367478057567&rtpof=true&sd=true

# Instruções de compilação

### Linux

Para os códigos da plataforma Linux, clone este repo em uma maquina com tal sistema operacional, que tenha GCC 11+, GNU Make e rode:

```
$ make memc
$ make fda
```

Para rodar os programas, rode:

```
$ ./memc <MEMÓRIA [em MiB]> <ITERAÇÕES>
$ ./fda  <IN> <OUT> <ITERAÇÕES>
...
```

Por exemplo, para rodar o `fda` com as imagens do repositório:

```
$ make fda
$ ./fda 01_lena_noise.pgm lena_out.pgm 1024
...
$ ./fda 02_tux.pgm tux_out.pgm 128
...
$ ./fda <(zcat 03_car.gpm.gz) car_out.pgm 16
...
```

Ou rodar o `memc`:

```
$ make memc
$ ./memc 2048 10
...
```
