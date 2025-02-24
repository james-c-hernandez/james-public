import java.util.ArrayList;

public class CodeWorkout {
    public static void main(String[] args) {
        System.out.println("Hello from java CodeWorkout!");

        //Person object
        CPerson person = new CPerson("alice",20);
        System.out.println("Person.name=" + person.getName() + " person.age=" + person.getAge());
        CPerson person2 = new CPerson("Bob", 42);
        System.out.println("Person2: " + person2);

        // Create and print Employee objects
        CEmployee emp1 = new CEmployee("Alice", 30, "E123");
        CEmployee emp2 = new CEmployee("Bob", 25, "E456");
        
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