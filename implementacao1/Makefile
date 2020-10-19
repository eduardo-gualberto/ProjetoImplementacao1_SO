PROGRAM = main.c SushiBar/SushiBar.c ../cQueue/src/cQueue.c

FOLDER = ./build/main

COMPILER = gcc

FLAGS = -pthread -Wall

all: 
	$(COMPILER) $(FLAGS) $(PROGRAM) -o $(FOLDER)

clean: 
	rm $(FOLDER)