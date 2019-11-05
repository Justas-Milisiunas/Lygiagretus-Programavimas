package main

import (
	"fmt"
	"reflect"
	"sync"
)

func main() {
	words := []string{"one", "two", "three", "four"}

	group := sync.WaitGroup{}
	main := make(chan string)
	dataChan := make(chan string)

	// Data worker
	group.Add(1)
	go DataWorkerRoutinee(main, dataChan, &group)
	//go func() {
	//	defer close(dataChan)
	//	defer group.Done()
	//	var data []string
	//
	//	channelClosed := false
	//	for {
	//		var cases []reflect.SelectCase
	//		if len(data) <= 1 && !channelClosed {
	//			cases = append(cases, reflect.SelectCase{
	//				Dir:  reflect.SelectRecv,
	//				Chan: reflect.ValueOf(main),
	//			})
	//		} else {
	//			cases = append(cases, reflect.SelectCase{
	//				Dir:  reflect.SelectRecv,
	//				Chan: reflect.ValueOf(nil),
	//			})
	//		}
	//
	//		if len(data) > 0 {
	//			cases = append(cases, reflect.SelectCase{
	//				Dir:  reflect.SelectDefault,
	//			})
	//		}
	//
	//		if !cases[0].Chan.IsValid() && len(cases) == 1 {
	//			fmt.Println("Data worker isejimas:")
	//			for _, item := range data {
	//				fmt.Println(item)
	//			}
	//
	//			fmt.Println("Data worker isejimas baigtas")
	//			return
	//		}
	//
	//		chosen, item, ok := reflect.Select(cases)
	//		switch chosen {
	//		case 0:
	//			if !ok {
	//				channelClosed = true
	//			} else {
	//				data = append(data, item.String())
	//			}
	//		default:
	//			if !ok && len(data) == 0 {
	//				return
	//			}
	//			temp := data[0]
	//			dataChan <- temp
	//			data = data[1:]
	//		}
	//		//select {
	//		//case word, ok := <-main:
	//		//	if !ok {
	//		//		fmt.Println("Data worker isejimas:")
	//		//		for _, item := range data {
	//		//			fmt.Println(item)
	//		//		}
	//		//
	//		//		fmt.Println("Data worker isejimas baigtas")
	//		//		return
	//		//	}
	//		//
	//		//	data = append(data, word)
	//		//default:
	//		//	if len(data) > 0 {
	//		//		dataChan <- data[0]
	//		//		data = data[1:]
	//		//	}
	//		//}
	//
	//	}
	//}()

	group.Add(1)
	go WorkerRoutinee(dataChan, &group)
	// Worker
	//go func() {
	//	defer group.Done()
	//	var items []string
	//	for item := range dataChan {
	//		items = append(items, item)
	//	}
	//
	//	fmt.Println("Worker isejimas:")
	//	for _, item := range items {
	//		fmt.Println(item)
	//	}
	//
	//	fmt.Println("Worker isejimo pabaiga")
	//}()

	for _, item := range words {
		main <- item
	}

	close(main)
	group.Wait() // Waits till all workers finish their jobs
	//close(filterCondition)
}

func ResultsRoutine(filtered chan string, results chan string, group sync.WaitGroup) {
	defer close(results)
	defer group.Done()
}

func WorkerRoutinee(dataChan chan string, group *sync.WaitGroup) {
	defer group.Done()
	var items []string
	for item := range dataChan {
		items = append(items, item)
	}

	fmt.Println("Worker isejimas:")
	for _, item := range items {
		fmt.Println(item)
	}

	fmt.Println("Worker isejimo pabaiga")
}

func DataWorkerRoutinee(main chan string, dataChan chan string, group *sync.WaitGroup) {
	defer close(dataChan)
	defer group.Done()
	var data []string

	channelClosed := false
	for {
		var cases []reflect.SelectCase
		if len(data) <= 1 && !channelClosed {
			cases = append(cases, reflect.SelectCase{
				Dir:  reflect.SelectRecv,
				Chan: reflect.ValueOf(main),
			})
		} else {
			cases = append(cases, reflect.SelectCase{
				Dir:  reflect.SelectRecv,
				Chan: reflect.ValueOf(nil),
			})
		}

		if len(data) > 0 {
			cases = append(cases, reflect.SelectCase{
				Dir:  reflect.SelectDefault,
			})
		}

		if !cases[0].Chan.IsValid() && len(cases) == 1 {
			fmt.Println("Data worker isejimas:")
			for _, item := range data {
				fmt.Println(item)
			}

			fmt.Println("Data worker isejimas baigtas")
			return
		}

		chosen, item, ok := reflect.Select(cases)
		switch chosen {
		case 0:
			if !ok {
				channelClosed = true
			} else {
				data = append(data, item.String())
			}
		default:
			if !ok && len(data) == 0 {
				return
			}
			temp := data[0]
			dataChan <- temp
			data = data[1:]
		}
		//select {
		//case word, ok := <-main:
		//	if !ok {
		//		fmt.Println("Data worker isejimas:")
		//		for _, item := range data {
		//			fmt.Println(item)
		//		}
		//
		//		fmt.Println("Data worker isejimas baigtas")
		//		return
		//	}
		//
		//	data = append(data, word)
		//default:
		//	if len(data) > 0 {
		//		dataChan <- data[0]
		//		data = data[1:]
		//	}
		//}

	}
}
