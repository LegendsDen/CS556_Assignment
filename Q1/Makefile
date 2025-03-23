# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall

# Executable name
TARGET = bank_simulation

# Source files
SRC = bank_simulation.cpp
OBJ = $(SRC:.cpp=.o)

# Python script for visualization
PLOT_SCRIPT = plot_results.py

# Default rule
all: $(TARGET)

# Compile C++ source
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Compile object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run simulation and store results
run: $(TARGET)
	./$(TARGET)

# Run Python script for plotting (if required)
plot: run
	python3 $(PLOT_SCRIPT)

# Clean build files
clean:
	rm -f $(TARGET) $(OBJ) output.txt queue_data.txt waiting_time.txt

# Clean everything and recompile
rebuild: clean all
