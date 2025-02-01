package arrayhandler

import "fmt"

/* An Array is a reference to an static array in memory.
Slices have to contain items of the same type.
Slices can be CAN NOT be resized */
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

/* A slice is a reference to an dynamic array in memory.
Slices have to contain items of the same type.
Slices can be resized by apppending items */
func ArraySlice() {

	s1 := make([]string, 5)
	s2 := make([]int, 5)

	s2 = append(s2, 10)

	fmt.Println("ArraySlice output", s1)
	fmt.Println(s2)
}

/* A Map is a reference to an dynamic collection of key-value pairs.
Maps have to contain items of the same type.
Maps can be resized by apppending items differenty than Slice */
func ArrayMap() {
	m1 := make(map[string]int)
	m1["Alice"] = 85
	m1["Bob"] = 92

	fmt.Println("ArrayMap output", m1)

	//Add more key-value pairs
	m1["Carrol"] = 78
	m1["Dave"] = 90

	fmt.Println("ArrayMap output", m1)
}
