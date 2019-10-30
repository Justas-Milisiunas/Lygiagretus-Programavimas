package main

import (
	"encoding/json"
	"io/ioutil"
	"log"
)

func main() {
	filePath := "data/IFF-7_2_MilisiunasJ_L1_dat_1.json"
	data := ReadData(filePath)
	cars := ParseJsonData(data)

	print(cars[0].Brand)

}

func ParseJsonData(jsonData []byte) []Car {
	var cars []Car
	err := json.Unmarshal([]byte(jsonData), &cars)
	if err != nil {
		log.Fatal(err)
	}

	return cars
}

func ReadData(filePath string) []byte {
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	return data
}
