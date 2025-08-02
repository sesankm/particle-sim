INC=-I/opt/homebrew/Cellar/sfml/3.0.1/include
LIBS=-L/opt/homebrew/Cellar/sfml/3.0.1/lib -lsfml-graphics -lsfml-window -lsfml-system

all:
	g++ -std=c++17 $(INC) $(LIBS) -o main main.cpp 
