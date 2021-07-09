COMPILER = gcc 
COMP_FLAGS = -Wall -fsanitize=address -g -pedantic -std=c99
OUTPUT_FILE = imdb
FRONTF = imdb.c fileManagement.c
FRONTH = fileManagement.h
BACKF = imdbADT.c
BACKH = imdbADT.h
QUERIES = query1.csv query2.csv query3.csv

all: compile


compile: $(FRONTF) $(BACKF) $(FRONTH) $(BACKH)
	$(COMPILER)$(FRONTF) $(BACKF) -o $(OUTPUT_FILE) $(COMP_FLAGS)

clean:
	rm -rf $(OUTPUT_FILE) $(QUERIES)

