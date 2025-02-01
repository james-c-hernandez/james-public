/*
Package main package that geneates the main executable.
to get started: go mod init project-layout  //defines the packages and versions used in the project
this is done by go mod init project-layout

to run the app: go run src/main.go

go only allows you to set one package per directory level, in this case we created local packages of
hello and arrayhander in the src directory.
*/
package main

/* personal note: Co-Pilot editor automatically imports used packages ,
deletes unused packages */
import (
	"fmt"
	"project-layout/src/arrayhandler"
	"project-layout/src/fibonacci"
	"project-layout/src/hello"
	"project-layout/src/prime"

	"github.com/enescakir/emoji"
)

// Employee struct
type Employee struct {
	Name string
	ID   string
}

// Manager struct
type Manager struct {
	Employee
	Reports []Employee
}

// this is the main function of the app
func main() {
	fmt.Println("Hello World from Go!")
	hello.Hello()
	fmt.Println("Sum of 5 and 6 is", sum(5, 6))

	fmt.Println(emoji.PersonShrugging)

	fmt.Println("Array, Slice and Map examples start")
	arrayhandler.Array()
	arrayhandler.ArraySlice()
	arrayhandler.ArrayMap()
	fmt.Println("Array, Slice and Map examples end\n")

	// Compute and print n prime numbers
	n := 10
	primeNumbers := prime.ComputePrimes(n)
	fmt.Printf("Prime numbers up to %d are %v\n", n, primeNumbers)

	// Compute and print n fibonacci numbers
	n = 10
	fibonacciNumbers := fibonacci.ComputeFibonacci(n)
	fmt.Printf("Fibonacci numbers up to %d are %v\n", n, fibonacciNumbers)

	m := Manager{
		Employee: Employee{
			Name: "John DoeMgr",
			ID:   "12345",
		},
		Reports: []Employee{},
	}

	fmt.Println(m)
	GoodBye()
}

func (e Employee) Description() {
	fmt.Printf("Name: %s ID: %s\n", e.Name, e.ID)
}

func (m Manager) FindNewEmployee() []Employee {
	return m.Reports
}

func sum(x, y int) int {
	return x + y
}
