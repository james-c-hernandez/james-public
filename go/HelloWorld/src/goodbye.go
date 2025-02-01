package main

import "fmt"

// Needs to be capitalized to be exported,  visiable under the Package namespace main
func GoodBye() {
	fmt.Println("Goodbye, this is called from file goodbye.go in same folder as main.go")
}
