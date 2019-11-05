package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"reflect"
	"sort"
	"strings"
	"sync"
	"time"
)

const filePath string = "data/IFF-7_2_MilisiunasJ_L1_dat_2.json"
const resultsFilePath string = "data/IFF-7_2_MilisiunasJ_L1_rez.txt"

const filterCondition int = 50000
const workersCount int = 4

//const bufferSize int = 5

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

	start := time.Now()

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

	// Retrieves filtered cars from result goroutine
	var filteredCars []Car
	for car := range results {
		filteredCars = append(filteredCars, car)
	}

	fmt.Println("Elapsed time:", time.Since(start))
	fmt.Printf("Filtered cars array size: %d", len(filteredCars))
	group.Wait()

	WriteResultsToFile(cars, filteredCars)
}

//ResultsRoutine Manages results array and send data to the main goroutine
func ResultsRoutine(filtered chan Car, resultsChan chan Car, group *sync.WaitGroup) {
	defer close(resultsChan)
	defer group.Done()

	var results []Car
	closedChannel := false

	for {
		var cases []reflect.SelectCase

		if !closedChannel {
			cases = append(cases, reflect.SelectCase{
				Dir:  reflect.SelectRecv,
				Chan: reflect.ValueOf(filtered),
			})
		} else {
			cases = append(cases, reflect.SelectCase{
				Dir:  reflect.SelectRecv,
				Chan: reflect.ValueOf(nil),
			})
		}

		if len(results) > 0 {
			cases = append(cases, reflect.SelectCase{
				Dir: reflect.SelectDefault,
			})
		}

		// Loop exit condition
		if !cases[0].Chan.IsValid() && len(cases) == 1 {
			return
		}

		chosen, item, ok := reflect.Select(cases)
		switch chosen {
		case 0:
			if !ok {
				closedChannel = true
			} else {
				results = append(results, item.Interface().(Car))
			}
		default:
			if closedChannel {
				resultsChan <- results[0]
				results = results[1:]
			}
		}
	}
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
		if len(data) <= int(n/2) && !channelClosed {
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

//WriteResultsToFile Writes original data and results data to the given results file in table format
func WriteResultsToFile(originalData []Car, results []Car) {
	file, err := os.OpenFile(resultsFilePath, os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		fmt.Printf("Failed writing to file %s", err)
	}

	dataWriter := bufio.NewWriter(file)

	// Table header row
	_, _ = dataWriter.WriteString(fmt.Sprintf("%55v\n", "Pradiniai duomenys"))
	_, _ = dataWriter.WriteString(strings.Repeat("-", 84) + "\n")
	_, _ = dataWriter.WriteString(fmt.Sprintf("|%20v|%20v|%20v|%20v|\n", "Gamintojas", "Pagaminimo metai",
		"Rida", "Filtravimo numeris"))
	_, _ = dataWriter.WriteString(strings.Repeat("-", 84) + "\n")

	for _, car := range originalData {
		_, _ = dataWriter.WriteString(car.toString() + "\n")
	}

	sort.Slice(results, func(i, j int) bool {
		return results[i].getNumber() < results[j].getNumber()
	})

	// Table header row
	_, _ = dataWriter.WriteString(fmt.Sprintf("\n\n%48v\n", "Rezultatas"))
	_, _ = dataWriter.WriteString(strings.Repeat("-", 84) + "\n")
	_, _ = dataWriter.WriteString(fmt.Sprintf("|%20v|%20v|%20v|%20v|\n", "Gamintojas", "Pagaminimo metai",
		"Rida", "Filtravimo numeris"))
	_, _ = dataWriter.WriteString(strings.Repeat("-", 84) + "\n")

	for _, car := range results {
		_, _ = dataWriter.WriteString(car.toString() + "\n")
	}

	_, _ = dataWriter.WriteString(fmt.Sprintf("Is viso masinu: %v", len(results)))

	_ = dataWriter.Flush()
	_ = file.Close()
}
