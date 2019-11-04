package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"sync"
)

const filePath string = "data/IFF-7_2_MilisiunasJ_L1_dat_2.json"
const filterCondition int = 50000
const bufferSize int = 5

func main() {
	data := ReadData(filePath)
	cars := ParseJsonData(data)
	//n := len(cars)

	group := sync.WaitGroup{}
	group.Add(4)

	// Channel creation
	dataChan := make(chan Car, 100)
	filtered := make(chan Car, 100)

	// Routines starting
	go WorkerRoutine(dataChan, filtered, &group)
	go WorkerRoutine(dataChan, filtered, &group)
	go WorkerRoutine(dataChan, filtered, &group)
	go WorkerRoutine(dataChan, filtered, &group)
	for _, car := range cars {
		dataChan <- car // Sends car to data channel
	}

	close(dataChan)

	// TODO: delete after creating results routine work
	go func() {
		counter := 0
		for car := range filtered {
			car.print()
			counter++
			fmt.Println(counter)
		}
	} ()

	group.Wait()
	close(filtered)

	fmt.Println("Work finished!")
}

// Receives cars from data channel, filters it, sends to filtered channel
func WorkerRoutine(data <-chan Car, filtered chan<- Car, group *sync.WaitGroup) {
	defer group.Done()

	for car := range data {
		if car.getNumber() <= filterCondition {
			filtered <- car
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
