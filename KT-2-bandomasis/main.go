package main

import (
	"fmt"
	"sync"
)

func main() {
	// TODO: use one channel for workers
	group := sync.WaitGroup{}
	workerChan1 := make(chan int)
	workerChan2 := make(chan int)
	printer1 := make(chan int)
	printer2 := make(chan int)
	finish := make(chan int)

	group.Add(5)
	go worker(0, workerChan1, finish, &group)
	go worker(11, workerChan2, finish, &group)

	go receiver(workerChan1, workerChan2, printer1, printer2, finish, &group)

	go printer(printer1, &group)
	go printer(printer2, &group)

	group.Wait()
}

func worker(startIndex int, data chan int, finish <-chan int, group *sync.WaitGroup) {
	defer close(data)
	defer group.Done()

	for i := startIndex; ; i++ {
		select {
		case <-finish:
			return
		case data <- i:
		}
	}
}

func receiver(worker1 chan int, worker2 chan int, printer1 chan int, printer2 chan int, finish chan<- int, group *sync.WaitGroup) {
	defer close(printer1)
	defer close(printer2)
	defer close(finish)
	defer group.Done()

	counter := 0
	for {
		select {
		case item := <-worker1:
			if item%2 == 0 {
				printer1 <- item
			} else {
				printer2 <- item
			}
			counter++
		case item := <-worker2:
			if item%2 == 0 {
				printer1 <- item
			} else {
				printer2 <- item
			}
			counter++
		}

		if counter >= 20 {
			return
		}
	}
}

func printer(data <-chan int, group *sync.WaitGroup) {
	defer group.Done()

	for item := range data {
		fmt.Println(item)
	}
}
