import java.util.*;

public class PageRank {
    private static final double d = 0.85;
    private final int N;        // number of nodes
    private final Map<Integer, List<Integer>> graphOut;  // List: the id of next node
    private final Map<Integer, int[]> graphIn;   // List: the index of prev node
    private final int[] ids;    // id of nodes
    private final int[] Ls;     // out degree of nodes
    private static final int DEFALUT_RANK_TURNS = 500;

    public PageRank(String graphPath) {
        var gr = new GraphReader(graphPath);
        graphOut = gr.read(GraphReader.OUT);
        N = gr.readNumberOfNodes();
        ids = gr.getSortedIds();
        Ls = new int[N];

        List<Integer> outDegreeZero = new ArrayList<>();
        for (int i = 0; i < N; i++) {
            Ls[i] = L(i);
            assert Ls[i] > 0;
            if (Ls[i] == 0) {
                outDegreeZero.add(i);
                Ls[i] = N - 1;
            }
        }

        graphIn = new HashMap<>();
        var graphInOrigin = gr.read(GraphReader.IN);
        for (var entry : graphInOrigin.entrySet()) {
            var idList = entry.getValue();
            idList.addAll(outDegreeZero);
            int[] idxList = idList.stream().
                    mapToInt(id -> Arrays.binarySearch(ids, id)).toArray();
            graphIn.put(entry.getKey(), idxList);
        }
    }

    private boolean checkIdx(int idx) {
        return idx >= 0 && idx < N;
    }

    /**
     * get out degree of a node
     * @param idx index of the node, ids[idx] = id of the node
     * @return out degree of the node, -1 if id is illegal
     */
    private int L(int idx) {
        var nodeList = graphOut.getOrDefault(ids[idx], null);
        return nodeList == null ? 0 : nodeList.size();
    }

    /**
     * compute part of a node's new PR
     * the sum of other node's PR dividing its out-degree which points to the node
     * @param PR a list of the PR of nodes
     * @param idx index of the node, which need a new PR
     * @return the result, -1 if id is illegal
     */
    private double sumOfAveragedPRVoteTo(double[] PR, int idx) {
        if (!checkIdx(idx))
            return -1;
        int id = ids[idx];
        var fromNodeList = graphIn.getOrDefault(id, null);
        if (fromNodeList == null)
            return 0;
        double ret = 0;
        for (int index : fromNodeList) {
            ret += PR[index] / Ls[index];
        }
        return ret;
    }

    private double nextPRi(double PR[], int idx) {
        double sum = sumOfAveragedPRVoteTo(PR, idx);
        return (1 - d) / N + d * sum;
    }

    private double[] originPR() {
        var ret = new double[N];
        Arrays.fill(ret, 1 / N);
        return ret;
    }

    private double[] nextPRList(double curPR[]) {
        var nextPR = new double[N];
        for (int i = 0; i < N; i++) {
            nextPR[i] = nextPRi(curPR, i);
        }
        return nextPR;
    }

    public RankResult rank(int n) {
        double[] PR = originPR();
        while ((n--) != 0) {
            PR = nextPRList(PR);
        }

        Integer[] indices = new Integer[N];
        for (int i = 0; i < indices.length; i++) {
            indices[i] = i;
        }
        final var sortedPR = PR;
        Arrays.sort(indices, Comparator.comparingDouble(i -> sortedPR[i]));
        Arrays.sort(sortedPR);

        for (int i = 0; i < indices.length; i++)
            indices[i] = ids[indices[i]];

        Map<Integer, int[]> copyGraphIn = new HashMap<>();
        for (int id : graphIn.keySet()) {
            var idxList = graphIn.get(id);
            int len = idxList.length;
            var idList = new int[len];
            for (int i = 0; i < len; i++)
                idList[i] = ids[idxList[i]];
            copyGraphIn.put(id, idList);
        }

        return new RankResult(sortedPR, indices, copyGraphIn);
    }

    public RankResult rank() { return rank(DEFALUT_RANK_TURNS); }

    /**
     * compare infectiousness between pagerank and random select
     * @return a double array containing the result of stimulation, the first is pagerank's. the second is random selection's;
     */
    public double[] testSIR() {
        var rankResult = rank();
        return rankResult.testSIR();

    }
}
