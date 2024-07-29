public class Main
{
    public static void main(String[] args) {
        for (int number = 2; number <= 100; number++) {
            isPrime(number);
        }
    }
    public static void isPrime(int number) {
        if (number < 2) {
            return;
        }
        for (int i = 2; i < number; i++) {
            if (number % i == 0) {
                return;
            }
        }
        System.out.println(number);
    }
}