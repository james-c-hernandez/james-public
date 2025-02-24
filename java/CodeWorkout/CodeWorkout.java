import java.util.ArrayList;

public class CodeWorkout {
    public static void main(String[] args) {
        System.out.println("Hello from java CodeWorkout!");

        //Person object
        Person person = new Person("alice",20);
        System.out.println("Person.name=" + person.getName() + " person.age=" + person.getAge());
        Person person2 = new Person("Bob", 42);
        System.out.println("Person2: " + person2);

        // Create and print Employee objects
        Employee emp1 = new Employee("Alice", 30, "E123");
        Employee emp2 = new Employee("Bob", 25, "E456");
        
        // Test the objects
        System.out.println("Employee 1: " + emp1);
        System.out.println("Employee 2: " + emp2);

        CMath myMath = new CMath();
        ArrayList<Integer> x = myMath.fibonacci(13);
        System.out.println(x);

        ArrayList<Integer> y = myMath.prime(13);
        System.out.println(y);

    }
}