# FugueGenerator
By Daniel Lopez

As partial completion of Fall 2016, CS - 491: Minds, Maths and Machines,
University of Nevada, Reno,
Department of Computer Science

## Building and Running
Building is easy; it only consists of make a directory called build, running cmake and make to get your executable

```bash
mkdir build && cd build
cmake .. && make
./FugueGenerator
```

## Dependencies
Tiny XML is a library used to quick exporting of XML files.  The two needed files that are used here may be found in `src/` and `include/`.
The rest of their work may be found at https://github.com/leethomason/tinyxml2


