package main

import (
	"fmt"
	"bufio"
	"os"
	"strconv"
)

func main() {
	number, err := strconv.Atoi(readLine("Введите число: "))
	if err != nil {
		os.Exit(1)
	}

	fmt.Printf("Ваше число: %d\n", number)
}

func readLine(greeting string) string {
	fmt.Print(greeting)
	reader := bufio.NewReader(os.Stdin)
	text, _, _ := reader.ReadLine()
	return string(text)
}

