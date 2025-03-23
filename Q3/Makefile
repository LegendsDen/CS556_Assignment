# Makefile for Coffee Shop Simulation

# Default Java and Python executables
JAVAC = javac
JAVA = java
PYTHON = python

# Java file and class
JAVA_SRC = CoffeShopSimulation.java
JAVA_CLASS = CoffeShopSimulation

# Python simulation runner
PYTHON_SCRIPT = plot.py

# Default values for simulation parameters
MU = 15
CAPACITY = 2
CUSTOMERS_LEAVE = false

# Detect OS for platform-specific commands
ifeq ($(OS),Windows_NT)
    # Windows commands
    RM = del /Q
    CLASS_FILES = *.class
    CSV_FILE = coffee_shop_simulation_results.csv
else
    # Unix-like commands
    RM = rm -f
    CLASS_FILES = *.class
    CSV_FILE = coffee_shop_simulation_results.csv
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
	$(JAVA) $(JAVA_CLASS) $(MU) $(CAPACITY) $(CUSTOMERS_LEAVE)

# Run full analysis with Python
result: compile
	$(PYTHON) $(PYTHON_SCRIPT)

# Clean compiled files
clean:
	$(RM) $(CLASS_FILES)
	$(RM) $(CSV_FILE)