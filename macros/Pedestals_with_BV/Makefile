CXX = g++
CXXFLAGS = -std=c++17 -g $(shell root-config --cflags) 
LDFLAGS = $(shell root-config --libs) -lstdc++fs
SRCS = TOT_analysis.cpp ../../headers/test_beam.C
TARGET = TOT_analysis

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET)

