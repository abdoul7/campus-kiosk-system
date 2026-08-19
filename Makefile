CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp \
      src/OrderQueue.cpp \
      src/StallAssignment.cpp \
      src/SessionHistory.cpp \
      src/MenuSearch.cpp \
      src/FoodCourtLayout.cpp

TARGET = kiosk_system

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
