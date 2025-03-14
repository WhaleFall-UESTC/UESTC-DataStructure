import java.util.*;
import java.util.stream.Collectors;

public class SIRModel {
    private final Map<Integer, int[]> graph;
    private final int N;
    private int nInfected = 0;
    private final Map<Integer, Person> persons;
    private Map<Integer, Person> theInfected;
    private enum Status {Susceptible, Infected, Recovered};
    public static final double DEFAULT_BETA = 0.25;
    public static final double DEFAULT_GAMMA = 1;
    public static final int DEFAULT_INITIAL_INFECTED_NUMBER = 19;
    public static final int DEFAULT_INFECT_DAYS = 3;

    public SIRModel(Map<Integer, int[]> graph) {
        graph = convertToUndirected(graph);
        this.graph = graph;
        N = graph.size();
        persons = new HashMap<>(N);
        for (int id : graph.keySet())
            persons.put(id, new Person(id));
    }

    private Map<Integer, int[]> convertToUndirected(Map<Integer, int[]> dg) {
        Map<Integer, List<Integer>> ud = new HashMap<>();
        for (int id : dg.keySet()) {
            var toIdList = Arrays.stream(dg.get(id))
                            .boxed()
                            .collect(Collectors.toList());
            ud.put(id, toIdList);
        }

        for (int fromId : dg.keySet()) {
            var toIdList = dg.get(fromId);
            for (var toId : toIdList) {
                ud.get(toId).add(fromId);
            }
        }

        Map<Integer, int[]> undirected = new HashMap<>();
        for (int id : ud.keySet()) {
            var toIdList = ud.get(id).stream()
                    .mapToInt(i -> i)
                    .toArray();
            undirected.put(id, toIdList);
        }

        return undirected;
    }

    private void initializeInfected(Integer[] initialInfectedId) {
        Arrays.sort(initialInfectedId);
//        System.out.println("Initializing infected " + Arrays.toString(initialInfectedId));
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
                            double infectP, double recoverP, int days)
    {
        for (var p : persons.values()) {
            p.formatted();
        }
        initializeInfected(initialInfectedId);
        while (!theInfected.isEmpty() && (days-- != 0)) {
//            System.out.println(theInfected.size() + " " + Arrays.toString(theInfected.keySet().toArray(Integer[]::new)));
            var curInfected = theInfected.values().toArray(Person[]::new);
            for (var p : curInfected) {
                var neighborIds = graph.get(p.getId());
                for (var neighborId : neighborIds) {
                    var neighbor = persons.get(neighborId);
                    neighbor.visited(infectP);
                }
                p.heal(recoverP);
            }
        }
//        System.out.println("nInfected: " + nInfected);
        return 1.0 * nInfected / N;
    }

    public double stimulate(Integer[] initialInfectedId, double infectP, double recoverP) {
        return stimulate(initialInfectedId, infectP, recoverP, Integer.MAX_VALUE);
    }

    public double stimulate(Integer[] initialInfectedId) {
        return stimulate(initialInfectedId,
                DEFAULT_BETA,
                DEFAULT_GAMMA,
                DEFAULT_INFECT_DAYS
        );
    }

    public double stimulate() {
        var allIds = set2Array(graph.keySet());
        return stimulate(
                selectRandomly(allIds, DEFAULT_INITIAL_INFECTED_NUMBER),
                DEFAULT_BETA,
                DEFAULT_GAMMA,
                DEFAULT_INFECT_DAYS
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

        public void formatted() {
            status = Status.Susceptible;
        }

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
            if (status == Status.Susceptible && chance(infectedP)) {
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