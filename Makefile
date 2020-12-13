DIR = build
ARGS = -g --std=c++17

.PHONY: all clean dir

all: dir main

#main.o: main.cpp
	#g++ -c main.cpp -o $(DIR)/main.o $(ARGS)

#blowfish.o: blowfish.cpp
	#g++ -c blowfish.cpp -o $(DIR)/blowfish.o $(ARGS)

%.o : %.cpp;
	g++ -c $^ -o $(DIR)/$@ $(ARGS)

main: blowfish.o simple_parser.o main.o
	g++ -o $(DIR)/main $(DIR)/main.o $(DIR)/blowfish.o $(DIR)/simple_parser.o $(ARGS)

clean:
	rm -rf ./$(DIR)/*

dir:
	mkdir -p $(DIR)