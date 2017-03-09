all: prog

prog: main.c
	clang++ -o $@ $<

clean:
	rm -f prog
