package main

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
	"sort"
)

func showRecord(out io.Writer, name string, size int64, prefix string, isRoot bool, isLast bool, isDir bool) error {
	if isRoot {
		return nil
	}

	firstChar := '├'
	if isLast {
		firstChar = '└'
	}

	suffix := ""
	if !isDir {
		suffix = " (empty)"
		if size > 0 {
			suffix = fmt.Sprintf(" (%db)", size)
		}
	}

	_, err := fmt.Fprintf(out, "%s%c───%s%s\n", prefix, firstChar, name, suffix)

	return err
}

func processDir(out io.Writer, printFiles bool, path string, f *os.File, prefix string, isRoot bool, isLast bool) error {
	fis, err := f.Readdir(0)
	if err != nil {
		return fmt.Errorf("readdir %s: %w", path, err)
	}

	sort.Slice(fis, func(i, j int) bool {
		return fis[i].Name() < fis[j].Name()
	})

	if !printFiles {
		dirs := fis[:0] // reuse backing array
		for _, e := range fis {
			if e.IsDir() {
				dirs = append(dirs, e)
			}
		}
		fis = dirs
	}

	if !isRoot {
		if isLast {
			prefix = prefix + "\t"
		} else {
			prefix = prefix + "│\t"
		}
	}
	
	n := len(fis)
	for i, fi := range fis {
		if i < n-1 {
			isLast = false
		} else {
			isLast = true
		}
		err := processFile(out, printFiles, filepath.Join(path, fi.Name()), prefix, false, isLast)
		if err != nil {
			return fmt.Errorf("process %s: %w", fi.Name(), err)
		}
	}

	return nil
}

func processFile(out io.Writer, printFiles bool, path string, prefix string, isRoot bool, isLast bool) error {
	f, err := os.Open(path)
	if err != nil {
		return fmt.Errorf("open %s: %w", path, err)
	}
	defer f.Close()

	fi, err := f.Stat()
	if err != nil {
		return fmt.Errorf("stat %s: %w", f.Name(), err)
	}
	size := fi.Size()
	name := fi.Name()

	switch mode := fi.Mode(); {
	case mode.IsRegular():
		return showRecord(out, name, size, prefix, isRoot, isLast, false)
	case mode.IsDir():
		err = showRecord(out, name, 0, prefix, isRoot, isLast, true)
		if err != nil {
			return fmt.Errorf("show directory %s %s: %w", path, name, err)
		}
		return processDir(out, printFiles, path, f, prefix, isRoot, isLast)
	}

	return nil
}

func dirTree(out io.Writer, path string, printFiles bool) error {
	err := processFile(out, printFiles, path, "", true, true)
	if err != nil {
		return fmt.Errorf("process file %s: %w", path, err)
	}
	return nil
}

func main() {
	out := os.Stdout
	if !(len(os.Args) == 2 || len(os.Args) == 3) {
		panic("usage go run main.go . [-f]")
	}
	path := os.Args[1]
	printFiles := len(os.Args) == 3 && os.Args[2] == "-f"
	err := dirTree(out, path, printFiles)
	if err != nil {
		panic(err.Error())
	}
}
