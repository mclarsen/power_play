all: prog

prog: main.c estimator.h
	g++ -o $@ $<

clean:
	rm -f prog
