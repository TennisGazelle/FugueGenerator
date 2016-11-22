CC=g++

CXXFLAGS=-g -Wall -std=c++11

INCLUDES=../include/

OUTPUT=../bin/MusicGenerator

$(OUTPUT): ../music_notation.cpp
	$(CC) ../music_notation.cpp -o $(OUTPUT) $(CXXFLAGS) -I$(INCLUDES)

clean:
	rm -f $(OUTPUT)