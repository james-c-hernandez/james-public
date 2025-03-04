import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class CLists {
    public static void Array() {
        int []myarray = {5,7,3,2,8};
        System.out.println(myarray);
    }

    public static void ArraySlice() {
        ArrayList<Integer> mylist = new ArrayList<>();
        mylist.add(5);
        mylist.add(7);
        mylist.add(3);
        mylist.add(2);
        mylist.add(8);
        System.out.println(mylist);
    }

    public static Map<String, Integer> ArrayMap() {
        Map<String, Integer> map = new HashMap<>();
        map.put("Alice", 25);
        map.put("Bob", 37);
        return map;
    } 

    /**
    * Sorts an ArrayList of integers in ascending order using the Bubble Sort algorithm.
    * 
    * @return A new ArrayList containing the sorted integers.
    */
    public static ArrayList<Integer> SortArrayInt() {
        // Initialize ArrayList with predefined values
        ArrayList<Integer> numbers = new ArrayList<>(List.of(5, 7, 3, 2, 8));

        // Bubble Sort: Repeatedly step through the list, compare adjacent elements,
        // and swap them if they are in the wrong order
        for (int pass=0; pass< numbers.size()-1; pass++) {
            for(int currentIndex=0; currentIndex< numbers.size()-pass-1;currentIndex++) {
                if (numbers.get(currentIndex) > numbers.get(currentIndex+1)) {
                    // Swap elements if the current element is greater than the next
                    int temp = numbers.get(currentIndex);
                    numbers.set(currentIndex,numbers.get(currentIndex+1));
                    numbers.set(currentIndex+1,temp);
                }
            }
        }
        return numbers;
    }

}
