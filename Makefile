DIR = build

.PHONY: all clean dir

all: dir main

main.o: main.cpp
	g++ -c main.cpp -o $(DIR)/main.o -g --std=c++17

blowfish.o: blowfish.cpp
	g++ -c blowfish.cpp -o $(DIR)/blowfish.o -g --std=c++17

main: main.o blowfish.o
	g++ -o $(DIR)/main -g $(DIR)/main.o $(DIR)/blowfish.o --std=c++17

clean:
	rm -rf ./$(DIR)/*

dir:
	mkdir -p $(DIR)