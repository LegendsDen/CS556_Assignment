import os
import pandas as pd
import numpy as np
from tabulate import tabulate
import time
import re

def run_simulation(mu, capacity, leave):
    """Run the coffee shop simulation with specified parameters"""
    command = f"java CoffeShopSimulation {mu} {capacity} {leave}"
    try:
        output = os.popen(command).read()
        return output
    except Exception as e:
        print(f"Error running simulation: {e}")
        return None

def parse_simulation_output(output):
    """Parse the output of the simulation into a dictionary of metrics"""
    if not output:
        return None
    
    metrics = {}
    
    # Extract all values using regex
    patterns = {
        "avg_waiting_time": r"Average Waiting Time: ([\d.]+)",
        "avg_time_in_system": r"Average Time in System: ([\d.]+)",
        "utilization_rate": r"Utilization Rate of Barista: ([\d.]+)",
        "ideal_rate": r"Ideal Rate of Barista: ([\d.]+)",
        "avg_customers_in_queue": r"Average Number of Customers in Queue: ([\d.]+)",
        "max_queue_length": r"Maximum Queue Length: ([\d.]+)",
        "peak_time": r"Peak Time: ([\d.]+)",
        "empty_queue_prob": r"Probability of Empty Queue: ([\d.]+)",
        "leaving_prob": r"Probability of Customers Leaving: ([\d.]+)",
        "loss_due_to_leaving": r"Loss due to long wait time: ([\d.]+)"
    }
    
    for metric, pattern in patterns.items():
        match = re.search(pattern, output)
        if match:
            metrics[metric] = float(match.group(1))
        else:
            metrics[metric] = None
    
    return metrics

def compile_java():
    """Compile the Java file if needed"""
    try:
        os.system("javac CoffeShopSimulation.java")
        print("Successfully compiled Java code")
        return True
    except Exception as e:
        print(f"Failed to compile Java code: {e}")
        return False

def run_all_simulations():
    """Run simulations with all combinations of parameters and collect results"""
    mu_values = [12, 14, 15, 18, 20, 22]
    capacity_values = list(range(1, 6))  # 1 to 5
    leave_values = [True, False]
    iterations = 5    
    # Create a DataFrame to store all results
    results = []
    
    total_simulations = len(mu_values) * len(capacity_values) * len(leave_values) * iterations
    completed = 0
    
    print(f"Starting {total_simulations} simulations...")
    
    for leave in leave_values:
        for mu in mu_values:
            for capacity in capacity_values:
                # Run multiple iterations for each configuration
                iteration_results = []
                for i in range(iterations):
                    output = run_simulation(mu, capacity, str(leave).lower())
                    parsed = parse_simulation_output(output)
                    if parsed:
                        parsed.update({
                            'mu': mu,
                            'capacity': capacity,
                            'customers_leave': leave,
                            'iteration': i
                        })
                        iteration_results.append(parsed)
                    
                    completed += 1
                    if completed % 10 == 0:
                        print(f"Completed {completed}/{total_simulations} simulations ({(completed/total_simulations)*100:.1f}%)")
                
                # Calculate averages across iterations
                if iteration_results:
                    avg_result = {k: np.mean([r[k] for r in iteration_results]) 
                                for k in iteration_results[0] 
                                if k not in ['mu', 'capacity', 'customers_leave', 'iteration']}
                    avg_result.update({
                        'mu': mu,
                        'capacity': capacity,
                        'customers_leave': leave
                    })
                    results.append(avg_result)
    
    return pd.DataFrame(results)

def display_results(results_df):
    """Display the results in two tables (for leave=True and leave=False)"""
    if results_df.empty:
        print("No results to display.")
        return
    
    # Create pivot tables for better visualization
    metrics = [
        'avg_waiting_time', 
        'avg_time_in_system', 
        'utilization_rate',
        'max_queue_length',
        'empty_queue_prob'
    ]
    
    if 'leaving_prob' in results_df.columns:
        metrics.extend(['leaving_prob', 'loss_due_to_leaving'])
    
    for leave in [True, False]:
        filtered_df = results_df[results_df['customers_leave'] == leave]
        
        if filtered_df.empty:
            continue
            
        print(f"\n{'=' * 80}")
        print(f"RESULTS FOR CUSTOMERS LEAVING = {leave}")
        print(f"{'=' * 80}")
        
        for metric in metrics:
            if metric in filtered_df.columns:
                pivot = filtered_df.pivot_table(
                    values=metric,
                    index='capacity',
                    columns='mu',
                    aggfunc='mean'
                )
                
                print(f"\n{metric.upper().replace('_', ' ')}:")
                print(tabulate(pivot, headers=pivot.columns, tablefmt="grid", 
                              floatfmt=".2f", showindex=True))
                print()

def main():
    print("Coffee Shop Simulation Runner")
    print("-" * 40)
    
    # First compile the Java code
    if not compile_java():
        return
    
    # Run all simulations and gather results
    print("Running simulations... This may take a while.")
    start_time = time.time()
    results = run_all_simulations()
    end_time = time.time()
    
    print(f"All simulations completed in {end_time - start_time:.2f} seconds.")
    
    # Display results in table format
    display_results(results)
    
    # Save results to a CSV file for further analysis
    results.to_csv("coffee_shop_simulation_results.csv", index=False)
    print("Results saved to coffee_shop_simulation_results.csv")

if __name__ == "__main__":
    main()