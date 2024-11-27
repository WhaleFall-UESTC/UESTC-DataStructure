import java.util.*;

public class SIRModel {
    private final Map<Integer, int[]> graph;
    private final int N;
    private int nInfected = 0;
    private final Map<Integer, Person> persons;
    private Map<Integer, Person> theInfected;
    private enum Status {Susceptible, Infected, Recovered};
    private static final double DEFAULT_INFECT_PROBABILITY = 0.72;
    private static final double DEFAULT_RECOVER_PROBABILITY = 0.5;
    private static final int DEFAULT_INITIAL_INFECTED_NUMBER = 11;

    public SIRModel(Map<Integer, int[]> graph) {
        this.graph = graph;
        N = graph.size();
        persons = new HashMap<>(N);
        for (int id : graph.keySet())
            persons.put(id, new Person(id));
    }

    private void initializeInfected(Integer[] initialInfectedId) {
        nInfected = 0;
        theInfected = new HashMap<>(initialInfectedId.length * 2);
        for (int id : initialInfectedId) {
            var p = persons.get(id);
            p.infected();
        }
    }

    /**
     * Stimulate SIR Module, return the prevalence of epidemic
     * @param initialInfectedId the IDs of initial infection
     * @param infectP probability of being infected
     * @param recoverP probability of recovery
     * @return The proportion of people who have been infected with the disease
     */
    public double stimulate(Integer[] initialInfectedId,
        double infectP, double recoverP)
    {
        initializeInfected(initialInfectedId);
        while (!theInfected.isEmpty()) {
            for (var p : theInfected.values()) {
                var neighborIds = graph.get(p.getId());
                for (var neighborId : neighborIds) {
                    var neighbor = persons.get(neighborId);
                    neighbor.visited(infectP);
                }
                p.heal(recoverP);
            }
        }

        return 1.0 * nInfected / N;
    }

    public double stimulate()

    public double stimulate() {
        var allIds = set2Array(graph.keySet());
        return stimulate(
                selectRandomly(allIds, DEFAULT_INITIAL_INFECTED_NUMBER),
                DEFAULT_INFECT_PROBABILITY,
                DEFAULT_RECOVER_PROBABILITY
        );
    }

    private Integer[] set2Array(Set<Integer> set) {
        return set.toArray(Integer[]::new);
    }

    private Integer[] selectRandomly(Integer[]  ids, int n) {
        assert n <= ids.length;
        var listCopy = Arrays.asList(ids);
        Collections.shuffle(listCopy);
        return listCopy.stream().limit(n).toArray(Integer[]::new);
    }

    private class Person {
        private Status status;
        private final int id;

        public Person(int id, Status status) {
            this.id = id;
            this.status = status;
        }

        public Person(int id) {
            this(id, Status.Susceptible);
        }

        public int getId() { return id; }

        public void recovered() {
            status = Status.Recovered;
            theInfected.remove(id);
        }

        public void infected() {
            status = Status.Infected;
            nInfected++;
            theInfected.put(id, this);
        }

        public void visited(double infectedP) {
            if (chance(infectedP)) {
                infected();
            }
        }

        public void heal(double recoverP) {
            if (chance(recoverP)) {
                recovered();
            }
        }
    }

    /**
     * Returns true with the specified probability
     * @param p the probability
     * @return true in p, false in (1 - p)
     */
    private boolean chance(double p) {
        return Math.random() < p;
    }
}
