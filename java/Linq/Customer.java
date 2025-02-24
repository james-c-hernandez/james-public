import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

public class Customer {
    private String name;
    private int age;

    // Constructor
    public Customer(String name, int age) {
        this.name = name;
        this.age = age;
    }

    // Getters
    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public static void main(String[] args) {
        // Sample list of customer objects
        List<Customer> customers = new ArrayList<>();
        customers.add(new Customer("Alice", 30));
        customers.add(new Customer("Bob", 25));
        customers.add(new Customer("Charlie", 35));
        customers.add(new Customer("David", 40));
        customers.add(new Customer("Eve", 20));

        // Query to select the customer with the third highest age
        List<Customer> sortedCustomers = customers.stream()
            .sorted(Comparator.comparingInt(Customer::getAge).reversed())
            .collect(Collectors.toList());

        // Get the third highest age customer
        Customer thirdHighestAgeCustomer = sortedCustomers.size() >= 3 ? sortedCustomers.get(2) : null;

        // Output the result
        if (thirdHighestAgeCustomer != null) {
            System.out.println("The customer with the third highest age is: " +
                thirdHighestAgeCustomer.getName() + ", Age: " + thirdHighestAgeCustomer.getAge());
        } else {
            System.out.println("There are fewer than three customers in the list.");
        }
    }
}
