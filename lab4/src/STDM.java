import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Stream;

public class STDM {
    private int duration = 0;
    private double inputRate;

    // Read input.txt file to Java stream
    private Map<Character, List<String>> readFile(String fileName) {
        Map<Character, List<String>> res = new HashMap<>();
        try (Stream<String> stream = Files.lines(Paths.get(fileName))) {
            stream.forEach(item -> parseSource(item, res));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return res;
    }

    //Using HashMap to store <Source, Time
    private void parseSource(String source, Map<Character, List<String>> res) {
        String[] cmps = source.split(":");
        char srcName = cmps[0].charAt(6);
        String srcItem = cmps[1];
        String[] frame = srcItem.split(",");
        int lastFrameIndex = Integer.parseInt(String.valueOf(frame[frame.length - 1].charAt(3)));
        duration = Math.max(duration, lastFrameIndex);
        List<String> frames = new ArrayList<>(Arrays.asList(frame));
        res.put(srcName, frames);
    }

    private int[][] inputBuffer(int numOfSrc, Map<Character, List<String>> src) {
        int[][] inBuf = new int[numOfSrc][duration];
        double numbOfPacket = 0;
        for (Map.Entry<Character, List<String>> pair : src.entrySet()) {
            int srcNum = pair.getKey() - 'A';
            List<String> frames = pair.getValue();
            for (String frame : frames) {
                String[] f = frame.substring(1).split(" ");
                int startTime = Integer.parseInt(f[0]);
                int endTime = Integer.parseInt(f[1]);
                int block = Integer.parseInt(String.valueOf(f[2].charAt(1)));
                for (int i = startTime; i < endTime; i++) inBuf[srcNum][i] = block;
                numbOfPacket++;
            }
        }
        inputRate = numbOfPacket / duration;
        return inBuf;
    }

    /* Convert a decimal into binary representation */
    private String toBinary(int num, int len) {
        StringBuilder res = new StringBuilder();
        for (int i = 0; i < len; i++) {
            res.append(num % 2);
            num /= 2;
        }
        return res.reverse().toString();
    }

    private void outputBuffer(int[][] inBuf) {
        String startFlag = "1", endFlag = "1";
        int outputRate = (int) Math.ceil(inputRate);
        int addrLen = (int) Math.ceil(Math.log(inBuf.length) / Math.log(2));
        Queue<Pair<Integer, Integer>> buffer = new ArrayDeque<>();

        System.out.println("Frame#  " + "TimePeriod  " + "    Frame");
        for (int t = 0; t < duration || !buffer.isEmpty(); t++) {
            StringBuilder frame = new StringBuilder(startFlag + ' ');
            int ct = 0;
            for (int i = 0; i < inBuf.length; i++) {
                int curr = inBuf[i][t];
                if (curr != 0) buffer.add(new Pair<>(i, curr));
            }
            int count = 0;
            while (ct++ < outputRate) {
                if (!buffer.isEmpty()) {
                    Pair<Integer, Integer> last = buffer.poll();
                    frame.append(toBinary(last.src, addrLen));
                    frame.append(String.valueOf((char) (65 + last.src)));
                    frame.append(last.val);
                    frame.append(' ');
                    count++;
                }
            }
            if (count == 2) {
                frame.append(' ' + endFlag);
            } else if (count == 1) {
                frame.append("      " + endFlag);
            } else {
                frame.append("           " + endFlag);
            }
            System.out.println("Frame" + t +"   (" + t + " - " + (t+1) + ")    " + frame.toString());
        }
    }

    public static void main(String args[]) {
        STDM s = new STDM();
        Map<Character, List<String>> src = s.readFile(args[0]);
        int[][] inBuffer = s.inputBuffer(src.size(), src);
        s.outputBuffer(inBuffer);
    }

    private class Pair<A, B> {
        private A src;
        private B val;

        public Pair(A src, B val) {
            super();
            this.src = src;
            this.val = val;
        }
    }
}
