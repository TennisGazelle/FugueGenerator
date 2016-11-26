CC=g++

CXXFLAGS=-g -Wall -std=c++11

INCLUDES=../include/

OUTPUT=../bin/MusicGenerator

$(OUTPUT): ../src/fugue_gen.cpp tinyxml2.o
	$(CC) ../src/fugue_gen.cpp ../bin/tinyxml2.o -o $(OUTPUT) $(CXXFLAGS) -I$(INCLUDES)

run: $(OUTPUT)
	$(OUTPUT)

tinyxml2.o:
	$(CC) -c ../src/tinyxml2.cpp -o ../bin/tinyxml2.o -I$(INCLUDES)

clean:
	rm -f $(OUTPUT) ../bin/tinyxml2.o