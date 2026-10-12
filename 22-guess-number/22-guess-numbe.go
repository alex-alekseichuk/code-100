package main

import (
    "bufio"
    "fmt"
    "math/rand"
    "os"
    "strconv"
    "strings"
    "time"
)

func main() {
    // Seed the random number generator
    // Note: In Go 1.20+, rand.Seed is no longer required for basic usage,
    // but it's good practice for older versions or specific distributions.
    rand.Seed(time.Now().UnixNano())

    min := 1
    max := 100
    target := rand.Intn(max-min+1) + min

    scanner := bufio.NewScanner(os.Stdin)
    attempts := 0

    fmt.Println("=== Welcome to the Guess the Number Game! ===")
    fmt.Printf("I have chosen a number between %d and %d.\n", min, max)
    fmt.Println("Can you guess it?")

    for {
        fmt.Printf("\nEnter your guess: ")
        
        // Read input from standard input
        if !scanner.Scan() {
            break
        }
        input := scanner.Text()

        // Clean the input (remove spaces/newlines)
        input = strings.TrimSpace(input)

        // Convert string to integer
        guess, err := strconv.Atoi(input)
        if err != nil {
            fmt.Println("Invalid input! Please enter a whole number.")
            continue
        }

        attempts++

        if guess < target {
            fmt.Println("Too low! Try again.")
        } else if guess > target {
            fmt.Println("Too high! Try again.")
        } else {
            fmt.Printf("Congratulations! You guessed it in %d attempts!\n", attempts)
            break
        }
    }

    fmt.Println("Thanks for playing!")
}
