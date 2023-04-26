.PHONY: build
build:
	gcc main.c -o shell.out

.PHONY: dev
run:
	gcc main.c -o shell.out && ./shell.out
