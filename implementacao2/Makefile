PROGRAM = teste.c

FOLDER = ./build/main

COMPILER = gcc

FLAGS = -pthread -Wall

all: $(FOLDER)

$(FOLDER): 
	$(COMPILER) $(FLAGS) $(PROGRAM) -o $(FOLDER)

clean: 
	rm $(FOLDER)