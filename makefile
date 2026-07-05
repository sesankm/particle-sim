CXX      = clang++
CXXFLAGS = -std=c++20 `pkg-config --cflags sfml-all`
LIBS     = `pkg-config --libs sfml-all`
SRC      = main.cpp
TARGET   = main

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
