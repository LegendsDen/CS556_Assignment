Bank Queue Simulation (M/M/c Model) 

Overview

This project implements a bank queue simulation based on the M/M/c queuing model, where:

    Customers arrive according to a Poisson process (random arrivals).

    Service times follow an exponential distribution (random service durations).

    Multiple tellers (servers) are available, but their count varies across time periods.

The simulation runs for an 8-hour workday (9:00 AM - 5:00 PM) and evaluates key performance metrics, including average waiting time, queue length, and teller utilization.
Problem Statement

A bank operates with a variable number of tellers throughout the day to handle customer transactions. The parameters are:

    Arrival rate: 40 customers per hour (Poisson distributed).

    Service rate: Each teller serves customers at an average of 4 minutes per customer (exponentially distributed).

    Teller allocation:

        2 tellers from 9:00 AM to 11:00 AM

        4 tellers from 11:00 AM to 2:00 PM (Peak Hours)

        3 tellers from 2:00 PM to 5:00 PM

The simulation must track and compute performance metrics for different time periods and the entire day.
Features Implemented

✅ Event-driven simulation of customer arrivals and service completion.
✅ Variable number of tellers adapting to time periods.
✅ Queue management for customers when all tellers are busy.
✅ Performance metrics calculation, including:

    Average waiting time in the queue.

    Average time spent in the system (waiting + service time).

    Teller utilization rate (percentage of time busy).

    Average number of customers in the queue.

    Probability of all tellers being busy.

    Total time duration when all tellers were continuously busy.
    ✅ File-based logging for queue length and waiting time tracking.
    ✅ Python visualization support for queue length and waiting times.

Code Structure
1. Data Structures

    Customer – Stores customer arrival time, service start time, departure time.

    Teller – Represents a bank teller with availability status.

    PeriodMetrics – Stores performance data for each time period.

2. Simulation Components

    generateInterarrivalTime() – Generates random interarrival times using an exponential distribution.

    generateServiceTime() – Generates random service times using an exponential distribution.

    adjustTellers() – Dynamically adjusts the number of tellers based on the current time period.

    processArrival() – Handles new customer arrivals.

    processAvailableTellers() – Assigns waiting customers to free tellers.

    updateTellerMetrics() – Updates busy, idle, and all-busy time durations.

    runSimulation() – The main event-driven loop that processes arrivals, departures, and teller updates.

    displayResults() – Computes and prints the final statistics.

How to Run the Simulation
    1. Compile and Run (C++)
                 g++ -o bank_simulation bank_simulation.cpp 
                ./bank_simulation

    2.To plot results (Python) -- place the generated "queue_data.txt" and "waiting_time.txt" in same directory
                python plot_results.py

    Or   using Makefile
    1. To Compile  -->make 
    2. To run  and stimulate --> make run 
    3. TO see plots --> make plot
    4. To clean  --> make clean 








Call Center Queue Simulation (M/M/1/c Model)

Overview

This project implements a call center simulation based on the M/M/1/c queuing model, where:

    Customers (calls) arrive following a Poisson distribution.

    Service times are exponentially distributed.

    A single agent handles calls, and there is a limited waiting queue (capacity c).

    If the system is full (agent + queue), new arrivals are rejected.

The simulation runs for 5 days, 8 hours per day and evaluates key performance metrics, including average waiting time, agent utilization, and call rejection probability.
Problem Statement

A call center operates with a single agent and a limited waiting capacity.
The simulation must model the M/M/1/c queue using these parameters:

    Arrival rate (λ): 20 calls per hour (Poisson distributed)

    Service rate (μ): 24 calls per hour (Exponential distribution)

    System capacity (c): 5 (including the customer being served)

Goals

    Simulate the call center over a workweek (5 days, 8 hours/day).

    Compute key performance metrics:

        Average waiting time per customer.

        Average time spent in the system (waiting + service).

        Agent utilization rate.

        Average number of customers in the queue.

        Probability of system being full (queue capacity reached).

        Probability of customer rejection (customer arrival when queue is full).

    Vary the system capacity (3 to 7) to analyze its impact.

    Visualize trends of waiting time, utilization, and rejection probability.

    Determine the optimal system capacity for balancing waiting time and rejection rate.

    Compare simulation results with analytical solutions for M/M/1/c queues.

Features Implemented

✅ Event-driven simulation using a priority queue for arrivals and departures.
✅ Limited queue capacity (customers are rejected if full).
✅ Performance metrics calculation, including:

    Average waiting time in the queue.

    Average time in the system (waiting + service time).

    Utilization rate of the agent.

    Average queue length.

    Probability of system full (queue at max capacity).

    Customer rejection probability.
    ✅ Capacity variation from 3 to 7 to analyze system performance.
    ✅ Python visualization support for queue behavior and rejection rates.
    ✅ Comparison with theoretical M/M/1/c formulas.

Code Structure
1. Classes & Data Structures

    Event – Represents a customer arrival or departure event, ordered by time.

    EventType – Enum for ARRIVAL and DEPARTURE events.

    PriorityQueue<Event> – Manages simulation events sorted by time.

