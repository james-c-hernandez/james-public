/*
A prime number is a positive integer having exactly two factors,
 i.e. 1 and the number itself. If p is a prime, then its only factors
 are necessarily 1 and p itself.
*/

package prime

import (
	"math"
)

func IsPrime(n int) bool {
	if n < 2 {
		return false //0 1 is not prime
	}
	if n == 2 {
		return true //even numbers cannot be prime
	}
	if n%2 == 0 {
		return false
	}
	for i := 3; i <= int(math.Floor(math.Sqrt(float64(n)))); i += 2 {
		if n%i == 0 {
			return false
		}
	}
	return true
}

func ComputePrimes(n int) []int {
	primes := []int{}
	for i := 2; i <= n; i++ {
		if IsPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}
