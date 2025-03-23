import java.util.Random;
import java.util.PriorityQueue;

//enum to define type 
enum EventType {
    ARRIVAL, DEPARTURE
}
// it compares 2 event based on time
class Event implements Comparable<Event> {
    double time;
    EventType type; // ARRIVAL or "departure"

    public Event(double time, EventType type) {
        this.time = time;
        this.type = type;
    }

    @Override
    public int compareTo(Event other) {
        return Double.compare(this.time, other.time);
    }


}



// generateSimulation gives a random variable based on exponential distribution 
public class CallCentreSimulation {
    public static double generateExponential(double lambda) {
        Random random = new Random();
        double u = random.nextDouble(); // Generates a random number in [0,1)
        return -Math.log(1 - u) / lambda;   
    }

    static double lambda = 1/180.0;
    static double mu = 1/150.0;
    static int capacity;
    static double simulationPeriod = 8*60*60*5; 
    static PriorityQueue<Event> eventQueue = new PriorityQueue<>(); // PriorityQueue based on arrival time to stimulate customer waiting in the queue 

    static double currTime = 0;
    static double lastEventTime = 0;
    static double numInSystem = 0;
    static boolean isSystemEmpty = true;
    static boolean isSystemFull = false;

    static double totalWaitingTime = 0;
    static double totalTimeInSystem = 0;
    static double timeQueueFull = 0;
    static double timeQueueEmpty = 0;
    static double numCustomersArrived = 0;
    static double numCustomersRejected = 0; // no of customer rejected fur to buffer getting full

    public static void main(String[] args) {
        
        capacity = args.length > 0 ? Integer.parseInt(args[0]) : 5;
        eventQueue.add(new Event(generateExponential(lambda), EventType.ARRIVAL));


        while (!eventQueue.isEmpty()) {
            Event event = eventQueue.poll();
            currTime = event.time;
            if(currTime > simulationPeriod) {
                break;
            }
            if (event.type == EventType.ARRIVAL) {
                handleArrival();
            } else {
                handleDeparture();
            }
            lastEventTime = currTime;
        }

        System.out.println("Average Waiting Time: "+totalWaitingTime/(numCustomersArrived-numCustomersRejected));
        System.out.println("Average Time in System: "+totalTimeInSystem/(numCustomersArrived-numCustomersRejected));
        System.out.println("Utilization Rate of Agent: "+(1-(timeQueueEmpty/simulationPeriod)));
        System.out.println("Average Number of Customers in Queue: "+totalWaitingTime/simulationPeriod);
        System.out.println("Probability of System Full: "+timeQueueFull/simulationPeriod);
        System.out.println("Probability of Customers Rejected: "+numCustomersRejected/numCustomersArrived);

    }

    public static void handleArrival() {
        numCustomersArrived++;
        if (numInSystem == 0) {
            timeQueueEmpty += currTime - lastEventTime;
            isSystemEmpty = false;
        }
        else{
            totalWaitingTime += (numInSystem-1) * (currTime - lastEventTime);
        }

        totalTimeInSystem += numInSystem * (currTime - lastEventTime);

        if (numInSystem == capacity) { // system is full
            isSystemFull = true;
            timeQueueFull += currTime - lastEventTime;
            numCustomersRejected++;   // so new packet is rejected
            eventQueue.add(new Event(currTime + generateExponential(lambda), EventType.ARRIVAL));
            return;
        }
        
        numInSystem++;
        eventQueue.add(new Event(currTime + generateExponential(lambda), EventType.ARRIVAL));
        if (numInSystem == 1) {
            eventQueue.add(new Event(currTime + generateExponential(mu), EventType.DEPARTURE));
        }

    }

    public static void handleDeparture() {
        
        totalWaitingTime += (numInSystem-1) * (currTime - lastEventTime);
        
        totalTimeInSystem += numInSystem * (currTime - lastEventTime);

        if (numInSystem == capacity) {
            isSystemFull = false; // since deperature is happenning , now it system will again become not full 
            timeQueueFull += currTime - lastEventTime;
        }
        numInSystem--;
        if (numInSystem >0) {
            eventQueue.add(new Event(currTime + generateExponential(mu), EventType.DEPARTURE));
        }
    }
}