2. Simulation Parameters

    lambda (arrival rate) = 1/180.0 (20 calls/hour).

    mu (service rate) = 1/150.0 (24 calls/hour).

    capacity (queue size) = 5 (default), can be varied from 3 to 7.

    simulationPeriod = 5 days × 8 hours/day.

3. Core Functions

    generateExponential(lambda) – Generates a random exponential arrival/service time.

    handleArrival() – Processes customer arrivals, queues them if space is available, or rejects them.

    handleDeparture() – Processes customer departures, freeing up the system.

    runSimulation() – Runs the event-driven simulation for a full workweek.

    displayResults() – Computes and prints performance metrics.


How to Run the Simulation 

Using MakeFile:

make run --> To compile and run the simulation
make run_in CAPACITY=2 --> To run the simulation with a queue capacity of 2
make result--> To plot the results using Python
make clean --> To clean the compiled files

Without MakeFile:
javac CallCenterSimulation.java
java CallCenterSimulation
or 
java CallCenterSimulation 2

python plot_results.py -> plots results for varying capacity


Coffee Shop Queue Simulation (M/M/1 Model)

Overview

This project simulates a small coffee shop with one barista serving customers in a First-Come, First-Served (FCFS) queue. Customers arrive randomly based on a Poisson process, and the barista serves them at an exponentially distributed rate.
The simulation runs until 500 customers have been served, and key performance metrics are calculated.
Problem Statement

A coffee shop operates with one barista and follows an M/M/1 queue model:

    Customers arrive at an average rate of λ = 10 customers per hour (Poisson distribution).

    Service time follows an exponential distribution with μ = 15 customers per hour.

    Customers must wait in line if the barista is busy.

    If the waiting time exceeds a certain threshold (5 minutes), some customers leave (reneging).

    The coffee shop operates until 500 customers have been processed.

Goals

    Simulate the coffee shop for 500 customers.

    Compute key performance metrics:

        Average waiting time before ordering.

        Average time spent in the shop (waiting + service time).

        Utilization factor of the barista.

        Idle time of the barista.

        Maximum queue length.

        Probability of an empty queue.

        Customer abandonment probability.

        Revenue loss due to long wait times.

    Analyze system behavior during peak hours.

    Experiment with different service rates (faster/slower barista).

    Simulate hiring a second barista and compare results.

    Determine revenue impact when customers leave due to long wait times.

Features Implemented

✅ Event-driven simulation using a priority queue for arrivals and departures.
✅ First-Come, First-Served (FCFS) queue management.
✅ Support for customer abandonment (reneging) when waiting time exceeds 5 minutes.
✅ Performance metrics calculation, including:

    Average waiting time before ordering.

    Total time spent in the coffee shop.

    Utilization of the barista.

    Barista idle time.

    Probability of an empty queue.

    Customer loss and revenue impact due to long waits.
    ✅ Dynamic parameter configuration (e.g., change service rate, add a second barista).
    ✅ Peak hour detection (time of max queue length).
    ✅ Multiple barista simulation (M/M/c model).

Code Structure
1. Classes & Data Structures

    Event – Represents a customer arrival, departure, or abandonment event.

    EventType – Enum for ARRIVAL, DEPARTURE, CUSTOMER_LEAVING.

    PriorityQueue<Event> – Manages simulation events sorted by time.

    Queue<Integer> (LinkedList) – Represents the customer queue.

2. Simulation Parameters

    lambda (arrival rate) = 10 customers/hour = 1 customer every 6 minutes.

    mu (service rate) = 15 customers/hour = 1 customer every 4 minutes.

    simulationCapacity = 500 customers.

    maxWaitingTime = 5 minutes (customer leaves if exceeded).

    lossDuetoLeaving = $5 per abandoned customer.

3. Core Functions

    generateExponential(lambda) – Generates a random exponential arrival/service time.

    handleArrival(int customerID) – Processes customer arrivals, queues them if necessary.

    handleDeparture(int customerID) – Processes customer departures, freeing up the barista.

    handleCustomerLeaving(int customerID) – Handles customers who abandon the queue after 5 minutes.

    removeCustomer(int customerID) – Cleans up abandoned customers from queue & event list.

    runSimulation() – Runs the event-driven simulation until 500 customers are processed.

    displayResults() – Computes and prints performance metrics.


How to Run the Simulation

Using MakeFile:

make run --> To compile and run the simulation
make run_in MU=12 SERVERS=2 CUSTOMER_LEAVE=true --> To run the simulation with a queue 2 baristas and a service rate of 12 and customers leaving after 5 mins waiting
make result--> To tabularise the results using Python
make clean --> To clean the compiled files

Without MakeFile:
javac CoffeShopSimulation.java
java CoffeShopSimulation
or 
java CoffeShopSimulation 12 2 true  --> To run the simulation with a queue 2 baristas and a service rate of 12 and customers leaving after 5 mins waiting

python plot_results.py -> get result for varying servers and mu


