// Rinnegan
#include<bits/stdc++.h>
#pragma GCC optimize("Ofast")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimization("unroll-loops")
using namespace std;





// Structure to represent a customer
struct Customer {
    double arrivalTime;
    double serviceStartTime;
    double departureTime;
    
    Customer(double arrival) : arrivalTime(arrival), serviceStartTime(0), departureTime(0) {}
};

// Structure to represent a teller
struct Teller {
    bool busy;
    double availableTime;
    
    Teller() : busy(false), availableTime(0) {}
      
    
};

// Structure to store simulation results for a time period
struct PeriodMetrics {
    string periodName;
    double startTime;
    double endTime;
    int numTellers;
    int customersServed;
    double totalWaitingTime;
    double totalSystemTime;
    double totalIdleTime;
    double totalBusyTime;
    int allTellersBusyCount;
    int checkCount;
	double all_server_busy;
    
    PeriodMetrics(const string& name, double start, double end, int tellers)
        : periodName(name), startTime(start), endTime(end), numTellers(tellers),
          customersServed(0), totalWaitingTime(0), totalSystemTime(0),
          totalIdleTime(0), totalBusyTime(0),all_server_busy(0),
          allTellersBusyCount(0), checkCount(0) {}
};

class BankSimulation {
private:
    // Simulation parameters
    double arrivalRate;         // customers per hour
    double serviceRate;         // customers per hour
    vector<PeriodMetrics> periodMetrics;
    
    // Random number generators
    mt19937 rng;
    exponential_distribution<double> arrivalDist;
    exponential_distribution<double> serviceDist;
    
    // State variables
    vector<Teller> tellers;
    queue<Customer> queuet;
    vector<Customer> completedCustomers;
    
    double currentTime;
    double nextArrivalTime;

public:
    BankSimulation(double arrRate, double servRate)
        : arrivalRate(arrRate), serviceRate(60.0 / servRate), 
          rng(random_device{}()),
          arrivalDist(arrivalRate / 60.0),    // Convert to customers per minute
          serviceDist(serviceRate / 60.0)     // Convert to customers per minute
    {
        // Initialize time periods
        periodMetrics.emplace_back("Morning (9:00 AM - 11:00 AM)", 0, 120, 2);
        periodMetrics.emplace_back("Peak (11:00 AM - 2:00 PM)", 120, 300, 4);
        periodMetrics.emplace_back("Afternoon (2:00 PM - 5:00 PM)", 300, 480, 3);
        
        // Initialize simulation
        currentTime = 0;
        nextArrivalTime = currentTime + generateInterarrivalTime();
    }
    
    double generateInterarrivalTime() {
        return arrivalDist(rng);
    }
    
    double generateServiceTime() {
        return serviceDist(rng);
    }
    
    int getCurrentPeriodIndex() {
        for (size_t i = 0; i < periodMetrics.size(); ++i) {
            if (currentTime >= periodMetrics[i].startTime && currentTime < periodMetrics[i].endTime) {
                return i;
            }
        }
        return -1; // Should not happen if simulation is within working hours
    }
    
