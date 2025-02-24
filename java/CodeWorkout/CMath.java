import java.util.ArrayList;

public class CMath {
   public static ArrayList<Integer> fibonacci(int n) {
     // Initialize the dynamic array as nums
     ArrayList<Integer> nums = new ArrayList();
     nums.add(0);
     nums.add(1);
     for (int x=2; x<n; x++) {
         nums.add(nums.get(x-1) + nums.get(x-2));
     }
     return nums;
   } 

   public static ArrayList<Integer> primeold(int n) {
    ArrayList<Integer> nums = new ArrayList<>();
    for (int x = 2; x < n; x++) {
        boolean isPrime = true; // Assume x is prime until proven otherwise
        for (int i = 2; i <= Math.sqrt(x); i++) { // Check divisors up to sqrt(x)
            if (x % i == 0) { // If x is divisible by i, it’s not prime
                isPrime = false;
                break; // No need to check further divisors
            }
        }
        if (isPrime) { // If still true, x is prime
            nums.add(x);
        }
    }
    return nums;
   }

   public static ArrayList<Integer> prime(int n) {
    ArrayList<Integer> nums = new ArrayList<>();
    int x = 2; // Start at the first prime
    while (nums.size() < n) { // Continue until we have n primes
        boolean isPrime = true; // Assume x is prime
        for (int i = 2; i <= Math.sqrt(x); i++) { // Check divisors up to sqrt(x)
            if (x % i == 0) { // If divisible, not prime
                isPrime = false;
                break;
            }
        }
        if (isPrime) { // If x is prime, add it
            nums.add(x);
        }
        x++; // Move to the next number
    }
    return nums;
  }
}
