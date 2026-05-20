package main

import (
	"fmt"
	"bufio"
	"os"
	"strconv"
)

func main() {
	number, err := strconv.Atoi(readLine("Enter number: "))
	if err != nil {
		os.Exit(1)
	}

	fmt.Printf("The number: %d\n", number)
}

func readLine(greeting string) string {
	fmt.Print(greeting)
	reader := bufio.NewReader(os.Stdin)
	text, _, _ := reader.ReadLine()
	return string(text)
}

