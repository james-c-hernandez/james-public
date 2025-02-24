public class Order {
    private int orderId;
    private int customerId;
    private java.sql.Date orderDate;
    private double amount;

    // Getters and Setters
    public int getOrderId() { return orderId; }
    public void setOrderId(int orderId) { this.orderId = orderId; }

    public int getCustomerId() { return customerId; }
    public void setCustomerId(int customerId) { this.customerId = customerId; }

    public java.sql.Date getOrderDate() { return orderDate; }
    public void setOrderDate(java.sql.Date orderDate) { this.orderDate = orderDate; }

    public double getAmount() { return amount; }
    public void setAmount(double amount) { this.amount = amount; }
}
