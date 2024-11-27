import java.util.*;

public class SIRModel {
    private final Map<Integer, List<Integer>> graph;
    private final int N;
    private enum Status {Susceptible, Infected, Recovered};
    private final Map<Integer, Person> theInfected;
    private final Map<Integer, Person> persons;
    private int nInfected = 0;
    private int nRecover = 0;
    private final int numberOfRecover;
    private static final double RECOVER_RATIOO = 0.81;
    private static final double INITIAL_INFECT_RATIO = 0.001;

    private boolean happenIn(double p) {
        return Math.random() <= p;
    }

    private class Person {
        private Status status;
        private int id;
        private static final double RECOVERY_LIMIT = 100;
        private double recovery = 0;
        private double recoveryRate = 0.5;
        private static final double RECOVERY_RATE_GROWTH = 0.1;
        private static final double BASIC_RESILIENCE = 25;
        private int NVisited = 0;
        private static final double BASIC_INFECTION = 10;
        private static final double RESISTANCE_COEFFICIENT = 60;
        private static final double INFECTION_PROBABILITY = 0.75;

        public Person(Status s, int id) {
            status = s;
            this.id = id;
        }

        public void changeStatus(Status s) {
            status = s;
        }

        public Status getStatus() {
            return status;
        }


        /**
         * compute the amount of recovery
         * @return amount of recovery
         */
        public double recovery() {
//            return BASIC_RESILIENCE;
            return BASIC_RESILIENCE * recoveryRate;
        }

        /**
         * compute the amount of decreasing recovery
         * @return amount of decreasing recovery
         */
        public double decovery() {
//            return BASIC_INFECTION;
            return BASIC_INFECTION / Math.pow(2, NVisited / RESISTANCE_COEFFICIENT);
        }

        private void reInfected() {
            if (recovery > 0) {
                recovery -= decovery();
                recoveryRate -= RECOVERY_RATE_GROWTH / 2;
                if (recoveryRate < 0) recoveryRate = 0;
            }
            if (recovery < 0)
                recovery = 0;
            NVisited++;
        }

        private void infected() {
            if (happenIn(INFECTION_PROBABILITY)) {
                status = Status.Infected;
                nInfected++;
                theInfected.put(id, this);
            }
        }

        public void visited() {
            switch (status) {
                case Susceptible -> infected();
//                case Infected -> reInfected();
//                case Recovered ->
            }
        }

        public void updateStatus() {
//            assert status == Status.Infected;
//            recovery += recovery();
//            recoveryRate += RECOVERY_RATE_GROWTH;

//            if (recovery >= RECOVERY_LIMIT) {
            if (happenIn(0.5)) {
                status = Status.Recovered;
                nInfected--;
                nRecover++;
                theInfected.remove(id);
            }
        }
    }

    public SIRModel(Map<Integer, List<Integer>> graph) {
        this.graph = graph;
        this.N = graph.size();
        theInfected = new HashMap<>();
        persons = new HashMap<>(N);
        for (int id : graph.keySet()) {
            persons.put(id, new Person(Status.Susceptible, id));
        }
        numberOfRecover = (int) Math.round(N * RECOVER_RATIOO);
    }

    private void selectInitialInfected() {
        int nInitialInfect = (int) Math.round(N * INITIAL_INFECT_RATIO);
        nInfected += nInitialInfect;
        var idList = graph.keySet();
        for (int id : idList) {
            if (nInitialInfect-- == 0) break;
            var person = persons.get(id);
            person.changeStatus(Status.Infected);
            theInfected.put(id, person);
        }
    }

    public Integer[] stimulate(int nDays) {
        selectInitialInfected();
        do {
            List<Integer> curIdList = new ArrayList<>(theInfected.keySet());
            for (int id : curIdList) {
//                var person = persons.get(id);
//                assert person.getStatus() == Status.Infected;
                var visitList = graph.get(id);
                for (int vid : visitList) {
                    var neighbor = persons.get(vid);
                    neighbor.visited();
                }
//                person.updateStatus();
            }

            for (int id : curIdList) {
                persons.get(id).updateStatus();
            }
        } while (nRecover <= numberOfRecover && (--nDays) != 0);

        return theInfected.keySet().toArray(new Integer[0]);
    }

    public Integer[] stimulate() {
        return stimulate(Integer.MAX_VALUE);
    }
}
