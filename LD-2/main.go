package files

import (
	"encoding/json"
	"io/ioutil"
	"log"
)

func main() {
	filePath := "data/IFF-7_2_MilisiunasJ_L1_dat_1.json"
	data := readData(filePath)

	var cars []Car
	json.Unmarshal([]byte(data), &cars)
	print(cars)
}

func readData(filePath string) []byte {
	data, err := ioutil.ReadFile(filePath)
	if err != nil {
		log.Fatal(err)
	}

	return data
}
