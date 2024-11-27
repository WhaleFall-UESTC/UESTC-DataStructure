import java.util.Arrays;
import java.util.Map;

public record RankResult(double[] sortedPR, Integer[] sortedIds, Map<Integer, int[]> graphIn) {
    public String toString() {
        var sb = new StringBuilder();
        sb.append("    \t\tID  \t\tPR     \t\tIn Degree\n");
        int len = sortedIds.length;
        for (int i = len - 1; i >= 0; i--) {
            int id = sortedIds[i];
            sb.append(String.format("%4d\t\t%4d\t\t%.7f\t\t%d\n", len - i, id, sortedPR[i], graphIn.get(id).length));
        }
        return sb.toString();
    }

    public RankResult getLargestNPage(int n) {
        int len = sortedIds.length;
       return new RankResult(
               Arrays.copyOfRange(sortedPR, len - n, len),
               Arrays.copyOfRange(sortedIds, len -n, len),      // immutable
               graphIn
       );
    }

    public double[] getPR() {
        return sortedPR;
    }

    public Integer[] getIds() {
        return sortedIds;
    }

    /**
     * compare infectiousness between pagerank and random select
     * @return a double array containing the result of stimulation, the first is pagerank's. the second is random selection's;
     */
    public double[] testSIR() {
        var ret = new double[3];
        var sir = new SIRModel(graphIn);
        int n = SIRModel.DEFAULT_INITIAL_INFECTED_NUMBER;
        var selectPR = getLargestNPage(n).getIds();
        ret[0] = sir.stimulate(selectPR);
        ret[2] = sir.stimulate();
        return ret;
    }
}
