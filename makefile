all:
	rm -rf ./bin
	mkdir ./bin
	clang ./src/qes.c -o ./bin/qes