    void adjustTellers() {
        int periodIndex = getCurrentPeriodIndex();
        if (periodIndex >= 0) {
            int requiredTellers = periodMetrics[periodIndex].numTellers;
            
            // Adjust teller count
            if (tellers.size() != requiredTellers) {
                // If we need more tellers, add them
                while (tellers.size() < requiredTellers) {
                    tellers.push_back(Teller());
                    if(queuet.size()>0){
                        Customer cust=queuet.front();
                        queuet.pop();
                        tellers[tellers.size()-1].busy=true;
                        if(periodIndex==1)cust.serviceStartTime=120;
                        if(periodIndex==2) cust.serviceStartTime=300;
                        double serviceTime = generateServiceTime();
                        cust.departureTime = (periodIndex==1)?(120+serviceTime):(300 + serviceTime);

                        tellers[tellers.size()-1].availableTime = cust.departureTime;
                        if (periodIndex >= 0) {
                            periodMetrics[periodIndex].customersServed++;
                            periodMetrics[periodIndex].totalWaitingTime += (cust.serviceStartTime - cust.arrivalTime);
                            periodMetrics[periodIndex].totalSystemTime += (cust.departureTime - cust.arrivalTime);
                        }
                    }
                }
                
                // If we have too many tellers, remove idle ones first
                while (tellers.size() > requiredTellers) {
                    // Find an idle teller
                    bool foundIdle = false;
                    for (size_t i = 0; i < tellers.size(); ++i) {
                        if (!tellers[i].busy) {
                            tellers.erase(tellers.begin() + i);
                            foundIdle = true;
                            break;
                        }
                    }
                    
                    // If no idle teller found, we'll have to wait until a teller becomes available
                    if (!foundIdle) {
                        break;
                    }
                }
            }
        }
    }
    
    void processArrival() {
        // Create a new customer
        Customer customer(currentTime);
        queuet.push(customer);
        
        // Find available teller
        bool foundTeller = false;
        for (size_t i = 0; i < tellers.size(); ++i) {
            if (!tellers[i].busy || tellers[i].availableTime <= currentTime) {
                // Teller is available
                tellers[i].busy = true;
                
                // Set service start time
                customer = queuet.front();
                queuet.pop();

                customer.serviceStartTime = currentTime;
                
                // Calculate departure time
                double serviceTime = generateServiceTime();
                customer.departureTime = currentTime + serviceTime;
                
                // Update teller availability
                tellers[i].availableTime = customer.departureTime;
                
                // Add to completed customers
                completedCustomers.push_back(customer);
                
                // Update metrics for current period
                int periodIndex = getCurrentPeriodIndex();
                if (periodIndex >= 0) {
                    periodMetrics[periodIndex].customersServed++;
                    periodMetrics[periodIndex].totalWaitingTime += (customer.serviceStartTime - customer.arrivalTime);
                    periodMetrics[periodIndex].totalSystemTime += (customer.departureTime - customer.arrivalTime);
                }
                
                foundTeller = true;
                break;
            }
        }
        
        if (!foundTeller) {
            // All tellers are busy, add customer to queuet
            
            // Update max queuet length for current period
            int periodIndex = getCurrentPeriodIndex();
            if (periodIndex >= 0) {
                // Check if all tellers are busy for statistics
                bool allBusy = true;
                for (const auto& teller : tellers) {
                    if (!teller.busy) {
                        allBusy = false;
                        break;
                    }
                }
                
                if (allBusy) {
                    periodMetrics[periodIndex].allTellersBusyCount++;
                }
                periodMetrics[periodIndex].checkCount++;
            }
        }
        
        // Schedule next arrival
        nextArrivalTime = currentTime + generateInterarrivalTime();
    }
    
    void processAvailableTellers() {

        for (int i=0;i<tellers.size();i++) {
            if (tellers[i].busy && tellers[i].availableTime <= currentTime && tellers.size()>periodMetrics[getCurrentPeriodIndex()].numTellers) {
                tellers.erase(tellers.begin() + i);
                return;
            }

            // Check if teller is busy but has completed their service
            if (tellers[i].busy && tellers[i].availableTime <= currentTime) {
                // Teller becomes available
                tellers[i].busy = false;
                
                // If there are customers in the queuet, assign one to this teller
                if (!queuet.empty()) {
                    Customer customer = queuet.front();
                    queuet.pop();
                    
                    // Update customer times
                    customer.serviceStartTime = currentTime;
                    double serviceTime = generateServiceTime();
                    customer.departureTime = currentTime + serviceTime;
                    
                    // Update teller
                    tellers[i].busy = true;
                    tellers[i].availableTime = customer.departureTime;
                    
                    // Add to completed customers
                    completedCustomers.push_back(customer);
                    
                    // Update metrics for current period
                    int periodIndex = getCurrentPeriodIndex();
                    if (periodIndex >= 0) {
                        periodMetrics[periodIndex].customersServed++;
                        periodMetrics[periodIndex].totalWaitingTime += (customer.serviceStartTime - customer.arrivalTime);
                        periodMetrics[periodIndex].totalSystemTime += (customer.departureTime - customer.arrivalTime);
                    }
                }
            }
        }
    }
    
