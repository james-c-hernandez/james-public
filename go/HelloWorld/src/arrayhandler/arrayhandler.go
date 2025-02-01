package arrayhandler

import "fmt"

func Array() {
	// Array is a collection of items of the same type
	var arr1 [5]int
	arr1[0] = 10
	arr1[1] = 20
	arr1[2] = 30
	arr1[3] = 40
	arr1[4] = 50

	// or initialize array with values
	arr2 := [5]int{10, 20, 30, 40, 50}

	fmt.Println("array output1", arr1[0])
	fmt.Println(arr2)

}

func ArraySlice() {
	// Slice is a reference to an array
	// Slices can be resized
	s1 := make([]string, 5)
	s2 := make([]int, 5)

	s2 = append(s2, 10)

	fmt.Println(s1)
	fmt.Println(s2)
}

func ArrayMap() {
	// Map is a collection of key-value pairs
	m1 := make(map[string]int)
	m1["key1"] = 20
	m1["key2"] = 30
	m1["Some other key"] = 40

	fmt.Println(m1["key1"])

}
