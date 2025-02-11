package main

import (
	"fmt"
)

func main() {
	fmt.Println("Hello world!")

	fmt.Print("Fibonacci using recursion=")
	for i := 0; i < 8; i++ {
		fmt.Print(fibonacciRecursion(i), ",")
	}
	fmt.Println("")

	fmt.Print("Fibonacci using three variables=")
	fibonacciThreeVars(8)
	fmt.Println("")

	fmt.Println("FibonacciPreallocatedArray(", 8, ")=", fibonacciPreallocatedArray(8))

	fmt.Println("FibonacciDynamicallyAllocatedArray=(", 8, ")= ", fibonacciDynamicallyAllocatedArray(8))

}

func fibonacciRecursion(n int) int {
	if n <= 1 {
		return n
	}
	return fibonacciRecursion(n-1) + fibonacciRecursion(n-2)
}

func fibonacciThreeVars(n int) {
	t1 := 0
	t2 := 1
	fmt.Print("0,1,")
	for i := 2; i < n; i++ {
		t3 := t1 + t2
		fmt.Print(t3, ",")
		t1 = t2
		t2 = t3
	}
}

func fibonacciPreallocatedArray(n int) []int {
	slice := make([]int, n)
	slice[1] = 1
	for i := 2; i < n; i++ {
		slice[i] = slice[i-2] + slice[i-1]
	}
	return slice
}

func fibonacciDynamicallyAllocatedArray(n int) []int {
	slice := make([]int, 2)
	slice[1] = 1

	for i := 2; i < n; i++ {
		slice = append(slice, slice[i-2]+slice[i-1])
	}
	return slice
}
