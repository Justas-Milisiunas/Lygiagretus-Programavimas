package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"reflect"
	"sync"
)

const filePath string = "data/IFF-7_2_MilisiunasJ_L1_dat_1.json"
const filterCondition int = 50000
const workersCount int = 4
const bufferSize int = 5

func main() {
	// Reads json data and parses it to Car array type
	data := ReadData(filePath)
	cars := ParseJsonData(data)
	n := len(cars)

	// Counters for synchronization
	group := sync.WaitGroup{}
	workers := sync.WaitGroup{}

	// Channels initialization
	main := make(chan Car)
	dataChan := make(chan Car)
	filtered := make(chan Car)
	results := make(chan Car)

	// Starts data array and results array management goroutines
	group.Add(2)
	go DataWorkerRoutine(n, main, dataChan, &group)
	go ResultsRoutine(filtered, results, &group)

	// Starts workers
	workers.Add(workersCount)
	for i := 0; i < workersCount; i++ {
		go WorkerRoutine(dataChan, filtered, &workers)
	}

	// Sends data items one by one to the data management goroutine
	for _, car := range cars {
		main <- car
	}

	// Closes channels for stopping goroutines
	close(main)
	workers.Wait()
	close(filtered)

	//var filteredCars []Car
	//for car := range results {
	//	filteredCars = append(filteredCars, car)
	//}

	group.Wait()
}

//ResultsRoutine Manages results array and send data to the main goroutine
func ResultsRoutine(filtered chan Car, resultsChan chan Car, group *sync.WaitGroup) {
	defer close(resultsChan)
	defer group.Done()

	var results []Car
	for car := range filtered {
		results = append(results, car)
	}

	fmt.Println("Rezultatu isejimas:")
	for _, item := range results {
		item.print()
	}
	fmt.Println("Rezultatu isejimo pabaiga")

}

//WorkerRoutine Filters given data and sends it to the results goroutine
func WorkerRoutine(dataChan chan Car, filtered chan Car, group *sync.WaitGroup) {
	defer group.Done()

	var cars []Car
	for car := range dataChan {
		cars = append(cars, car)
		//filtered <- car
		if car.getNumber() <= filterCondition {
			filtered <- car
		}
	}

	fmt.Println("Worker isejimas:")
	for _, item := range cars {
		item.print()
	}

	fmt.Println("Worker isejimo pabaiga")
}

//DataWorkerRoutine Manages given data from the main and sends it to the workers
func DataWorkerRoutine(n int, main chan Car, dataChan chan Car, group *sync.WaitGroup) {
	defer close(dataChan)
	defer group.Done()
	var data []Car

	channelClosed := false
	for {
		// Activates and deactivates channels
		var cases []reflect.SelectCase

		// If data array is full deactivates channel from the main
		if len(data) <= int(n / 2) && !channelClosed {
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

		// If there is data in the data array, opens channel to workers
		if len(data) > 0 {
			cases = append(cases, reflect.SelectCase{
				Dir: reflect.SelectDefault,
			})
		}

		// Loop exit condition
		if !cases[0].Chan.IsValid() && len(cases) == 1 {
			fmt.Println("Data worker isejimas:")
			for _, item := range data {
				item.print()
			}

			fmt.Println("Data worker isejimas baigtas")
			return
		}

		// Dynamic select for managing channels
		chosen, item, ok := reflect.Select(cases)
		switch chosen {
		case 0:
			if !ok {
				channelClosed = true
			} else {
				data = append(data, item.Interface().(Car))
			}
		default:
			dataChan <- data[0]
			data = data[1:]
		}
	}
}

// Parse json data to Car array
func ParseJsonData(jsonData []byte) []Car {
	var cars []Car
	err := json.Unmarshal([]byte(jsonData), &cars)
	if err != nil {
		log.Fatal(err)
	}

	return cars
}

// Reads data in bytes from given file
func ReadData(filePath string) []byte {
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	return data
}
