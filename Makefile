C_COMPILER = gcc
FLAGS = #-Wall -Wextra



main: src/main.c src/scheduler.c src/input.c src/process.c src/generator.c src/tests.c src/simulator.c
	$(C_COMPILER) $(FLAGS) -o $@ $^

run: main
	./main