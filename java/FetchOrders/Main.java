import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Comparator;

public class Main {
    public static void main(String[] args) {
        String url = "jdbc:postgresql://192.168.1.201:5432/postgres";
        String user = "postgres";
        String password = "postgres";

        List<Order> orders = new ArrayList<>();

        try {
            // Establish connection
            Class.forName(org.postgresql.Driver); 
            Connection connection = DriverManager.getConnection(url, user, password);
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT * FROM orders");

            // Fetch data and populate orders list
            while (resultSet.next()) {
                Order order = new Order();
                order.setOrderId(resultSet.getInt("order_id"));
                order.setCustomerId(resultSet.getInt("customer_id"));
                order.setOrderDate(resultSet.getDate("order_date"));
                order.setAmount(resultSet.getDouble("amount"));
                orders.add(order);
            }

            // Close connection
            resultSet.close();
            statement.close();
            connection.close();

            // Find the order with the 5th highest amount
            orders.stream()
                .sorted(Comparator.comparingDouble(Order::getAmount).reversed())
                .skip(4) // Skip the first 4 highest
                .findFirst()
                .ifPresent(order -> {
                    System.out.println("Order ID: " + order.getOrderId());
                    System.out.println("Customer ID: " + order.getCustomerId());
                    System.out.println("Order Date: " + order.getOrderDate());
                    System.out.println("Amount: " + order.getAmount());
                });

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
