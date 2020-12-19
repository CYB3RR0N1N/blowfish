BUILD_DIR = build

ARGS = -g --std=c++17

.PHONY: all clean dir

all: dir main

%.o : source/%.cpp;
	g++ -c $^ -o $(BUILD_DIR)/$@ $(ARGS)

main: blowfish.o simple_parser.o main.o
	g++ -o $(BUILD_DIR)/main $(BUILD_DIR)/main.o $(BUILD_DIR)/blowfish.o $(BUILD_DIR)/simple_parser.o $(ARGS)

clean:
	rm -rf ./$(BUILD_DIR)/*

dir:
	mkdir -p $(BUILD_DIR)