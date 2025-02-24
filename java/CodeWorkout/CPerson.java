public class CPerson {
    private String name;
    private int age;

    public CPerson(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return this.name;
    }

    public int getAge() {
        return this.age;
    }
    @Override
    public String toString() {
        return "name='" + name + "', age=" + age;
    }
}
