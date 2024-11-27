import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        List<String> filePaths = getFilePaths("test");
//        testFile("src\\CA-HepTh.txt");
        for (String filePath : filePaths) {
            testFile(filePath);
        }
    }

    private static void testFile(String filePath) {
        var pr = new PageRank(filePath);
        var pageRankResult = pr.rank();
        System.out.println("Test " + filePath);
        System.out.println("Page Rank Result: Largest 15 PR:");
        System.out.println(pageRankResult.getLargestNPage(15));
        var testResult = pr.testSIR();
        printTest("Page Rank", testResult[0]);
        printTest("Randomly Select", testResult[2]);
        System.out.println("\n----------------------------------------------------------\n\n\n");
    }

    private static void printTest(String test, double res) {
        System.out.printf("Test %s, the infection ratio is: %f\n", test, res);
    }

    private static List<String> getFilePaths(String directoryPath) {
        List<String> filePaths = new ArrayList<>();
        File directory = new File(directoryPath);
        if (directory.exists() && directory.isDirectory()) {
            File[] files = directory.listFiles();
            if (files != null) {
                for (File file : files) {
                    if (file.isFile()) {
                        filePaths.add(file.getAbsolutePath());
                    }
                }
            }
        }
        return filePaths;
    }
}
