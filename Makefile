CXX = g++
CXXFLAGS = -Iinclude

LDFLAGS = -Llib -lglfw3 -lglew32 -lgdi32 -lopengl32

SRC = main.cpp
TARGET = main.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	del /Q $(TARGET)
