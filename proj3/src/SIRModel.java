import java.util.*;

public class SIRModel {
    private final Map<Integer, int[]> graph;
    private final int N;
    private final Map<Integer, Person> persons;
    private final Map<Integer, Person> theInfected;
    private enum Status {Susceptible, Infected, Recovered};

    public SIRModel(Map<Integer, int[]> graph, Integer[] initialInfected) {
        this.graph = graph;
        N = graph.size();
        persons = new HashMap<>(N);
        for (int id : graph.keySet())
            persons.put(id, new Person());

        theInfected = new HashMap<>(initialInfected.length * 2);
        for (int id : initialInfected) {
            var p = persons.get(id);
            p.changeStatus(Status.Infected);
            theInfected.put(id, p);
        }

    }

    private class Person {
        Status status = Status.Susceptible;

        public void changeStatus(Status newStatus) {
            status = newStatus;
        }
    }
}
