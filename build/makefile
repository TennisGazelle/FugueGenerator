CC=g++

CXXFLAGS=-g -Wall -std=c++11

INCLUDES=../include/

OUTPUT=../bin/MusicGenerator

$(OUTPUT): ../src/music_notation.cpp
	$(CC) ../src/music_notation.cpp -o $(OUTPUT) $(CXXFLAGS) -I$(INCLUDES)

run: $(OUTPUT)
	$(OUTPUT)

clean:
	rm -f $(OUTPUT)