import java.util.Arrays;

public record RankResult(double[] sortedPR, Integer[] sortedIds) {
    public String toString() {
        var sb = new StringBuilder();
        sb.append("    \t\tID  \t\tPR\n");
        int len = sortedIds.length;
        for (int i = len - 1; i >= 0; i--) {
            sb.append(String.format("%4d\t\t%4d\t\t%.7f\n", len - i, sortedIds[i], sortedPR[i]));
        }
        return sb.toString();
    }

    public RankResult getLargestNPage(int n) {
        int len = sortedIds.length;
       return new RankResult(
               Arrays.copyOfRange(sortedPR, len - n, len),
               Arrays.copyOfRange(sortedIds, len -n, len)      // immutable
       );
    }

    public double[] getPR() {
        return sortedPR;
    }

    public Integer[] getIds() {
        return sortedIds;
    }
}
