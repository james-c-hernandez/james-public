public class CEmployee extends CPerson {
    private String employeeId;

    public CEmployee(String name, int age, String employeeId) {
        super(name,age);
        this.employeeId=employeeId;
    }

    public String getEmployeeId() {
        return this.employeeId;
    }

    public void setEmployeeId(String employeeId) {
        this.employeeId=employeeId;
    }

    // Override toString to include employeeId
    @Override
    public String toString() {
        return "Employee{" + super.toString() + ", employeeId='" + employeeId + "'}";
    }
}
