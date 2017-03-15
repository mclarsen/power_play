all: prog

prog: main.c estimator.h
	clang++ -o $@ $<

clean:
	rm -f prog
