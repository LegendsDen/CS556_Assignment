# 📊 CS556 Queue Simulation Projects

This repository contains simulations for various real-world queuing systems using **event-driven models** based on the **M/M/c**, **M/M/1/c**, and **M/M/1** queuing theories.

---

## 🏦 Bank Queue Simulation (M/M/c Model)

### 📋 Overview

Simulates an 8-hour workday in a bank where:
- Customers arrive following a **Poisson process** (λ = 40/hour).
- Service time is **exponentially distributed** (μ = 15/hour).
- Number of tellers **varies over time**.

#### 📆 Teller Allocation:
| Time Period        | No. of Tellers |
|--------------------|----------------|
| 9:00 AM – 11:00 AM | 2              |
| 11:00 AM – 2:00 PM | 4 _(Peak)_     |
| 2:00 PM – 5:00 PM  | 3              |

---

### ✅ Features Implemented

- Event-driven simulation of **arrivals** and **departures**.
- **Dynamic teller allocation** over different time periods.
- Full **queue management** for customers.
- Calculation of:
  - Average waiting time
  - Average system time
  - Teller utilization
  - Average queue length
  - Probability and duration of all tellers being busy
- File-based logging of queue and wait time
- Python-based **visualization support**

---

### 🧱 Code Structure

#### 🧾 Data Structures
- `Customer`: Stores arrival, start, and departure times.
- `Teller`: Availability and service tracking.
- `PeriodMetrics`: Performance metrics per time period.

#### 🧠 Core Components
- `generateInterarrivalTime()`
- `generateServiceTime()`
- `adjustTellers()`
- `processArrival()`
- `processAvailableTellers()`
- `updateTellerMetrics()`
- `runSimulation()`
- `displayResults()`

---

### ▶️ How to Run

#### ✅ With Makefile:
```bash
make            # Compile
make run        # Run the simulation
make plot       # Plot results using Python
make clean      # Clean files

g++ -o bank_simulation bank_simulation.cpp
./bank_simulation

python plot_results.py
```
---

## 📞 Call Center Queue Simulation (M/M/1/c Model)

### 📋 Overview

Simulates a call center operating with:
- A **single agent** serving calls.
- Calls arrive via **Poisson process** (λ = 20/hour).
- Service times are **exponentially distributed** (μ = 24/hour).
- A **limited waiting queue** (capacity `c`).
- **New arrivals are rejected** when the system is full.

---

### ✅ Features Implemented

- Event-driven simulation using a **priority queue**.
- Queue rejection handling when the system is full.
- Performance metrics calculation:
  - Average waiting time
  - Average time in the system
  - Agent utilization
  - Average queue length
  - Probability of full queue
  - Call rejection probability
- Capacity variation (`c = 3` to `7`)
- Python-based **visualization** support
- Comparison with **theoretical M/M/1/c formulas**

---

### 🧱 Code Structure

#### 🧾 Classes
- `Event`: Represents customer arrival or departure.
- `EventType`: Enum with values `ARRIVAL` and `DEPARTURE`.
- `PriorityQueue<Event>`: Manages simulation events ordered by time.

#### ⚙️ Parameters
- `lambda = 1/180.0` (20 calls/hour)
- `mu = 1/150.0` (24 calls/hour)
- `capacity = 5` (default, configurable)
- `simulationPeriod = 5 days × 8 hours/day`

#### 🧠 Core Functions
- `generateExponential()`: Generates exponential random variable.
- `handleArrival()`: Processes arrivals, queues or rejects if full.
- `handleDeparture()`: Frees up the agent.
- `runSimulation()`: Main simulation loop.
- `displayResults()`: Computes and prints all metrics.

---

### ▶️ How to Run

#### ✅ With Makefile
```bash
make run                     # Compile and run simulation
make run_in CAPACITY=2       # Run with custom queue capacity
make result                  # Plot the results using Python
make clean                   # Remove compiled files

javac CallCenterSimulation.java
java CallCenterSimulation
# or with capacity
java CallCenterSimulation 2

python plot_results.py

```
---
## ☕ Coffee Shop Queue Simulation (M/M/1 with Reneging)

### 📋 Overview

Simulates a coffee shop where:
- One **barista** serves customers in **First-Come, First-Served (FCFS)** order.
- Customers arrive via **Poisson process** (λ = 10/hour).
- Service times are **exponentially distributed** (μ = 15/hour).
- **Reneging**: Customers leave if waiting time exceeds **5 minutes**.
- Simulation ends after **500 customers** are served.

---

### ✅ Features Implemented

- Event-driven simulation using **priority queue**.
- First-Come, First-Served **queue model**.
- Handles customer abandonment (**reneging**).
- Performance metrics:
  - Average waiting time and total time in system
  - Barista utilization and idle time
  - Maximum queue length
  - Probability of customer abandonment
  - **Revenue loss** due to long wait times
- Support for:
  - Varying **service rate**
  - Adding a **second barista** (M/M/c)
  - **Peak hour detection** based on max queue
- Python-based **visualizations**

---

### 🧱 Code Structure

#### 🧾 Classes
- `Event`: Represents arrival, departure, or abandonment.
- `EventType`: Enum for `ARRIVAL`, `DEPARTURE`, `CUSTOMER_LEAVING`.
- `PriorityQueue<Event>`: Manages simulation events by time.
- `Queue<Integer>`: Represents the customer queue (LinkedList).

#### ⚙️ Parameters
- `lambda = 1/6.0` (10 customers/hour)
- `mu = 1/4.0` (15 customers/hour)
- `maxWaitingTime = 5 minutes`
- `lossDueToLeaving = $5`
- `simulationCapacity = 500`

#### 🧠 Core Functions
- `generateExponential()`: Random interarrival/service time.
- `handleArrival(int id)`: Handles new customer arrivals.
- `handleDeparture(int id)`: Handles customer departures.
- `handleCustomerLeaving(int id)`: Handles abandonment.
- `removeCustomer(int id)`: Cleans queue & events.
- `runSimulation()`: Main simulation loop.
- `displayResults()`: Computes and prints metrics.

---

### ▶️ How to Run

#### ✅ With Makefile
```bash
make run                                # Default simulation
make run_in MU=12 SERVERS=2 CUSTOMER_LEAVE=true  # Custom: slower barista + 2 servers + reneging
make result                             # Plot results using Python
make clean                              # Clean compiled files

javac CoffeShopSimulation.java
java CoffeShopSimulation                   # Default
java CoffeShopSimulation 12 2 true         # With custom parameters

python plot_results.py                     # Visualize results

```
---

## 👨‍💻 Authors

- **Sushant Kumar**  
- **Tanmay Mittal**  

**Course**: CS348 – Performance Modelling of Communication and Computer Systems  
**Institution**: Indian Institute of Technology Guwahati  
**Year**: 2025
