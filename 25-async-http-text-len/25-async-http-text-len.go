package main

import (
	"fmt"
	"io"
	"net/http"
	"time"
)

type result struct {
	index int
	url   string
	size  int
	err   error
}

func getData(client *http.Client, index int, url string) result {
	res, err := client.Get(url)
	if err != nil {
		return result{index: index, url: url, err: err}
	}
	defer res.Body.Close()

	body, err := io.ReadAll(res.Body)
	if err != nil {
		return result{index: index, url: url, err: err}
	}

	if res.StatusCode < 200 || res.StatusCode >= 300 {
		return result{
			index: index,
			url:   url,
			err:   fmt.Errorf("HTTP status: %s", res.Status),
		}
	}

	return result{
		index: index,
		url:   url,
		size:  len(body),
	}
}

func main() {
	urls := []string{
		"https://jsonplaceholder.typicode.com/posts/1",
		"https://http.codes/200",
	}

	client := &http.Client{
		Timeout: 10 * time.Second,
	}

	results := make(chan result, len(urls))

	for i, url := range urls {
		go func(i int, url string) {
			results <- getData(client, i, url)
		}(i, url)
	}

	ordered := make([]result, len(urls))

	for range urls {
		r := <-results
		ordered[r.index] = r
	}

	for _, r := range ordered {
		if r.err != nil {
			fmt.Printf("%s: error: %v\n", r.url, r.err)
			continue
		}

		fmt.Printf("%s: %d bytes\n", r.url, r.size)
	}
}
