CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET   = sms
SRC      = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) students.dat

.PHONY: all run clean
