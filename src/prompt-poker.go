package main

import (
	"fmt"
	"os"
)

var usage = fmt.Sprintf("USAGE: %s FILEPATH\n", os.Args[0])

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintf(os.Stderr, usage)
		os.Exit(2)
	}
	if err := createWriteChmodFile(os.Args[1]); err != nil {
		fmt.Fprintf(os.Stderr, "%v\n", err)
		os.Exit(1)
	}
}

func createWriteChmodFile(filepath string) error {
	// create
	f, err := os.OpenFile(filepath, os.O_WRONLY|os.O_CREATE, 0o600)
	if err != nil {
		return err
	}
	defer f.Close()
	// write
	if _, err := f.WriteString("hello prompting\n"); err != nil {
		return err
	}
	// chmod
	if err := f.Chmod(0o644); err != nil {
		return err
	}
	if err := f.Sync(); err != nil {
		return err
	}
	// write again
	if _, err := f.WriteString("hello again\n"); err != nil {
		return err
	}
	return nil
}