    double getNextEventTime() {
        double nextTellerAvailable = numeric_limits<double>::max();
        
        for (const auto& teller : tellers) {
            if (teller.busy && teller.availableTime < nextTellerAvailable) {
                nextTellerAvailable = teller.availableTime;
            }
        }
        return min(nextArrivalTime, nextTellerAvailable);
    }
    
    void updateTellerMetrics(double oldTime, double newTime) {
        int periodIndex = getCurrentPeriodIndex();
        if (periodIndex >= 0) {
            double timeDiff = newTime - oldTime;
            bool flag=true;
            if (timeDiff > 0) {
                // Count busy and idle time for each teller
                for (const auto& teller : tellers) {
                    if (teller.busy) {
                        periodMetrics[periodIndex].totalBusyTime += timeDiff;
                    } else {
						flag=false;
                        periodMetrics[periodIndex].totalIdleTime += timeDiff;
                    }
                }
				if(flag) periodMetrics[periodIndex].all_server_busy+=timeDiff;
            }

        }
    }
    void runSimulation() {
		ofstream queuetData("queuet_data.txt"); // File to store queuet length over time
		ofstream waitingTimeData("waiting_time.txt"); 
        // Initialize tellers for the first period
        tellers.resize(periodMetrics[0].numTellers);
        
        // Run simulation until the end of the day
        while (currentTime < periodMetrics.back().endTime) {
            // Check if we need to adjust teller count at period transition
            adjustTellers();
            
            // Determine next event time
            double oldTime = currentTime;
            double nextEventTime = getNextEventTime();
            
            // Update the current time
            currentTime = nextEventTime;
             // Log queuet length at each time step
    		queuetData << currentTime << " " << queuet.size() << endl;
            // Update teller utilization metrics
            updateTellerMetrics(oldTime, currentTime);
			
            // Process next event
            if (currentTime >= periodMetrics.back().endTime) {
                // End of day reached
                break;
            } else if (currentTime == nextArrivalTime) {
                // Process customer arrival
                processArrival();
            } else {

                // Process available tellers
                processAvailableTellers();
            }
        }  
				// Log final waiting time for each period
		for (const auto& period : periodMetrics) {
			waitingTimeData << period.startTime << " " << period.totalWaitingTime / max(1, period.customersServed) << endl;
		}

		queuetData.close();
		waitingTimeData.close();
    }
    
