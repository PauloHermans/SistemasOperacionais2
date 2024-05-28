C++   = c++
CC    = cc
CARGS = 

.PHONY: clean default

default:
	@echo 'Please type a target to run.'

clean:
	rm -vf *.exe memc fda

memc: memory_cost.cpp
	$(C++) $^ -o $@

fda: fda_lib/diff2d.c fda_lib/diff2d.h fda_lib/pgmfiles.c fda_lib/pgmfiles.h fda_lib/pgmtolist.c fda.c
	$(CC) $^ -o $@ -lm

thread: thread.c
	$(CC) -std=c11 -Wall -Wextra -o $@ $^ -lpthread -lrt
