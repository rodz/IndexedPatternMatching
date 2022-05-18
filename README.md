# IndexedPatternMatching
Indexed Pattern Matching Tool


This project as been created as part of the IF767 classes.

Students:
 - Rodrigo Farias Rodrigues Lemos (rfrl)
 - Amanda Nunes Silvestre Costa (ansc)

# How to compile

This program uses `g++` as the default compiler but could also use `gcc`.

To compile just run the following command:

`make ipmt`

Obs: The option `-lstdc++fs` was added to prevent compilation files on Linux operating systems. If running in other environments, we recommend it to be removed.

# How to run

By running the previous command a `ipmt` binary file will be included in the `/bin/` folder and can be executen by running `./bin/ipmt` or can be added directly to the PATH variable and executed everywhere.


```
Usage: ipmt [command] [options] 

Command:
  index TEXT_FILE               Generates an index file based on the first file, the index will contain a copy of the original file and the sufix array
  search PATTERN INDEX_FILE     Given an index file and a pattern will return the list of occurencies of this patten on the file text 
  zip TEXT_FILE                 Generates a compressed .myzip file given the original file
  unzip MYZIP_FILE              Receives a myzip file and restores it back to its original content

Options:
  -p, --pattern PATTERN_FILE    Grabs multiple patterns at once from a file, when this option is passed, the pattern parameter should not be passed.
  -c, --count                   Hides the line results and displays only the total number of occurences
  -h, --help                    Shows this guide
```