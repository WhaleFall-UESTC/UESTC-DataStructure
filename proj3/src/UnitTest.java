import org.junit.jupiter.api.Test;

import java.util.*;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class UnitTest {
    @Test
    public void testGraphReader() {
        var gr = new GraphReader("src\\CA-HepTh.txt");
        var graph = gr.read(GraphReader.OUT);
        assertEquals(9877, graph.size(), "Test Nodes");

        int cnt = 0;
        for (var entry : graph.entrySet()) {
            var nodeList = entry.getValue();
            for (var node : nodeList)
                cnt++;
        }
        assertEquals(51971, cnt, "Test Edges");

        assertEquals(9877, gr.readNumberOfNodes());
        assertEquals(51971, gr.readNumberOfEdges());
    }

}
