# Makefile for Branch Prediction Experiment

CXX = g++
CXXFLAGS = -std=c++20 -O0 -Wall
TARGET = main
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o *.csv *.png
