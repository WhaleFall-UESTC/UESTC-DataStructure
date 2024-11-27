import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        String filePath;
        if (args.length == 1) filePath =args[0];
        else filePath = "src\\CA-HepTh.txt";

        var pr = new PageRank(filePath);
        System.out.println(pr.accuracy());

    }

}