    void displayResults() {
        cout << fixed << setprecision(2);
        cout << "======================================================\n";
        cout << "              BANK SIMULATION RESULTS                 \n";
        cout << "======================================================\n\n";
        
        // Aggregate metrics for the entire day
        PeriodMetrics wholeDayMetrics("Entire Day (9:00 AM - 5:00 PM)", 0, 480, 0);
        wholeDayMetrics.customersServed = 0;
        wholeDayMetrics.totalWaitingTime = 0;
        wholeDayMetrics.totalSystemTime = 0;
        wholeDayMetrics.totalBusyTime = 0;
        wholeDayMetrics.totalIdleTime = 0;
        wholeDayMetrics.allTellersBusyCount = 0;
        wholeDayMetrics.checkCount = 0;
        
        // Display results for each period
        for (const auto& period : periodMetrics) {
            cout << "Period: " << period.periodName << "\n";
            cout << "Number of Tellers: " << period.numTellers << "\n";
            cout << "Customers Served: " << period.customersServed << "\n";
            
            double avgWaitingTime = period.customersServed > 0 ? 
                period.totalWaitingTime / period.customersServed : 0;
            double avgSystemTime = period.customersServed > 0 ? 
                period.totalSystemTime / period.customersServed : 0;
            
            // Calculate total teller time and utilization
            double totalTellerTime = period.endTime - period.startTime;
            totalTellerTime *= period.numTellers; // Multiply by number of tellers
            
            double utilizationRate = period.totalBusyTime / totalTellerTime;
            
            double avgqueuetLength = period.totalWaitingTime/(period.endTime-period.startTime);
            
            // Probability of all tellers being busy
            double probAllBusy = 
                static_cast<double>(period.all_server_busy) / (period.endTime-period.startTime);
            
            cout << "a. Average Waiting Time: " << avgWaitingTime << " minutes\n";
            cout << "b. Average Time in System: " << avgSystemTime << " minutes\n";
            cout << "c. Teller Utilization Rate: " << utilizationRate * 100 << "%\n";
            cout << "d. Average queuet Length: " << avgqueuetLength << " customers\n";
            cout << "e. Probability of All Tellers Busy: " << probAllBusy * 100 << "%\n\n";
            
            // Add to whole day metrics
            wholeDayMetrics.customersServed += period.customersServed;
            wholeDayMetrics.totalWaitingTime += period.totalWaitingTime;
            wholeDayMetrics.totalSystemTime += period.totalSystemTime;
            wholeDayMetrics.totalBusyTime += period.totalBusyTime;
            wholeDayMetrics.totalIdleTime += period.totalIdleTime;
            wholeDayMetrics.allTellersBusyCount += period.allTellersBusyCount;
            wholeDayMetrics.checkCount += period.checkCount;
            wholeDayMetrics.all_server_busy += period.all_server_busy;

        }
        
        // Calculate and display whole day metrics
        cout << "======================================================\n";
        cout << "              ENTIRE DAY SUMMARY                      \n";
        cout << "======================================================\n";
        cout << "Period: " << wholeDayMetrics.periodName << "\n";
        cout << "Total Customers Served: " << wholeDayMetrics.customersServed << "\n";
        
        double avgWaitingTime = wholeDayMetrics.customersServed > 0 ? 
            wholeDayMetrics.totalWaitingTime / wholeDayMetrics.customersServed : 0;
        double avgSystemTime = wholeDayMetrics.customersServed > 0 ? 
            wholeDayMetrics.totalSystemTime / wholeDayMetrics.customersServed : 0;
        
        // Calculate the total teller-minutes across all periods
        double totalTellerTime = 0;
        for (const auto& period : periodMetrics) {
            totalTellerTime += (period.endTime - period.startTime) * period.numTellers;
        }
        
        double utilizationRate = wholeDayMetrics.totalBusyTime / totalTellerTime;
        
        double avgqueuetLength =wholeDayMetrics.totalWaitingTime/480;
        
        // Probability of all tellers being busy across the day
        double probAllBusy = 
            static_cast<double>(wholeDayMetrics.all_server_busy) / 480;
        
        cout << "a. Average Waiting Time: " << avgWaitingTime << " minutes\n";
        cout << "b. Average Time in System: " << avgSystemTime << " minutes\n";
        cout << "c. Overall Teller Utilization Rate: " << utilizationRate * 100 << "%\n";
        cout << "d. Average queuet Length: " << avgqueuetLength << " customers\n";
        cout << "e. Probability of All Tellers Busy: " << probAllBusy * 100 << "%\n";
    }
};

int main() {
    freopen("output.txt","w",stdout);
    // Simulation parameters
    double arrivalRate = 40.0;   // customers per hour
    double serviceTime = 4.0;    // minutes per customer
    
    // Create and run simulation
    BankSimulation simulation(arrivalRate, serviceTime);
    simulation.runSimulation();
    simulation.displayResults();
    
    return 0;
}