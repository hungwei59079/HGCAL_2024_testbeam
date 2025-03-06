CXX = g++
CXXFLAGS = -std=c++17 -g $(shell root-config --cflags) 
LDFLAGS = $(shell root-config --libs) -lstdc++fs
SRCS = Pedestals_with_BV.cpp ../../headers/test_beam.C
TARGET = Pedestals_with_BV

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET)

