DIR = build
ARGS = -g --std=c++17

.PHONY: all clean dir

all: dir main

%.o : %.cpp;
	g++ -c $^ -o $(DIR)/$@ $(ARGS)

main: blowfish.o simple_parser.o main.o
	g++ -o $(DIR)/main $(DIR)/main.o $(DIR)/blowfish.o $(DIR)/simple_parser.o $(ARGS)

clean:
	rm -rf ./$(DIR)/*

dir:
	mkdir -p $(DIR)