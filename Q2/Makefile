# Makefile for Coffee Shop Simulation

# Default Java and Python executables
JAVAC = javac
JAVA = java
PYTHON = python

# Java file and class
JAVA_SRC = CallCentreSimulation.java
JAVA_CLASS = CallCentreSimulation

# Python simulation runner
PYTHON_SCRIPT = plot.py

# Default values for simulation parameters
CAPACITY = 5

# Detect OS for platform-specific commands
ifeq ($(OS),Windows_NT)
    # Windows commands
    RM = del /Q
    CLASS_FILES = *.class
    CSV_FILE = .\simulation_plots\*.png
else
    # Unix-like commands
    RM = rm -f
    CLASS_FILES = *.class
    CSV_FILE = ./simulation_plots/*.png
endif

.PHONY: all clean run sim_with_params result

# Default target: compile and run
all: run

# Compile Java file
compile:
	$(JAVAC) $(JAVA_SRC)

# Run simulation with default parameters
run: compile
	$(JAVA) $(JAVA_CLASS)

# Run simulation with provided parameters
# Usage: make sim_with_params MU=18 CAPACITY=3 CUSTOMERS_LEAVE=true
run_in: compile
	$(JAVA) $(JAVA_CLASS) $(CAPACITY)

# Run full analysis with Python
result: compile
	$(PYTHON) $(PYTHON_SCRIPT)

# Clean compiled files
clean:
	$(RM) $(CLASS_FILES)
	$(RM) $(CSV_FILE)