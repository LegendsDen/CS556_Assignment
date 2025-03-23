import java.util.Random;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.LinkedList;


enum EventType {
    ARRIVAL, DEPARTURE, CUSTOMER_LEAVING
}

class Event implements Comparable<Event> {
    double time;
    EventType type; // ARRIVAL or "departure"
    int customerID;
    public Event(double time, EventType type, int customerID) {
        this.time = time;
        this.type = type;
        this.customerID = customerID;
    }

    @Override
    public int compareTo(Event other) {
        return Double.compare(this.time, other.time);
    }


}



public class CoffeShopSimulation {
    public static double generateExponential(double lambda) {
        Random random = new Random();
        double u = random.nextDouble(); // Generates a random number in [0,1)
        return -Math.log(1 - u) / lambda;   
    }

    static double lambda = 1/360.0;
    static double mu = 1/240.0;
    static double simulationCapacity = 500; 
    static PriorityQueue<Event> eventQueue = new PriorityQueue<>();
    static Queue<Integer> customersInQueue = new LinkedList<>();
    static int numofBaristas = 1;
    static int maxWaitingTime = 5*60;
    static boolean doCustomersLeave = true;
    static int lossDuetoLeaving = 5;

    static double currTime = 0;
    static double lastEventTime = 0;
    static double numInSystem = 0;

    static double totalWaitingTime = 0;
    static double totalTimeInSystem = 0;
    static double timeQueueEmpty = 0;
    static double timeBaristaFree = 0;
    static double numCustomersArrived = 0;
    static double maximumQueueLength = 0;
    static double numCustomersAbandoned = 0;
    static double numCustomersServed = 0;
    static double peakTime = 0;

    public static void main(String[] args) {
        
        mu = args.length > 0 ? Integer.parseInt(args[0])/3600.0 : 1/240.0;
        numofBaristas = args.length > 1 ? Integer.parseInt(args[1]) : 1;
        doCustomersLeave = args.length > 2 ? Boolean.parseBoolean(args[2]) : false;


        eventQueue.add(new Event(generateExponential(lambda), EventType.ARRIVAL,0));

        while (!eventQueue.isEmpty()) {
            Event event = eventQueue.poll();
            if ((event.type == EventType.ARRIVAL)&&(numCustomersArrived < simulationCapacity)) {
                currTime = event.time;
                handleArrival(event.customerID);
                lastEventTime = currTime;

            } else if(event.type == EventType.DEPARTURE) {
                currTime = event.time;
                handleDeparture(event.customerID);
                lastEventTime = currTime;

            }
            else if(event.type == EventType.CUSTOMER_LEAVING){
                currTime = event.time;
                handleCustomerLeaving(event.customerID);
                lastEventTime = currTime;
            }
        }

        System.out.println("Average Waiting Time: "+totalWaitingTime/(numCustomersArrived) + " seconds");
        System.out.println("Average Time in System: "+totalTimeInSystem/(numCustomersArrived) + " seconds");
        System.out.println("Utilization Rate of Barista: "+(1-(timeBaristaFree/currTime)));
        System.out.println("Ideal Rate of Barista: "+((timeBaristaFree/currTime)));
        System.out.println("Average Number of Customers in Queue: "+totalWaitingTime/currTime);
        System.out.println("Maximum Queue Length: "+maximumQueueLength);
        System.out.println("Peak Time: "+peakTime);
        System.out.println("Probability of Empty Queue: "+(timeQueueEmpty/currTime));
        System.out.println("Probability of Customers Leaving: "+(numCustomersAbandoned/numCustomersArrived));
        System.out.println("Loss due to long wait time: "+(numCustomersAbandoned*lossDuetoLeaving));

    }

    public static void handleArrival(int customerID) {
        numCustomersArrived++;

        if(numInSystem == 0){
            timeBaristaFree += currTime - lastEventTime;
        }
        if (numInSystem <= numofBaristas) {
            timeQueueEmpty += currTime - lastEventTime;
        }
        else{
            totalWaitingTime += (numInSystem-numofBaristas) * (currTime - lastEventTime);
        }
        totalTimeInSystem += numInSystem * (currTime - lastEventTime);

        // System.out.println("Customer Arrived: "+ customerID + " Time: "+currTime);
        
        numInSystem++;
        if(numInSystem > maximumQueueLength){
            peakTime = currTime;
        }
        maximumQueueLength = Math.max(maximumQueueLength, numInSystem);
        eventQueue.add(new Event(currTime + generateExponential(lambda), EventType.ARRIVAL, (int) Math.floor(numCustomersArrived)));
        if (numInSystem <= numofBaristas) {
            eventQueue.add(new Event(currTime + generateExponential(mu), EventType.DEPARTURE,customerID));
        }
        else{

            if(doCustomersLeave){
                eventQueue.add(new Event(currTime + maxWaitingTime, EventType.CUSTOMER_LEAVING, customerID));
            }
            customersInQueue.add(customerID);
        }

    }

    public static void handleDeparture(int customerID) {
        
        if(numInSystem > numofBaristas){
            totalWaitingTime += (numInSystem-numofBaristas) * (currTime - lastEventTime);
        }
        else{
            timeQueueEmpty += currTime - lastEventTime;
        }
        totalTimeInSystem += numInSystem * (currTime - lastEventTime);

        numInSystem--;
        // System.out.println("Customer Served: "+ customerID + " Time: "+currTime);
        if (numInSystem >= numofBaristas) {
            int newCustomerID = customersInQueue.poll();
            removeCustomer(newCustomerID);
            eventQueue.add(new Event(currTime + generateExponential(mu), EventType.DEPARTURE, newCustomerID));

        }
    }

    public static void handleCustomerLeaving(int customerID) {
        numCustomersAbandoned++;
        if(numInSystem > numofBaristas){
            totalWaitingTime += (numInSystem-numofBaristas) * (currTime - lastEventTime);
        }
        else{
            timeQueueEmpty += currTime - lastEventTime;
        }
        totalTimeInSystem += numInSystem * (currTime - lastEventTime);
       
        // System.out.println("Customer Left: "+ customerID + " Time: "+currTime);
        numInSystem--;
        
        removeCustomer(customerID);
    }

    public static void removeCustomer(int customerID){
        customersInQueue.remove(customerID);
        eventQueue.removeIf(event -> event.customerID == customerID);
    }
}