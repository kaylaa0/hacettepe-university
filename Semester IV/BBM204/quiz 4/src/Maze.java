/*
 Hacettepe University
 Kayla Akyüz
 21726914
*/

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Maze {
    private int size;
    private int[] data;
    private int[] bestFoundPath;
    private int bestFoundPathValue;
    private int[] currentPath;
    private int currentPathValue;

    public Maze() {
        this.readFile();
        this.bestFoundPath = new int[(this.size * 2) - 1];
        this.currentPath = new int[(this.size * 2) - 1];
        this.bestFoundPathValue = -1111;
        this.currentPathValue = 0;
        this.move(0, 0);
        System.out.println(this.toString());
    }

    private void readFile() {
        // My reading method.
        List<String> result;
        try (Stream<String> lines = Files.lines(Paths.get("input.txt"))) {
            result = lines.collect(Collectors.toList());
        } catch (IOException e) {
            result = new ArrayList<String>();
            System.err.println("Couldn't read (possibly find) input.txt");
        }
        this.size = Integer.parseInt(result.get(0));
        this.data = new int[this.size * this.size];

        for (int i = 1; i < result.size(); i++) {
            if (result.get(i).equals("")) {
                continue;
            } else if (result.get(i).equals(" ")) {
                continue;
            }
            String[] parts = result.get(i).split(",");
            for (int p = 0; p < parts.length; p++) {
                this.data[((i - 1) * this.size) + p] = Integer.parseInt(parts[p]);
            }
        }
    }

    private void overRide() {
        for (int i = 0; i < this.currentPath.length; i++) {
            this.bestFoundPath[i] = this.currentPath[i];
        }
        this.bestFoundPathValue = this.currentPathValue;
    }

    private void move(int steppingOn, int step) {
        if (steppingOn == this.size * this.size - 1) {
            this.currentPath[step] = steppingOn;
            // Reached to the end, will see if our current path value is better than current
            // best value if so override.
            if (this.bestFoundPathValue == -1111) {
                this.overRide();
            } else if (this.currentPathValue < this.bestFoundPathValue) {
                this.overRide();
            }
            return;
        } else if (steppingOn % this.size == this.size - 1) {
            this.currentPath[step] = steppingOn;
            this.currentPathValue += this.data[steppingOn];
            this.move(steppingOn + this.size, step + 1);
            this.currentPathValue -= this.data[steppingOn];
            return;
        } else if (steppingOn + this.size > (this.size * this.size) - 1) {
            this.currentPath[step] = steppingOn;
            this.currentPathValue += this.data[steppingOn];
            this.move(steppingOn + 1, step + 1);
            this.currentPathValue -= this.data[steppingOn];
            return;
        } else {
            this.currentPath[step] = steppingOn;
            this.currentPathValue += this.data[steppingOn];
            this.move(steppingOn + 1, step + 1);
            this.move(steppingOn + this.size, step + 1);
            this.currentPathValue -= this.data[steppingOn];
            return;
        }
    }

    @Override
    public String toString() {
        StringBuilder string = new StringBuilder("Path:");
        for (int i = 0; i < this.bestFoundPath.length - 1; i++) {
            string.append(this.bestFoundPath[i]);
            string.append("->");
            string.append(this.bestFoundPath[i + 1]);
            string.append(" ");
        }
        return string.toString();
    }
}
