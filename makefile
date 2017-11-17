all:
	rm -rf ./bin
	mkdir ./bin
	clang ./qes.c -o ./bin/qes
