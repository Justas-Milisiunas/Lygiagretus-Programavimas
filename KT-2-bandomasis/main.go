package main

import (
	"fmt"
	"sync"
)

func main() {
	group := sync.WaitGroup{}
	workers := sync.WaitGroup{}

	data := make(chan int)
	printer1 := make(chan int)
	printer2 := make(chan int)
	finish := make(chan int)

	workers.Add(2)
	group.Add(3)

	// Starts workers
	go worker(0, data, finish, &workers)
	go worker(11, data, finish, &workers)

	// Starts receiver
	go receiver(data, printer1, printer2, finish, &group)

	// Starts printers
	go printer(printer1, &group, &workers)
	go printer(printer2, &group, &workers)

	// Waits until each go routine job is one
	group.Wait()
}

// Prints numbers from given start index until signal from the finish channel
func worker(startIndex int, data chan int, finish <-chan int, workers *sync.WaitGroup) {
	defer workers.Done()

	for i := startIndex; ; i++ {
		select {
		case <-finish:
			return
		case data <- i:
		}
	}
}

// Receives numbers from workers, filters them for the print channels
func receiver(data chan int, printer1 chan int, printer2 chan int, finish chan<- int, group *sync.WaitGroup) {
	defer close(printer1)
	defer close(printer2)
	defer close(finish)
	defer group.Done()

	counter := 0
	for {
		item := <-data
		if item%2 == 0 {
			printer1 <- item
			counter++
		} else {
			printer2 <- item
			counter++
		}

		if counter >= 20 {
			break
		}
	}
}

// Prints number from the data channel
func printer(data <-chan int, group *sync.WaitGroup, workers *sync.WaitGroup) {
	defer group.Done()

	var results []int
	for item := range data {
		results = append(results, item)
	}

	workers.Wait()
	fmt.Println(results)
}
