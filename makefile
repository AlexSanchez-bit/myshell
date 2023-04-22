.PHONY: build
build:
	gcc main.c -o shell.out

.PHONY: dev
run:
	./shell.out
