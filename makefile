CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

SRC = src/main.cpp src/Database.cpp src/Table.cpp src/Parser.cpp
OUT = DadeNegar

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
