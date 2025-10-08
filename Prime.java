public class Prime {
    public static void sieveAndPrint(int limit, int perLine) {
        if (limit < 2) {
            return;
        }
        boolean[] isPrime = new boolean[limit + 1];
        for (int i = 2; i <= limit; i++) {
            isPrime[i] = true;
        }
        for (int p = 2; p * p <= limit; p++) {
            if (isPrime[p]) {
                for (int i = p * p; i <= limit; i += p) {
                    isPrime[i] = false;
                }
            }
        }
        int count = 0;

        for (int i = 2; i <= limit; i++) {
            if (isPrime[i]) {
                System.out.printf("%d", i);
                count++;
                if (count % perLine == 0) {
                    System.out.println();
                }
            }
        }

        if (count % perLine != 0) {
            System.out.println();
        }
    }

    public static void main(String[] args) {
        sieveAndPrint(20000, 5);
    }
}