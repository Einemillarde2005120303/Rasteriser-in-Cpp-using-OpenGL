BIN = bin/main.exe
SRC = src/main.cpp
INC = -Iinclude
LIB = -Llib
LIBS = -lglfw3 -lopengl32 -lUser32 -lGdi32 -lglad
CXX = g++
CXXFLAGS = -std=c++23

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN) $(INC) $(LIB) $(LIBS)

clean:
	rm -f $(BIN)
