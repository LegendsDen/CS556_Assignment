import os
import matplotlib.pyplot as plt
import re

# Function to run the Java simulation with a specific capacity
def run_simulation(capacity):
    command = f"java CallCentreSimulation {capacity}"
    output = os.popen(command).read()
    return output

# Function to parse simulation results
def parse_results(output):
    metrics = {}
    
    # Extract values using regex
    waiting_time_match = re.search(r"Average Waiting Time: ([\d.]+)", output)
    system_time_match = re.search(r"Average Time in System: ([\d.]+)", output)
    utilization_match = re.search(r"Utilization Rate of Agent: ([\d.]+)", output)
    queue_length_match = re.search(r"Average Number of Customers in Queue: ([\d.]+)", output)
    system_full_match = re.search(r"Probability of System Full: ([\d.]+)", output)
    rejection_match = re.search(r"Probability of Customers Rejected: ([\d.]+)", output)
    
    if waiting_time_match:
        metrics['waiting_time'] = float(waiting_time_match.group(1))
    if system_time_match:
        metrics['system_time'] = float(system_time_match.group(1))
    if utilization_match:
        metrics['utilization'] = float(utilization_match.group(1))
    if queue_length_match:
        metrics['queue_length'] = float(queue_length_match.group(1))
    if system_full_match:
        metrics['system_full'] = float(system_full_match.group(1))
    if rejection_match:
        metrics['rejection'] = float(rejection_match.group(1))
    
    return metrics

# Main function to run simulations and create plots
def main():
    capacities = range(1, 30)
    results = {}
    num_simulations = 20  # Number of simulations to average
    
    print(f"Running simulations for capacities 1 to 30, averaging over {num_simulations} runs...")
    
    # Run simulations for each capacity
    for capacity in capacities:
        print(f"Simulating capacity {capacity}...")
        aggregated_metrics = {
            'waiting_time': 0,
            'system_time': 0,
            'utilization': 0,
            'queue_length': 0,
            'system_full': 0,
            'rejection': 0
        }
        
        # Run the simulation multiple times and aggregate results
        for _ in range(num_simulations):
            output = run_simulation(capacity)
            metrics = parse_results(output)
            for key in aggregated_metrics:
                if key in metrics:
                    aggregated_metrics[key] += metrics[key]
        
        # Calculate the average for each metric
        results[capacity] = {key: aggregated_metrics[key] / num_simulations for key in aggregated_metrics}
    
    # Create a directory for the plots
    plot_dir = "simulation_plots"
    os.makedirs(plot_dir, exist_ok=True)
    
    # Define the metrics to plot
    metrics = {
        'waiting_time': 'Average Waiting Time (seconds)',
        'system_time': 'Average Time in System (seconds)',
        'utilization': 'Utilization Rate of Agent',
        'queue_length': 'Average Number of Customers in Queue',
        'system_full': 'Probability of System Full',
        'rejection': 'Probability of Customers Rejected'
    }
    
    # Create individual plots for each metric
    for metric_key, metric_name in metrics.items():
        plt.figure(figsize=(10, 6))
        plt.plot(capacities, [results[cap][metric_key] for cap in capacities], 
                 marker='o', linestyle='-', linewidth=2)
        plt.title(f"{metric_name} vs. System Capacity")
        plt.xlabel("System Capacity")
        plt.ylabel(metric_name)
        plt.grid(True)
        plt.xticks(capacities)
        plt.savefig(f"{plot_dir}/{metric_key}_vs_capacity.png")
        plt.close()
    
    print(f"Plots have been saved to the '{plot_dir}' directory.")

if __name__ == "__main__":
    main()
