package main

import "fmt"

type Car struct {
	Brand    string
	MakeYear int
	Mileage  float32
}

func (c *Car) toString() string {
	//fmt.Printf("%s %d %f\n", c.Brand, c.MakeYear, c.Mileage)
	return fmt.Sprintf("|%20v|%20v|%20v|%20v|", c.Brand, c.MakeYear, c.Mileage, c.getNumber())
}

func (c *Car) getNumber() int {
	number := c.MakeYear + int(c.Mileage) + int(c.Brand[0])
	return c.fibonacci(number % 40)
}

func (c *Car) fibonacci(n int) int {
	if n <= 1 {
		return n
	}

	return c.fibonacci(n-1) + c.fibonacci(n-2)
}
