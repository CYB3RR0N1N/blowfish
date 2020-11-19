DIR = build

.PHONY: all clean dir

all: dir main

main: main.cpp
	g++ main.cpp -o $(DIR)/main -g

clean:
	rm -rf ./$(DIR)/*

dir:
	mkdir -p $(DIR)