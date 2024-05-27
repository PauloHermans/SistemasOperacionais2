C++    = c++
CARGS = 

.PHONY: clean default

default:
	@echo 'Please type a target to run.'

clean:
	rm -vf *.exe memc

memc: memory_cost.cpp
	$(C++) $^ -o $@
