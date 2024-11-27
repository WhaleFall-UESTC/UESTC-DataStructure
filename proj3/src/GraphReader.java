import java.io.FileNotFoundException;
import java.util.*;
import java.io.File;

public class GraphReader {
    private final File file;
    private final int nodes;
    private final int edges;
    public static final boolean OUT = true;
    public static final boolean IN = false;

    public GraphReader(String filePath) {
        file = new File(filePath);
        assert file.exists();
        nodes = readNumberOf("Nodes:");
        edges = readNumberOf("Edges:");
    }

    private Scanner getScanner() {
        Scanner in;
        try {
            in = new Scanner(file);
        } catch (FileNotFoundException e) {
            in = null;
            e.printStackTrace();
            System.exit(-1);
        }
        return in;
    }

    public Map<Integer, List<Integer>> read(boolean mode) {
        Map<Integer, List<Integer>> graph = new HashMap<>(nodes);;
        var in = getScanner();
        String lineRead;
        do {
            lineRead = in.nextLine();
        } while (lineRead.charAt(0) == '#');

        var firstEdge = lineRead.split("\t");
        assert firstEdge.length == 2 : "Edge Should have two Nodes but read:\n" + lineRead;
        int fromId = Integer.parseInt(firstEdge[0]);
        int toId = Integer.parseInt(firstEdge[1]);

        do {
            if (mode == IN) {
               int tmp = fromId;
               fromId = toId;
               toId = tmp;
            }

            var nodeList = graph.getOrDefault(fromId, null);
            if (nodeList == null) {
                var newNodeList = new ArrayList<Integer>();
                newNodeList.add(toId);
                graph.put(fromId, newNodeList);
            } else {
                nodeList.add(toId);
            }

            if (in.hasNextInt()) {
                fromId = in.nextInt();
                toId = in.nextInt();
            }
            else break;
        } while (true);

        in.close();
        return graph;
    }

    private int readNumberOf(String s) {
        var sc = getScanner();
        String lineRead;
        while (sc.hasNextLine()) {
            lineRead = sc.nextLine();
            if (lineRead.charAt(0) != '#')
                break;
            var finder = new Scanner(lineRead);
            if (finder.findInLine(s) != null)
                return finder.nextInt();
        }
        return -1;
    }

    public int readNumberOfNodes() {
        return nodes;
    }

    public int readNumberOfEdges() {
        return edges;
    }
}
