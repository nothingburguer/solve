CXX      := g++
CXXFLAGS := -Wall -Wextra -O2 -pthread
TARGET   := solve
SRC      := src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS)

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS := -Wall -Wextra -g -O0 -pthread
debug: clean all

sanitize: CXXFLAGS := -fsanitize=address,undefined -g -O1 -pthread
sanitize: clean all

clean:
	rm -f $(TARGET)

re: clean all

.PHONY: all run debug sanitize clean re
