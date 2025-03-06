# Compiler and flags
CXX = g++
ROOTCFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --libs`
CXXFLAGS = -Wall -std=c++11 $(ROOTCFLAGS)

# Source files
SRCS1 = GetLongrunPedestals.cpp headers/data_of_test_beam.cpp
SRCS2 = PlotPedestals.cpp

# Object files
OBJS1 = $(SRCS1:.cpp=.o)
OBJS2 = $(SRCS2:.cpp=.o)

# Executable names
TARGET1 = GetLongrunPedestals
TARGET2 = PlotPedestals

# Default rule
all: $(TARGET1) $(TARGET2)

# Linking for TARGET1
$(TARGET1): $(OBJS1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(OBJS1) $(ROOTLIBS)

# Linking for TARGET2
$(TARGET2): $(OBJS2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(OBJS2) $(ROOTLIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS1) $(OBJS2) $(TARGET1) $(TARGET2)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild
